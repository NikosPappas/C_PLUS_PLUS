#include "activity_selection.hpp"
#include <algorithm>

std::vector<Activity>
select_max_activities(std::vector<Activity> activities) {
    if (activities.empty())
        return {};

    // Sort activities by finish time
    std::sort(activities.begin(), activities.end(),
              [](const Activity& a, const Activity& b) {
                  return a.finish < b.finish;
              });

    std::vector<Activity> selected;
    selected.push_back(activities[0]);

    int lastFinish = activities[0].finish;

    for (size_t i = 1; i < activities.size(); ++i) {
        if (activities[i].start >= lastFinish) {
            selected.push_back(activities[i]);
            lastFinish = activities[i].finish;
        }
    }

    return selected;
}

