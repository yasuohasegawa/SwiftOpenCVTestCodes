//
//  OpenCVUtils.cpp
//  openCV_test
//
//  Created by Yasuo Hasegawa on 2021/12/02.
// note: The color of the iOS needs 4 channels.
// Use the syslog as the NSLog. Before using it, please include the <sys/syslog.h>

#include "OpenCVUtils.hpp"
#include <string>
#include <sys/syslog.h>
using namespace cv;
using namespace std;

Mat OpenCVUtils::detect(Mat image) {
    //return detect_lines(image);
    return detect_circles(image);
    //syslog(LOG_WARNING, "log string");
}

Mat OpenCVUtils::detect_lines(Mat image) {
    
    Mat edgesOnly = edges(image,50,120);
    
    vector<Vec4i> lines;
    int minLineLength = 100;
    int maxLineGap = 10;
    HoughLinesP(edgesOnly, lines, 1, CV_PI/180, 100, minLineLength, maxLineGap);
    
    return draw_lines(image, lines);
}

Mat OpenCVUtils::draw_lines(Mat image, vector<Vec4i> lines) {
    for (int i=0; i<lines.size(); i++) {
        Vec4i l = lines[i];
        double x1 = l[0];
        double y1 = l[1];
        double x2 = l[2];
        double y2 = l[3];
        
        Scalar lineColor(0,255,0,255);
        line(image, Point(x1,y1), Point(x2,y2), lineColor, 2);
    }

    putText(image,
            "cols:"+to_string(image.cols)+", rows:"+to_string(image.rows),
            Point(20, 50), // location
            FONT_HERSHEY_SIMPLEX, // font
            2.0, // font size
            cv::Scalar(255, 0, 0,255),
            2, // thickness
            8); // line type
    
    return image;
}

Mat OpenCVUtils::detect_circles(Mat image) {
    vector<Vec3f> circles;
    
    Mat gray = grayscale(image);
    gray = blur(gray, Size(9,9));
    
    int dp = 2;
    double minDist = gray.rows/6.0;
    
    HoughCircles(gray, circles, HOUGH_GRADIENT, dp, minDist, 100, 200, 50, 150);
    
    return draw_circles(image, circles);
}

Mat OpenCVUtils::draw_circles(Mat image, vector<Vec3f> circles) {
    for( int i = 0; i < circles.size(); i++ )
    {
        if(i < 10){
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            circle( image, center, 3, Scalar(0,255,0, 255), -1, 8, 0 );
            circle( image, center, radius, Scalar(0,0,255, 255), 3, 8, 0 );
        }
    }
    
    return image;
}

Mat OpenCVUtils::detect_motion(Mat image, Mat prevImage) {
    
    Mat gray = grayscale(image);
    gray = blur(gray,Size(21,21));
    //syslog(LOG_WARNING, "detect_motion1: %i", prevImage.rows);
    //syslog(LOG_WARNING, "detect_motion2: %i", prevImage.size());
    
    if(prevImage.rows == 0){
        return image;
    }
    
    Mat gray2 = grayscale(prevImage);
    gray2 = blur(gray2,Size(21, 21));
    
    /* // example of using the accumulateWeighted
     Mat acc(gray2.rows, gray2.cols,CV_32FC(gray2.channels())); // We need to use the same number of channels as input image.
     accumulateWeighted(gray, acc, 0.00001);
     */
    
    Mat frameDelta;
    absdiff(gray, gray2, frameDelta);
    
    Mat thresh;
    threshold(frameDelta, thresh, 25, 255, THRESH_BINARY);
    
    dilate(thresh, thresh, Mat(), Point(-1,-1), 2);
    
    vector<vector<Point> > contours;
    findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    for(int i = 0; i< contours.size(); i++) {
        if(contourArea(contours[i]) < 500) {
            continue;
        }
        
        Rect rect = boundingRect(contours[i]);
        rectangle(image, Point(rect.x,rect.y), Point(rect.x+rect.width,rect.y+rect.height), Scalar(0, 0, 255, 255), 3, 8, 0);
        
        putText(image,
                "Motion Detected",
                Point(20, 50), // location
                FONT_HERSHEY_SIMPLEX, // font
                2.0, // font size
                cv::Scalar(255, 0, 0,255),
                2, // thickness
                8); // line type
    }
    
    return image;
}

