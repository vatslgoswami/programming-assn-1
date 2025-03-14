#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>
using namespace std;

double T = 0;

struct Point {
    int x, y;
    bool operator<(const Point& other) const {
        if (x == other.x) return y > other.y;
        T++; //everytime our sort function calls this operator <, we increment T
        return x > other.x; //defining opposite operators so that we can sort in decreasing order
    }
};

class Staircase {
public:
    map<int, vector<int>> steps; //(BBST) mapping each maximal point's y-coord -> all x-values with that y
    int yMax = -1;  //used to track the max y-value for each layer
    Staircase() {}
    void insert(Point p) {
        steps[p.y].push_back(p.x);
        yMax = max(yMax, p.y);
    }
};

class layeredStaircase {
public:
    map<pair<int,int>, int> layers; //map with keys(y_max, -layer) allowing us to correctly sort layers when they have the same y-max: map = RBT = O(log n) finds
    int totalLayers = 0; //keep track of how many layers we have added for when we add se new layers

//function to find which layer a point belongs to
int pointBelongsTo(Point p) {
    auto nextLayer = layers.lower_bound({p.y, INT_MIN}); //find the first layer q s.t. y_maxq >= p.y//binary search on map with x=totalLayers entries + 2 comparision for each if/elseif block
    if (totalLayers > 0) T+= log2(totalLayers)+2; //add comparisons for Binary Search to get lower_bound plus 2 comparisons for each if/else if block
    if (nextLayer == layers.begin()) {//if p is lower than all the layers, add new layer
        totalLayers++;
        return totalLayers;
    } else if (nextLayer == layers.end()) return 1;//if q is above all layers, add it to the topmost layer M_1
    else { //else add it to the highest layer whose y_max<p.y
    auto it = prev(nextLayer);
    int assignedLayer = it->second;
    int yMax = it->first.first;
    return assignedLayer;
    }
    }
};

//function to find maximalLayers of a set of points
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
        l.layers[{p.y, -newLayer}] = newLayer;//output p.y as the new max (which is the correct max since we are sweeping the points left-to-right) 
        T+=2*log2(l.totalLayers) + 2; //logn for insertion/deletion into layers map + 2 for emplace and insert into vector 
    }
    return maximalLayers;
}

//function to store all points from input file into a vector
vector<Point> loadPointsfromFile(string fileName){
    ifstream inputFile(fileName);
    //if we can't find file, return empty vector
    if (!inputFile) {
        cerr << "Unable to open file" << endl;
        vector<Point> points;
        return points;
    }
    //else for each line, store the correct (x,y) as the i^th point
    int n;
    inputFile >> n;
    vector<Point> points(n);
    for (int i = 0; i < n; i++) {
        inputFile >> points[i].x >> points[i].y;
    }
    inputFile.close();
    return points;
}

//function to write the maximal layers into an output file
void writeToOutputFile(string fileName, const vector<Staircase>& maximalLayers) {
    ofstream outFile(fileName);
    //if we can't open the output file, return
    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }
    //else for each layer M_i in our maximalLayers vector
    for (size_t i = 1; i < maximalLayers.size(); i++) {
        //for each point in the staircase/maximal point set of that layer
        for (const auto& point : maximalLayers[i].steps) {
            //we output all the x-coordinates for each y (point.first)
            for (const int x : point.second){
                outFile << x << " " << point.first << endl;
            } 
        }
        if (i != maximalLayers.size()-1) outFile << endl;//formatting
    }
}

int main() {
    vector<Point> points = loadPointsfromFile("input.txt");
    vector<Staircase> maximalLayers = findMaximalLayers(points);
    writeToOutputFile("output.txt", maximalLayers);
    cout << "N: "<< points.size()<<"; T: "<< T << endl;
    return 0;
}
