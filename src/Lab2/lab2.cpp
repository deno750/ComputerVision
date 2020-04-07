#include <iostream>
#include <dirent.h>
#include <string.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
int main() {
    cout << "Hello world\n";
    DIR* dir;
    string path = "data/checkerboard_images/";
    dir = opendir(path.c_str());
    if (dir != NULL) {
        cout << "Ottimo\n";
    } else {
        cout << "Non Ottimo\n";
    }
    struct dirent* image;
    
    std::vector<std::vector<cv::Point3f>> objectPoints;
    std::vector<std::vector<cv::Point2f>> imagePoints;
    cv::Mat cameraMatrix, distCoeffs, rvecs, tvecs;
    cv::Mat mat;
    cv::Size chessSize(5, 6);
    while ((image = readdir(dir)) != NULL) {
        if (strcmp(image->d_name, ".") == 0 || strcmp(image->d_name, "..") == 0) {
            continue;
        }
        string imagePath = path + image->d_name;
        mat = cv::imread(imagePath);
        if (mat.empty()) {
            cout << "DIO CANE\n";
        }
        std::vector<cv::Point2f> corners;
        bool patternFound = cv::findChessboardCorners(mat, chessSize, corners); //This function works only with grey scale images. In this homeworks, images are already in grey. But if the images are not grey, we have to make the images in grey.
        
        if (patternFound) {
            imagePoints.push_back(corners);
            
            //MARK: Not sure if it's correct. Leave it as it is for now
            std::vector<cv::Point3f> objc;
            for (int i = 0; i < corners.size(); ++i) {
                cv::Point3f point = cv::Point3f(corners[i].x, corners[i].y, 0);
                objc.push_back(point);
            }
            objectPoints.push_back(objc);
            
        }
        
    }
    cv::calibrateCamera(objectPoints, imagePoints, cv::Size(mat.cols, mat.rows), cameraMatrix, distCoeffs, rvecs, tvecs);
    return 0;
}
