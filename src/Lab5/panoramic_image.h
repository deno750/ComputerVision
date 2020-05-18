#ifndef LAB5__PANORAMIC__IMAGE__H
#define LAB5__PANORAMIC__IMAGE__H

#include <memory>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/ccalib.hpp>
#include <opencv2/stitching.hpp>

#include "panoramic_utils.h"

using namespace std;
class PanoramicImage {
public:
    PanoramicImage(vector<cv::Mat> images, double FOV, int ratio, bool useSIFT = true, bool equalizeImages = false);
    cv::Mat computeLandscape();
    
private:
    int ratio;
    bool useSIFT;
    vector<cv::Mat> cylindricalImages;
    vector<vector<cv::KeyPoint>> keyPoints;
    vector<cv::Mat> descriptors;
    vector<vector<cv::Point2f>> translations;
    cv::Mat computeCylindricReprojection(cv::Mat input, double FOV);
    void computeFeatures();
    cv::Mat computeMerge();

};


#endif // LAB5__PANORAMIC__IMAGE__H
