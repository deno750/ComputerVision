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

/*
 Mean: [54.9753, 166.309, 218, 0]
 Eucledian: 3.31662
 Mean: [49.6049, 157.432, 205.16, 0]
 Eucledian: 1.73205
 Mean: [46.5309, 148.704, 196.568, 0]
 Eucledian: 5.09902
 Mean: [30.8765, 125.185, 174.815, 0]
 Eucledian: 12.8452
 Mean: [21.9383, 95.9012, 129.111, 0]
 Eucledian: 22
 Mean: [25.1852, 107.568, 155.728, 0]
 Eucledian: 5.09902
 Mean: [36.7654, 148.247, 207.457, 0]
 Eucledian: 3.4641
 Mean: [52.9753, 178.481, 236.926, 0]
 Eucledian: 14.2127
 Mean: [53.1111, 173.123, 226.765, 0]
 Eucledian: 7.34847
 Mean: [14.0123, 97.642, 141.963, 0]
 Eucledian: 8.60233
 Mean: [34.3333, 154.506, 206.975, 0]
 Eucledian: 6.48074
 Mean: [33.0741, 147.778, 200.358, 0]
 Eucledian: 9.27362
 Mean: [23.037, 130.148, 183.235, 0]
 Eucledian: 8.544
 Mean: [14.7407, 76.8395, 101.778, 0]
 Eucledian: 54.7905
 Mean: [24.2222, 65.7778, 80.1358, 0]
 Eucledian: 7.61577
 Mean: [17.5062, 90.1358, 122.691, 0]
 Eucledian: 23.4094
 Mean: [14.6543, 70.284, 91.0247, 0]
 Eucledian: 8.06226
 Mean: [23.4938, 121.802, 171.642, 0]
 Eucledian: 3.31662
 Mean: [1.09877, 66.4938, 113.802, 0]
 Eucledian: 7
 Mean: [1.45679, 100.321, 155.864, 0]
 Eucledian: 4.12311
 Mean: [13.9877, 134.321, 201.605, 0]
 Eucledian: 6.16441
 Mean: [23.7778, 146.642, 212.222, 0]
 Eucledian: 30.2985
 Mean: [9.82716, 108.272, 160.889, 0]
 Eucledian: 29.0689
 Mean: [10.7654, 94.716, 130.272, 0]
 Eucledian: 13.7477
 Mean: [11.0864, 117.481, 177.358, 0]
 Eucledian: 7.81025
 Mean: [0.938272, 66.8395, 112.259, 0]
 Eucledian: 10.7238
 Mean: [12.5432, 107.383, 165.037, 0]
 Eucledian: 18.6815
 Mean: [25.1605, 130.531, 189.691, 0]
 Eucledian: 12.083
 Mean: [22.9259, 137.741, 200.148, 0]
 Eucledian: 16.3095
 Mean: [2.17284, 87.5679, 136.568, 0]
 Eucledian: 6.08276
 Mean: [0.938272, 43.642, 75.3457, 0]
 Eucledian: 12.083
 Mean: [3.01235, 107.086, 169.543, 0]
 Eucledian: 12.6886
 Mean: [1.24691, 63.0247, 102.444, 0]
 Eucledian: 3
 Mean: [0.987654, 51.321, 81.5185, 0]
 */


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

