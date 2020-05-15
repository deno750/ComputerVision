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

int userRatio = 3;

int main() {
    vector<cv::String> images;
    string imagesDirectory = IMAGES_DATA;
    string imagesPattern;
    string pattern;
    if (imagesDirectory == IMAGES_DATA || imagesDirectory == KITCHEN) {
        pattern = "i*.bmp";
    } else {
        pattern = "i*.png";
    }
    
    double alpha = 0.0;
    if (imagesDirectory == DOLOMITES) {
        alpha = 27;
    } else {
        alpha = 33;
    }
    cv::utils::fs::glob(imagesDirectory, pattern, images);
    vector<cv::Mat> cylindricalProjImages;
    //cv::Ptr<cv::Feature2D> feature2d = cv::ORB::ORB::create();
    //cv::Ptr<cv::BFMatcher> bfMatcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
    cv::Ptr<cv::Feature2D> feature2d = cv::xfeatures2d::SIFT::create();
    cv::Ptr<cv::BFMatcher> bfMatcher = cv::BFMatcher::create(cv::NORM_L2, true);
    vector<vector<cv::KeyPoint>> keyPoints;
    for (int i = 0; i < images.size(); ++i) {
        cv::String imagePath = images[i];
        cv::Mat mat = cv::imread(imagePath);
        cv::Mat projected = PanoramicUtils::cylindricalProj(mat, alpha);
        cylindricalProjImages.push_back(projected);
    }
    /*for (int i = 0; i < cylindricalProjImages.size(); ++i) {
        cv::equalizeHist(cylindricalProjImages[i], cylindricalProjImages[i]);
    }*/
    
    feature2d->detect(cylindricalProjImages, keyPoints);
    vector<cv::Mat> descriptors;
    feature2d->compute(cylindricalProjImages, keyPoints, descriptors);
    vector<vector<cv::Point2f>> translations;
    for (int i = 0; i < cylindricalProjImages.size() - 1; ++i) {
        cv::Mat img1 = cylindricalProjImages[i];
        cv::Mat img2 = cylindricalProjImages[i+1];
        cv::Mat desc1 = descriptors[i];
        cv::Mat desc2 = descriptors[i+1];
        vector<cv::KeyPoint> keyPoint1 = keyPoints[i];
        vector<cv::KeyPoint> keyPoint2 = keyPoints[i+1];
        vector<cv::DMatch> matches;
        bfMatcher->match(desc1, desc2, matches);
        
        float min_distance = FLT_MAX;
        for (int i = 0; i < matches.size(); ++i) {
            if (matches[i].distance < min_distance) {
                min_distance = matches[i].distance;
            }
        }
        
        vector<cv::DMatch> goodMatches;
        for (int i = 0; i < matches.size(); ++i) {
            if (matches[i].distance <= userRatio * min_distance) {
                goodMatches.push_back(matches[i]);
            }
        }
        
        vector<cv::Point2f> points1, points2;
        for (int i = 0; i < goodMatches.size(); ++i) {
            points1.push_back(keyPoint1[goodMatches[i].queryIdx].pt);
            points2.push_back(keyPoint2[goodMatches[i].trainIdx].pt);
        }
        cv::Mat outImage;
        vector<int> mask;
        cv::Mat H = cv::findHomography(points1, points2, cv::RANSAC, 3, mask);
        cout << H << endl;
        vector<cv::Point2f> inlines1, inlines2;
        for (int i = 0; i < mask.size(); ++i) {
            if (mask[i] == 1) {
                inlines1.push_back(points1[i]);
                inlines2.push_back(points2[i]);
            }
        }
        cv::Point2f left, right;
        
        for (int i = 0; i < inlines1.size(); ++i) {
            left += keyPoint1[goodMatches[i].queryIdx].pt;
            right += keyPoint2[goodMatches[i].trainIdx].pt;
        }
        if (inlines1.size() > 0) {
            left /= static_cast<int>(inlines1.size());
            right /= static_cast<int>(inlines2.size());
        } else {
            left = cv::Point2f(img1.rows, img1.cols);
            right = cv::Point2f(img1.rows, img1.cols);
        }
       
        translations.push_back({left, right});
        
        cv::Mat img1copy = img1.clone();
        cv::Mat img2copy = img2.clone();
        cv::line(img1copy, cv::Point(left.x, 0), cv::Point(left.x, img1.rows), cv::Scalar(0, 0, 255));
        cv::line(img2copy, cv::Point(right.x, 0), cv::Point(right.x, img2.rows), cv::Scalar(0, 0, 255));
    
        cv::drawMatches(img1copy, keyPoint1, img2copy, keyPoint2, goodMatches, outImage);
        
        cv::imshow("Matches " + to_string(i), outImage);
    }
    cout << "Translations size " << translations.size() << endl;
    cv::Mat landscape(cylindricalProjImages[0].rows, 0, cylindricalProjImages[0].type());
    for (int i = 0; i < cylindricalProjImages.size(); ++i) {
        cv::Mat img = cylindricalProjImages[i];
        
        int rangeMin, rangeMax;
        if (i == 0) {
            rangeMin = 0;
            rangeMax = static_cast<int>(translations[i][0].x);
        } else if (i == cylindricalProjImages.size() - 1) {
            rangeMin = static_cast<int>(translations[i - 1][1].x);
            rangeMax = static_cast<int>(img.cols);
        } else {
            rangeMin = static_cast<int>(translations[i - 1][1].x);
            rangeMax = static_cast<int>(translations[i][0].x);
        }
        if (rangeMax < rangeMin) {
            rangeMin = 0;
            rangeMax = img.cols;
        }
        cv::Range rows = cv::Range(0, img.rows);
        cv::Range cols = cv::Range(rangeMin, rangeMax);
        cv::Mat cutted = img(rows, cols);
        cv::hconcat(landscape, cutted, landscape);
        cv::imshow("cutted", cutted);
        cout << "I " << i << endl;
    }
    //cv::equalizeHist(landscape, landscape);
    cv::namedWindow("Landscape", cv::WINDOW_NORMAL);
    cv::imshow("Landscape", landscape);
    cv::waitKey(0);
    return 0;
}
