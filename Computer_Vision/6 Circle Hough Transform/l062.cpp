#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>
#include <chrono>

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
    int col;
    int row;
public:
    vector<vector<vector<int>>> vec;
    int color;

    //non max suppression
    explicit Image(vector<vector<double>> ang, vector<vector<vector<int>>> gray) {
        col = (int) ang[0].size();
        row = (int) ang.size();
        vec = vector<vector<vector<int>>>(row, vector<vector<int>>(col, vector<int>(3, 0)));
        color = 1;
        for (int i = 1; i < row - 1; ++i) {
            for (int j = 1; j < col - 1; ++j) {
                if (ang[i][j] < 22.5 || ang[i][j] > 157.5) {
                    if (gray[i][j][0] > gray[i][j + 1][0] && gray[i][j][0] > gray[i][j - 1][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                } else if (ang[i][j] >= 22.5 && ang[i][j] <= 67.5) {
                    if (gray[i][j][0] > gray[i + 1][j - 1][0] && gray[i][j][0] > gray[i - 1][j + 1][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                } else if (ang[i][j] > 67.5 && ang[i][j] <= 112.5) {
                    if (gray[i][j][0] > gray[i + 1][j][0] && gray[i][j][0] > gray[i - 1][j][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                } else {
                    if (gray[i][j][0] > gray[i + 1][j + 1][0] && gray[i][j][0] > gray[i - 1][j - 1][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                }
            }
        }
    }

    explicit Image(int col, int row, int color, vector<vector<vector<int>>> file) {
        this->col = col;
        this->row = row;
        this->color = color;
        this->vec = move(file);
    }

    explicit Image(int col = 400, int row = 400, int color = 255, int initColor = 1) {
        this->col = col;
        this->row = row;
        this->color = color;
        vec = vector<vector<vector<int>>>(row, vector<vector<int>>(col, vector<int>(3, initColor)));
    }

    explicit Image(const string &inFile) {
        ifstream inStream(inFile, ios::in);
        string line;
        getline(inStream, line);
        getline(inStream, line, ' ');
        col = stoi(line);
        getline(inStream, line);
        row = stoi(line);
        getline(inStream, line);
        color = stoi(line);
        vec = vector<vector<vector<int>>>(row, vector<vector<int>>(col, vector<int>(3, 1)));
        int i = 0;
        int j = 0;
        int k = 0;
        while (getline(inStream, line)) {
            stringstream iss(line);
            int val;
            while (iss >> val) {
                vec[i][j][k] = val;
                ++k;
                if (k == 3) {
                    k = 0;
                    ++j;
                }
                if (j == col) {
                    j = 0;
                    ++i;
                }
            }
        }
    }

    void plot(int x, int y, int r = 0, int g = 0, int b = 0, bool accum = false) {
        if (0 <= x and x < row and 0 <= y and y < col) {
            if (accum) {
                ++vec[x][y][0];
                ++vec[x][y][1];
                ++vec[x][y][2];
            } else {
                vec[x][y][0] = r;
                vec[x][y][1] = g;
                vec[x][y][2] = b;
            }
        }
    }

    void drawLine(Point a, Point b) {
        drawLine(a.getX(), a.getY(), b.getX(), b.getY());
    }

    void drawLine(double dx1, double dy1, double dx2, double dy2) {
        int x1 = (int) (dx1 * row);
        int x2 = (int) (dx2 * row);
        int y1 = (int) (dy1 * col);
        int y2 = (int) (dy2 * col);
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

    void accumLine(double dx1, double dy1, double dx2, double dy2) {
        int x1 = (int) dx1;
        int x2 = (int) dx2;
        int y1 = (int) dy1;
        int y2 = (int) dy2;
        int dx = abs(x2 - x1);
        int sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1);
        int sy = y1 < y2 ? 1 : -1;
        int err = dx + dy;
        while ((x1 != x2 || y1 != y2) && !((x1 < 0 || x1 > row) && (y1 < 0 || y1 > col))) {
            plot(x1, y1, 0, 0, 0, true);
            if (2 * err >= dy) {
                err += dy;
                x1 += sx;
            }
            if ((x1 == x2 && y1 == y2) || ((x1 < 0 || x1 > row) && (y1 < 0 || y1 > col))) {
                break;
            }
            if (2 * err <= dx) {
                err += dx;
                y1 += sy;
            }
        }
        plot(x1, y1, 0, 0, 0, true);
    }

    pair<int, int> drawCircle(double dcx, double dcy, int r, int red, int g, int b) {
        return drawCircle(dcx, dcy, double(r) / row, red, g, b);
    }

    pair<int, int> drawCircle(double dcx, double dcy, double dr, int r, int g, int b) {
        int cx = (int) (dcx * row);
        int cy = (int) (dcy * col);
        int rad = (int) (dr * row);
        int xMax = (int) ((rad + 2) * 0.70710678);
        int y = rad;
        int yt = y * y;
        int ty = (2 * y) - 1;
        int y2_new = yt;
        for (int x = 0; x <= xMax; x++) {
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

    pair<int, int> drawCircle(int cx, int cy, int rad, int r, int g, int b) {
        int xMax = (int) ((rad + 2) * 0.70710678);
        int y = rad;
        int yt = y * y;
        int ty = (2 * y) - 1;
        int y2n = yt;
        for (int x = 0; x <= xMax; x++) {
            if ((yt - y2n) >= ty) {
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
            y2n -= (2 * x) - 3;
        }
        return {cx, cy};
    }

    void drawPoint(Point p, int ptR = 2, int r = 0, int g = 0, int b = 0) {
        drawCircle(p.getX(), p.getY(), ptR, r, g, b);
    }

    void grayscale() {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                fill(vec[i][j].begin(), vec[i][j].end(),
                     .2126 * vec[i][j][0] + .7152 * vec[i][j][1] + .0722 * vec[i][j][2]);
            }
        }
    }

    tuple<Image, vector<vector<double>>> sobel() {
        vector<vector<vector<int>>> magGrad(row, vector<vector<int>>(col, vector<int>(3, 0)));
        vector<vector<double>> angGrad(row, vector<double>(col, 0));
        int mx = 0;
        for (int r = 1; r <= row - 2; ++r) {
            for (int c = 1; c <= col - 2; ++c) {
                int gx = vec[r - 1][c + 1][0] + 2 * vec[r][c + 1][0] + vec[r + 1][c + 1][0] - vec[r - 1][c - 1][0] -
                         2 * vec[r][c - 1][0] - vec[r + 1][c - 1][0];
                int gy = vec[r - 1][c - 1][0] + 2 * vec[r - 1][c][0] + vec[r - 1][c + 1][0] - vec[r + 1][c - 1][0] -
                         2 * vec[r + 1][c][0] - vec[r + 1][c + 1][0];
                int mag = (int) hypot(gx, gy);
                mx = max(mx, mag);
                fill(magGrad[r][c].begin(), magGrad[r][c].end(), mag);
                angGrad[r][c] = atan2(gy, gx) * 180 / 3.14159265;
                if (angGrad[r][c] < 0) {
                    angGrad[r][c] += 180;
                }
            }
        }
        return {Image(col, row, mx, magGrad), angGrad};
    }

    void threshold(int threshold) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (vec[i][j][0] >= threshold) {
                    fill(vec[i][j].begin(), vec[i][j].end(), 1);
                } else {
                    fill(vec[i][j].begin(), vec[i][j].end(), 0);
                }
            }
        }
        color = 1;
    }

    void doubleThreshold(double low, double high) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (vec[i][j][0] >= high) {
                    fill(vec[i][j].begin(), vec[i][j].end(), 2);
                } else if (vec[i][j][0] >= low) {
                    fill(vec[i][j].begin(), vec[i][j].end(), 1);
                } else {
                    fill(vec[i][j].begin(), vec[i][j].end(), 0);
                }
            }
        }
        color = 2;
    }

    void hysFill(vector<vector<vector<int>>> &end, int i, int j) {
        if (end[i][j][0] == 0 && vec[i][j][0] > 0) {
            fill(end[i][j].begin(), end[i][j].end(), 1);
            if (i - 1 >= 0) {
                hysFill(end, i - 1, j);
            }
            if (j - 1 >= 0) {
                hysFill(end, i, j - 1);
            }
            if (i < row) {
                hysFill(end, i + 1, j);
            }
            if (j < col) {
                hysFill(end, i, j + 1);
            }
        }
    }

    void hysteresis() {
        vector<vector<vector<int>>> end(row, vector<vector<int>>(col, vector<int>(3, 0)));
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (vec[i][j][0] == 2) {
                    hysFill(end, i, j);
                }
            }
        }
        vec = end;
        color = 1;
    }


    void combine(const Image &ang) {
        vector<vector<vector<int>>> aFile = ang.getVec();
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (vec[i][j][0] == 1 && aFile[i][j][0] == 1) {
                    fill(vec[i][j].begin(), vec[i][j].end(), 1);
                } else {
                    fill(vec[i][j].begin(), vec[i][j].end(), 0);
                }
            }
        }
    }

    tuple<Image, vector<Point>> findCenter(vector<vector<double>> &ang, Image &edge, int threshold, int rMax) {
        Image vote = Image(col, row, 255, 0);
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if (edge.vec[i][j][0] == 1) {
                    double slope = tan(ang[i][j] * 3.14159265 / 180.0);
                    if (j - 80 < 0) {
                        vote.accumLine(-slope * (0 - j) + i, 0, -slope * (col - j) + i, col);
                    } else {
                        vote.accumLine(-slope * (-80) + i, j - 80, -slope * (80) + i, j + 80);
                    }
                }
            }
        }
        vector<Point> centers;
        for (int i = 0; i < row; i += 1) {
            for (int j = 0; j < col; j += 1) {
                if (vote.vec[i][j][0] > threshold) {
                    centers.emplace_back(i, j);
//                    Point pot = Point(i, j);
//                    bool flag = false;
//                    for (auto &&c: centers) {
//                        if (c.distTo(pot) < rMax) {
//                            if (vote.vec[c.getX()][c.getY()][0] < vote.vec[pot.getX()][pot.getY()][0]) {
//                                c = Point(i, j);
//                            }
//                            flag = true;
//                            break;
//                        }
//                    }
//                    if (!flag) {
//                        centers.emplace_back(i, j);
//                    }
                }
            }
        }
        for (auto &&c: centers) {
            drawCircle((int) c.getX(), (int) c.getY(), 3, 255, 0, 0);
        }
        return {vote, centers};
    }

    double findCircle(const vector<Point> &centers, Image edge, int rMin, int rMax, double thresh) {
        double cost = 0;
        vector<tuple<int, int, int, double>> circles;
        for (Point cen: centers) {
            int maxR = -1;
            double maxScore = -1;
            int i = (int) cen.getX();
            int j = (int) cen.getY();
            for (int k = rMin; k < rMax; ++k) {
                int rad = k;
                int xMax = (int) ((rad + 2) * 0.70710678);
                int y = rad;
                int yt = y * y;
                int ty = (2 * y) - 1;
                int y2n = yt;
                int matchCtr = 0;
                int totCtr = 0;
                for (int x = 0; x <= xMax; x++) {
                    if ((yt - y2n) >= ty) {
                        yt -= ty;
                        y -= 1;
                        ty -= 2;
                    }
                    vector<tuple<int, int>> circ = {{x,  y},
                                                    {x,  -y},
                                                    {-x, y},
                                                    {-x, -y}};
                    for (tuple<int, int> cur: circ) {
                        if (0 <= i + get<0>(cur) and i + get<0>(cur) < row and
                            0 <= j + get<1>(cur) and j + get<1>(cur) < col) {
                            ++totCtr;
                            if (edge.vec[i + get<0>(cur)][j + get<1>(cur)][0] == 1) {
                                ++matchCtr;
                            }
                        }
                        if (0 <= i + get<1>(cur) and i + get<1>(cur) < row and
                            0 <= j + get<0>(cur) and j + get<0>(cur) < col) {
                            ++totCtr;
                            if (edge.vec[i + get<1>(cur)][j + get<0>(cur)][0] == 1) {
                                ++matchCtr;
                            }
                        }
                    }
                    y2n -= (2 * x) - 3;
                }
                double score = (double) matchCtr / totCtr;
                if (score > max(maxScore, thresh)) {
                    maxScore = score;
                    maxR = k;
                }
            }
            if (maxScore != -1) {
                bool flag = false;
                for (auto &&c: circles) {
                    if (Point(get<0>(c), get<1>(c)).distTo(Point(i, j)) < rMax) {
                        if (get<3>(c) < maxScore) {
                            c = {i, j, maxR, maxScore};
                        }
                        flag = true;
                        break;
                    }
                }
                if (!flag) {
                    circles.emplace_back(i, j, maxR, maxScore);
                }
            }
        }
        for (auto &&i: circles) {
            if (get<2>(i) > 120) {
                drawCircle(get<0>(i), get<1>(i), get<2>(i), 255, 255, 0);
                cost += .5;
            } else if (get<2>(i) > 103) {
                drawCircle(get<0>(i), get<1>(i), get<2>(i), 0, 255, 0);
                cost += .25;
            } else if (get<2>(i) > 90) {
                drawCircle(get<0>(i), get<1>(i), get<2>(i), 0, 0, 255);
                cost += .1;
            } else if (get<2>(i) > 80 && vec[get<0>(i)][get<1>(i)][2] < 100) {
                drawCircle(get<0>(i), get<1>(i), get<2>(i), 255, 0, 0);
                cost += .01;
            } else if (get<2>(i) > 80) {
                drawCircle(get<0>(i), get<1>(i), get<2>(i), 255, 0, 255);
                cost += .05;
            }
        }
        return cost;
    }

    vector<vector<vector<int>>> getVec() const {
        return vec;
    }

    Image copy() const {
        vector<vector<vector<int>>> vecCopy = vec;
        return Image(col, row, color, vecCopy);
    }

    void print(const string &outfile) {
        ofstream fileO;
        fileO.open(outfile + ".ppm", ios::out);
        fileO << "P3\n" << col << " " << row << "\n" << color << "\n";
        for (auto &i: vec) {
            for (auto &j: i) {
                for (int k: j) {
                    fileO << k << " ";
                }
            }
        }
        fileO.close();
    }
};

