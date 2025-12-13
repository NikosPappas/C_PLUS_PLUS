#include "rope_merging.hpp"
#include <queue>

long long min_cost_to_merge_ropes(const std::vector<int>& ropes) {
    if (ropes.size() <= 1)
        return 0;

    // Min-heap
    std::priority_queue<
        int,
        std::vector<int>,
        std::greater<int>
    > minHeap;

    for (int r : ropes)
        minHeap.push(r);

    long long totalCost = 0;

    while (minHeap.size() > 1) {
        int first  = minHeap.top(); minHeap.pop();
        int second = minHeap.top(); minHeap.pop();

        int merged = first + second;
        totalCost += merged;

        minHeap.push(merged);
    }

    return totalCost;
}

