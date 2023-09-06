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

using namespace std;

struct MeanStdResult
{
    int columnIndex;
    std::string columnName;
    float mean;
    float variance;
};

class CSVReader
{
    vector<vector<float>> content;
    vector<string> headers;

public:
    CSVReader(){};
    void loadFile(string filePath)
    {
        vector<float> row;
        string line, word;

        fstream file(filePath, ios::in);
        if (file.is_open())
        {
            bool headersLoaded = false;
            while (getline(file, line))
            {
                row.clear();

                stringstream str(line);

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
                content.push_back(row);
            }
        }
    }
    vector<vector<float>> getContent()
    {
        return content;
    }
    vector<string> getHeaders()
    {
        return headers;
    }
    bool fileSelected()
    {
        return content.size() > 0;
    }
    MeanStdResult getMeanStd(int col)
    {
        vector<float> inX;
        for (int i = 0; i < content.size(); i++)
        {
            inX.push_back(content[i][col]);
        }

        auto res = getAvgVar(inX);
        return {col, headers[col], get<0>(res), get<0>(res)};
    }
    tuple<Eigen::VectorXf, float, float, vector<float>, float, float, float, float> getLeastSquareAndPR(int xIndex, int yIndex, int inDegree)
    {
        vector<float> inX, inY;
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
            get<0>(res),
            get<1>(res),
            get<2>(res),
            range(minX, maxX, inDegree + 5),
            minX,
            maxX,
            minY,
            maxY};
    }

    vector<float> range(float min, float max, size_t N)
    {
        vector<float> range;
        float delta = (max - min) / float(N - 1);
        for (int i = 0; i < N; i++)
        {
            range.push_back(min + i * delta);
        }
        return range;
    }
    tuple<vector<vector<float>>, float, float> getCorrelationMatrix(vector<int> cols, bool isPearson)
    {
        vector<vector<float>> inMat;
        for (auto col : cols)
        {
            vector<float> temp;
            for (auto row : content)
            {

                temp.push_back(row[col]);
            }
            inMat.push_back(temp);
        }

        auto res = getCovariance(inMat);
        if (isPearson)
        {
            vector<float> vars;
            for (auto vec : inMat)
            {
                auto avgVar = getAvgVar(vec);
                vars.push_back(std::get<1>(avgVar));
            }
            res = getPearsonCorr(res, vars);
        }
        vector<vector<float>> result;
        float min = 0, max = 0;
        for (int i = 0; i < cols.size(); i++)
        {
            vector<float> temp;
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
    vector<vector<float>> getPca(vector<int> cols, int dim)
    {
        vector<vector<float>> inMat;
        for (auto row : content)
        {
            vector<float> temp;
            for (auto col : cols)
            {
                temp.push_back(row[col+1]);
            }
            inMat.push_back(temp);
        }
        auto res = pca(inMat, dim);
        std::cout << "res: \n" << res << std::endl;
        vector<vector<float>> result;
        // float min = 0, max = 0;
        for (int i = 0; i < content.size(); i++)
        {
            vector<float> temp;
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
};