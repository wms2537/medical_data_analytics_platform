#include "common.h"
#include "kmeans.hpp"

using namespace Eigen;

// Function to calculate Gaussian similarity between two data points
inline float gaussianSimilarity(const std::vector<float> &point1, const std::vector<float> &point2, float sigma)
{
    float distanceSquared = 0.0;
    for (size_t i = 0; i < point1.size(); ++i)
    {
        float diff = point1[i] - point2[i];
        distanceSquared += diff * diff;
    }
    return exp(-distanceSquared / (2 * sigma * sigma));
}

inline std::vector<int> spectralClustering(const std::vector<std::vector<float>> &dataset, int numClusters, float sigma, int k)
{
    int dataSize = static_cast<int>(dataset.size());


    // Step 1: Create similarity matrix
    Eigen::MatrixXf similarityMatrix(dataSize, dataSize);
    for (int i = 0; i < dataSize; ++i)
    {
        for (int j = i; j < dataSize; ++j)
        {
            float similarity = gaussianSimilarity(dataset[i], dataset[j], sigma);
            similarityMatrix(i, j) = similarity;
            similarityMatrix(j, i) = similarity;
        }
    }


    // Step 2: Compute the degree matrix
    Eigen::VectorXf degreeVector = (similarityMatrix.rowwise().sum().array().sqrt()).matrix();

    Eigen::MatrixXf degreeMatrix = degreeVector.asDiagonal();

    // Step 3: Compute Laplacian matrix
    Eigen::MatrixXf laplacianMatrix = degreeMatrix.inverse() * similarityMatrix * degreeMatrix.inverse();

    // Step 4: Compute the first k eigenstd::vectors of the Laplacian matrix
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eigenSolver(laplacianMatrix);
    Eigen::MatrixXf eigenVectors = eigenSolver.eigenvectors().block(0, 0, dataSize, k);

    // Convert Eigen Matrix to std::vector of std::vector<float>
    std::vector<std::vector<float>> eigenVectorsVec;
    for (int i = 0; i < eigenVectors.rows(); ++i)
    {
        eigenVectorsVec.emplace_back(eigenVectors.row(i).data(), eigenVectors.row(i).data() + eigenVectors.cols());
    }

    // Step 5: Perform K-means clustering on the rows of the eigenstd::vector matrix
    std::vector<int> clusterAssignments = clusterKMeans(eigenVectorsVec, numClusters, 100); // Adjust maxIterations as needed

    return clusterAssignments;
}