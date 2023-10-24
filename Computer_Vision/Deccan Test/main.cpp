// Deccan Maniam
// Period 4
// 10/29/21

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <list>
#include <iterator>
#include <algorithm>
#include <time.h>
using namespace std;

class Point {
private:
    double x, y;

public:
    Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

    Point() {
        x = 0;
        y = 0;
    }

    void setX(double x) {
        this->x = x;
    }

    void setY(double y) {
        this->y = y;
    }

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }

    double find_distance(Point B) {
        return pow(this->x-B.getX(), 2) + pow(this->y-B.getY(), 2);
    }
};

class Distance {
private:
    double d;
    vector<Point> p;

public:
    Distance(double d, vector<Point> &p) {
        this->d = d;
        this->p = p;
    }

    double getD() {
        return d;
    }

    vector<Point> getP() {
        return p;
    }
};

// GLOBAL VARIABLES
vector<vector<vector<int>>> grid; // used for ppm but not for this lab
int POINTS = 100000; // controls the number of points the algorithm runs for
vector<Point> p2, p3;
double d2, d3, t2, t3;

// DRAWING METHOD (unused for this lab)
void initialize() {
    for (int i = 0; i < 800; i++) {
        vector<vector<int>> row;
        for (int j = 0; j < 800; j++) {
            row.push_back({1, 1, 1});
        }
        grid.push_back(row);
    }
}

void draw(int i, int j, vector<int> rgb) {
    if (i < 800 && i >= 0 && j < 800 && j >= 0) {
        grid[i][j] = rgb;
    }
}

