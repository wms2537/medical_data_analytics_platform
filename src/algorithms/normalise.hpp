#include "common.h"

using namespace Eigen;

// Function to normalize each column of the input data using Min-Max scaling
inline std::vector<std::vector<float>> normalizeData(const std::vector<std::vector<float>>& inputData) {
    int rows = inputData.size();
    int cols = inputData[0].size();
    std::vector<std::vector<float>> normalizedData(rows, std::vector<float>(cols));

    // Find the minimum and maximum values for each column
    std::vector<float> minVals(cols, std::numeric_limits<float>::max());
    std::vector<float> maxVals(cols, std::numeric_limits<float>::min());

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            normalizedData[i][j] = inputData[i][j];
            minVals[j] = std::min(minVals[j], normalizedData[i][j]);
            maxVals[j] = std::max(maxVals[j], normalizedData[i][j]);
        }
    }

    // Perform Min-Max scaling for each column
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            if (maxVals[j] != minVals[j]) {
                normalizedData[i][j] = (normalizedData[i][j] - minVals[j]) / (maxVals[j] - minVals[j]);
            } else {
                // Handle the case where max and min are the same (constant feature)
                normalizedData[i][j] = 0.0;
            }
        }
    }

    return normalizedData;
}