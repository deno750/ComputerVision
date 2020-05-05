#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#define CANNY_HOUGLINE_WINDOW "Canny Houghline Window"
#define CANNY_HOUGCIRCLE_WINDOW "Canny HoughCircle Window"
#define HOUGH_LINE_TUNING_WINDOW "Houghline tuning window"
#define ELABORATED_IMAGE_WINDOW_NAME "Elaborated image"
#define HOUGH_CIRCLE_TUNING_WINDOW "HoughCircle tuning window"

#define RHO 1
#define THETA 0.383972
#define HOUGH_LINE_THRESHOLD 103

#define CANNY_THRESHOLD_1 592
#define CANNY_THRESHOLD_2 372

using namespace std;

void onHouglinesCannyThreshold1TrackPad(int pos, void* data);
void onHouglinesCannyThreshold2TrackPad(int pos, void* data);
void onHougCirlesCannyThreshold1TrackPad(int pos, void* data);
void onHougCirclesCannyThreshold2TrackPad(int pos, void* data);
void onRhoTrackPad(int pos, void* data);
void onThetaTrackPad(int pos, void* data);
void onThresoldTrackpad(int pos, void* data);
void onHoughCircleParam1Trackpad(int pos, void* data);
void onHoughCircleParam2TrackPad(int pos, void* data);

cv::Mat drawLines(cv::Mat input, vector<cv::Vec2f> lines);
void drawPolygon(cv::Mat input, vector<cv::Vec2f> lines);
void drawCircles(cv::Mat input, vector<cv::Vec3f> circles);

int cannyLinethreshold1 = CANNY_THRESHOLD_1;
int cannyLinethreshold2 = CANNY_THRESHOLD_2;

int cannyCirclethreshold1 = 1309;
int cannyCirclethreshold2 = 649;

//Hough lines tuning prameters
double rho = RHO;
double theta = THETA;
int thresold = HOUGH_LINE_THRESHOLD;

//Hough circles tuning parameters
int houghCircleParam1 = 200;
int houghCircleParam2 = 10;

struct images {
    cv::Mat input;
    cv::Mat canny;
};

int main() {
    
    cv::Mat input = cv::imread("input.png");
    cv::Mat image = input.clone();
    if (image.empty()) {
        cout << "Unable to find image" << endl;
        return -1;
    }
    cv::Mat cannyLinesInput;
    cv::Canny(image, cannyLinesInput, cannyLinethreshold1, cannyLinethreshold2);
    cout << "Image size " << image.cols << " " << image.rows << endl;
    struct images houghLinesImages;
    houghLinesImages.input = input;
    houghLinesImages.canny = cannyLinesInput;
    cv::imshow(CANNY_HOUGLINE_WINDOW, cannyLinesInput);
    cv::createTrackbar("Threshold 1", CANNY_HOUGLINE_WINDOW, &cannyLinethreshold1, 2000, onHouglinesCannyThreshold1TrackPad, &houghLinesImages);
    cv::createTrackbar("Threshold 2", CANNY_HOUGLINE_WINDOW, &cannyLinethreshold2, 2000, onHouglinesCannyThreshold2TrackPad, &houghLinesImages);
    int th1 = rho, th2 = (int) (theta * 180.0 / CV_PI), th3 = thresold;
    cv::imshow(HOUGH_LINE_TUNING_WINDOW, image);
    cv::createTrackbar("HoughLines Rho", HOUGH_LINE_TUNING_WINDOW, &th1, 100, onRhoTrackPad, &houghLinesImages);
    cv::createTrackbar("HoughLines Theta", HOUGH_LINE_TUNING_WINDOW, &th2, 360, onThetaTrackPad, &houghLinesImages);
    cv::createTrackbar("HoughLines Thresold", HOUGH_LINE_TUNING_WINDOW, &th3, 1000, onThresoldTrackpad, &houghLinesImages);
    std::vector<cv::Vec2f> lines;
    cv::HoughLines(cannyLinesInput, lines, rho, theta, thresold);
    cv::Mat lineImage = image.clone();
    drawLines(lineImage, lines);
    cv::imshow(HOUGH_LINE_TUNING_WINDOW, lineImage);
    drawPolygon(image, lines);
    
    cv::Mat cannyCircleImage;
    cv::Canny(input, cannyCircleImage, cannyCirclethreshold1, cannyCirclethreshold2);
    cv::imshow(CANNY_HOUGCIRCLE_WINDOW, cannyCircleImage);
    struct images houghCircleImages;
    houghCircleImages.input = input;
    houghCircleImages.canny = cannyCircleImage;
    cv::createTrackbar("Threshold 1", CANNY_HOUGCIRCLE_WINDOW, &cannyCirclethreshold1, 2000, onHougCirlesCannyThreshold1TrackPad, &houghCircleImages);
    cv::createTrackbar("Threshold 2", CANNY_HOUGCIRCLE_WINDOW, &cannyCirclethreshold2, 2000, onHougCirclesCannyThreshold2TrackPad, &houghCircleImages);
    cv::imshow(HOUGH_CIRCLE_TUNING_WINDOW, input);
    cv::createTrackbar("Param1", HOUGH_CIRCLE_TUNING_WINDOW, &houghCircleParam1, 300, onHoughCircleParam1Trackpad, &houghCircleImages);
    cv::createTrackbar("Param2", HOUGH_CIRCLE_TUNING_WINDOW, &houghCircleParam2, 100, onHoughCircleParam2TrackPad, &houghCircleImages);
    vector<cv::Vec3f> circles;
    cv::HoughCircles(cannyCircleImage, circles, cv::HOUGH_GRADIENT, 1, cannyLinesInput.rows / 1, houghCircleParam1, houghCircleParam2 );
    cv::Mat circleImage = input.clone();
    drawCircles(circleImage, circles);
    drawCircles(image, circles);
    
    cv::imshow(HOUGH_CIRCLE_TUNING_WINDOW, circleImage);
    cv::imshow(ELABORATED_IMAGE_WINDOW_NAME, image);
    cv::waitKey(0);
    return 0;
}

