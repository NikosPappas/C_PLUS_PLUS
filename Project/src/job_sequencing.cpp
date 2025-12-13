#include "job_sequencing.hpp"
#include <algorithm>
#include <vector>

int max_profit_job_sequence(std::vector<Job> jobs) {
    if (jobs.empty())
        return 0;

    // Sort jobs by descending profit
    std::sort(jobs.begin(), jobs.end(),
              [](const Job& a, const Job& b) {
                  return a.profit > b.profit;
              });

    // Find maximum deadline
    int maxDeadline = 0;
    for (const auto& job : jobs)
        maxDeadline = std::max(maxDeadline, job.deadline);

    // Slot array: index 1..maxDeadline
    std::vector<bool> slot(maxDeadline + 1, false);

    int totalProfit = 0;

    for (const auto& job : jobs) {
        // Try to schedule job at latest available slot
        for (int t = job.deadline; t > 0; --t) {
            if (!slot[t]) {
                slot[t] = true;
                totalProfit += job.profit;
                break;
            }
        }
    }

    return totalProfit;
}

