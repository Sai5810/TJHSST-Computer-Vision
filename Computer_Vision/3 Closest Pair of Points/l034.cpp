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

#define ULL unsigned long long
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
    static constexpr int n = 800;
    vector<vector<vector<int>>> file{n, vector<vector<int>>(n, vector<int>(3, 1))};

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

void gen(int n) {
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> distr(0.0, 1.0);
    fstream ptO;
    ptO.open("points.txt", ios::out);
    for (int i = 0; i < n; ++i) {
        ptO << setprecision(24) << distr(gen) << " " << distr(gen) << "\n";
    }
    ptO.close();
}

tuple<double, Point, Point, double> part1(int n) {
    list<Point> pt;
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> distr(0.0, 1.0);
    for (int i = 0; i < n; ++i) {
        pt.emplace_back(distr(gen), distr(gen));
    }
    fstream ptO;
    ptO.open("points.txt", ios::out);
    Image img;
    for (auto &it: pt) {
        img.drawPoint(it, 3);
        ptO << setprecision(24) << it.getX() << " " << it.getY() << "\n";
    }
    ptO.close();
    auto stT = chrono::high_resolution_clock::now();
    tuple<double, Point, Point> dPP = {999999, *pt.begin(), *(next(pt.begin()))};
    for (auto i = pt.begin(); i != pt.end(); ++i) {
        for (auto j = next(i); j != pt.end(); ++j) {
            double cDist = (*i).magTo((*j));
            if (cDist < get<0>(dPP)) {
                get<0>(dPP) = cDist;
                get<1>(dPP) = *i;
                get<2>(dPP) = *j;
            }
        }
    }
    get<0>(dPP) = sqrt(get<0>(dPP));
    img.drawPoint(get<1>(dPP), 3, 1, 0, 0);
    img.drawPoint(get<2>(dPP), 3, 1, 0, 0);
    img.print("points");
    chrono::duration<double, std::milli> dur = chrono::high_resolution_clock::now() - stT;
    return tuple_cat(dPP, make_tuple(dur.count()));
}

tuple<double, Point, Point> part2(const vector<Point> *v, int st, int end) {
    if (end - st > 2) {
        int mid = (st + end) / 2 + 1;
        tuple<double, Point, Point> lDPP = part2(v, st, mid - 1);
        tuple<double, Point, Point> rDPP = part2(v, mid, end);
        if (get<0>(lDPP) < get<0>(rDPP)) {
            rDPP = lDPP;
        }
        double midline = ((*v)[mid - 1].getX() + (*v)[mid].getX()) / 2;
        int l = mid;
        while (l - 1 >= st && pow((*v)[l - 1].getX() - midline, 2) < get<0>(rDPP)) {
            --l;
        }
        if (l != mid) {
            int r = mid - 1;
            while (r + 1 <= end && pow((*v)[r + 1].getX() - midline, 2) < get<0>(rDPP)) {
                ++r;
            }
            if (r != mid - 1) {
                //cout << l << "-" << r << '\n';
                for (int i = l; i <= mid - 1; ++i) {
                    for (int j = mid; j <= r; ++j) {
                        //cout << "i " << (*v)[i].print() << " j " << (*v)[j].print() << '\n';
                        double cDist = (*v)[i].magTo((*v)[j]);
                        if (cDist < get<0>(rDPP)) {
                            rDPP = {cDist, (*v)[i], (*v)[j]};
                        }
                    }
                }
            }
        }
        return rDPP;
    } else if (end - st == 2) {
        tuple<double, Point, Point> dPP = {(*v)[st].magTo((*v)[st + 1]), (*v)[st], (*v)[st + 1]};
        tuple<double, Point, Point> cha = {(*v)[st + 1].magTo((*v)[end]), (*v)[st + 1], (*v)[end]};
        if (get<0>(cha) < get<0>(dPP)) {
            dPP = cha;
        }
        cha = {(*v)[end].magTo((*v)[st]), (*v)[end], (*v)[st]};
        if (get<0>(cha) < get<0>(dPP)) {
            dPP = cha;
        }
        return dPP;
    } else {
        return {((*v)[st]).magTo((*v)[end]), (*v)[st], (*v)[end]};
    }
}

