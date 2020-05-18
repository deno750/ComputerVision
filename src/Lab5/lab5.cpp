#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "panoramic_utils.h"
#include "panoramic_image.h"

#define IMAGES_DATA "data/data"
#define LAB_19_AUTOMATIC "data/dataset_lab_19_automatic"
#define LAB_19_MANUAL "data/dataset_lab_19_manual"
#define DOLOMITES "data/dolomites"
#define KITCHEN "data/kitchen"

using namespace std;

int main() {
    vector<cv::String> images;
    string imagesDirectory = IMAGES_DATA;
    string imagesPattern;
    string pattern = "i*.png";
    double FOV = 66;
    int userRatio = 3;
    
    if (imagesDirectory == IMAGES_DATA || imagesDirectory == KITCHEN) {
        pattern = "i*.bmp";
    }
    
    if (imagesDirectory == DOLOMITES) {
        FOV = 54;
    }
    cv::utils::fs::glob(imagesDirectory, pattern, images);
    vector<cv::Mat> imagesMat;
    
    for (int i = 0; i < images.size(); ++i) {
        cv::String imagePath = images[i];
        cv::Mat mat = cv::imread(imagePath);
        imagesMat.push_back(mat);
    }
    PanoramicImage panoramicImage = PanoramicImage(imagesMat, FOV, userRatio, true, false);
    cv::Mat panoramic = panoramicImage.computeLandscape();
    cv::imshow("Panoramic with SIFT", panoramic);
    
    PanoramicImage panoramicImage2 = PanoramicImage(imagesMat, FOV, userRatio, false, false);
    cv::Mat panoramic2 = panoramicImage2.computeLandscape();
    cv::imshow("Panoramic with ORB", panoramic2);
    
    cv::waitKey(0);
    return 0;
}
