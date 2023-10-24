#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv) {
    Mat img = imread(string(argv[1]));
    if (img.data) {
        Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);
        imwrite("imageg.jpg", gray);
        GaussianBlur(gray, gray, Size(5, 5), 10);
        Mat edges;
        Canny(gray, edges, stoi(string(argv[3])) / 2.0, stoi(string(argv[3])));
        imwrite("imagef.jpg", edges);
        vector<Vec3f> circles;
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1, stoi(string(argv[7])), stoi(string(argv[3])),
                     stoi(string(argv[5])), stoi(string(argv[9])), stoi(string(argv[11])));
        vector<int> coins = {0, 0, 0, 0, 0};
        double cost = 0;
        for (Vec3f c: circles) {
            Point center(cvRound(c[0]), cvRound(c[1]));
            int rad = cvRound(c[2]);
            Scalar color;
            if (rad > 150) {
                color = Scalar(255, 255, 0);
                coins[4] += 1;
                cost += 1;
            } else if (rad > stoi(string(argv[13]))) {
                color = Scalar(0, 255, 0);
                coins[0] += 1;
                cost += .25;
            } else if (rad > stoi(string(argv[15]))) {
                color = Scalar(255, 0, 0);
                coins[2] += 1;
                cost += .05;
            } else {
                double avRVB = 0;
                for (int i = 0; i < 10; ++i) {
                    for (int j = 0; j < 10; ++j) {
                        Vec3b ori = img.at<Vec3b>(center.y + i, center.x + j);
                        avRVB += (double) ori[2] - (double) ori[0];
                    }
                }
                avRVB /= 100;
                if (avRVB > 10) {
                    color = Scalar(0, 0, 255);
                    coins[3] += 1;
                    cost += .01;
                } else {
                    color = Scalar(255, 0, 255);
                    coins[1] += 1;
                    cost += .1;
                }
            }
            circle(img, center, 3, color, -1);
            circle(img, center, rad, color, 3);
        }
        imwrite("coins.jpg", img);
        cout << "Dollars:" << coins[4] << '\n';
        cout << "Quarters:" << coins[0] << '\n';
        cout << "Dimes:" << coins[1] << '\n';
        cout << "Nickels:" << coins[2] << '\n';
        cout << "Pennies:" << coins[3] << '\n';
        cout << "Cost:" << cost << '\n';
        ofstream res;
        res.open("results.txt");
        res << "Dollars:" << coins[4] << '\n';
        res << "Quarters:" << coins[0] << '\n';
        res << "Dimes:" << coins[1] << '\n';
        res << "Nickels:" << coins[2] << '\n';
        res << "Pennies:" << coins[3] << '\n';
        res << "Cost:" << cost << '\n';
        res.close();
    }
}