#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>
#include <iomanip>

#define f first
#define s second

using namespace std;

fstream txtOut;

void randomize(int st, int e, pair<double, double> ran[4]) {
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = st; i <= e; ++i) {
        ran[i].f = dist(gen);
        ran[i].s = dist(gen);
    }
}

void genPoints() {
    fstream fileOut;
    fileOut.open("points.txt", ios::out);
    pair<double, double> r[4];
    randomize(0, 2, r);
    double asX, asY;
    bool sAB;
    do {
        randomize(3, 3, r);
        asX = r[3].f - r[0].f;
        asY = r[3].s - r[0].s;
        sAB = (r[1].f - r[0].f) * asY - (r[1].s - r[0].s) * asX > 0;
    } while (((r[2].f - r[0].f) * asY - (r[2].s - r[0].s) * asX > 0 != sAB) and
             ((r[2].f - r[1].f) * (r[3].s - r[1].s) - (r[2].f - r[1].s) * (r[2].f - r[1].f) > 0 == sAB));
    for (int i = 0; i < 3; ++i) {
        fileOut << setprecision(17) << "(" << r[i].f << ", " << r[i].s << "), ";
    }
    fileOut << setprecision(17) << "(" << r[3].f << ", " << r[3].s << ")";
}

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

    string print() const {
        return ("(" + to_string(x) + ", " + to_string(y) + ")");
    }
};

class Line {
private:
    Point a, b;
    double m, len, yint;
public:
    Line() {
        m = 0;
        len = 0;
        yint = 0;
    }

    Line(Point a, Point b) {
        this->a = a;
        this->b = b;
        m = (double) (a.getY() - b.getY()) / (a.getX() - b.getX());
        len = sqrt(pow(a.getX() - b.getX(), 2) + pow(a.getY() - b.getY(), 2));
        yint = a.getY() - m * a.getX();
    }

    Line(Point a, double m, double len, bool right = true) {
        this->m = m;
        this->len = len;
        this->a = a;
        double adx = len / sqrt(1 + pow(m, 2));
        double ady = (len * m) / sqrt(1 + pow(m, 2));
        if (right) {
            b.setX(a.getX() + adx);
            b.setY(a.getY() + ady);
        } else {
            b.setX(a.getX() - adx);
            b.setY(a.getY() - ady);
        }
        yint = a.getY() - m * a.getX();
    }

    void extendLine(double top = 1, double bottom = 0, double left = 0, double right = 1) {
        double tb = left * m + yint;
        if (bottom < tb && tb < top) {
            a = Point(left, tb);
            b = Point(right, right * m + yint);
        } else {
            a = Point((top - yint) / m, top);
            b = Point((bottom - yint) / m, bottom);
        }
    }

    double getSlope() const {
        return m;
    }

    double getLen() const {
        return len;
    }

    Point getA() {
        return a;
    }

    Point getB() {
        return b;
    }

    double getYint() const {
        return yint;
    }

    //debugging method
    string print() {
        return a.print() + ", " + b.print();
    }

    Point intersection(Line other) const {
        Point p((yint - other.getYint()) / (other.getSlope() - m), 0);
        p.setY(p.getX() * m + yint);
        return p;
    }
};

class Square {
private:
    vector<Point> pt;
    vector<Line> vl;
    double area;
public:
    Square(Point p1, Point p2, Point p3, Point p4) {
        pt.push_back(p1);
        pt.push_back(p2);
        pt.push_back(p3);
        pt.push_back(p4);
        vl.emplace_back(p1, p2);
        vl.emplace_back(p2, p3);
        vl.emplace_back(p3, p4);
        vl.emplace_back(p4, p1);
        area = pow(vl[0].getLen(), 2);
    }

    double getArea() const {
        return area;
    }

    vector<Line> getVl() {
        return vl;
    }

    vector<Point> getPoints() {
        return pt;
    }

};

class Image {
private:
    static constexpr int n = 800;
    vector<vector<int>> file{n, vector<int>(n, 1)};

public:
    Image() = default;

    void plot(int x, int y) {
        if (0 <= x and x < n and 0 <= y and y < n) {
            file[x][y] = 0;
        }
    }