Mat OpenCVUtils::blur(Mat image, Size s) {
    Mat blur;
    GaussianBlur(image, blur, s, 0);
    return blur;
}

Mat OpenCVUtils::grayscale(Mat image) {
    Mat greyScaledImage;
    cvtColor(image, greyScaledImage, COLOR_BGR2GRAY);
    return greyScaledImage;
}

Mat OpenCVUtils::edges(Mat image, double min, double max) {
    Mat greyScaledImage = grayscale(image); // to gray scale color
    Mat edgedOnlyImage;
    Canny(greyScaledImage, edgedOnlyImage, min, max); // do the Canny edge detection and get the result image
    return edgedOnlyImage;
}

// rotation image
Mat OpenCVUtils::rotated_image(Mat image, float angle) {
    Point2f center((image.cols - 1) / 2.0, (image.rows - 1) / 2.0);
    Mat rotation_matix = getRotationMatrix2D(center, angle, 1.0); // pivot with angle set up
    Mat rotated_image;
    
    // apply affine transformation
    warpAffine(image, rotated_image, rotation_matix, image.size());
    
    return rotated_image;
}

// translate image
Mat OpenCVUtils::translated_image(Mat image, float tx, float ty) {
    float warp_values[] = { 1.0, 0.0, tx, 0.0, 1.0, ty };
    Mat translation_matrix = Mat(2, 3, CV_32F, warp_values);
    Mat translatedImage;
    
    // apply affine transformation
    warpAffine(image, translatedImage, translation_matrix, image.size());
    return translatedImage;
}

Mat OpenCVUtils::resized_image(Mat image, int w, int h) {
    Mat resizedImage;
    resize(image, resizedImage, Size(w, h), INTER_LINEAR);
    return resizedImage;
}

Mat OpenCVUtils::customFilter(Mat image) {
    Mat kernel1 = (Mat_<double>(3,3) << 0, 0, 0,
                                        0, 1, 0,
                                        0, 0, 0);
    Mat res;
    //filter2D(image, res, -1 , kernel1, Point(-1, -1), 0, 4);
    
    // Initialize matrix with all ones 6x4
    Mat kernel2 = Mat::ones(5,5, CV_64F);
    
    // Normalize the elements
    kernel2 = kernel2 / 25;
    //filter2D(image, res, -1 , kernel2, Point(-1, -1), 0, 4);
    
    // sharpening
    Mat kernel3 = (Mat_<double>(3,3) << 0, -1,  0,
                                       -1,  5, -1,
                                        0, -1, 0);
    filter2D(image, res, -1 , kernel3, Point(-1, -1), 0, BORDER_DEFAULT);

    
    return res;
}


/* test codes */

// draw shapes
void OpenCVUtils::testDrawShapes(Mat image) {
    // draw circle
    Point circle_center(620,100);
    int radius = 100;
    circle(image, circle_center, radius, Scalar(0, 0, 255, 255), 3, 8, 0);
    
    // draw rect
    Point start_point(300,115);
    Point end_point(475,225);
    rectangle(image, start_point, end_point, Scalar(0, 0, 255, 255), 3, 8, 0);
}

// In iOS, it's 4 channels.
void OpenCVUtils::testPixelControl(Mat image) {
    for(int row=0; row<image.rows; row++)
    {
      for(int col=0; col<image.cols; col++)
      {
          int R = image.at<cv::Vec4b>(row, col)[0];
          int G = image.at<cv::Vec4b>(row, col)[1];
          int B = image.at<cv::Vec4b>(row, col)[2];
          int A = image.at<cv::Vec4b>(row, col)[3];

          image.at<cv::Vec4b>(row, col)[0] = R*3;
          image.at<cv::Vec4b>(row, col)[1] = G;
          image.at<cv::Vec4b>(row, col)[2] = B;
          image.at<cv::Vec4b>(row, col)[3] = A;
      }
    }
}

void OpenCVUtils::testApplyFilters(Mat image) {
    //image = customFilter(image);
    //testPixelControl(image);
}

/*
 int height = image.cols;
 int width = image.rows;
 */

