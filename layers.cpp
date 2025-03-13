#include <iostream>
#include <map>
#include<vector>
using namespace std;

int T = 0;
struct Point {
    int x, y;
    bool operator<(const Point& other) const {
        return x > other.x; //we will use this to sort x coordinates in decreasing order
    }
};

class Staircase {
public:
    map<int, int> steps;
    int yMax;
    Staircase() {
        this->yMax = -1;
    }
    void insert(Point p) {
        steps[p.y] = p.x;
        yMax = p.y;
    }
};

class layeredStaircase{
public:
    map<int, int> layers;

    int pointBelongsTo(Point p) {
        if (layers.empty()) return 0; 
        auto q = layers.lower_bound(p.y); //finds the point q st y_q> y
        if (q == layers.begin()){
            return layers.begin()->second + 1; //it is the smallest and thus should be in a layer of its own
        } else return prev(q)->second;
    }
};

vector<Staircase> findMaximalLayers(vector<Point>& points) {
    sort(points.begin(), points.end()); //sort x in decreasing order using our < operator in Point struct 
    layeredStaircase l;
    vector<Staircase> maximalLayers;

    for (const auto& p : points) {
        int M_p = l.pointBelongsTo(p);
        if (M_p >= maximalLayers.size()) {
            maximalLayers.emplace_back();
        }
        maximalLayers[M_p].insert(p);
        if (l.layers.find(M_p) != l.layers.end()) l.layers.erase(M_p);
        l.layers[p.y] = M_p;
    }
    return maximalLayers;
}

int main() {
    vector<Point> points = {
        {1,4}, {9,5}, {3,14}, {6,10}, {7,4}, {2,18}, {4,3}
    };

    vector<Staircase> maximal = findMaximalLayers(points);

    cout << "Maximal Points:\n";
    for (const auto& stair : maximal) {
        for (const auto& p : stair.steps){
        cout << "(" << p.second << ", " << p.first << ")\n";
        }
        cout << endl;
    }

    return 0;
}