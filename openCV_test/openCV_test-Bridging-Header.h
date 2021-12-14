//
//  Use this file to import your target's public headers that you would like to expose to Swift.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "OpenCVMain.h"

@interface OpenCVMain : NSObject

+ (UIImage *) gray:(UIImage *)image;
+ (UIImage *) canny:(UIImage *)image;
+ (UIImage *) blur:(UIImage *)image;
+ (UIImage *) testDetect:(UIImage *)image;
+ (UIImage *) motionDetect:(UIImage*)image previousImage:(UIImage *)prevImage;

@end
