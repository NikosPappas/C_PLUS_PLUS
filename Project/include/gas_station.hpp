#ifndef GAS_STATION_HPP
#define GAS_STATION_HPP

#include <vector>

/**
 * Represents a gas station at a certain distance with available fuel.
 */
struct GasStation {
    int distance; // distance from start
    int fuel;     // fuel available
};

/**
 * Computes the minimum number of refueling stops to reach the target.
 *
 * @param target_distance Distance to destination
 * @param start_fuel Initial fuel amount
 * @param stations List of gas stations
 * @return Minimum number of stops, or -1 if unreachable
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
int min_refuel_stops(
    int target_distance,
    int start_fuel,
    std::vector<GasStation> stations
);

#endif // GAS_STATION_HPP

