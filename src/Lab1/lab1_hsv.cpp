//
//  main.cpp
//  Lab1
//
//  Created by Denis Deronjic on 24/03/2020.
//  Copyright © 2020 Denis Deronjic. All rights reserved.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#define WINDOW_NAME "Display Image"
#define NEIBORHOOD_MATRIX_ROWS 9
#define NEIBORHOOD_MATRIX_COLUMNS 9


using namespace cv;
using namespace std;

Vec3b color(92, 37, 201);

void onMouse(int event, int x, int y, int flags, void *userdata);

int main(int argc, char** argv) {
   
    Mat image = imread("robocup.jpg");
    if (image.empty()) {
        cout << "No Image\n";
        return -1;
    }
    resize(image, image, Size(image.cols / 1.0, image.rows / 1.0));
    imshow(WINDOW_NAME, image);
    setMouseCallback(WINDOW_NAME, onMouse, (void *) &image);
    waitKey(0);
    return 0;
}


void onMouse(int event, int x, int y, int flags, void *userdata) {
    
    if (event == EVENT_LBUTTONDOWN) {
        Mat img = *((Mat*) userdata);
        Mat imagergb = img.clone();
        Mat image;
        cvtColor(imagergb, image, COLOR_BGR2HSV);
        if (x + NEIBORHOOD_MATRIX_ROWS > image.cols || y + NEIBORHOOD_MATRIX_COLUMNS > image.rows) {
            return;
        }
        Rect rect(x - NEIBORHOOD_MATRIX_ROWS / 2, y - NEIBORHOOD_MATRIX_COLUMNS / 2, NEIBORHOOD_MATRIX_ROWS, NEIBORHOOD_MATRIX_COLUMNS);
        cv::Scalar mean = cv::mean(image(rect));
        cout << "Mean: " << mean << endl;
        Vec3b hsv = image.at<Vec3b>(y, x);
        Vec3b rgb = img.at<Vec3b>(y, x);
        //Vec3s colorMean = Vec3s(mean[0], mean[1], mean[2]);
        cout << "HSV: " << hsv << endl;
        //cout << "RBG: " << rgb << endl;
       Vec3b lowerBound = Vec3b(15, 0, 0);
       Vec3b upperBound = Vec3b(53, 0, 0);
       Vec3b dst;
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                
                Vec3b hsv = image.at<Vec3b>(i, j);
                cv::inRange(hsv, lowerBound, upperBound, dst);
                if (dst[0] == 255) {
                    image.at<Vec3b>(i, j)[0] += 230;
                }
                
                
            }
        }
        cvtColor(image, image, COLOR_HSV2BGR);
        imshow(WINDOW_NAME, image);
    }
    
}