tuple<double, Point, Point> part3(const vector<Point> *v, int st, int end) {
    if (end - st > 2) {
        int mid = (st + end) / 2 + 1;
        tuple<double, Point, Point> lDPP = part3(v, st, mid - 1);
        tuple<double, Point, Point> rDPP = part3(v, mid, end);
        if (get<0>(lDPP) < get<0>(rDPP)) {
            rDPP = lDPP;
        }
        double midline = ((*v)[mid - 1].getX() + (*v)[mid].getX()) / 2;
        int l = mid;
        while (l - 1 >= st && pow((*v)[l - 1].getX() - midline, 2) < get<0>(rDPP)) {
            --l;
        }
        if (l != mid) {
            int r = mid - 1;
            while (r + 1 <= end && pow((*v)[r + 1].getX() - midline, 2) < get<0>(rDPP)) {
                ++r;
            }
            if (r != mid - 1) {
                vector<Point> strip((*v).begin() + l, (*v).begin() + r + 1);
                //cout << l << "-" << r << " " << strip.size() << '\n';
                sort(strip.begin(), strip.end(), [](Point const &p1, Point const &p2) -> bool {
                    return p1.getY() < p2.getY();
                });
                for (int i = 0; i < strip.size(); ++i) {
//                    for (int j = i + 1; j < strip.size() && j <= i + 7; ++j) {
                    for (int j = i + 1;
                         j < strip.size() && pow(strip[j].getY() - strip[i].getY(), 2) <= get<0>(rDPP); ++j) {
                        //cout << "i " << strip[i].print() << " j " << strip[j].print() << '\n';
                        double cDist = strip[i].magTo(strip[j]);
                        if (cDist < get<0>(rDPP)) {
                            rDPP = {cDist, strip[i], strip[j]};
                        }
                    }
                }
            }
        }
        return rDPP;
    } else if (end - st == 2) {
        tuple<double, Point, Point> dPP = {(*v)[st].magTo((*v)[st + 1]), (*v)[st], (*v)[st + 1]};
        tuple<double, Point, Point> cha = {(*v)[st + 1].magTo((*v)[end]), (*v)[st + 1], (*v)[end]};
        if (get<0>(cha) < get<0>(dPP)) {
            dPP = cha;
        }
        cha = {(*v)[end].magTo((*v)[st]), (*v)[end], (*v)[st]};
        if (get<0>(cha) < get<0>(dPP)) {
            dPP = cha;
        }
        return dPP;
    } else {
        return {((*v)[st]).magTo((*v)[end]), (*v)[st], (*v)[end]};
    }
}


tuple<double, Point, Point, double> process(const string &infile,
                                            const function<tuple<double, Point, Point>(const vector<Point> *v, int st,
                                                                                       int end)> &func) {
    vector<Point> pt;
    ifstream ptI;
    ptI.open(infile);
    string line;
    while (getline(ptI, line)) {
        pt.emplace_back(stod(line.substr(0, line.find(' '))), stod(line.substr(line.find(' '))));
    }
    auto stT = chrono::high_resolution_clock::now();
    sort(pt.begin(), pt.end());
    tuple<double, Point, Point> dPP = func(&pt, 0, (int) pt.size() - 1);
    get<0>(dPP) = sqrt(get<0>(dPP));
    chrono::duration<double, std::milli> dur = chrono::high_resolution_clock::now() - stT;
    return tuple_cat(dPP, make_tuple(dur.count()));
}

struct hash_pair {
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2> &p) const {
        return hash<T1>{}(p.first) ^ hash<T2>{}(p.second);
    }
};

ULL convert(double d, double scale) {
    return floor(d * scale + .5);
}


