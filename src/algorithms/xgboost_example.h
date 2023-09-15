#ifndef XGBOOST_EXAMPLE_H
#define XGBOOST_EXAMPLE_H

#include <iostream>
#include <vector>
#include <dmlc/data.h>
#include <xgboost/c_api.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>


#define safe_xgboost(call)                                                                              \
    {                                                                                                   \
        int err = (call);                                                                               \
        if (err != 0)                                                                                   \
        {                                                                                               \
            fprintf(stderr, "%s:%d: error in %s: %s\\n", __FILE__, __LINE__, #call, XGBGetLastError()); \
            exit(1);                                                                                    \
        }                                                                                               \
    }

// Function to calculate the AUC (Area Under the ROC Curve)
inline double calculateAUC(const std::vector<float> &labels, const std::vector<float> &predictions)
{
    // Combine labels and predictions for sorting
    std::vector<std::pair<int, float>> label_pred_pairs;
    for (size_t i = 0; i < predictions.size(); ++i)
    {
        label_pred_pairs.emplace_back(labels[i], predictions[i]);
    }

    // Sort by prediction values in ascending order
    std::sort(label_pred_pairs.begin(), label_pred_pairs.end(),
              [](const std::pair<int, float> &a, const std::pair<int, float> &b)
              {
                  return a.second < b.second;
              });

    // Calculate AUC using the trapezoidal rule
    double auc = 0.0;
    double tpr = 0.0;
    double prev_fpr = 0.0;
    double prev_tpr = 0.0;

    for (const auto &pair : label_pred_pairs)
    {
        if (pair.first == 1)
        {
            tpr += 1.0;
        }
        else
        {
            if (tpr > 0.0)  // Avoid division by zero
            {
                auc += (tpr + prev_tpr) * (pair.second - prev_fpr) / 2.0;
            }
            prev_fpr = pair.second;
            prev_tpr = tpr;
        }
    }

    if (prev_tpr > 0.0 && prev_fpr < 1.0)  // Avoid division by zero
    {
        auc += (1.0 + prev_tpr) * (1.0 - prev_fpr) / 2.0;
    }

    return auc;
}

inline std::tuple<std::vector<float>, float, float, float, std::vector<float>> trainXgBoost(std::vector<std::vector<float>> input_data, std::vector<float> labels, float trainRatio, int epochs, int depth, std::function<void(int)> epochCallback)
{
    BoosterHandle booster;
    // Convert input data to DMatrix
    bst_ulong num_rows = input_data.size();
    bst_ulong num_cols = input_data[0].size();

    // Initialize the data matrix
    DMatrixHandle data_dmatrix;
    XGDMatrixCreateFromMat(reinterpret_cast<float *>(input_data.data()), num_rows, num_cols, NAN, &data_dmatrix);

    // Set the labels
    XGDMatrixSetFloatInfo(data_dmatrix, "label", labels.data(), num_rows);

    // Set the random seed for reproducibility
    std::default_random_engine rng(42);

    // Create a random permutation of row indices
    std::vector<int> row_indices(num_rows);
    std::iota(row_indices.begin(), row_indices.end(), 0);
    std::shuffle(row_indices.begin(), row_indices.end(), rng);

    // Calculate the number of rows for training and testing
    size_t train_size = num_rows * trainRatio;
    size_t test_size = num_rows - train_size;

    // Split the dataset into train and test DMatrix
    DMatrixHandle train_dmatrix, test_dmatrix;

    // Set the split points for training and testing
    std::vector<int> train_row_indices(row_indices.begin(), row_indices.begin() + train_size);
    std::vector<int> test_row_indices(row_indices.begin() + train_size, row_indices.end());

    // Set the training and testing DMatrix
    XGDMatrixSliceDMatrix(data_dmatrix, train_row_indices.data(), train_row_indices.size(), &train_dmatrix);
    XGDMatrixSliceDMatrix(data_dmatrix, test_row_indices.data(), test_row_indices.size(), &test_dmatrix);

    XGBoosterCreate(&train_dmatrix, 1, &booster);
    bst_ulong trainrows, traincols;
    XGDMatrixNumRow(train_dmatrix, &trainrows);
    XGDMatrixNumCol(train_dmatrix, &traincols);
    // std::cout << "Train Data:" << trainrows << " rows, " << traincols << " columns" << endl;
    bst_ulong testrows, testcols;
    XGDMatrixNumRow(test_dmatrix, &testrows);
    XGDMatrixNumCol(test_dmatrix, &testcols);
    // std::cout << "Test Data:" << testrows << " rows, " << testcols << " columns" << endl;
    // Set XGBoost parameters
    std::vector<std::string> xgboost_args = {
        "objective=reg:squarederror",
        "eval_metric=error",
        "max_depth=" + std::to_string(depth),
        "eta=0.1",
        "silent=0",
    };

    // Train the XGBoost model
    XGBoosterCreate(&train_dmatrix, 1, &booster);
    for (auto p : xgboost_args)
    {
        const char *param = p.c_str();
        XGBoosterSetParam(booster, param, param + strlen(param));
    }
    for (int i = 0; i < epochs; ++i)
    {
        XGBoosterUpdateOneIter(booster, i, train_dmatrix);
        epochCallback(i);
    }

    // Predict on the test dataset
    const float *test_preds;
    bst_ulong test_preds_len;
    std::vector<float> test_preds_vector;
    XGBoosterPredict(booster, test_dmatrix, 0, 0, 0, &test_preds_len, &test_preds);
    // std::cout << "Test len: " << test_preds_len <<endl;
    // Calculate accuracy
    int correct_predictions = 0;
    for (bst_ulong i = 0; i < test_preds_len; ++i)
    {
        int predicted_label = (test_preds[i] >= 0.5) ? 1 : 0;
        if (predicted_label == labels[test_row_indices[i]])
        {
            correct_predictions++;
        }
        test_preds_vector.push_back(test_preds[i]);
    }
    double accuracy = static_cast<double>(correct_predictions) / test_preds_len;

    // Calculate AUC
    double auc = calculateAUC(labels, test_preds_vector);

    // Calculate F1 score
    double epsilon = 1e-15;
    double true_positives = 0.0;
    double false_positives = 0.0;
    double false_negatives = 0.0;

    for (bst_ulong i = 0; i < test_preds_len; ++i)
    {
        int predicted_label = (test_preds[i] >= 0.5) ? 1 : 0;
        int actual_label = labels[test_row_indices[i]];

        if (predicted_label == 1 && actual_label == 1)
        {
            true_positives += 1.0;
        }
        else if (predicted_label == 1 && actual_label == 0)
        {
            false_positives += 1.0;
        }
        else if (predicted_label == 0 && actual_label == 1)
        {
            false_negatives += 1.0;
        }
    }

    double precision = true_positives / (true_positives + false_positives + epsilon);
    double recall = true_positives / (true_positives + false_negatives + epsilon);
    double f1_score = 2 * (precision * recall) / (precision + recall + epsilon);

    // Output results
    std::cout << "Accuracy: " << accuracy << std::endl;
    std::cout << "AUC: " << auc << std::endl;
    std::cout << "F1 Score: " << f1_score << std::endl;

    // std::cout << "Train 1" << endl;
    bst_ulong out_len;
    const float *out_result;
    XGBoosterPredict(booster, test_dmatrix, 4, 0, 0, &out_len, &out_result);
    // std::cout << "Train 2 len: " << out_len <<endl;
    std::vector<float> sorted_scores;

    for (int i = 0; i < num_cols; i++)
    {
        float score = 0;
        for (int j = 0; j < out_len/num_cols; j++)
        {
            score += out_result[i+j*num_cols];
        }
        
        // std::cout << score << endl;
        sorted_scores.push_back(score);
    }
    // std::cout << "Train 3" << endl;
    // Clean up
    XGDMatrixFree(train_dmatrix);
    XGDMatrixFree(test_dmatrix);
    XGDMatrixFree(data_dmatrix);
    XGBoosterFree(booster);
    return {test_preds_vector, accuracy, auc, f1_score, sorted_scores};
}

// void testXgboost()
// {
//     // Load data from memory
//     float train[4][2] = {{1, 2}, {3, 4}, {5, 6}, {7, 8}};
//     float label[4] = {0, 1, 0, 1};
//     DMatrixHandle dtrain;
//     safe_xgboost(XGDMatrixCreateFromMat((float *)train, 4, 2, -1, &dtrain));
//     safe_xgboost(XGDMatrixSetFloatInfo(dtrain, "label", label, 4)); // set label parameter
//     // Create booster
//     BoosterHandle booster;
//     safe_xgboost(XGBoosterCreate(&dtrain, 1, &booster));

//     // Set parameters
//     safe_xgboost(XGBoosterSetParam(booster, "objective", "reg:squarederror"));
//     safe_xgboost(XGBoosterSetParam(booster, "max_depth", "3"));
//     safe_xgboost(XGBoosterSetParam(booster, "eta", "0.1"));

//     // Train for 10 iterations
//     for (int iter = 0; iter < 2; iter++)
//     {
//         safe_xgboost(XGBoosterUpdateOneIter(booster, iter, dtrain));
//     }

//     // Use train data as test data
//     DMatrixHandle dtest = dtrain;

//     // Predict
//     bst_ulong out_len;
//     const float *out_result;
//     safe_xgboost(XGBoosterPredict(booster, dtest, 4, 2, 0, &out_len, &out_result));

//     // Print predictions
//     for (int i = 0; i < out_len; i++)
//     {
//         printf("%f\n", out_result[i]);
//     }

//     safe_xgboost(XGBoosterPredict(booster, dtest, 4, 2, 1, &out_len, &out_result));

//     // Print feature contributions
//     for (int i = 0; i < out_len; i++)
//     {
//         printf("%f\n", out_result[i]);
//     }
// }
#endif // XGBOOST_EXAMPLE_H
