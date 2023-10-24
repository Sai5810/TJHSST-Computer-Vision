#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void drawLine(int i, int j, Mat3b &img, vector<Vec2f> &vec, int width, int height) {
    line(img, Point(vec[i][0] + width / 2, vec[i][1] + height / 2),
         Point(vec[j][0] + width / 2, vec[j][1] + height / 2),
         (255, 255, 255));
}

void scale(vector<Vec3f> &vec, int scale) {
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] *= scale;
    }
}

void rotate(vector<Vec3f> &vec, Mat1f &rot) {
    for (int i = 0; i < vec.size(); ++i) {
        Mat1f temp = rot * Mat1f(vec[i]);
        vec[i] = temp.reshape(3).at<Vec3f>();
    }
}

vector<Vec3f> project(Vec3f eye, Vec3f plPt, Vec3f norm, vector<Vec3f> &ver) {
    vector<Vec3f> proj;
    for (int i = 0; i < ver.size(); ++i) {
        proj.push_back(((plPt - eye).dot(norm)) / ((ver[i] - eye).dot(norm)) * (ver[i] - eye) + eye);
    }
    return proj;
}

vector<Vec2f> screenConv(Vec3f eye, Vec3f plPt, Vec3f norm, vector<Vec3f> &ver, Vec3f w1, Vec3f w2, Vec3f p0) {
    vector<Vec2f> screen;
    for (int i = 0; i < ver.size(); ++i) {
        screen.push_back(Vec2f((ver[i] - p0).dot(w1) / w1.dot(w1), (ver[i] - p0).dot(w2) / w2.dot(w2)));
    }
    return screen;
}

void drawCube(Mat3b &img, vector<Vec2f> &ver, int width, int height) {
    vector<pair<int, int>> edgePairs = {{0, 1},
                                        {1, 2},
                                        {2, 3},
                                        {3, 0},
                                        {4, 5},
                                        {5, 6},
                                        {6, 7},
                                        {7, 4},
                                        {0, 4},
                                        {1, 5},
                                        {2, 6},
                                        {3, 7}};
    for (auto ep: edgePairs) {
        drawLine(ep.first, ep.second, img, ver, width, height);
    }
}

void drawTet(Mat3b &img, vector<Vec2f> &ver, int width, int height) {
    vector<pair<int, int>> edgePairs = {{1, 0},
                                        {1, 2},
                                        {1, 3},
                                        {0, 3},
                                        {3, 2},
                                        {2, 0}};
    for (auto ep: edgePairs) {
        drawLine(ep.first, ep.second, img, ver, width, height);
    }
}

void part3(int width, int height) {
    ofstream logs;
    logs.open("logs.txt");
    Vec3f plPt(100, 100, 100);
    Vec3f norm(-100, -100, -100);
    Vec3f eye(150, 150, 150);
    logs << "The plane defined by (x-a)*n =0 is:\n"
         << "  a = " << plPt << "\n"
         << "  n = " << norm << "\n"
         << "The eye e is:\n"
         << "  e = " << eye << "\n";
    vector<Vec3f> cube = {Vec3f(1, 1, 1), Vec3f(1, 1, -1),
                          Vec3f(1, -1, -1), Vec3f(1, -1, 1),
                          Vec3f(-1, 1, 1), Vec3f(-1, 1, -1),
                          Vec3f(-1, -1, -1), Vec3f(-1, -1, 1)};
    VideoWriter vid = VideoWriter("rotation.avi", VideoWriter::fourcc('H', '2', '6', '4'), 30, Size(width, height),
                                  true);
    double alpha = 3.14159265359 / 180;
    double beta = 3.14159265359 / 180;
    double gamma = 3.14159265359 / 180;
    Mat1f rot = (Mat1f(3, 3) << cos(alpha) * cos(beta),
            cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma),
            cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma),
            sin(alpha) * cos(beta),
            sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma),
            sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma),
            -sin(beta),
            cos(beta) * sin(gamma),
            cos(beta) * cos(gamma));
    scale(cube, 60);
    Vec3f a = cube[0] - cube[1];
    Vec3f w1 = (1 / (pow(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2), .5))) * a;
    Vec3f b = cube[2] - cube[1];
    Vec3f c = b - (b.dot(a) / a.dot(a)) * a;
    Vec3f w2 = (1 / (pow(pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2), .5))) * c;
    Vec3f p0 = (((plPt - eye).dot(norm)) / ((-eye).dot(norm))) * -eye + eye;
    logs << "The coordinates in the plane x = p0 + u*w1 + v*w2 are:\n"
         << "  p0 = " << p0 << "\n"
         << "  w1 = " << w1 << "\n"
         << "  w2 = " << w2 << "\n";
    for (int n = 0; n <= 360; n += 1) {
        rotate(cube, rot);
        Mat3b img(height, width, Vec3b(0, 0, 0));
        vector<Vec3f> proj = project(eye, plPt, norm, cube);
        vector<Vec2f> screen = screenConv(eye, plPt, norm, cube, w1, w2, p0);
        if (n < 4) {
            logs << "The frame1 in 3d has the following edges:\n";
            vector<pair<int, int>> edgePairs = {{0, 1},
                                                {1, 2},
                                                {2, 3},
                                                {3, 0},
                                                {4, 5},
                                                {5, 6},
                                                {6, 7},
                                                {7, 4},
                                                {0, 4},
                                                {1, 5},
                                                {2, 6},
                                                {3, 7}};
            for (auto ep: edgePairs) {
                logs << "  " << proj[ep.first] << ", " << proj[ep.second] << "\n";
            }
            logs << "The frame" << n + 1 << " in 2d has the following edges:\n";
            for (auto ep: edgePairs) {
                logs << "  " << screen[ep.first] << ", " << screen[ep.second] << "\n";
            }
            logs << "\n";
        }
        drawCube(img, screen, width, height);
        vid.write(img);
    }
    vector<Vec3f> tetra = {{1 / sqrt(3),  0,   0},
                           {0,            0,   2 / sqrt(6)},
                           {-sqrt(3) / 6, .5,  0},
                           {-sqrt(3) / 6, -.5, 0}};
    scale(tetra, 60);
    a = tetra[0] - tetra[1];
    w1 = (1 / (pow(pow(a[0], 2) + pow(a[1], 2) + pow(a[2], 2), .5))) * a;
    b = tetra[2] - tetra[1];
    c = b - (b.dot(a) / a.dot(a)) * a;
    w2 = (1 / (pow(pow(c[0], 2) + pow(c[1], 2) + pow(c[2], 2), .5))) * c;
    for (int n = 0; n <= 360; n += 1) {
        rotate(tetra, rot);
        Mat3b img(height, width, Vec3b(0, 0, 0));
        vector<Vec3f> proj = project(eye, plPt, norm, tetra);
        vector<Vec2f> screen = screenConv(eye, plPt, norm, tetra, w1, w2, p0);
        drawTet(img, screen, width, height);
        vid.write(img);
    }
    logs.close();
    vid.release();
}

int main() {
    part3(800, 600);
}


