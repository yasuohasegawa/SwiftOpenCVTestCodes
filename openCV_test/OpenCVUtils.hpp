//
//  TestDetector.hpp
//  openCV_test
//
//  Created by Yasuo Hasegawa on 2021/12/02.
//

#ifndef OpenCVUtils_hpp
#define OpenCVUtils_hpp

#include <stdio.h>

#endif /* OpenCVUtils_hpp */


#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class OpenCVUtils {
    
public:
    Mat detect(Mat image);
    Mat detect_motion(Mat image, Mat prevImage);
    Mat detect_lines(Mat image);
    Mat detect_circles(Mat image);
    
    Mat blur(Mat image, cv::Size s);
    Mat grayscale(Mat image);
    Mat edges(Mat image, double min, double max);
    Mat rotated_image(Mat image, float angle);
    Mat translated_image(Mat image, float tx, float ty);
    Mat resized_image(Mat image, int w, int h);
    Mat customFilter(Mat image);
    
private:
    Mat draw_lines(Mat image, vector<Vec4i> lines);
    Mat draw_circles(Mat image, vector<Vec3f> circles);
    void testDrawShapes(Mat image);
    void testPixelControl(Mat image);
    void testApplyFilters(Mat image);
};
