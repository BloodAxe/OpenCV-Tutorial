//
//  ObjectTrackingClass.cpp
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 26/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "ObjectTrackingClass.h"

// set maxCorners
void ObjectTrackingClass::setMaxCorners(int max_c) {
    maxCorners = max_c;
}

// initialise tracker
void ObjectTrackingClass::init(cv::Mat& image, // ...
                               std::vector<cv::Point2f>& points) // ...
{
    // initialise tracker
    cv::goodFeaturesToTrack(image, // input image
                            points, // points array
                            maxCorners, // max corners ot detect
                            qualityLevel, // ...
                            minDistance, // ...
                            cv::Mat(), // ...
                            blockSize, // ...
                            useHarrisDetector, // ...
                            k); // ...
    
    // refine corner locations
    cv::cornerSubPix(image, points, subPixWinSize, cv::Size(-1,-1), termcrit);
}

// track object
void ObjectTrackingClass::track(cv::Mat& image, // output image
                                cv::Mat& image1, // previous frame
                                cv::Mat& image2, // next frame
                                std::vector<cv::Point2f>& points1, // previous points 
                                std::vector<cv::Point2f>& points2, // next points
                                cv::vector<uchar>& status, // ...
                                cv::vector<float>& err) // ...
{
    // tracking code
    cv::calcOpticalFlowPyrLK(image1, // ...
                             image2, // ...
                             points1, // ...
                             points2, // ...
                             status, // ...
                             err, // ...
                             winSize, // ...
                             maxLevel, // ...
                             termcrit, // ...
                             flags, // ...
                             minEigThreshold); // ...
    
    // refactor the points array to remove points lost due to tracking error
    size_t i, k;
    for( i = k = 0; i < points2.size(); i++ )
    {
        if( !status[i] )
            continue;
        
        points2[k++] = points2[i];
        
        // draw points
        cv::circle( image, points2[i], 3, cv::Scalar(0,255,0), -1, 8);
    }
    points2.resize(k);
}