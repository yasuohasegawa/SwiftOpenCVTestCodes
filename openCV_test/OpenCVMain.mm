//
//  OpenCVMain.m
//  openCV_test
//
//  Created by Yasuo Hasegawa on 2021/12/02.
//

#ifdef __cplusplus
#import <opencv2/opencv.hpp>
#import <opencv2/highgui.hpp>
#import <opencv2/imgcodecs/ios.h>
#import "OpenCVMain.h"
#endif
#import <Foundation/Foundation.h>
#include "OpenCVUtils.hpp"

@implementation OpenCVMain : NSObject

+ (UIImage*)gray:(UIImage*)image {
    cv::Mat img_Mat;
    UIImageToMat(image, img_Mat, true);
    
    OpenCVUtils openCVUtils;
    img_Mat = openCVUtils.grayscale(img_Mat);

    return MatToUIImage(img_Mat);
}

+ (UIImage *) canny:(UIImage *)image{
    cv::Mat img_Mat;
    UIImageToMat(image, img_Mat, true);

    OpenCVUtils openCVUtils;
    img_Mat = openCVUtils.edges(img_Mat,10,100);

    return MatToUIImage(img_Mat);
}

+ (UIImage*)blur:(UIImage*)image {
    cv::Mat img_Mat;
    UIImageToMat(image, img_Mat, true);
    
    OpenCVUtils openCVUtils;
    img_Mat = openCVUtils.blur(img_Mat,cv::Size(17,17));
    
    return MatToUIImage(img_Mat);
}

+ (UIImage*)testDetect:(UIImage*)image {
    cv::Mat img_Mat;
    UIImageToMat(image, img_Mat, true);
    
    OpenCVUtils openCVUtils;
    cv::Mat res = openCVUtils.detect(img_Mat);

    return MatToUIImage(res);
}

+ (UIImage*)motionDetect:(UIImage*)image previousImage:(UIImage *)prevImage {
    cv::Mat img_Mat;
    UIImageToMat(image, img_Mat, true);
    
    cv::Mat prev_Mat;
    UIImageToMat(prevImage, prev_Mat, true);
    
    OpenCVUtils openCVUtils;
    cv::Mat res = openCVUtils.detect_motion(img_Mat, prev_Mat);

    return MatToUIImage(res);
}

@end
