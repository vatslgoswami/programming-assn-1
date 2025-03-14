#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

int T = 0;

struct Point {
    int x, y;
    bool operator<(const Point& other) const {
        if (x == other.x) return y > other.y;
        return x > other.x; //defining opposite operators so that we can sort in deccreasing order
    }
};

class Staircase {
public:
    map<int, vector<int>> steps; //map (BBST) with each maximal points y-coord -> x
    int yMax = -1;  //used to track the max y-value for each layer
    Staircase() {}
    void insert(Point p) {
        steps[p.y].push_back(p.x);
        yMax = max(yMax, p.y);
    }
};

class layeredStaircase {
public:
    map<pair<int,int>, int> layers;  //map storing each layers current y-Max -> layer number
    int totalLayers = 0;  //keep track of how many layers we have added to help increment new layers

int pointBelongsTo(Point p) {
        if (p.x == 60 && p.y == 110) {
            int i = 0;
            for (const auto& l : layers){
                i++;
                cout<< "Position "<< i <<": "<< "("<<l.first.first<<","<<l.second<<")"<< endl;
            }
        }
        auto nextLayer = layers.lower_bound({p.y, INT_MIN}); // find the first layer q s.t. y_maxq >= p.y
        if (nextLayer == layers.begin()) {
            totalLayers++;
            cout << "Creating new layer " << totalLayers << " for point: (" << p.x << ", " << p.y << ")" << endl;
            return totalLayers; //new layer needed
        }
        if (nextLayer == layers.end()) return 1;
        auto it = prev(nextLayer);
        int assignedLayer = it->second;
        int yMax = it->first.first;
        cout << "Upper layer:" << nextLayer->second << ", yMax: " << nextLayer->first.first << endl;
        cout << "Point (" << p.x << ", " << p.y << ") assigned to layer " << assignedLayer << endl;
        return assignedLayer;
    }
};

vector<Staircase> findMaximalLayers(vector<Point>& points) {
    sort(points.begin(), points.end());//sort x in descending order
    layeredStaircase l; //create instance of layerStaircase class to track each layer in our points
    vector<Staircase> maximalLayers; //keep a output vector where vector[i] stores the maximal layer M_i
    maximalLayers.emplace_back(); //append empty staircase at 0 since we start from M_1

    for (const auto& p : points) {//traverse the sorted points left to right
        int newLayer = l.pointBelongsTo(p); //find the layer current point belongs to
        if (newLayer >= maximalLayers.size()) {//if we have created a new layer, add space to output vector 
            maximalLayers.emplace_back();
        }
        l.layers.erase({maximalLayers[newLayer].yMax, -newLayer});//remove the old y_max entry for the layer we have just modified
        maximalLayers[newLayer].insert(p);//insert p into the staircase it belongs to
        l.layers[{p.y, -newLayer}] = newLayer;//output p.y as the new max (it must be since we are sweeping the points left-to-right) 
    }
    return maximalLayers;
}

int main() {
    ifstream inputFile("input2.txt");
    if (!inputFile) {
        cerr << "Unable to open file" << endl;
        return 0;
    }

    int n;
    inputFile >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        inputFile >> points[i].x >> points[i].y;
    }
    inputFile.close();

    vector<Staircase> maximalLayers = findMaximalLayers(points);

    cout << "Maximal Points:\n";
    for (size_t i = 1; i < maximalLayers.size(); i++) {
        cout << "Layer " << i << ":\n";
        for (const auto& step : maximalLayers[i].steps) {
            for (const int x : step.second){
                cout << "(" << x << ", " << step.first << ")\n";
            } 
        }
        cout << endl;
    }

    return 0;
}
