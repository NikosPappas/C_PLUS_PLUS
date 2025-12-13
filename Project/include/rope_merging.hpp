#ifndef ROPE_MERGING_HPP
#define ROPE_MERGING_HPP

#include <vector>

/**
 * Computes the minimum cost to merge all ropes into one.
 *
 * Each merge cost is the sum of the two rope lengths.
 *
 * @param ropes Vector of rope lengths
 * @return Minimum total merge cost
 *
 * Time Complexity: O(n log n)
 * Space Complexity: O(n)
 */
long long min_cost_to_merge_ropes(const std::vector<int>& ropes);

#endif // ROPE_MERGING_HPP

