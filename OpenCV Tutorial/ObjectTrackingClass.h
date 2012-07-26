//
//  ObjectTrackingClass.h
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 26/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_ObjectTrackingClass_h
#define OpenCV_Tutorial_ObjectTrackingClass_h

class ObjectTrackingClass
{
private:
    int maxCorners;
    double qualityLevel;
    double minDistance;
    int blockSize;
    bool useHarrisDetector;
    double k;
    cv::Size subPixWinSize, winSize;
    cv::TermCriteria termcrit;
    int maxLevel;
    int flags;
    double minEigThreshold;
    
public:
    ObjectTrackingClass()
    : maxCorners(500)
    , qualityLevel(0.01)
    , minDistance(10)
    , blockSize(3)
    , useHarrisDetector(false)
    , subPixWinSize(10,10)
    , winSize(31,31)
    , termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)
    , maxLevel(3)
    , flags(0)
    , minEigThreshold(0.001)
    
    {
        // empty
    }
    
    // set maxcorners
    void setMaxCorners(int maxCorners);
    
    // initialise tracker
    void init(cv::Mat& image,
              std::vector<cv::Point2f>& points1);
    
    // track optical flow
    void track(cv::Mat& image,
               cv::Mat& image1,
               cv::Mat& image2,
               std::vector<cv::Point2f>& points1,
               std::vector<cv::Point2f>& points2,
               cv::vector<uchar>& status,
               cv::vector<float>& err);
};

#endif
