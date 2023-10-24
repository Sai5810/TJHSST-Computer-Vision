#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <list>
#include <functional>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>
#include <string>
#include <set>
#include <stack>

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
        this->x = 0;
        this->y = 0;
    }

    double getX() const {
        return x;
    }

    void setX(double i) {
        x = i;
    }

    double getY() const {
        return y;
    }

    void setY(double i) {
        y = i;
    }

    double distTo(Point b) const {
        return sqrt(pow(this->x - b.getX(), 2) + pow(this->y - b.getY(), 2));
    }

    double magTo(Point b) const {
        return pow(this->x - b.getX(), 2) + pow(this->y - b.getY(), 2);
    }

    string print() const {
        return ("(" + to_string(x) + ", " + to_string(y) + ")");
    }

    bool operator==(const Point &other) const {
        return (this->x == other.getX() && this->y == other.getY());
    }

    bool operator!=(const Point &other) const {
        return (this->x != other.getX() || this->y != other.getY());
    }

    bool operator<(const Point &other) const {
        return this->x < other.getX();
    }

    bool operator>(const Point &other) const {
        return this->x > other.getX();
    }
};

class Image {
private:
    static constexpr int n = 400;
    vector<vector<vector<int>>> file{n, vector<vector<int >>(n, vector<int>(3, 1))};

public:
    Image() = default;

    void plot(int x, int y, int r = 0, int g = 0, int b = 0) {
        if (0 <= x and x < n and 0 <= y and y < n) {
            file[x][y][0] = r;
            file[x][y][1] = g;
            file[x][y][2] = b;
        }
    }

    void drawLine(Point a, Point b) {
        drawLine(a.getX(), a.getY(), b.getX(), b.getY());
    }

    void drawLine(double dx1, double dy1, double dx2, double dy2) {
        int x1 = dx1 * n;
        int x2 = dx2 * n;
        int y1 = dy1 * n;
        int y2 = dy2 * n;
        int dx = abs(x2 - x1);
        int sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1);
        int sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;
        while (x1 != x2 || y1 != y2) {
            plot(x1, y1);
            if (2 * err >= dy) {
                err += dy;
                x1 += sx;
            }
            if (x1 == x2 && y1 == y2) {
                break;
            }
            if (2 * err <= dx) {
                err += dx;
                y1 += sy;
            }
        }
        plot(x1, y1);
    }

    pair<int, int> drawCircle(double dcx, double dcy, int r, int red, int g, int b) {
        return drawCircle(dcx, dcy, double(r) / n, red, g, b);
    }

    pair<int, int> drawCircle(double dcx, double dcy, double dr, int r, int g, int b) {
        int cx = dcx * n;
        int cy = dcy * n;
        int rad = dr * n;
        int xmax = (int) ((rad + 2) * 0.70710678);
        int y = rad;
        int yt = y * y;
        int ty = (2 * y) - 1;
        int y2_new = yt;
        for (int x = 0; x <= xmax; x++) {
            if ((yt - y2_new) >= ty) {
                yt -= ty;
                y -= 1;
                ty -= 2;
            }
            plot(cx + x, cy + y, r, g, b);
            plot(cx + x, cy - y, r, g, b);
            plot(cx - x, cy + y, r, g, b);
            plot(cx - x, cy - y, r, g, b);
            plot(cx + y, cy + x, r, g, b);
            plot(cx + y, cy - x, r, g, b);
            plot(cx - y, cy + x, r, g, b);
            plot(cx - y, cy - x, r, g, b);
            y2_new -= (2 * x) - 3;
        }
        return {cx, cy};
    }


    void drawPoint(Point p, int ptR = 2, int r = 0, int g = 0, int b = 0) {
        drawCircle(p.getX(), p.getY(), ptR, r, g, b);
    }

    void print(const string &outfile) {
        fstream fileO;
        fileO.open(outfile + ".ppm", ios::out);
        fileO << "P3 " << n << " " << n << " 1" << "\n";
        for (auto &i: file) {
            for (auto &j: i) {
                for (int k: j) {
                    fileO << k << " ";
                }
            }
            fileO << "\n";
        }
        fileO.close();
    }
};

