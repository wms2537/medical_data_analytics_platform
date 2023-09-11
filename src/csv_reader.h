#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "algorithms/rowfeature.hpp"
#include "algorithms/leastsquare.hpp"
#include "algorithms/covariance.hpp"
#include "algorithms/pca.hpp"
#include "algorithms/kmeans.hpp"
#include "algorithms/dbscan.hpp"
#include "algorithms/spectral.hpp"
#include "algorithms/mean_shift.hpp"
#include "algorithms/normalise.hpp"
#include "algorithms/xgboost_example.h"

struct MeanStdResult
{
    int columnIndex;
    std::string columnName;
    float mean;
    float variance;
};

enum ClusteringAlgorithm {
    K_Means,
    DBSCAN,
    Spectral,
    MeanShift
};

class CSVReader
{
    std::vector<std::vector<float>> content;
    std::vector<std::string> headers;

public:
    CSVReader(){};
    void loadFile(std::string filePath)
    {
        std::vector<float> row;
        std::string line, word;

        std::fstream file(filePath, std::ios::in);
        if (file.is_open())
        {
            bool headersLoaded = false;
            while (std::getline(file, line))
            {
                row.clear();

                std::stringstream str(line);

                while (getline(str, word, ','))
                {
                    if (!headersLoaded)
                    {
                        headers.push_back(word);
                        continue;
                    }
                    if (word.compare("B") == 0)
                    {
                        row.push_back(0.0);
                    }
                    else if (word.compare("M") == 0)
                    {
                        row.push_back(1.0);
                    }
                    else
                    {
                        row.push_back(stod(word));
                    }
                }
                if (!headersLoaded)
                {
                    headersLoaded = true;
                    continue;
                }
                row.push_back(0.0);
                row.push_back(0.0);
                row.push_back(0.0);
                row.push_back(0.0);
                content.push_back(row);
            }
        }
        clustering(ClusteringAlgorithm::K_Means, {1, 2, 3, 4, 5}, 2);
        clustering(ClusteringAlgorithm::DBSCAN, {1, 2, 3, 4, 5}, 2);
        clustering(ClusteringAlgorithm::Spectral, {1, 2, 3, 4, 5}, 2);
        clustering(ClusteringAlgorithm::MeanShift, {1, 2, 3, 4, 5}, 2);
    }
    std::vector<std::vector<float>> getContent()
    {
        return content;
    }
    std::vector<std::string> getHeaders()
    {
        return headers;
    }
    bool fileSelected()
    {
        return content.size() > 0;
    }
    MeanStdResult getMeanStd(int col)
    {
        std::vector<float> inX;
        for (int i = 0; i < content.size(); i++)
        {
            inX.push_back(content[i][col]);
        }

        auto res = getAvgVar(inX);
        return {col, headers[col], std::get<0>(res), std::get<1>(res)};
    }
    std::tuple<Eigen::VectorXf, float, float, std::vector<float>, float, float, float, float> getLeastSquareAndPR(int xIndex, int yIndex, int inDegree)
    {
        std::vector<float> inX, inY;
        for (int i = 0; i < content.size(); i++)
        {
            inX.push_back(content[i][xIndex]);
            inY.push_back(content[i][yIndex]);
        }
        auto res = fitLeastSquareAndPR(inX, inY, inDegree);

        float minX = *min_element(inX.begin(), inX.end());
        float maxX = *max_element(inX.begin(), inX.end());

        float minY = *min_element(inY.begin(), inY.end());
        float maxY = *max_element(inY.begin(), inY.end());

        return {
            std::get<0>(res),
            std::get<1>(res),
            std::get<2>(res),
            range(minX, maxX, inDegree + 5),
            minX,
            maxX,
            minY,
            maxY};
    }

