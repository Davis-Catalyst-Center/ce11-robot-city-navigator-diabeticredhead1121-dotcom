#include "CityMap.hpp"
#include <iostream>
#include <limits>
#include <queue>
#include <algorithm>
#include <tuple>

CityMap::CityMap() {
    locations.resize(8);

    locations[0].name = "Downtown";
    locations[0].x = 4; locations[0].y = 4;
    locations[0].neighbors = {{1, 8}, {3, 15}, {6, 12}};

    locations[1].name = "Harbor";
    locations[1].x = 4; locations[1].y = 0;
    locations[1].neighbors = {{0, 8}, {2, 20}, {4, 10}};

    locations[2].name = "Airport";
    locations[2].x = 10; locations[2].y = 0;
    locations[2].neighbors = {{1, 20}, {4, 5}, {7, 18}};

    locations[3].name = "University";
    locations[3].x = 0; locations[3].y = 6;
    locations[3].neighbors = {{0, 15}, {5, 9}, {6, 7}};

    locations[4].name = "Industrial";
    locations[4].x = 9; locations[4].y = 1;
    locations[4].neighbors = {{1, 10}, {2, 5}, {7, 8}};

    locations[5].name = "Medical Center";
    locations[5].x = 2; locations[5].y = 9;
    locations[5].neighbors = {{3, 9}, {6, 11}, {7, 14}};

    locations[6].name = "Suburb North";
    locations[6].x = 1; locations[6].y = 5;
    locations[6].neighbors = {{0, 12}, {3, 7}, {5, 11}};

    locations[7].name = "Suburb South";
    locations[7].x = 8; locations[7].y = 8;
    locations[7].neighbors = {{2, 18}, {4, 8}, {5, 14}};
}

void CityMap::printCity() const {
    std::cout << "City Locations:\n";
    for (int i = 0; i < (int)locations.size(); i++) {
        std::cout << "  [" << i << "] " << locations[i].name << "\n";
        std::cout << "       neighbors: ";
        for (int j = 0; j < (int)locations[i].neighbors.size(); j++) {
            auto [idx, time] = locations[i].neighbors[j];
            std::cout << locations[idx].name << "(" << time << ")";
            if (j < (int)locations[i].neighbors.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

std::pair<std::vector<std::string>, int> CityMap::greedyPath(int start, int end) {
    if (start == end) {
        return {{locations[start].name}, 0};
    }

    std::vector<int> prev(locations.size(), -1);
    std::vector<bool> visited(locations.size(), false);
    int current = start;
    int totalCost = 0;
    visited[start] = true;

    while (current != end) {
        int bestNeighbor = -1;
        int bestCost = std::numeric_limits<int>::max();
        for (auto [neighbor, cost] : locations[current].neighbors) {
            if (!visited[neighbor] && cost < bestCost) {
                bestCost = cost;
                bestNeighbor = neighbor;
            }
        }
        if (bestNeighbor == -1) {
            return {{}, -1};
        }
        visited[bestNeighbor] = true;
        prev[bestNeighbor] = current;
        totalCost += bestCost;
        current = bestNeighbor;
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);
    return {path, totalCost};
}

std::pair<std::vector<std::string>, int> CityMap::dijkstraPath(int start, int end) {
    if (start == end) {
        return {{locations[start].name}, 0};
    }

    std::vector<int> dist(locations.size(), std::numeric_limits<int>::max());
    std::vector<int> prev(locations.size(), -1);
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        if (cost > dist[current]) continue;

        for (auto [neighbor, weight] : locations[current].neighbors) {
            int newDist = dist[current] + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }

    if (dist[end] == std::numeric_limits<int>::max()) {
        return {{}, -1};
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);
    return {path, dist[end]};
}

std::pair<std::vector<std::string>, int> CityMap::aStarPath(int start, int end) {
    if (start == end) {
        return {{locations[start].name}, 0};
    }

    std::vector<int> gScore(locations.size(), std::numeric_limits<int>::max());
    std::vector<int> fScore(locations.size(), std::numeric_limits<int>::max());
    std::vector<int> prev(locations.size(), -1);
    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<std::tuple<int, int, int>>> pq;

    gScore[start] = 0;
    fScore[start] = heuristic(start, end);
    pq.push({fScore[start], 0, start});

    while (!pq.empty()) {
        auto [f, g, current] = pq.top();
        pq.pop();

        if (current == end) {
            break;
        }

        for (auto [neighbor, weight] : locations[current].neighbors) {
            int tentativeG = gScore[current] + weight;
            if (tentativeG < gScore[neighbor]) {
                gScore[neighbor] = tentativeG;
                fScore[neighbor] = tentativeG + heuristic(neighbor, end);
                prev[neighbor] = current;
                pq.push({fScore[neighbor], tentativeG, neighbor});
            }
        }
    }

    if (gScore[end] == std::numeric_limits<int>::max()) {
        return {{}, -1};
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);
    return {path, gScore[end]};
}

std::vector<std::string> CityMap::reconstructPath(const std::vector<int>& prev, int start, int end) const {
    std::vector<std::string> path;
    for (int at = end; at != -1; at = prev[at]) {
        path.push_back(locations[at].name);
        if (at == start) break;
    }
    std::reverse(path.begin(), path.end());
    return path;
}