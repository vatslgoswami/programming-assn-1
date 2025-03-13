#include <iostream>
#include <map>
using namespace std;

struct Point {
    int x, y, z;
    bool operator<(const Point& other) const {
        return x > other.x; //we will use this to sort x coordinates in decreasing order
    }
};

class Staircase {
private:
    map<int, int> steps;
public:
    Staircase() {}
    bool isDominated(int y, int z) {
        auto q = steps.upper_bound(y); //find the first point q such that y_q>y
        //if q exists and its z' > z, then q dominates (y,z)
        return (q != steps.end() && q->second > z);
    }

    void insert(int y, int z) {
        if (isDominated(y, z)) return; //don't insert into staircase if dominated
        auto q = steps.lower_bound(y); //finds the point q whose y_q>=y
        while (q != steps.begin()) {
            auto before = prev(q); //get the point in staircase beside q_y
            if (before->second < z) {
                steps.erase(before);
            } else {
                break; //break when we find a non-dominated point
            }
        }
        steps[y] = z;
    }
};

vector<Point> findMaximalPoints(vector<Point>& points) {
    sort(points.begin(), points.end()); //sort x in decreasing order using our < operator in Point struct 
    Staircase staircase;
    vector<Point> maximalPoints;

    for (const auto& p : points) {
        if (!staircase.isDominated(p.y, p.z)) {
            maximalPoints.push_back(p);
            staircase.insert(p.y, p.z);
        }
    }
    return maximalPoints;
}

int main() {
    vector<Point> points = {
        {3, 7, 4}, {8, 2, 6}, {5, 9, 3}, {10, 6, 7},
        {2, 4, 5}, {7, 1, 8}, {6, 10, 2}, {9, 5, 9}, {2,2,2}, {4,8,10}
    };

    vector<Point> maximal = findMaximalPoints(points);

    cout << "Maximal Points:\n";
    for (const auto& p : maximal) {
        cout << "(" << p.x << ", " << p.y << ", " << p.z << ")\n";
    }

    return 0;
}