void part2(ofstream &res, const string &inFile, double eMin = -1, double eMax = -1, int cenT = 30, double circT = .1) {
    int rMin = 78;
    int rMax = 120;
    auto stT = chrono::high_resolution_clock::now();
    Image img = Image(inFile);
    Image oriCenter = img.copy();
    Image oriCircle = img.copy();
    chrono::duration<double, std::milli> dur = chrono::high_resolution_clock::now() - stT;
    cout << "Processing Time: " << dur.count() << '\n';
    res << "Processing Time: " << dur.count() << '\n';
    stT = chrono::high_resolution_clock::now();
    img.grayscale();
    tuple<Image, vector<vector<double>>> sob = img.sobel();
    Image NMS = Image(get<1>(sob), get<0>(sob).getVec());
    if (eMin == -1) {
        eMin = (int) (.06 * get<0>(sob).color);
    }
    if (eMax == -1) {
        eMax = (int) (.17 * get<0>(sob).color);
    }
    get<0>(sob).doubleThreshold(eMin, eMax);
    get<0>(sob).hysteresis();
    get<0>(sob).combine(NMS);
    get<0>(sob).print("imagef");
    dur = chrono::high_resolution_clock::now() - stT;
    cout << "Edge Detection Time: " << dur.count() << '\n';
    res << "Edge Detection Time: " << dur.count() << '\n';
    stT = chrono::high_resolution_clock::now();
    tuple<Image, vector<Point>> voCe = oriCenter.findCenter(get<1>(sob), get<0>(sob), cenT, rMax);
    get<0>(voCe).print("imagev");
    oriCenter.print("imageCC");
    dur = chrono::high_resolution_clock::now() - stT;
    cout << "Center Detection Time: " << dur.count() << '\n';
    res << "Center Detection Time: " << dur.count() << '\n';
    stT = chrono::high_resolution_clock::now();
    cout << "# of Centers: " << get<1>(voCe).size() << '\n';
    res << "# of Centers: " << get<1>(voCe).size() << '\n';
    double cost = oriCircle.findCircle(get<1>(voCe), get<0>(sob), rMin, rMax, circT);
    oriCircle.print("coins");
    dur = chrono::high_resolution_clock::now() - stT;
    cout << "Circle Detection Time: " << dur.count() << '\n';
    cout << "Money: $" << cost << '\n';
    res << "Circle Detection Time: " << dur.count() << '\n';
    res << "Money: $" << cost << '\n';
}

int main(int argc, char *argv[]) {
    ofstream res;
    res.open("results.txt");
    auto stT = chrono::high_resolution_clock::now();
    if (argc == 1) {
        part2(res, "9circles.ppm");
    } else {
        part2(res, string(argv[7]), stod(string(argv[3])), stod(string(argv[5])),
              stoi(string(argv[9])), stod(string(argv[11])));
    }
    chrono::duration<double, std::milli> dur = chrono::high_resolution_clock::now() - stT;
    cout << "Total Time (ms): " << dur.count() << '\n';
    res << "Total Time (ms): " << dur.count() << '\n';
    res.close();
}