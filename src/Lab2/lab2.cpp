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
        cout << "Direcotry found!\n";
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
    for (int y = 1; y <= chessSize.height; ++y) {
        
        for (int x = 1; x <= chessSize.width; ++x) {
            objc.push_back(cv::Point3f(x * 0.11, y * 0.11, 0));
        }
    }
    
    for (int i = 0; i < objc.size(); ++i) {
        cout << objc[i] << " ";
    }
    cout << endl;
    
    while ((image = readdir(dir)) != NULL) {
        if (strcmp(image->d_name, ".") == 0 || strcmp(image->d_name, "..") == 0) {
            continue;
        }
        
        string imagePath = path + image->d_name;
        mat = cv::imread(imagePath);
        if (mat.empty()) {
            continue;
        }
        
        std::vector<cv::Point2f> corners;
        bool patternFound = cv::findChessboardCorners(mat, chessSize, corners); //This function works only with grey scale images. In this homework, images are already in grey.
        
        if (patternFound) {
            imagePoints.push_back(corners);
            objectPoints.push_back(objc);
            
        }
        
    }
    cv::calibrateCamera(objectPoints, imagePoints, mat.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
    cv::Mat R, map1, map2;
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, mat.size(), CV_32FC1, map1, map2);
    cv::Mat src, dst;
    cout << "map1" << endl;
    cout << map1 << endl;
    src = cv::imread("data/test_image.png");
    if (src.empty()) {
        cout << "Image not found!\n";
    }
    cv::remap(src, dst, map1, map2, cv::INTER_NEAREST);
    cv::imshow("ORIGINAL", src);
    cv::imshow("REMAPPED", dst);
    cv::waitKey(0);
    return 0;
}
