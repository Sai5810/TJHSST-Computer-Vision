#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <utility>
#include <string>
#include <vector>
#include <sstream>
#include <tuple>

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
    vector<vector<vector<int>>> vec;
public:
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
                    if (gray[i][j][0] > gray[i + 1][j][0] && gray[i][j][0] > gray[i - 1][j][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                } else if (ang[i][j] >= 22.5 && ang[i][j] <= 67.5) {
                    if (gray[i][j][0] > gray[i + 1][j - 1][0] && gray[i][j][0] > gray[i - 1][j + 1][0]) {
                        fill(vec[i][j].begin(), vec[i][j].end(), 1);
                    }
                } else if (ang[i][j] > 67.5 && ang[i][j] <= 112.5) {
                    if (gray[i][j][0] > gray[i][j + 1][0] && gray[i][j][0] > gray[i][j - 1][0]) {
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

    explicit Image(int n = 400, int m = 400, int color = 255, int initColor = 1) {
        this->col = n;
        this->row = m;
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

    void plot(int x, int y, int r = 0, int g = 0, int b = 0) {
        if (0 <= x and x < row and 0 <= y and y < col) {
            vec[x][y][0] = r;
            vec[x][y][1] = g;
            vec[x][y][2] = b;
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

    void doubleThreshold(int low, int high) {
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


    void combine(Image ang) {
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

    vector<vector<vector<int>>> getVec() {
        return vec;
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

void part1(const string &inFile) {
    Image img = Image(inFile);
    img.print("test");
    img.grayscale();
    img.print("imageg");
    tuple<Image, vector<vector<double>>> sob = img.sobel();
    img = get<0>(sob);
    img.threshold((int) (.2 * get<0>(sob).color));
    img.print("imagem");
}

void part2(const string &inFile) {
    Image img = Image(inFile);
    img.grayscale();
    img.print("imageg");
    tuple<Image, vector<vector<double>>> sob = img.sobel();
    img = get<0>(sob);
    img.doubleThreshold((int) (.2 * get<0>(sob).color), (int) (.5 * get<0>(sob).color));
    img.hysteresis();
    img.print("image1");
}

void part3(const string &inFile, int low = -1, int high = -1) {
    Image img = Image(inFile);
    img.grayscale();
    img.print("imageg");
    tuple<Image, vector<vector<double>>> sob = img.sobel();
    Image NMS = Image(get<1>(sob), get<0>(sob).getVec());
    NMS.print("image2");
    if (low == -1) {
        low = (int) (.2 * get<0>(sob).color);
    }
    if (high == -1) {
        high = (int) (.5 * get<0>(sob).color);
    }
    get<0>(sob).doubleThreshold(low, high);
    get<0>(sob).hysteresis();
    get<0>(sob).print("image1");
    get<0>(sob).combine(NMS);
    get<0>(sob).print("imagef");
}


int main(int argc, char *argv[]) {
    if (argc == 1) {
        part3("image.ppm");
    } else {
        part3(string(argv[6]), stoi(string(argv[2])), stoi(string(argv[4])));
    }
}