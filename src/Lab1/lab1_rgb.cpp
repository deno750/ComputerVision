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
        Mat image = img.clone();
        if (x + NEIBORHOOD_MATRIX_ROWS > image.cols || y + NEIBORHOOD_MATRIX_COLUMNS > image.rows) {
            return;
        }
        Rect rect(x - NEIBORHOOD_MATRIX_ROWS / 2, y - NEIBORHOOD_MATRIX_COLUMNS / 2, NEIBORHOOD_MATRIX_ROWS, NEIBORHOOD_MATRIX_COLUMNS);
        cv::Scalar mean = cv::mean(image(rect));
        //img.at<Vec3b>(x, y) = Vec3b(255, 255, 255);
        Vec3b colorMean = Vec3b(mean[0], mean[1], mean[2]);
        Vec3b rgb = image.at<Vec3b>(y, x);
        //cout << "RGB " << rgb << endl;
        double distance = cv::norm(rgb, colorMean);
        cout << "Eucledian: " << distance << endl;
        Vec3b lowerBound = Vec3b(0, 35, 70);
        Vec3b upperBound = Vec3b(54, 180, 235);
        Vec3b dst;
        for (int i = 0; i < image.rows; ++i) {
            for (int j = 0; j < image.cols; ++j) {
                
                Vec3b rgb = image.at<Vec3b>(i, j);
                
                cv::inRange(rgb, lowerBound, upperBound, dst);
                if (dst[0] == 255 && dst[1] == 255 && dst[2] == 255) {
                    rgb -= colorMean;
                    rgb += color;
                    
                    image.at<Vec3b>(i, j) = rgb;
                }
                    
                
                /*double distance = cv::norm(rgb, colorMean);
                //cout << "Distance " << distance << endl;
                if (distance >= 20 && distance <= 80) {
                    rgb -= colorMean;
                    rgb += color;
                    
                    //rgb = color;
                    
                    
                    image.at<Vec3b>(i, j) = rgb;
                }*/
            }
        }
        //image.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
        imshow(WINDOW_NAME, image);
        cout << "Mean: " << mean << endl;
    }
    
}

