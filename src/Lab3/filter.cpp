#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "filter.h"

//using namespace cv;

// constructor
Filter::Filter(cv::Mat input_img, int size) {

    input_image = input_img;
    if (size % 2 == 0)
        size++;
    filter_size = size;
}

// for base class do nothing (in derived classes it performs the corresponding filter)
void Filter::doFilter() {

    // it just returns a copy of the input image
    result_image = input_image.clone();

}

// get output of the filter
cv::Mat Filter::getResult() {

    return result_image;
}

//set window size (it needs to be odd)
void Filter::setSize(int size) {

    if (size % 2 == 0)
        size++;
    filter_size = size;
}

//get window size
int Filter::getSize() {

    return filter_size;
}



// Write your code to implement the Gaussian, median and bilateral filters

MedianFilter::MedianFilter(cv::Mat input_img, int filter_size): Filter(input_img, filter_size) {
    
}

void MedianFilter::doFilter() {
    //float scalar = filter_size * filter_size;
    //cv::Mat kernel = cv::Mat::ones(filter_size, filter_size, CV_32F) / scalar; //Not median just a normalized box
    //cv::filter2D(input_image, result_image, -1, kernel);
    cv::medianBlur(input_image, result_image, filter_size);
}

GaussianFilter::GaussianFilter(cv::Mat input_img, int filter_size, double sigma): Filter(input_img, filter_size) {
    this->sigma = sigma;
}

void GaussianFilter::setSigma(double sigma) {
    this->sigma = sigma;
}

void GaussianFilter::doFilter() {
    cv::GaussianBlur(input_image, result_image, cv::Size(filter_size, filter_size), sigma);
}

BilateralFilter::BilateralFilter(cv::Mat input_img, int filter_size, double sigma_range, double sigma_space): Filter(input_img, filter_size) {
    this->sigma_range = sigma_range;
    this->sigma_space = sigma_space;
}

void BilateralFilter::setSigmaRange(double sigmaRange) {
    this->sigma_range = sigmaRange;
}

void BilateralFilter::setSigmaSpace(double sigmaSpace) {
    this->sigma_space = sigmaSpace;
}

void BilateralFilter::doFilter() {
    cv::bilateralFilter(input_image, result_image, filter_size, sigma_range, sigma_space);
}
