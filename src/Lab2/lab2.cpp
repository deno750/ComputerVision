#include <iostream>
#include <dirent.h>
#include <string.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
int main() {
    DIR* dir;
    string path = "data/checkerboard_images/";
    dir = opendir(path.c_str());
    if (dir != NULL) {
        cout << "Directory found!\n";
    } else {
        cout << "Directory not found!\n";
    }
    struct dirent* image;
    
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    cv::Mat cameraMatrix, distCoeffs, rvecs, tvecs;
    cv::Mat mat;
    cv::Size chessSize(6, 5);
    
    std::vector<cv::Point3f> objc;
    
    //The origin (0,0,0) of the chessboard is located at the first square intersection in the top right.
    for (int y = 0; y < chessSize.height; ++y) {
        
        for (int x = 0; x < chessSize.width; ++x) {
            //0.11 m for each square in chessboard
            objc.push_back(cv::Point3f(x * 0.11, y * 0.11, 0));
        }
    }
    
    std::vector<std::string> imageNames;
    while ((image = readdir(dir)) != NULL) {
        
        string imagePath = path + image->d_name;
        mat = cv::imread(imagePath);
        if (mat.empty()) {
            continue;
        }
        std::vector<cv::Point2f> corners;
        bool patternFound = cv::findChessboardCorners(mat, chessSize, corners); //This function works only with grey scale images. In this homework, images are already in grey.
        
        if (patternFound) {
            imageNames.push_back(image->d_name);
            imagePoints.push_back(corners);
            objectPoints.push_back(objc);
        }
        
    }
    
    //Takes the size of the latest image because the images are all the same size.
    double cameraError = cv::calibrateCamera(objectPoints, imagePoints, mat.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
    cout << "Intrinsic coeffs:\n " << cameraMatrix << endl;
    cout << "Distortion coefficents:\n " << distCoeffs << endl;
    cout << "Camera error: " << cameraError << endl;
    
    
    double reprojectionError = 0;
    std::vector<cv::Point2f> reprojectedPoints;
    std::vector<double> errors;
    int totalPoints = 0;
    for (int i = 0; i < objectPoints.size(); ++i) {
        cv::projectPoints(objectPoints[i], rvecs.row(i), tvecs.row(i), cameraMatrix, distCoeffs, reprojectedPoints);
        double error = cv::norm(imagePoints[i], reprojectedPoints, cv::NORM_L2SQR);
        errors.push_back(error);
        reprojectionError += error;
        totalPoints += objectPoints[i].size();
    }
    
    double meanReprojectionError = sqrt(reprojectionError / totalPoints);
    cout << "Reprojection error: " << meanReprojectionError << endl;
    
    double minError, maxError;
    minError = errors.size();
    maxError = minError;
    int minIndex, maxIndex;
    for (int i = 0; i < errors.size(); ++i) {
        if (errors[i] < minError) {
            minError = errors[i];
            minIndex = i;
        }
        if (errors[i] > maxError) {
            maxError = errors[i];
            maxIndex = i;
        }
    }
    
    cout << "Image " << imageNames[minIndex] << " has got minimum error which is " << minError / objectPoints[minIndex].size() << endl;
    cout << "Image " << imageNames[maxIndex] << " has got maximum error which is " << maxError / objectPoints[maxIndex].size() << endl;
    
    cv::Mat R, map1, map2;
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, mat.size(), CV_32FC1, map1, map2);
    
    
    
    cv::Mat src, dst;
    src = cv::imread("data/test_image.png");
    if (src.empty()) {
        cout << "Image not found!\n";
    }
    cv::remap(src, dst, map1, map2, cv::INTER_LINEAR);
    cv::resize(src, src, cv::Size(src.cols / 3, src.rows / 3));
    cv::resize(dst, dst, cv::Size(dst.cols / 3, dst.rows / 3));
    
    
    cv::imshow("ORIGINAL", src);
    cv::imshow("REMAPPED", dst);
    cv::moveWindow("REMAPPED", src.cols, 0);
    cout << "\n=============================================" << endl;
    cout << "                      END                      " << endl;
     cout << "=============================================\n" << endl;
    cv::waitKey(0);
    return 0;
}
