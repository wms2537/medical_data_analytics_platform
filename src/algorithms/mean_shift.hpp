#include "common.h"

using namespace Eigen;

// Define a structure to represent a data point
struct MSPoint {
    VectorXf coordinates;
    bool visited;
};

// Mean Shift clustering algorithm
inline std::vector<int> meanShiftClustering(const std::vector<std::vector<float>>& input_data, float bandwidth) {
    std::vector<VectorXf> dataset = convertToEigenVectors(input_data);
    int dataSize = dataset.size();
    std::vector<MSPoint> points(dataSize);

    // Initialize points
    for (int i = 0; i < dataSize; ++i) {
        points[i].coordinates = dataset[i];
        points[i].visited = false;
    }

    std::vector<int> clusterAssignments(dataSize);
    int clusterIndex = 0;

    for (int i = 0; i < dataSize; ++i) {
        if (!points[i].visited) {
            std::vector<int> clusterIndices;
            VectorXf meanShiftVector = points[i].coordinates;
            
            while (true) {
                // Find data points within the bandwidth
                clusterIndices.clear();
                for (int j = 0; j < dataSize; ++j) {
                    if (!points[j].visited) {
                        float distance = calculateDistance(points[j].coordinates, meanShiftVector);
                        if (distance <= bandwidth) {
                            clusterIndices.push_back(j);
                        }
                    }
                }

                // Calculate the mean shift std::vector
                VectorXf sumShift = VectorXf::Zero(meanShiftVector.size());
                float sumWeights = 0.0;
                for (int idx : clusterIndices) {
                    float distance = calculateDistance(points[idx].coordinates, meanShiftVector);
                    float weight = exp(-0.5 * (distance / bandwidth) * (distance / bandwidth));
                    sumShift += weight * points[idx].coordinates;
                    sumWeights += weight;
                }
                VectorXf newMeanShiftVector = sumShift / sumWeights;

                // Check for convergence
                if (calculateDistance(newMeanShiftVector, meanShiftVector) < 1e-4) {
                    break;
                }
                meanShiftVector = newMeanShiftVector;
            }

            // Assign points to the cluster
            for (int idx : clusterIndices) {
                points[idx].visited = true;
                clusterAssignments[idx] = clusterIndex;
            }

            // Move to the next cluster
            clusterIndex++;
        }
    }

    return clusterAssignments;
}