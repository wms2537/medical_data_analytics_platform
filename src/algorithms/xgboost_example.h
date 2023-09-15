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

inline double calculateAUC(const std::vector<float> &labels, const std::vector<float> &predictions) {
    // Make sure labels and predictions have the same size
    if (labels.size() != predictions.size()) {
        std::cout << labels.size() << " vs " << predictions.size() << std::endl;
        throw std::invalid_argument("Input vectors must have the same size");
    }

    // Create a vector of pairs to store labels and corresponding predictions
    std::vector<std::pair<float, float>> labelPredictionPairs;
    for (size_t i = 0; i < labels.size(); ++i) {
        labelPredictionPairs.push_back(std::make_pair(labels[i], predictions[i]));
    }

    // Sort the pairs in descending order of predictions
    std::sort(labelPredictionPairs.begin(), labelPredictionPairs.end(),
              [](const std::pair<float, float> &a, const std::pair<float, float> &b) {
                  return a.second > b.second;
              });

    double auc = 0.0;
    double truePositives = 0.0;
    double falsePositives = 0.0;
    double prevTruePositives = 0.0;
    double prevFalsePositives = 0.0;
    double prevPrediction = -std::numeric_limits<float>::infinity();

    // Calculate the AUC using the trapezoidal rule
    for (const auto &pair : labelPredictionPairs) {
        double label = pair.first;
        double prediction = pair.second;

        if (prediction != prevPrediction) {
            auc += ((truePositives + prevTruePositives) / 2) * (falsePositives - prevFalsePositives);
            prevTruePositives = truePositives;
            prevFalsePositives = falsePositives;
            prevPrediction = prediction;
        }

        if (label == 1.0) {
            truePositives += 1.0;
        } else {
            falsePositives += 1.0;
        }
    }

    // Calculate the final AUC
    auc += ((truePositives + prevTruePositives) / 2) * (falsePositives - prevFalsePositives);

    return auc / (truePositives * falsePositives);
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
    std::vector<float> test_labels;
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
        test_labels.push_back(labels[test_row_indices[i]]);
    }
    double accuracy = static_cast<double>(correct_predictions) / test_preds_len;

    // Calculate AUC
    double auc = calculateAUC(test_labels, test_preds_vector);

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
#endif // XGBOOST_EXAMPLE_H
