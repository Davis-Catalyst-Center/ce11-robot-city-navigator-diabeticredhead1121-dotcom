#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "CityMap.hpp"

void printResult(const std::string& algorithm, const std::pair<std::vector<std::string>, int>& result);

int main() {
    CityMap city;

    std::cout << "=== George's City Navigation System ===\n\n";

    city.printCity();
    std::cout << "\n";

    std::cout << "--- Route: Downtown -> Airport ---\n\n";

    /* Test all three algorithms on this route */
    auto result1 = city.greedyPath(0, 2);
    printResult("Greedy", result1);

    auto result2 = city.dijkstraPath(0, 2);
    printResult("Dijkstra", result2);

    auto result3 = city.aStarPath(0, 2);
    printResult("A*", result3);

    std::cout << "\n";

    std::cout << "--- Route: Downtown -> Suburb South ---\n\n";

    /* Test all three algorithms on this route */
    auto result4 = city.greedyPath(0, 7);
    printResult("Greedy", result4);

    auto result5 = city.dijkstraPath(0, 7);
    printResult("Dijkstra", result5);

    auto result6 = city.aStarPath(0, 7);
    printResult("A*", result6);

    std::cout << "\n";

    std::cout << "--- Route: Suburb South -> Downtown ---\n\n";

    /* Pick your own start/end pair and test all three algorithms */
    auto result7 = city.greedyPath(7, 0);
    printResult("Greedy", result7);

    auto result8 = city.dijkstraPath(7, 0);
    printResult("Dijkstra", result8);

    auto result9 = city.aStarPath(7, 0);
    printResult("A*", result9);

    return 0;
}

void printResult(const std::string& algorithm, const std::pair<std::vector<std::string>, int>& result) {
    std::cout << "[" << algorithm << "]\n";
    if (result.first.empty() || result.second == -1) {
        std::cout << "  No path found.\n";
        return;
    }
    std::cout << "  Path: ";
    for (int i = 0; i < (int)result.first.size(); i++) {
        std::cout << result.first[i];
        if (i < (int)result.first.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n  Total time: " << result.second << " min\n";
}
