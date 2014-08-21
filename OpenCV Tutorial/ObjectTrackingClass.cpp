//
//  ObjectTrackingClass.cpp
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 26/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <float.h>
#include <ctype.h>
#include "ObjectTrackingClass.h"

inline float Min(float a, float b) { return a < b ? a : b; }
inline float Max(float a, float b) { return a > b ? a : b; }

// set maxCorners
void ObjectTrackingClass::setMaxCorners(int max_c) {
    maxCorners = max_c;
}

// initialise tracker
void ObjectTrackingClass::init(cv::Mat& image, // output image
                               cv::Mat& image1, // input image
                               std::vector<cv::Point2f>& points1) // output points array
{
    // initialise tracker
    cv::goodFeaturesToTrack(image1,
                            points1,
                            maxCorners,
                            qualityLevel,
                            minDistance,
                            cv::Mat(),
                            blockSize,
                            useHarrisDetector,
                            k);
    
    // refine corner locations
    cv::cornerSubPix(image1, points1, subPixWinSize, cv::Size(-1,-1), termcrit);
    
    size_t i;
    for( i = 0; i < points1.size(); i++ )
    {        
        // draw points
        cv::circle( image, points1[i], 3, cv::Scalar(0,255,0), -1, 8);
    }
}

// track object
void ObjectTrackingClass::track(cv::Mat& image, // output image
                                cv::Mat& image1, // previous frame
                                cv::Mat& image2, // next frame
                                std::vector<cv::Point2f>& points1, // previous points 
                                std::vector<cv::Point2f>& points2, // next points
                                std::vector<uchar>& status, // status array
                                std::vector<float>& err) // error array
{
    // tracking code
    cv::calcOpticalFlowPyrLK(image1,
                             image2,
                             points1,
                             points2,
                             status,
                             err,
                             winSize,
                             maxLevel,
                             termcrit,
                             flags,
                             minEigThreshold);
    
    // work out maximum X,Y keypoint values in the next_points keypoint vector
    cv::Point2f min(FLT_MAX, FLT_MAX);
    cv::Point2f max(FLT_MIN, FLT_MIN);
    
    // refactor the points array to remove points lost due to tracking error
    size_t i, k;
    for( i = k = 0; i < points2.size(); i++ )
    {
        if( !status[i] )
            continue;
        
        points2[k++] = points2[i];
        
        // find keypoints at the extremes
        min.x = Min(min.x, points2[i].x);
        min.y = Min(min.y, points2[i].y);
        max.x = Max(max.x, points2[i].x);
        max.y = Max(max.y, points2[i].y);
        
        // draw points
        cv::circle( image, points2[i], 3, cv::Scalar(0,255,0), -1, 8);
    }
    points2.resize(k);
    
    // Draw lines between the extreme points (square)
    cv::Point2f point0(min.x, min.y);
    cv::Point2f point1(max.x, min.y);
    cv::Point2f point2(max.x, max.y);
    cv::Point2f point3(min.x, max.y);
    cv::line( image, point0, point1, cv::Scalar( 0, 255, 0 ), 4 );
    cv::line( image, point1, point2, cv::Scalar( 0, 255, 0 ), 4 );
    cv::line( image, point2, point3, cv::Scalar( 0, 255, 0 ), 4 );
    cv::line( image, point3, point0, cv::Scalar( 0, 255, 0 ), 4 );
}