void genPoints(int n) {
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> distr(0.0, 1.0);
    fstream ptO;
    ptO.open("points.txt", ios::out);
    for (int i = 0; i < n; ++i) {
        ptO << setprecision(24) << distr(gen) << " " << distr(gen) << "\n";
    }
    ptO.close();
}

void process(vector<Point> &pt, Image &img, const string &infile) {
    ifstream ptI;
    ptI.open(infile);
    string line;
    while (getline(ptI, line)) {
        pt.emplace_back(stod(line.substr(0, line.find(' '))), stod(line.substr(line.find(' '))));
    }
    for (auto &it: pt) {
        img.drawPoint(it, 3);
    }
}

void recHull(const vector<Point> &pt, int mn, int mx, set<Point> &hull, Image &img) {
    double mDist = 0;
    int ind = -1;
    for (int i = 0; i < pt.size(); i++) {
        double cDist = (pt[i].getY() - pt[mn].getY()) * (pt[mx].getX() - pt[mn].getX()) -
                       (pt[mx].getY() - pt[mn].getY()) * (pt[i].getX() - pt[mn].getX());
        if (cDist > 0 && cDist > mDist && i != mx && i != mn) {
            ind = i;
            mDist = cDist;
        }
    }
    if (ind == -1) {
        hull.insert(pt[mx]);
        hull.insert(pt[mn]);
        img.drawLine(pt[mx], pt[mn]);
        return;
    }
    recHull(pt, mn, ind, hull, img);
    recHull(pt, ind, mx, hull, img);
}


void part1(const string &infile, const string &outfile) {
    vector<Point> pt;
    Image img;
    process(pt, img, infile);
    int mx = distance(pt.begin(), max_element(pt.begin(), pt.end()));
    int mn = distance(pt.begin(), min_element(pt.begin(), pt.end()));
    set<Point> hull;
    recHull(pt, mx, mn, hull, img);
    recHull(pt, mn, mx, hull, img);
    img.print(outfile);
    cout << "Points of the Hull:\n";
    for (auto i: hull) {
        cout << i.print() << "\n";
    }
    cout << '\n';
}

double cross(Point bot, Point p1, Point p2) {
    return ((p1.getX() - bot.getX()) * (p2.getY() - bot.getY()) -
            (p1.getY() - bot.getY()) * (p2.getX() - bot.getX()));
}

struct SlopeComp {
    Point bot;

    explicit SlopeComp(Point bot) { this->bot = bot; }

    bool operator()(Point const &p1, Point const &p2) const {
        if (p1 == bot){
            return true;
        }
        if (p2 == bot){
            return false;
        }
        double c = cross(bot, p1, p2);
        if (c != 0){
            return c < 0;
        } else {
            return p1.magTo(bot) > p2.magTo(bot);
        }
    }
};

void part2(const string &infile, const string &outfile) {
    vector<Point> pt;
    Image img;
    process(pt, img, infile);
    int idx = 0;
    for (int i = 0; i < pt.size(); ++i) {
        if (pt[idx].getY() < pt[i].getY()) {
            idx = i;
        }
    }
    sort(pt.begin(), pt.end(), SlopeComp(pt[idx]));
    stack<Point> st;
    st.push(pt[0]);
    st.push(pt[1]);
    st.push(pt[2]);
    Point underTop = pt[1];
    for(int i = 3; i < pt.size(); ++i){
        while (st.size() > 1 && cross(st.top(), underTop, pt[i]) <= 0) {
            st.pop();
            Point temp = st.top();
            st.pop();
            underTop = st.top();
            st.push(temp);
        }
        underTop = st.top();
        st.push(pt[i]);
    }
    underTop = st.top();
    Point prev = st.top();
    st.pop();
    while(!st.empty()){
        img.drawLine(prev, st.top());
        prev = st.top();
        st.pop();
    }
    img.drawLine(underTop, prev);
    img.print(outfile);
}

int main() {
    genPoints(60);
    part1("points.txt", "quickhull");
    part2("points.txt", "grahamscan");
}
