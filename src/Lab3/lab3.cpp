#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "filter.h"

#define ORIGINAL_IMAGE_WINDOW "Original image"
#define WRONG_EQUALIZATION_WINDOW "Wrong Equalized Image"
#define CORRECT_EQUALIZATION_WINDOW "Correct Equalization Image"

void showHistogram(std:: string* wname, std::vector<cv::Mat>& hists);
void onMedianTrackPad(int pos, void* data);
void onGaussianTrackPad(int pos, void* data);
void onBilinearTrackPad(int pos, void* data);

using namespace std;
int main() {
    
    cv::Mat mat = cv::imread("data/image.jpg");
    if (mat.empty()) {
        cout << "Unable to find image" << endl;
        return -1;
    }
    cv::resize(mat, mat, cv::Size(mat.cols / 3, mat.rows / 3));
    cv::imshow(ORIGINAL_IMAGE_WINDOW, mat);
    vector<cv::Mat> channelVectors;
    cv::split(mat, channelVectors);
    
    const int channels[] = {0, 1, 2};
    int histSize = 256;
    float range[] = {0, 255};
    const float* histRanges = { range };
    cv::Mat bHist, gHist, rHist;
    cv::calcHist(&channelVectors[0], 1, channels, cv::Mat(), bHist, 1, &histSize, &histRanges);
    cv::calcHist(&channelVectors[1], 1, channels, cv::Mat(), gHist, 1, &histSize, &histRanges);
    cv::calcHist(&channelVectors[2], 1, channels, cv::Mat(), rHist, 1, &histSize, &histRanges);
    vector<cv::Mat> hists = {bHist, gHist, rHist};
    std::string originalWindowNames[] = {"original B", "original G", "original R"};
    showHistogram(originalWindowNames, hists);
    
    cv::Mat equalizedB, equalizedG, equalizedR;
    cv::equalizeHist(channelVectors[0], equalizedB);
    cv::equalizeHist(channelVectors[1], equalizedG);
    cv::equalizeHist(channelVectors[2], equalizedR);
    vector<cv::Mat> equalizedHists = {equalizedB, equalizedG, equalizedR};
    cv::Mat mergedImage;
    cv::merge(equalizedHists, mergedImage);
    cv::imshow(WRONG_EQUALIZATION_WINDOW, mergedImage);
    
    cv::calcHist(&equalizedB, 1, channels, cv::Mat(), bHist, 1, &histSize, &histRanges);
    cv::calcHist(&equalizedG, 1, channels, cv::Mat(), gHist, 1, &histSize, &histRanges);
    cv::calcHist(&equalizedR, 1, channels, cv::Mat(), rHist, 1, &histSize, &histRanges);
    hists = {bHist, gHist, rHist};
    std::string mergedRGBWindowNames[] = {"Wrong B", "Wrong G", "Wrong R"};
    showHistogram(mergedRGBWindowNames, hists);
    //============== Ended points 1, 2, 3, 4 ===================
    
    cv::Mat hsv;
    cv::cvtColor(mat, hsv, cv::COLOR_BGR2HSV);
    vector<cv::Mat> hsvSplit;
    cv::split(hsv, hsvSplit);
    
    cv::Mat eqV;
    //The best equalized image is obtained equalizing the V channel
    cv::equalizeHist(hsvSplit[2], eqV);
    cv::Mat mergedHsv;
    vector<cv::Mat> equalizedHSV = {hsvSplit[0], hsvSplit[1], eqV};
    cv::merge(equalizedHSV, mergedHsv);
    cv::cvtColor(mergedHsv, mergedHsv, cv::COLOR_HSV2BGR);
    cv::imshow(CORRECT_EQUALIZATION_WINDOW, mergedHsv);
    
    cv::split(mergedHsv, hsvSplit);
    
    cv::Mat hHist, sHist, vHist;
    cv::calcHist(&hsvSplit[0], 1, channels, cv::Mat(), hHist, 1, &histSize, &histRanges);
    cv::calcHist(&hsvSplit[1], 1, channels, cv::Mat(), sHist, 1, &histSize, &histRanges);
    cv::calcHist(&hsvSplit[2], 1, channels, cv::Mat(), vHist, 1, &histSize, &histRanges);
    
    vector<cv::Mat> hsvHists = {hHist, sHist, vHist};
    std::string correctEqualizedWindowNames[] = {"correct B", "correct G", "correct R"};
    showHistogram(correctEqualizedWindowNames, hsvHists);
    //============= Ended point 5 ================
    
    
    int medianTrackPadValue = 0;
    int gaussianTrackPadValue = 0;
    int bilinearTrackPadValue = 0;
    MedianFilter medianFilter = MedianFilter(mergedHsv, 3);
    cv::createTrackbar("Median", CORRECT_EQUALIZATION_WINDOW, &medianTrackPadValue, 21, onMedianTrackPad, (void *) &medianFilter);
    cv::createTrackbar("Gaussian", CORRECT_EQUALIZATION_WINDOW, &gaussianTrackPadValue, 20, onGaussianTrackPad, (void *) &mergedHsv);
    cv::createTrackbar("Bilateral", CORRECT_EQUALIZATION_WINDOW, &bilinearTrackPadValue, 20, onBilinearTrackPad, (void *) &mergedHsv);
    cv::waitKey(0);
    return 0;
}

// hists = vector of 3 cv::mat of size nbins=256 with the 3 histograms
// e.g.: hists[0] = cv:mat of size 256 with the red histogram
//       hists[1] = cv:mat of size 256 with the green histogram
//       hists[2] = cv:mat of size 256 with the blue histogram
void showHistogram(std:: string* wname, std::vector<cv::Mat>& hists) {
    // Min/Max computation
    double hmax[3] = {0,0,0};
    double min;
    cv::minMaxLoc(hists[0], &min, &hmax[0]);
    cv::minMaxLoc(hists[1], &min, &hmax[1]);
    cv::minMaxLoc(hists[2], &min, &hmax[2]);

    //std::string wname[3] = { "blue", "green", "red" };
    cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                             cv::Scalar(0,0,255) };

    std::vector<cv::Mat> canvas(hists.size());

    // Display each histogram in a canvas
    for (int i = 0, end = hists.size(); i < end; i++) {
      canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

      for (int j = 0, rows = canvas[i].rows; j < hists[0].rows-1; j++) {
        cv::line(
              canvas[i],
              cv::Point(j, rows),
              cv::Point(j, rows - (hists[i].at<float>(j) * rows/hmax[i])),
              hists.size() == 1 ? cv::Scalar(200,200,200) : colors[i],
              1, 8, 0
              );
      }

      cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
    }
}

void onMedianTrackPad(int pos, void* data) {
    MedianFilter filter = *(MedianFilter *) data;
    filter.setSize(pos);
    filter.doFilter();
    cv::imshow(CORRECT_EQUALIZATION_WINDOW, filter.getResult());
}

void onGaussianTrackPad(int pos, void* data) {
    
}

void onBilinearTrackPad(int pos, void* data) {
    
}