tuple<double, Point, Point, double> part4(const string &infile) {
    vector<Point> pt;
    ifstream ptI;
    ptI.open(infile);
    string line;
    while (getline(ptI, line)) {
        pt.emplace_back(stod(line.substr(0, line.find(' '))), stod(line.substr(line.find(' '))));
    }
    auto stT = chrono::high_resolution_clock::now();
    random_device rd;
    mt19937 rng(rd());
    for (int i = 0; i <= (int) pt.size() - 2; ++i) {
        uniform_int_distribution<int> uni(i, (int) pt.size() - 1);
        swap(pt[uni(rng)], pt[i]);
    }
    tuple<double, Point, Point> dPP = {pt[0].magTo(pt[1]), pt[0], pt[1]};
    double scale = 2 / sqrt(get<0>(dPP));
    unordered_map<pair<ULL, ULL>, Point, hash_pair> dict =
            {{{convert(pt[0].getX(), scale), convert(pt[0].getY(), scale)}, pt[0]},
             {{convert(pt[1].getX(), scale), convert(pt[1].getY(), scale)}, pt[1]},};
    for (int i = 2; i < pt.size(); ++i) {
        pair<ULL, ULL> gridC = {convert(pt[i].getX(), scale), convert(pt[i].getY(), scale)};
        if (dict.find(gridC) != dict.end()) {
            dPP = {dict[gridC].magTo(pt[i]), dict[gridC], pt[i]};
            scale = 2 / sqrt(get<0>(dPP));
            dict.clear();
            for (int a = 0; a <= i; ++a) {
                dict[{convert(pt[a].getX(), scale), convert(pt[a].getY(), scale)}] = pt[a];
            }
        }
        tuple<double, Point, Point> curDPP = dPP;
        for (ULL j = max((ULL) 0, gridC.second - 2); j <= gridC.second + 2; ++j) {
            for (ULL k = max((ULL) 0, gridC.first - 2); k <= gridC.first + 2; ++k) {
                if (j != gridC.second || k != gridC.first) {
                    pair<ULL, ULL> cur = {k, j};
                    if (dict.find(cur) != dict.end()) {
                        double cDist = pt[i].magTo(dict[cur]);
                        if (cDist < get<0>(curDPP)) {
                            curDPP = {cDist, pt[i], dict[cur]};
                        }
                    }
                }
            }
        }
        if (curDPP != dPP) {
            dPP = curDPP;
            dict.clear();
            scale = 2 / sqrt(get<0>(dPP));
            for (int a = 0; a <= i; ++a) {
                dict[{convert(pt[a].getX(), scale), convert(pt[a].getY(), scale)}] = pt[a];
            }
        } else {
            dict[gridC] = pt[i];
        }
    }
    chrono::duration<double, std::milli> dur = chrono::high_resolution_clock::now() - stT;
    get<0>(dPP) = sqrt(get<0>(dPP));
    return tuple_cat(dPP, make_tuple(dur.count()));
}

void output(fstream &resultO, const string &method, tuple<double, Point, Point, double> dPPd) {
    resultO << setprecision(24) << method << ": " << get<3>(dPPd) << "ms\n";
    resultO << "Point 1: (" << get<1>(dPPd).getX() << ", " << get<1>(dPPd).getY() << ")\n";
    resultO << "Point 2: (" << get<2>(dPPd).getX() << ", " << get<2>(dPPd).getY() << ")\n";
    resultO << "Distance: " << get<0>(dPPd) << "\n\n";
    cout << setprecision(24) << method << ": " << get<3>(dPPd) << "ms\n";
    cout << "Point 1: (" << get<1>(dPPd).getX() << ", " << get<1>(dPPd).getY() << ")\n";
    cout << "Point 2: (" << get<2>(dPPd).getX() << ", " << get<2>(dPPd).getY() << ")\n";
    cout << "Distance: " << get<0>(dPPd) << "\n\n";
}

int main() {
    fstream resultO;
    resultO.open("results.txt", ios::out);
    gen(1000000);
//    output(resultO, "Brute Force", part1(100));
//    output(resultO, "Intermediate Divide and Conquer", process("points100k.txt", part2));
    output(resultO, "Divide and Conquer", process("points.txt", part3));
    output(resultO, "Randomize", part4("points.txt"));
    resultO.close();
}