    std::vector<float> range(float min, float max, size_t N)
    {
        std::vector<float> range;
        float delta = (max - min) / float(N - 1);
        for (int i = 0; i < N; i++)
        {
            range.push_back(min + i * delta);
        }
        return range;
    }
    std::tuple<std::vector<std::vector<float>>, float, float> getCorrelationMatrix(std::vector<int> cols, bool isPearson)
    {
        std::vector<std::vector<float>> inMat;
        for (auto col : cols)
        {
            std::vector<float> temp;
            for (auto row : content)
            {

                temp.push_back(row[col]);
            }
            inMat.push_back(temp);
        }

        auto res = getCovariance(inMat);
        if (isPearson)
        {
            std::vector<float> vars;
            for (auto vec : inMat)
            {
                auto avgVar = getAvgVar(vec);
                vars.push_back(std::get<1>(avgVar));
            }
            res = getPearsonCorr(res, vars);
        }
        std::vector<std::vector<float>> result;
        float min = 0, max = 0;
        for (int i = 0; i < cols.size(); i++)
        {
            std::vector<float> temp;
            for (int j = 0; j < cols.size(); j++)
            {
                if (res(i, j) < min)
                    min = res(i, j);
                if (res(i, j) > max)
                    max = res(i, j);
                temp.push_back(res(i, j));
            }
            result.push_back(temp);
        }
        return {result, min, max};
    }
    std::vector<std::vector<float>> getPca(std::vector<int> cols, int dim)
    {
        std::vector<std::vector<float>> inMat;
        for (auto row : content)
        {
            std::vector<float> temp;
            for (auto col : cols)
            {
                temp.push_back(row[col+1]);
            }
            inMat.push_back(temp);
        }
        auto res = pca(inMat, dim);
        std::vector<std::vector<float>> result;
        // float min = 0, max = 0;
        for (int i = 0; i < content.size(); i++)
        {
            std::vector<float> temp;
            for (int j = 0; j < dim; j++)
            {
                // if (res(i, j) < min)
                //     min = res(i, j);
                // if (res(i, j) > max)
                //     max = res(i, j);
                temp.push_back(res(i, j));
            }
            result.push_back(temp);
        }
        return result;
    }
    std::vector<int> clustering(ClusteringAlgorithm algo, std::vector<int> cols, int k) {
        std::vector<std::vector<float>> inMat;
        for (auto row : content)
        {
            std::vector<float> temp;
            for (auto col : cols)
            {
                temp.push_back(row[col+1]);
            }
            inMat.push_back(temp);
        }
        std::vector<int> res;
        switch (algo)
        {
        case ClusteringAlgorithm::K_Means:
            res = clusterKMeans(normalizeData(inMat), k);
            for (int i = 0; i < content.size(); i++)
            {
                content[i][headers.size()] = res[i];
            }
            break;
        case ClusteringAlgorithm::DBSCAN:
            res = dbscanClustering(normalizeData(inMat), 0.1, 2);
            for (int i = 0; i < content.size(); i++)
            {
                content[i][headers.size()+1] = res[i];
            }
            break;
        case ClusteringAlgorithm::Spectral:
            res = spectralClustering(normalizeData(inMat), k, 0.5, 2);
            for (int i = 0; i < content.size(); i++)
            {
                content[i][headers.size()+2] = res[i];
            }
            break;
        case ClusteringAlgorithm::MeanShift:
            res = meanShiftClustering(normalizeData(inMat), 0.5);
            for (int i = 0; i < content.size(); i++)
            {
                content[i][headers.size()+3] = res[i];
            }
            break;
        default:
            break;
        }
        return res;
    }
    std::tuple<std::vector<float>, float, float, float, std::vector<float>> trainModel(float trainRatio, int epochs, int depth, std::function<void(int)> epochCallback) {
        std::vector<std::vector<float>> inMat;
        std::vector<float> labels;
        for (auto row : content)
        {
            std::vector<float> temp;
            for (int i = 1; i < row.size()-4; i++)
            {
                if(i == 1) {
                    labels.push_back(row[i]);
                } else {
                    temp.push_back(row[i]);
                }
            }
            inMat.push_back(temp);
        }
        auto res = trainXgBoost(inMat, labels, trainRatio, epochs, depth, epochCallback);
        return res;
    }
};