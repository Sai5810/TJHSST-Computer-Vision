#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>

#define f first
#define s second

using namespace std;
const int n = 800;
int file[n][n];
pair<double, double> ran[3];

void randomize() {
    mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
    uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto &i: ran) {
        i.f = dist(gen);
        i.s = dist(gen);
    }
}

void plot(int x, int y) {
    if (0 <= x and x < n and 0 <= y and y < n) {
        file[x][y] = 0;
    }
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

void triangle() {
    drawLine(ran[0].f, ran[0].s, ran[1].f, ran[1].s);
    drawLine(ran[0].f, ran[0].s, ran[2].f, ran[2].s);
    drawLine(ran[1].f, ran[1].s, ran[2].f, ran[2].s);
}

pair<int, int> eqCircle(double dcx, double dcy, double dr) {
    int cx = dcx * n;
    int cy = dcy * n;
    int r = dr * n;
    int xmax = (int) ((r + 2) * 0.70710678);
    int y = r;
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

pair<int, int> pointCircle(double x1, double y1, double x2, double y2, double x3, double y3) {
    double py = x2 * x2 + y2 * y2;
    double bc = (x1 * x1 + y1 * y1 - py) / 2.0;
    double cd = (py - x3 * x3 - y3 * y3) / 2.0;
    double det = (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
    double cx = (bc * (y2 - y3) - cd * (y1 - y2)) / det;
    double cy = ((x1 - x2) * cd - (x2 - x3) * bc) / det;
    return eqCircle(cx, cy, pow((pow((cx - x1), 2) + pow((cy - y1), 2)), .5));
}

void eulerLine(pair<int, int> cc) {
    pair<double, double> ctrd = make_pair((ran[0].f + ran[1].f + ran[2].f) / 3.0,
                                          (ran[0].s + ran[1].s + ran[2].s) / 3.0);
    double m = (double) (ctrd.s - cc.s) / (ctrd.f - cc.f);
    if (m == 0) m = 1;
    double b = cc.s - m * cc.f;
    drawLine(-b / m, 0, (n - b) / m, n);
}

void incircle() {
    double a = pow(pow(ran[1].f - ran[0].f, 2) + pow(ran[1].s - ran[0].s, 2), .5);
    double b = pow(pow(ran[2].f - ran[0].f, 2) + pow(ran[2].s - ran[0].s, 2), .5);
    double c = pow(pow(ran[2].f - ran[1].f, 2) + pow(ran[2].s - ran[1].s, 2), .5);
    double sp = (a + b + c) / 2;
    eqCircle((a * ran[2].f + b * ran[1].f + c * ran[0].f) / (a + b + c),
             (a * ran[2].s + b * ran[1].s + c * ran[0].s) / (a + b + c),
             pow(((sp - a) * (sp - b) * (sp - c)) / sp, .5));
}

int main() {
    fstream fout;
    fout.open("triangle.ppm", ios::out);
    fill_n(&file[0][0], n * n, 1);
    randomize();
    triangle();
    pair<double, double> cc = pointCircle(ran[0].f, ran[0].s, ran[1].f, ran[1].s, ran[2].f, ran[2].s);
    pointCircle((ran[0].f + ran[1].f) / 2, (ran[0].s + ran[1].s) / 2, (ran[0].f + ran[2].f) / 2,
                (ran[0].s + ran[2].s) / 2, (ran[1].f + ran[2].f) / 2, (ran[1].s + ran[2].s) / 2);
    eulerLine(cc);
    incircle();
    fout << "P3 " << n << " " << n << " 1" << "\n";
    for (auto &i: file) {
        for (int j: i) {
            fout << j << " " << j << " " << j << " ";
        }
        fout << "\n";
    }
    fout.close();
}
