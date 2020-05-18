#include "panoramic_image.h"

PanoramicImage::PanoramicImage(vector<cv::Mat> images, double FOV, int ratio, bool useSIFT, bool equalizeImages) {
    for (int i = 0; i < images.size(); ++i) {
        cv::Mat reprojected = computeCylindricReprojection(images[i], FOV);
        if (equalizeImages) {
            cv::equalizeHist(reprojected, reprojected);
        }
        cylindricalImages.push_back(reprojected);
    }
    this->ratio = ratio;
    this->useSIFT = useSIFT;
}

cv::Mat PanoramicImage::computeCylindricReprojection(cv::Mat input, double FOV) {
    return PanoramicUtils::cylindricalProj(input, FOV / 2);
}

void PanoramicImage::computeFeatures() {
    cv::Ptr<cv::Feature2D> feature2d;
    cv::Ptr<cv::BFMatcher> bfMatcher;
    if (useSIFT) {
        feature2d = cv::xfeatures2d::SIFT::create();
        bfMatcher = cv::BFMatcher::create(cv::NORM_L2, true);
    } else {
        feature2d = cv::ORB::ORB::create();
        bfMatcher = cv::BFMatcher::create(cv::NORM_HAMMING, true);
    }
    keyPoints.clear();
    descriptors.clear();
    translations.clear();
    
    feature2d->detect(cylindricalImages, keyPoints);
    feature2d->compute(cylindricalImages, keyPoints, descriptors);
    
    for (int i = 0; i < cylindricalImages.size() - 1; ++i) {
        
        cv::Mat img1 = cylindricalImages[i];
        cv::Mat img2 = cylindricalImages[i+1];
        
        cv::Mat desc1 = descriptors[i];
        cv::Mat desc2 = descriptors[i+1];
        
        vector<cv::KeyPoint> keyPoint1 = keyPoints[i];
        vector<cv::KeyPoint> keyPoint2 = keyPoints[i+1];
        
        //Compute the matches
        vector<cv::DMatch> matches;
        bfMatcher->match(desc1, desc2, matches);
        
        //Seeking the minimum distance from among the matches found
        float min_distance = FLT_MAX;
        for (int i = 0; i < matches.size(); ++i) {
            if (matches[i].distance < min_distance) {
                min_distance = matches[i].distance;
            }
        }
        
        //Seeking the good matches
        vector<cv::DMatch> goodMatches;
        for (int i = 0; i < matches.size(); ++i) {
            if (matches[i].distance <= ratio * min_distance) {
                goodMatches.push_back(matches[i]);
            }
        }
        
        //Add the points which result in goodMatches
        vector<cv::Point2f> points1, points2;
        for (int i = 0; i < goodMatches.size(); ++i) {
            points1.push_back(keyPoint1[goodMatches[i].queryIdx].pt);
            points2.push_back(keyPoint2[goodMatches[i].trainIdx].pt);
        }
    
        //Seeking the inlines
        vector<int> mask;
        cv::findHomography(points1, points2, cv::RANSAC, 3, mask);
        vector<cv::Point2f> inlines1, inlines2;
        for (int i = 0; i < mask.size(); ++i) {
            if (mask[i] == 1) { //The inlines are the points which has the flag 1 in mask
                inlines1.push_back(points1[i]);
                inlines2.push_back(points2[i]);
            }
        }
        
        //Computing the points mean for both images
        cv::Point2f left, right; //Variables which stores correspondigly the mean of the poins on the left and right image.
        for (int i = 0; i < inlines1.size(); ++i) {
            left += keyPoint1[goodMatches[i].queryIdx].pt;
            right += keyPoint2[goodMatches[i].trainIdx].pt;
        }
        if (inlines1.size() > 0) {
            left /= static_cast<int>(inlines1.size());
            right /= static_cast<int>(inlines2.size());
        } else { //When no inlines are here, we add the poins which corresponts to the image size in order to avoid any crash during the merge.
            left = cv::Point2f(img1.rows, img1.cols);
            right = cv::Point2f(img1.rows, img1.cols);
        }
       
        translations.push_back({left, right});
        
        //Uncomment this block if you want to see the matches visually of the two images
        /*
        cv::Mat outImage;
        cv::Mat img1copy = img1.clone();
        cv::Mat img2copy = img2.clone();
        cv::line(img1copy, cv::Point(left.x, 0), cv::Point(left.x, img1.rows), cv::Scalar(0, 0, 255));
        cv::line(img2copy, cv::Point(right.x, 0), cv::Point(right.x, img2.rows), cv::Scalar(0, 0, 255));
    
        cv::drawMatches(img1copy, keyPoint1, img2copy, keyPoint2, goodMatches, outImage);
        
        cv::imshow("Matches " + to_string(i), outImage);*/
    }
}

cv::Mat PanoramicImage::computeMerge() {
    cv::Mat merged(cylindricalImages[0].rows, 0, cylindricalImages[0].type());
    for (int i = 0; i < cylindricalImages.size(); ++i) {
        cv::Mat img = cylindricalImages[i];
        
        int rangeMin, rangeMax;
        
        if (i == 0) { //First image so just cut it on the right side
            rangeMin = 0;
            cv::Point2f endCutImagePoint = translations[i][0];
            rangeMax = static_cast<int>(endCutImagePoint.x);
        } else if (i == cylindricalImages.size() - 1) { //Last image so just cut to the left side
            cv::Point2f startCutImagePoint = translations[i - 1][1];
            rangeMin = static_cast<int>(startCutImagePoint.x);
            rangeMax = static_cast<int>(img.cols);
        } else { //Cut on both sizes of the image
            cv::Point2f endCutMedianPoint = translations[i][0];
            cv::Point2f startCutMedianPoint = translations[i - 1][1];
            rangeMin = static_cast<int>(startCutMedianPoint.x);
            rangeMax = static_cast<int>(endCutMedianPoint.x);
        }
        if (rangeMax <= rangeMin) {
            rangeMin = 0;
            rangeMax = img.cols;
        }
        cv::Range rows = cv::Range(0, img.rows);
        cv::Range cols = cv::Range(rangeMin, rangeMax);
        cv::Mat cutted = img(rows, cols);
        cv::hconcat(merged, cutted, merged);
    }
    return merged;
}

cv::Mat PanoramicImage::computeLandscape() {
    computeFeatures();
    return computeMerge();
}
