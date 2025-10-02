#include <complex>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
class Point {
private:
    double x;
    double y;
public:
    Point():x(0.0),y(0.0){};
    Point(double x,double y):x(x),y(y){};
    void set_x(double x) {
        this->x=x;
    }
    void set_y(double y) {
        this->y=y;
    }
    double get_x() const {
        return x;
    }
    double get_y() const {
        return y;
    }
};
class ClosestPairs {
private:
    const vector<Point> *points;
public:
    ClosestPairs():points(nullptr){};
    ClosestPairs(const std::vector<Point> &vector):points(&vector){};
    vector<Point> find_closest() {
        double min_distance=std::numeric_limits<double>::infinity();
        std::vector<Point> best_pair;
        double d;
        int size=points->size();
        for (int i=0;i<size;i++) {
            for (int j=i+1;j<size;j++) {
                d=distance((*points)[i],(*points)[j]);
                if (d<min_distance) {
                    min_distance=d;
                    best_pair = { (*points)[i], (*points)[j] };
                }
            }

        }
        return best_pair;
    }
private:
    double distance(const Point &p1, const Point &p2) {
        double d_x=p2.get_x()-p1.get_x();
        double d_y=p2.get_y()-p1.get_y();
        return std::sqrt( std::pow(d_x,2.00)+std::pow(d_y,2.00));
    }
};
int main() {
    vector<Point> pts = {
        Point(0, 0),
        Point(1, 1),
        Point(2, 2),
        Point(5, 5)
    };
    ClosestPairs cpl(pts);
    vector pair=cpl.find_closest();
    std::cout<<"Closest Pair: "<<std::endl;
    std::cout<<"("<<pair[0].get_x()<<", "<<pair[0].get_y()<<")"<<std::endl;
    std::cout<<"("<<pair[1].get_x()<<"."<<pair[1].get_y()<<")"<<std::endl;

    return 0;
}
