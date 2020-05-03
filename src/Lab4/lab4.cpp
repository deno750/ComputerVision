#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define WINDOW_NAME "Image"
#define RHO 1
#define THETA 0.383972
#define THRESHOLD 103

using namespace std;

void onRhoTrackPad(int pos, void* data);
void onThetaTrackPad(int pos, void* data);
void onThresoldTrackpad(int pos, void* data);

cv::Mat drawLines(cv::Mat input, vector<cv::Vec2f> lines);
void drawPolygon(cv::Mat input, vector<cv::Vec2f> lines);

int threshold1 = 592;
int thresold2 = 372;

double rho = 1;
double theta = CV_PI/180;
int thresold = 1;

//Rho 1 Theta 0.383972 Thresold 103
struct images {
    cv::Mat input;
    cv::Mat canny;
};

int main() {
    
    cv::Mat input = cv::imread("input.png");
    if (input.empty()) {
        cout << "Unable to find image" << endl;
        return -1;
    }
    cv::Mat cannyInput;
    cv::Canny(input, cannyInput, threshold1, thresold2);
    cout << "Image size " << input.cols << " " << input.rows << endl;
    struct images imgs;
    imgs.input = input;
    imgs.canny = cannyInput;
    cv::imshow(WINDOW_NAME, cannyInput);
    int th1 = 1, th2 = 0, th3 = 0;
    /*cv::createTrackbar("Rho", WINDOW_NAME, &th1, 100, onRhoTrackPad, &imgs);
    cv::createTrackbar("Theta", WINDOW_NAME, &th2, 360, onThetaTrackPad, &imgs);
    cv::createTrackbar("Thresold", WINDOW_NAME, &th3, 1000, onThresoldTrackpad, &imgs);*/
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(cannyInput, lines, RHO, THETA, THRESHOLD);
    drawPolygon(input, lines);
    cv::imshow(WINDOW_NAME, input);
    cv::waitKey(0);
    return 0;
}

void onRhoTrackPad(int pos, void* data) {
    if (pos == 0) return;
    rho = (double) pos;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec2f> output;
    cv::HoughLines(canny, output, rho, theta, thresold);
    cout << "Rhos " << rho << " Theta " << theta << "Thresold " << thresold << endl;
    
    input = drawLines(input, output);
    cv::imshow(WINDOW_NAME, input);
    
}
void onThetaTrackPad(int pos, void* data) {
    if (pos == 0) return;
    theta = (double) pos * CV_PI / 180.0;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec2f> output;
    cv::HoughLines(canny, output, rho, theta, thresold);
    cout << "Rhos " << rho << " Theta " << theta << "Thresold " << thresold << endl;
    
    input = drawLines(input, output);
    cv::imshow(WINDOW_NAME, input);
}

void onThresoldTrackpad(int pos, void* data) {
    if (pos == 0) return;
    thresold = pos;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec2f> output;
    cv::HoughLines(canny, output, rho, theta, thresold);
    
    cout << "Rhos " << rho << " Theta " << theta << " Thresold " << thresold << endl;
    input = drawLines(input, output);
    cv::imshow(WINDOW_NAME, input);
}

cv::Mat drawLines(cv::Mat input, vector<cv::Vec2f> lines) {
    for(size_t i = 0; i < lines.size(); ++i) {
        double rho = lines[i][0];
        double theta = lines[i][1];
        
        
        
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        cout << pt1 << " " << pt2 << endl;
        cout << "x0 " << x0 << " y0 " << y0 << endl;
        cv::line(input, pt1, pt2, cv::Scalar(0,0,255));
    }
    return input;
}

void drawPolygon(cv::Mat input, vector<cv::Vec2f> lines) {
    vector<cv::Point> points;
    double rho1 = lines[0][0];
    double theta1 = lines[0][1];
    double a1 = cos(theta1), b1 = sin(theta1);
    
    double rho2 = lines[1][0];
    double theta2 = lines[1][1];
    double a2 = cos(theta2), b2 = sin(theta2);
    
    for (int x = 0; x < input.cols; ++x) {
        double eq1 = - (a1 / b1) * x + rho1 / b1;
        double eq2 = - (a2 / b2) * x + rho2 / b2;
        if (round(eq1) == round(eq2)) { //Finds intersection between two lines
            cv::Point point;
            point.x = x;
            point.y = round(eq1);
            points.push_back(point);
        }
        if (round(eq1) == input.rows || round(eq2) == input.rows) { //Finds intersection of the two lines with the bottom of the image
            cv::Point point;
            point.x = x;
            point.y = input.rows;
            points.push_back(point);
        }
    }
    cv::fillConvexPoly(input, points, cv::Scalar(0, 0, 255));
}
