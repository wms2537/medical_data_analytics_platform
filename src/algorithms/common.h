#pragma once
#include <iostream>
#include <vector>
#include "Eigen/Dense"

using namespace Eigen;

// Function to calculate Euclidean distance between two data points
inline float calculateDistance(const VectorXf& point1, const VectorXf& point2) {
    return (point1 - point2).norm();
}

// Function to convert std::vector<std::vector<float>> to std::vector<VectorXf>
inline std::vector<VectorXf> convertToEigenVectors(const std::vector<std::vector<float>>& dataset) {
    std::vector<VectorXf> eigenVectors;
    for (const auto& row : dataset) {
        VectorXf eigenVector(row.size());
        for (size_t i = 0; i < row.size(); ++i) {
            eigenVector(i) = row[i];
        }
        eigenVectors.push_back(eigenVector);
    }
    return eigenVectors;
}