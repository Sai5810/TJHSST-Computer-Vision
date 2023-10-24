#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void drawLine(int i, int j, Mat &img, vector<Mat> &vec, int width, int height) {
    line(img, Point(vec[i].at<double>(0, 1) + width / 2, vec[i].at<double>(0, 2) + height / 2),
         Point(vec[j].at<double>(0, 1) + width / 2, vec[j].at<double>(0, 2) + height / 2),
         (0, 50, 200));
}

void scale(vector<Mat> &vec, int scale) {
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] *= scale;
    }
}

void rotate(vector<Mat> &vec, Mat &rot) {
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = rot * vec[i];
    }
}

void display(ofstream &out, vector<Mat> &vec) {
    for (int i = 0; i < vec.size(); ++i) {
        out << "(" << vec[i].at<double>(0, 0) << ", " << vec[i].at<double>(0, 1) << ", " << vec[i].at<double>(0, 2)
            << ")";
        if (i != vec.size() - 1) {
            out << ", ";
        }
    }
    out << '\n';
}

vector<Mat> convert(Mat &eye, vector<Mat> &vec, double plane) {
    vector<Mat> conv;
    for (int i = 0; i < vec.size(); ++i) {
        double t = (plane - 2 * plane) / (vec[i].at<double>(0, 0) - plane);
        Mat res = vec[i] + (1 - t) * eye;
        conv.push_back(res);
    }
    return conv;
}

void drawCube(Mat &img, vector<Mat> &vec, int width, int height) {
    drawLine(0, 1, img, vec, width, height);
    drawLine(1, 2, img, vec, width, height);
    drawLine(2, 3, img, vec, width, height);
    drawLine(3, 0, img, vec, width, height);
    drawLine(4, 5, img, vec, width, height);
    drawLine(5, 6, img, vec, width, height);
    drawLine(6, 7, img, vec, width, height);
    drawLine(7, 4, img, vec, width, height);
    drawLine(0, 4, img, vec, width, height);
    drawLine(1, 5, img, vec, width, height);
    drawLine(2, 6, img, vec, width, height);
    drawLine(3, 7, img, vec, width, height);
}

void part2(int width, int height, double plane) {
    vector<Mat> cube = {(Mat1d(3, 1) << 1, 1, 1), (Mat1d(3, 1) << 1, 1, -1),
                        (Mat1d(3, 1) << 1, -1, -1), (Mat1d(3, 1) << 1, -1, 1),
                        (Mat1d(3, 1) << -1, 1, 1), (Mat1d(3, 1) << -1, 1, -1),
                        (Mat1d(3, 1) << -1, -1, -1), (Mat1d(3, 1) << -1, -1, 1)};
    scale(cube, 60);
    VideoWriter vid = VideoWriter("rotation.avi", VideoWriter::fourcc('H', '2', '6', '4'), 30, Size(width, height),
                                  true);
    double a = 3.14159265359 / 180;
    double b = 3.14159265359 / 180;
    double g = 3.14159265359 / 180;
    Mat rot = (Mat1d(3, 3) << cos(a) * cos(b),
            cos(a) * sin(b) * sin(g) - sin(a) * cos(g),
            cos(a) * sin(b) * cos(g) + sin(a) * sin(g),
            sin(a) * cos(b),
            sin(a) * sin(b) * sin(g) + cos(a) * cos(g),
            sin(a) * sin(b) * cos(g) - cos(a) * sin(g),
            -sin(b),
            cos(b) * sin(g),
            cos(b) * cos(g));
    ofstream coord;
    coord.open("coordinates.txt");
    coord << "Cube Coordinates:\n";
    Mat eye = (Mat1d(3, 1) << 0, 0, 2 * plane);
    for (int n = 0; n <= 360; n += 1) {
        if (n < 4) {
            display(coord, cube);
        }
        rotate(cube, rot);
        Mat img(height, width, CV_8UC3, Scalar(0, 0, 0));
        vector<Mat> conv = convert(eye, cube, plane);
        drawCube(img, conv, width, height);
        vid.write(img);
    }
//    vector<Mat> tetra = {(Mat1d(3, 1) << 1 / sqrt(3), 0, 0), (Mat1d(3, 1) << 0, 0, 2 / sqrt(6)),
//                         (Mat1d(3, 1) << -sqrt(3) / 6, .5, 0), (Mat1d(3, 1) << -sqrt(3) / 6, -.5, 0)};
//    scale(tetra, 60);
//    coord << "Tetrahedron Coordinates:\n";
//    for (int n = 0; n <= 360; n += 1) {
//        if (n < 4) {
//            display(coord, tetra);
//        }
//        rotate(tetra, rot);
//        Mat img(height, width, CV_8UC3, Scalar(0, 0, 0));
//        drawLine(1, 0, img, tetra, width, height);
//        drawLine(1, 2, img, tetra, width, height);
//        drawLine(1, 3, img, tetra, width, height);
//        drawLine(0, 3, img, tetra, width, height);
//        drawLine(3, 2, img, tetra, width, height);
//        drawLine(2, 0, img, tetra, width, height);
//        vid.write(img);
//    }
    coord.close();
    vid.release();
}

int main() {
    part2(800, 600, 5);
}