void circle(int r, Point p1, vector<int> rgb) {
    int y_c = (int) (p1.getX()*800);
    int x_c = (int) (p1.getY()*800);
    int xmax = (int) (r/sqrt(2));

    int y = r;
    int y2 = y * y;
    int ty = (2 * y) - 1;
    int y2_new = y2;

    for (int x=0; x<=xmax; x++) {
        if ((y2-y2_new) >= ty) {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        draw(x_c+x, y_c+y, rgb);
        draw(x_c+x, y_c-y, rgb);
        draw(x_c-x, y_c+y, rgb);
        draw(x_c-x, y_c-y, rgb);
        draw(x_c+y, y_c+x, rgb);
        draw(x_c+y, y_c-x, rgb);
        draw(x_c-y, y_c+x, rgb);
        draw(x_c-y, y_c-x, rgb);

        // correction to method to finish circle
        y2_new -= (2 * x) + 1;
    }
}

void generate_points(ofstream &output, list<Point> &allpoints) { // unused for this lab
    srand((unsigned) time(NULL));
    for(int i=0; i<POINTS; i++) {
        Point p((double) rand()/RAND_MAX, (double) rand()/RAND_MAX);
        allpoints.push_back(p);
        // circle(3, p, {0, 0, 0});
        output << p.getX() << "  " << p.getY() << endl;
    }
}

void generate_points(ofstream &output, vector<Point> &allpoints) {
    srand((unsigned) time(NULL));
    for(int i=0; i<POINTS; i++) {
        Point p((double) rand()/RAND_MAX, (double) rand()/RAND_MAX);
        allpoints.push_back(p);
        output << p.getX() << "  " << p.getY() << endl;
    }
}

// PART 1
vector<Point> find_closest_pair(list<Point> &allpoints) {
    double min = 2;
    vector<Point> closest_pair;
    for (list<Point>::iterator it1 = allpoints.begin(); it1 != allpoints.end(); ++it1){
        for (list<Point>::iterator it2 = allpoints.begin(); it2 != it1; ++it2){
            double dist = (*it1).find_distance((*it2));
            if(dist < min) {
                min = dist;
                closest_pair = {(*it1), (*it2)};
            }
        }
    }
    return closest_pair;
}

void part1() { // commented out lines are for drawing, from last lab
    // ofstream points("points.ppm");
    // points << "P3 800 800 1" << endl;
    ofstream output("points.txt");
    output << fixed;
    output << setprecision(17);

    list<Point> allpoints;
    generate_points(output, allpoints);

    clock_t time = clock(); // measures time in "clock ticks"
    vector<Point> closest_pair = find_closest_pair(allpoints);
    double d = closest_pair[0].find_distance(closest_pair[1]);
    time = clock() - time;

    // for(int i=0; i<closest_pair.size(); i++) {
    //   circle(2, closest_pair[i], {1, 0, 0});
    //   circle(3, closest_pair[i], {1, 0, 0});
    // }

    // for (int i = 0; i < 800; i++) {
    //       for (int j = 0; j < 800; j++) {
    //         points << grid[i][j][0] << " " << grid[i][j][1] << " " << grid[i][j][2] << " ";
    //     }
    //     points << endl;
    // }

    // p1 = closest_pair;
    // d1 = d;
    // t1 = 1000*((double) time/CLOCKS_PER_SEC); // converts clock ticks to milliseconds
}

// PART 2
Distance min_dist(vector<Point> &allpoints, int start, int end) {
    vector<Point> pair;
    double d;
    int size = end-start+1;

    if(size == 2) {
        pair = {allpoints[start], allpoints[end]};
        d = allpoints[start].find_distance(allpoints[end]);
    } else if(size == 3) {
        double a = allpoints[start].find_distance(allpoints[end]);
        double b = allpoints[start].find_distance(allpoints[start+1]);
        double c = allpoints[start+1].find_distance(allpoints[end]);

        if(a <= b && a <= c) {
            pair = {allpoints[start], allpoints[end]};
            d = a;
        } else if(b <= a && b <= c) {
            pair = {allpoints[start], allpoints[start+1]};
            d = b;
        } else {
            pair = {allpoints[start+1], allpoints[end]};
            d = c;
        }
    } else if(size > 3) {
        int mid = start + size/2;
        Distance l1 = min_dist(allpoints, start, mid-1);
        Distance l2 = min_dist(allpoints, mid, end);
        double d1 = l1.getD();
        double d2 = l2.getD();
        vector<Point> p1 = l1.getP();
        vector<Point> p2 = l2.getP();

        if(d1 < d2) {
            d = d1;
            pair = p1;
        } else {
            d = d2;
            pair = p2;
        }

        double m = (allpoints[mid].getX() + allpoints[mid-1].getX())/2;
        double dl = m - sqrt(d);
        double dr = m + sqrt(d);
        int l_index = start;
        int r_index = end;

        for(int i=mid-1; i>=start; i--) {
            if(allpoints[i].getX() < dl) {
                break;
            }
            l_index = i;
        }
        for(int i=mid; i<=end; i++) {
            if(allpoints[i].getX() > dr) {
                break;
            }
            r_index = i;
        }

        for(int i=l_index; i<mid; i++) {
            for(int j=mid; j<=r_index; j++) {
                double dist = allpoints[i].find_distance(allpoints[j]);
                if(dist < d) {
                    pair = {allpoints[i], allpoints[j]};
                    d = dist;
                }
            }
        }
    }

    return Distance(d, pair);
}

bool comp1(Point a, Point b) {
    if (a.getX() <= b.getX()) {
        return true;
    }
    return false;
}

void part2() {
    ifstream input("points100k.txt");

    vector<Point> allpoints;
    for(int i=0; i<POINTS; i++) {
        double x, y;
        input >> x >> y;
        allpoints.push_back(Point(x, y));
    }

    // ofstream output("points.txt");
    // output << fixed;
    // output << setprecision(17);
    // generate_points(output, allpoints);

    clock_t time = clock();
    sort(allpoints.begin(), allpoints.end(), comp1);
    Distance closest_pair = min_dist(allpoints, 0, allpoints.size()-1);
    time = clock() - time;

    p2 = closest_pair.getP();
    d2 = sqrt(closest_pair.getD());
    t2 = 1000*((double) time/CLOCKS_PER_SEC);
}

// PART 3
bool comp2(Point a, Point b) {
    if (a.getY() <= b.getY()) {
        return true;
    }
    return false;
}

Distance min_dist_2(vector<Point> &allpoints, int start, int end) {
    vector<Point> pair;
    double d;
    int size = end-start+1;

    if(size == 2) {
        pair = {allpoints[start], allpoints[end]};
        d = allpoints[start].find_distance(allpoints[end]);
    } else if(size == 3) {
        double a = allpoints[start].find_distance(allpoints[end]);
        double b = allpoints[start].find_distance(allpoints[start+1]);
        double c = allpoints[start+1].find_distance(allpoints[end]);

        if(a <= b && a <= c) {
            pair = {allpoints[start], allpoints[end]};
            d = a;
        } else if(b <= a && b <= c) {
            pair = {allpoints[start], allpoints[start+1]};
            d = b;
        } else {
            pair = {allpoints[start+1], allpoints[end]};
            d = c;
        }
    } else if(size > 3) {
        int mid = start + size/2;
        Distance l1 = min_dist_2(allpoints, start, mid-1);
        Distance l2 = min_dist_2(allpoints, mid, end);
        double d1 = l1.getD();
        double d2 = l2.getD();
        vector<Point> p1 = l1.getP();
        vector<Point> p2 = l2.getP();

        if(d1 < d2) {
            d = d1;
            pair = p1;
        } else {
            d = d2;
            pair = p2;
        }

        double m = (allpoints[mid].getX() + allpoints[mid-1].getX())/2;
        double dl = m - sqrt(d);
        double dr = m + sqrt(d);
        int l_index = start;
        int r_index = end;

        for(int i=mid-1; i>=start; i--) {
            if(allpoints[i].getX() < dl) {
                break;
            }
            l_index = i;
        }
        for(int i=mid; i<=end; i++) {
            if(allpoints[i].getX() > dr) {
                break;
            }
            r_index = i;
        }

        vector<Point> strip;
        for(int i=l_index; i<=r_index; i++) {
            strip.push_back(allpoints[i]);
        }

        sort(strip.begin(), strip.end(), comp2);
        for(int i=0; i<strip.size(); i++) {
            for(int j=1; j<=15; j++) {
                if(i+j < strip.size()) {
                    double dist = strip[i].find_distance(strip[i+j]);
                    if(dist < d) {
                        pair = {strip[i], strip[i+j]};
                        d = dist;
                    }
                }
            }
        }
    }

    return Distance(d, pair);
}

void part3() {
    ifstream input("points100k.txt");

    vector<Point> allpoints;
    for(int i=0; i<POINTS; i++) {
        double x, y;
        input >> x >> y;
        allpoints.push_back(Point(x, y));
    }

    // ofstream output("points.txt");
    // output << fixed;
    // output << setprecision(17);
    // generate_points(output, allpoints);

    clock_t time = clock();
    sort(allpoints.begin(), allpoints.end(), comp1);
    Distance closest_pair = min_dist_2(allpoints, 0, allpoints.size()-1);
    time = clock() - time;

    p3 = closest_pair.getP();
    d3 = sqrt(closest_pair.getD());
    t3 = 1000*((double) time/CLOCKS_PER_SEC);
}

int main() {
    ofstream results("results.txt");
    results << setprecision(17);
    cout << setprecision(17);

    part1();

    part2();
    cout << "Approach 2: " << endl << "Points: (" << p2[0].getX() << ", " << p2[0].getY() << ")" << ", " << "(" << p2[1].getX() << ", " << p2[1].getY() << ")" << endl << "Distance of " << d2 << endl << "In " << t2 << " milliseconds" << endl << endl;
    results << "Approach 1: " << endl << "Points: (" << p2[0].getX() << ", " << p2[0].getY() << ")" << ", " << "(" << p2[1].getX() << ", " << p2[1].getY() << ")" << endl << "Distance of " << d2 << endl << "In " << t2 << " milliseconds" << endl << endl;

    part3();
    cout << "Approach 3: " << endl << "Points: (" << p3[0].getX() << ", " << p3[0].getY() << ")" << ", " << "(" << p3[1].getX() << ", " << p3[1].getY() << ")" << endl << "Distance of " << d3 << endl << "In " << t3 << " milliseconds" << endl;
    results << "Approach 2: " << endl << "Points: (" << p3[0].getX() << ", " << p3[0].getY() << ")" << ", " << "(" << p3[1].getX() << ", " << p3[1].getY() << ")" << endl << "Distance of " << d3 << endl << "In " << t3 << " milliseconds";

    // this code printed the data for the graph to a file called graph.csv, to be imported into google sheets for a graph to be made
    // ofstream graph("graph.csv");
    // graph << setprecision(17);
    // graph << "# of points,Brute force (ms),Recursive approach (ms)" << endl;
    // for(int i=2; i<=1000; i++) {
    //   cout << i << endl;
    //   POINTS = i;
    //   double s2 = 0;
    //   double s3 = 0;
    //   for(int j=0; j<5; j++) {
    //       part2();
    //       part3();
    //       s2 += t2;
    //       s3 += t3;
    //   }
    //   graph << i << "," << s2/5 << "," << s3/5 << endl;
    // }
}