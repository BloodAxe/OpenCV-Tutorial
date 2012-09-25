//
//  VideoTracking.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 7/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "VideoTracking.hpp"

static const char * TrackingAlgorithmKLT           = "KLT";
static const char * TrackingAlgorithmBRIEF         = "BRIEF";
static const char * TrackingAlgorithmORB           = "ORB";

VideoTrackingSample::VideoTrackingSample()
: m_orbMatcher(cv::NORM_HAMMING, true)
, m_briefMatcher(cv::NORM_HAMMING, true)
, m_fastDetector(cv::Ptr<cv::FeatureDetector>(new cv::FastFeatureDetector()), 500)
{
    m_maxNumberOfPoints = 50;
    
    m_detector = cv::FeatureDetector::create("GridFAST");
    
    std::vector<std::string> trackingAlgorithms;
    
    trackingAlgorithms.push_back(TrackingAlgorithmKLT);
    trackingAlgorithms.push_back(TrackingAlgorithmORB);
    trackingAlgorithms.push_back(TrackingAlgorithmBRIEF);
    
    registerOption("Algorithm", "", &m_activeTrackingAlgorithm, trackingAlgorithms, 0);
    
    registerOption("Points count", "KLT", &m_maxNumberOfPoints, 1, 100);
}

//! Gets a sample name
std::string VideoTrackingSample::getName() const
{
    return "Video tracking";
}

//! Returns a user-friendly name for displaying as description
std::string VideoTrackingSample::getUserFriendlyName() const
{
    return "Video tracking using " + m_activeTrackingAlgorithm;
}

//! Returns a detailed sample description
std::string VideoTrackingSample::getDescription() const
{
    return "";
}

std::string VideoTrackingSample::getSampleIcon() const
{
    return "VideoTrackingSampleIcon.png";
}

//! Processes a frame and returns output image
bool VideoTrackingSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    inputFrame.copyTo(outputFrame);
    
    getGray(inputFrame, m_nextImg);
    
    if (m_activeTrackingAlgorithm == TrackingAlgorithmKLT)
    {
        
        if (m_mask.rows != inputFrame.rows || m_mask.cols != inputFrame.cols)
            m_mask.create(inputFrame.rows, inputFrame.cols, CV_8UC1);
        
        if (m_prevPts.size() > 0)
        {
            cv::calcOpticalFlowPyrLK(m_prevImg, m_nextImg, m_prevPts, m_nextPts, m_status, m_error);
        }
        
        m_mask = cv::Scalar(255);
        
        std::vector<cv::Point2f> trackedPts;
        
        for (size_t i=0; i<m_status.size(); i++)
        {
            if (m_status[i])
            {
                trackedPts.push_back(m_nextPts[i]);
                
                cv::circle(m_mask, m_prevPts[i], 15, cv::Scalar(0), CV_FILLED);
                cv::line(outputFrame, m_prevPts[i], m_nextPts[i], CV_RGB(0,250,0));
                cv::circle(outputFrame, m_nextPts[i], 3, CV_RGB(0,250,0), CV_FILLED);
            }
        }
        
        bool needDetectAdditionalPoints = trackedPts.size() < m_maxNumberOfPoints;
        if (needDetectAdditionalPoints)
        {
            m_detector->detect(m_nextImg, m_nextKeypoints, m_mask);
            int pointsToDetect = m_maxNumberOfPoints -  trackedPts.size();
            
            if (m_nextKeypoints.size() > pointsToDetect)
            {
                std::random_shuffle(m_nextKeypoints.begin(), m_nextKeypoints.end());
                m_nextKeypoints.resize(pointsToDetect);
            }
            
            std::cout << "Detected additional " << m_nextKeypoints.size() << " points" << std::endl;
            
            for (size_t i=0; i<m_nextKeypoints.size(); i++)
            {
                trackedPts.push_back(m_nextKeypoints[i].pt);
                cv::circle(outputFrame, m_nextKeypoints[i].pt, 5, cv::Scalar(255,0,255), -1);
            }
        }
        
        m_prevPts = trackedPts;
        m_nextImg.copyTo(m_prevImg);
    }
    if (m_activeTrackingAlgorithm == TrackingAlgorithmORB)
    {
        m_orbFeatureEngine(m_nextImg, cv::Mat(), m_nextKeypoints, m_nextDescriptors);
        
        if (m_prevKeypoints.size() > 0)
        {
            std::vector< std::vector<cv::DMatch> > matches;
            m_orbMatcher.radiusMatch(m_nextDescriptors, m_prevDescriptors, matches, 10);
            
            for (size_t i=0; i<matches.size(); i++)
            {
                cv::Point prevPt = m_prevKeypoints[matches[i][0].trainIdx].pt;
                cv::Point nextPt = m_nextKeypoints[matches[i][0].queryIdx].pt;
                
                cv::circle(outputFrame, prevPt, 5, cv::Scalar(250,0,250), CV_FILLED);
                cv::line(outputFrame, prevPt, nextPt, CV_RGB(0,250,0));
                cv::circle(outputFrame, nextPt, 3, CV_RGB(0,250,0), CV_FILLED);
            }
        }
        
        m_prevKeypoints.swap(m_nextKeypoints);
        m_nextDescriptors.copyTo(m_prevDescriptors);
    }
    else if(m_activeTrackingAlgorithm == TrackingAlgorithmBRIEF)
    {
        m_fastDetector.detect(m_nextImg, m_nextKeypoints);
        m_briefExtractor.compute(m_nextImg, m_nextKeypoints, m_nextDescriptors);
        
        if (m_prevKeypoints.size() > 0)
        {
            std::vector< std::vector<cv::DMatch> > matches;
            m_orbMatcher.radiusMatch(m_nextDescriptors, m_prevDescriptors, matches, 10);
            
            for (size_t i=0; i<matches.size(); i++)
            {
                cv::Point prevPt = m_prevKeypoints[matches[i][0].trainIdx].pt;
                cv::Point nextPt = m_nextKeypoints[matches[i][0].queryIdx].pt;
                
                cv::circle(outputFrame, prevPt, 5, cv::Scalar(250,0,250), CV_FILLED);
                cv::line(outputFrame, prevPt, nextPt, CV_RGB(0,250,0));
                cv::circle(outputFrame, nextPt, 3, CV_RGB(0,250,0), CV_FILLED);
            }
        }
        
        m_prevKeypoints.swap(m_nextKeypoints);
        m_nextDescriptors.copyTo(m_prevDescriptors);
    }
    
    return true;
}