#ifndef ACTIVITY_SELECTION_HPP
#define ACTIVITY_SELECTION_HPP
#include <vector>
struct Activity{
	int start;
	int finish;
};
std::vector<Activity> select_max_activities(std::vector<Activity> activities);
#endif