    void drawLine(Line line) {
        drawLine(line.getA(), line.getB());
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

    pair<int, int> drawCircle(double dcx, double dcy, int r) {
        return drawCircle(dcx, dcy, double(r) / n);
    }

    pair<int, int> drawCircle(double dcx, double dcy, double dr) {
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
            plot(cx + x, cy + y);
            plot(cx + x, cy - y);
            plot(cx - x, cy + y);
            plot(cx - x, cy - y);
            plot(cx + y, cy + x);
            plot(cx + y, cy - x);
            plot(cx - y, cy + x);
            plot(cx - y, cy - x);
            y2_new -= (2 * x) - 3;
        }
        return make_pair(cx, cy);
    }

    void drawSquareEx(Square sq) {
        for (auto i: sq.getVl()) {
            i.extendLine();
            drawLine(i);
        }
        for (auto i: sq.getPoints()) {
            drawPoint(i);
        }
    }

    void drawPoint(Point p, int ptR = 2) {
        drawCircle(p.getX(), p.getY(), ptR);
    }

    void print(const string &outfile) {
        fstream fileOut;
        fileOut.open(outfile + ".ppm", ios::out);
        fileOut << "P3 " << n << " " << n << " 1" << "\n";
        for (auto &i: file) {
            for (int j: i) {
                fileOut << j << " " << j << " " << j << " ";
            }
            fileOut << "\n";
        }
        fileOut.close();
    }
};


Square genSqHelp(Point pt[], int b, int c, int d, bool lr) {
    Line ac = Line(pt[0], pt[c]);
    Line be = Line(pt[b], -1 / ac.getSlope(), ac.getLen(), lr);
    Line ed = Line(be.getB(), pt[d]);
    Line ll1 = Line(pt[0], -1 / ed.getSlope(), 1);
    Line ll2 = Line(pt[c], -1 / ed.getSlope(), 1);
    Line ll3 = Line(pt[b], ed.getSlope(), 1);
    Line ll4 = Line(pt[d], ed.getSlope(), 1);
    return {ll1.intersection(ll3), ll3.intersection(ll2), ll2.intersection(ll4), ll4.intersection(ll1)};
}


Square genSquares(Point pt[], int b, int c, int d) {
    Square sqT = genSqHelp(pt, b, c, d, true);
    Square sqF = genSqHelp(pt, b, c, d, false);
    for (auto i: sqT.getPoints()) {
        txtOut << setprecision(17) << "(" << i.getX() << ", " << i.getY() << "), ";
    }
    txtOut << setprecision(17) << "Area=" << sqT.getArea() << '\n';
    for (auto i: sqF.getPoints()) {
        txtOut << setprecision(17) << "(" << i.getX() << ", " << i.getY() << "), ";
    }
    txtOut << setprecision(17) << "Area=" << sqF.getArea() << '\n';
    if (sqT.getArea() < sqF.getArea()) {
        return sqT;
    } else {
        return sqF;
    }
}


int main() {
    Point pt[4];
    genPoints();
    ifstream infile;
    infile.open("points.txt");
    string line;
    while (getline(infile, line)) {
        for (int i = 0; i <= 3; ++i) {
            unsigned int q = line.find(',');
            if (q < 0) {
                throw invalid_argument("The correct character weren't found");
            }
            pt[i].setX(stod(line.substr(line.find('0'), q)));
            line = line.substr(q, line.length());
            q = line.find(')');
            if (q < 0) {
                throw invalid_argument("The correct character weren't found");
            }
            pt[i].setY(stod(line.substr(line.find('0'), q)));
            line = line.substr(line.length() >= q + 3 ? q + 3 : q, line.length());
        }
    }
    txtOut.open("sampleoutput.txt", ios::out);
    for (auto i: pt) {
        txtOut << setprecision(17) << "(" << i.getX() << ", " << i.getY() << "), ";
    }
    txtOut << "\n";
    Square cur = genSquares(pt, 1, 2, 3);
    Square cha = genSquares(pt, 2, 1, 3);
    if (cha.getArea() < cur.getArea()) {
        cur = cha;
    }
    cha = genSquares(pt, 2, 3, 1);
    if (cha.getArea() < cur.getArea()) {
        cur = cha;
    }
    txtOut.close();
    Image img;
    img.drawSquareEx(cur);
    for (auto i: pt) {
        img.drawPoint(i);
    }
    img.print("output");
}




