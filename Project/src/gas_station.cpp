#include "gas_station.hpp"
#include <algorithm>
#include <queue>

int min_refuel_stops(
    int target_distance,
    int start_fuel,
    std::vector<GasStation> stations
) {
    // Sort stations by distance from start
    std::sort(stations.begin(), stations.end(),
              [](const GasStation& a, const GasStation& b) {
                  return a.distance < b.distance;
              });

    // Max-heap of fuel amounts
    std::priority_queue<int> maxHeap;

    int refuels = 0;
    int currentFuel = start_fuel;
    size_t i = 0;

    while (currentFuel < target_distance) {
        // Add all reachable stations
        while (i < stations.size() && stations[i].distance <= currentFuel) {
            maxHeap.push(stations[i].fuel);
            ++i;
        }

        // No station to refuel from → impossible
        if (maxHeap.empty())
            return -1;

        // Refuel with the largest available fuel
        currentFuel += maxHeap.top();
        maxHeap.pop();
        refuels++;
    }

    return refuels;
}

