#ifndef JOB_SEQUENCING_HPP
#define JOB_SEQUENCING_HPP

#include <vector>

/**
 * Represents a job with profit and deadline.
 * Each job takes exactly 1 unit of time.
 */
struct Job {
    int id;        // job identifier
    int profit;    // profit if scheduled
    int deadline;  // latest time slot (1-based)
};

/**
 * Schedules jobs to maximize total profit.
 *
 * @param jobs Input list of jobs
 * @return Maximum achievable profit
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
int max_profit_job_sequence(std::vector<Job> jobs);

#endif // JOB_SEQUENCING_HPP

