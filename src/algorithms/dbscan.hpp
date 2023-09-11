#include <unordered_set>
#include "common.h"

using namespace Eigen;

// Define a structure to represent a data point
struct Point {
    VectorXf coordinates;
    bool visited;
    int cluster;
};

// Function to find neighbor points within epsilon distance
inline std::vector<int> findNeighbors(const std::vector<Point>& points, int pointIdx, float epsilon) {
    std::vector<int> neighbors;
    for (int i = 0; i < points.size(); ++i) {
        if (calculateDistance(points[pointIdx].coordinates, points[i].coordinates) <= epsilon) {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// Function to expand a cluster
inline void expandCluster(std::vector<Point>& points, int pointIdx, int cluster, float epsilon, int minPts) {
    points[pointIdx].cluster = cluster;
    std::vector<int> neighbors = findNeighbors(points, pointIdx, epsilon);
    
    if (neighbors.size() < minPts) {
        return; // Not a core point, stop recursion
    }
    
    for (int i = 0; i < neighbors.size(); ++i) {
        int neighborIdx = neighbors[i];
        if (!points[neighborIdx].visited) {
            points[neighborIdx].visited = true;
            expandCluster(points, neighborIdx, cluster, epsilon, minPts);
        }
        if (points[neighborIdx].cluster == 0) {
            points[neighborIdx].cluster = cluster;
        }
    }
}

// DBSCAN clustering algorithm
inline std::vector<int> dbscanClustering(const std::vector<std::vector<float>>& input_data, float epsilon, int minPts) {
    std::vector<Point> points(input_data.size());
    int cluster = 0;

    // Initialize points
    for (int i = 0; i < input_data.size(); ++i) {
        VectorXf coordinates(input_data[i].size());
        for (int j = 0; j < input_data[i].size(); ++j) {
            coordinates(j) = input_data[i][j];
        }
        points[i].coordinates = coordinates;
        points[i].visited = false;
        points[i].cluster = 0;  // 0 represents unclassified
    }

    for (int i = 0; i < points.size(); ++i) {
        if (!points[i].visited) {
            points[i].visited = true;
            std::vector<int> neighbors = findNeighbors(points, i, epsilon);
            
            if (neighbors.size() < minPts) {
                // Mark this point as noise
                points[i].cluster = -1;  // Noise
            } else {
                ++cluster;
                expandCluster(points, i, cluster, epsilon, minPts);
            }
        }
    }

    // Extract cluster assignments
    std::vector<int> clusterAssignments(points.size());
    for (int i = 0; i < points.size(); ++i) {
        clusterAssignments[i] = points[i].cluster;
    }

    return clusterAssignments;
}