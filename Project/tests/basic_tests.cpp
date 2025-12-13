#include <cassert>
#include <vector>
#include <iostream>

#include "activity_selection.hpp"
#include "job_sequencing.hpp"
#include "rope_merging.hpp"
#include "gas_station.hpp"

/* ---------------- Activity Selection ---------------- */

void test_activity_selection() {
    std::vector<Activity> activities = {
        {1, 3}, {2, 5}, {4, 6}, {6, 7}, {5, 8}, {8, 9}
    };

    auto selected = select_max_activities(activities);

    // Optimal solution size is 4
    assert(selected.size() == 4);

    // Verify non-overlapping
    for (size_t i = 1; i < selected.size(); ++i) {
        assert(selected[i].start >= selected[i - 1].finish);
    }
}

/* ---------------- Job Sequencing ---------------- */

void test_job_sequencing() {
    std::vector<Job> jobs = {
        {1, 100, 2},
        {2, 19, 1},
        {3, 27, 2},
        {4, 25, 1},
        {5, 15, 3}
    };

    int profit = max_profit_job_sequence(jobs);
    assert(profit == 142);
}

/* ---------------- Rope Merging ---------------- */

void test_rope_merging() {
    std::vector<int> ropes = {4, 3, 2, 6};
    long long cost = min_cost_to_merge_ropes(ropes);

    assert(cost == 29);
}

/* ---------------- Gas Station ---------------- */

void test_gas_station() {
    std::vector<GasStation> stations = {
        {10, 60},
        {20, 30},
        {30, 30},
        {60, 40}
    };

    int target = 100;
    int startFuel = 10;

    int result = min_refuel_stops(target, startFuel, stations);
    assert(result == 2);
}

/* ---------------- Main Test Runner ---------------- */

int main() {
    test_activity_selection();
    test_job_sequencing();
    test_rope_merging();
    test_gas_station();

    std::cout << "All tests passed successfully.\n";
    return 0;
}