void onHouglinesCannyThreshold1TrackPad(int pos, void* data) {
    struct images* imgs= (struct images *) data;
    cv::Mat input = imgs->input;
    cv::Mat canny;
    cv::Canny(input, canny, cannyLinethreshold1, cannyLinethreshold2);
    imgs->canny = canny;
    cv::imshow(CANNY_HOUGLINE_WINDOW, canny);
    cout << "Threshold1 " << cannyLinethreshold1 << " Threshold2 " << cannyLinethreshold2 << endl;
}
void onHouglinesCannyThreshold2TrackPad(int pos, void* data) {
    struct images* imgs= (struct images *) data;
    cv::Mat input = imgs->input;
    cv::Mat canny;
    cv::Canny(input, canny, cannyLinethreshold1, cannyLinethreshold2);
    imgs->canny = canny;
    cv::imshow(CANNY_HOUGLINE_WINDOW, canny);
    cout << "Threshold1 " << cannyLinethreshold1 << " Threshold2 " << cannyLinethreshold2 << endl;
}

void onHougCirlesCannyThreshold1TrackPad(int pos, void* data) {
    struct images* imgs= (struct images *) data;
    cv::Mat input = imgs->input;
    cv::Mat canny;
    cv::Canny(input, canny, cannyCirclethreshold1, cannyCirclethreshold2);
    imgs->canny = canny;
    cv::imshow(CANNY_HOUGCIRCLE_WINDOW, canny);
    cout << "Circle Threshold1 " << cannyCirclethreshold1 << " Circle Threshold2 " << cannyCirclethreshold2 << endl;
}
void onHougCirclesCannyThreshold2TrackPad(int pos, void* data) {
    struct images* imgs= (struct images *) data;
    cv::Mat input = imgs->input;
    cv::Mat canny;
    cv::Canny(input, canny, cannyCirclethreshold1, cannyCirclethreshold2);
    imgs->canny = canny;
    cv::imshow(CANNY_HOUGCIRCLE_WINDOW, canny);
    cout << "Circle Threshold1 " << cannyCirclethreshold1 << " Circle Threshold2 " << cannyCirclethreshold2 << endl;
}

void onRhoTrackPad(int pos, void* data) {
    if (pos == 0) return;
    rho = (double) pos;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec2f> output;
    cv::HoughLines(canny, output, rho, theta, thresold);
    cout << "Rhos " << rho << " Theta " << theta << " Thresold " << thresold << endl;
    
    input = drawLines(input, output);
    cv::imshow(HOUGH_LINE_TUNING_WINDOW, input);
    
}
void onThetaTrackPad(int pos, void* data) {
    if (pos == 0) return;
    theta = (double) pos * CV_PI / 180.0;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec2f> output;
    cv::HoughLines(canny, output, rho, theta, thresold);
    cout << "Rhos " << rho << " Theta " << theta << " Thresold " << thresold << endl;
    
    input = drawLines(input, output);
    cv::imshow(HOUGH_LINE_TUNING_WINDOW, input);
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
    cv::imshow(HOUGH_LINE_TUNING_WINDOW, input);
}

void onHoughCircleParam1Trackpad(int pos, void* data) {
    if (pos == 0) return;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(canny, circles, cv::HOUGH_GRADIENT, 1, input.rows, houghCircleParam1, houghCircleParam2);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         circle( input, center, radius, cv::Scalar(0,255,0), cv::FILLED, 8, 0 );
    }
    cv::imshow(HOUGH_CIRCLE_TUNING_WINDOW, input);
    cout << "Hough circle Param1 " << houghCircleParam1 << " Param2 " << houghCircleParam2 << endl;
}
void onHoughCircleParam2TrackPad(int pos, void* data) {
    if (pos == 0) return;
    struct images imgs = *(struct images *) data;
    cv::Mat input = imgs.input.clone();
    cv::Mat canny = imgs.canny.clone();
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(canny, circles, cv::HOUGH_GRADIENT, 1, input.rows, houghCircleParam1, houghCircleParam2);
    for( size_t i = 0; i < circles.size(); i++ )
    {
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
        
         circle( input, center, radius, cv::Scalar(0,255,0), cv::FILLED, 8, 0 );
    }
    cv::imshow(HOUGH_CIRCLE_TUNING_WINDOW, input);
    cout << "Hough circle Param1 " << houghCircleParam1 << " Param2 " << houghCircleParam2 << endl;
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

void drawCircles(cv::Mat input, vector<cv::Vec3f> circles) {
    for( size_t i = 0; i < circles.size(); i++ )
    {
         cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
         
         circle(input, center, radius, cv::Scalar(0,255,0), cv::FILLED, 8, 0 );
    }
}
