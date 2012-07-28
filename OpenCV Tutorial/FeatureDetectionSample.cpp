//
//  FeatureDetection.cpp
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "FeatureDetectionClass.h"
#include "FeatureDetectionSample.h"
#include "Globals.h"

FeatureDetectionSample::FeatureDetectionSample()
: m_fdAlgorithmName("ORB")
, m_feAlgorithmName("FREAK")
, m_hessianThreshold(400)
, m_nFeatures(500)
, m_minMatches(4)
, m_drawEpipolarLines(false)
, m_drawMatches(true)
, m_drawPerspective(true)
{
    
    std::vector<std::string> fdAlgos, feAlgos;
    // feature detection options
    fdAlgos.push_back("ORB");
    fdAlgos.push_back("SURF");
    registerOption("Detector",       "", &m_fdAlgorithmName, fdAlgos);
    
    // feature extraction options
    feAlgos.push_back("ORB");
    feAlgos.push_back("SURF");
    feAlgos.push_back("FREAK");
    registerOption("Extractor",       "", &m_feAlgorithmName, feAlgos);
    
    // SURF feature detector options
    registerOption("hessianThreshold", "SURF", &m_hessianThreshold, 300, 500);
    
    // ORB feature detector options
    registerOption("nFeatures", "ORB", &m_nFeatures, 0, 1500);
    
    // Matcher options
    registerOption("Minumum matches", "Matcher", &m_minMatches, 4, 200);
    
    // Display options
    registerOption("Matches", "Draw", &m_drawMatches);
    registerOption("Epipolar", "Draw", &m_drawEpipolarLines);
    registerOption("Perspective", "Draw", &m_drawPerspective);
}

//! Gets a sample name
std::string FeatureDetectionSample::getName() const
{
    return "Feature detection";
}

std::string FeatureDetectionSample::getSampleIcon() const
{
    return "FeatureDetectionSampleIcon.png";
}

//! Returns a detailed sample description
std::string FeatureDetectionSample::getDescription() const
{
    return "Feature detection sample.";
}

//! Returns true if this sample requires setting a reference image for latter use
bool FeatureDetectionSample::isReferenceFrameRequired() const
{
    return true;
}

//! Sets the reference frame for latter processing
void FeatureDetectionSample::setReferenceFrame(const cv::Mat& reference)
{
    getGray(reference, objectImage);
    computeObject = true;
}

// Reset object keypoints and descriptors
void FeatureDetectionSample::resetReferenceFrame() const
{
    detectObject = false;
    computeObject = false;
}

//! Processes a frame and returns output image 
bool FeatureDetectionSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    // display the frame
    inputFrame.copyTo(outputFrame);
    
    // try to find the object in the scene
    if (detectObject) {
        
        // convert input frame to gray scale
        getGray(inputFrame, grayImage);
        
        // prepare the robust matcher and set paremeters
        FeatureDetectionClass rmatcher;
        rmatcher.setConfidenceLevel(0.98);
        rmatcher.setMinDistanceToEpipolar(1.0);
        rmatcher.setRatio(0.65f);
        
        // feature detector setup
        if (m_fdAlgorithmName == "SURF")
        {
            // prepare keypoints detector
            cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(m_hessianThreshold);
            rmatcher.setFeatureDetector(detector);
        }
        else if (m_fdAlgorithmName == "ORB")
        {
            // prepare feature detector and detect the object keypoints
            cv::Ptr<cv::FeatureDetector> detector = new cv::OrbFeatureDetector(m_nFeatures);
            rmatcher.setFeatureDetector(detector);
        }
        else
        {
            std::cerr << "Unsupported algorithm:" << m_fdAlgorithmName << std::endl;
            assert(false);
        }
            
        // feature extractor and matcher setup
        if (m_feAlgorithmName == "SURF")
        {
            // prepare feature extractor
            cv::Ptr<cv::DescriptorExtractor> extractor = new cv::SurfDescriptorExtractor;
            rmatcher.setDescriptorExtractor(extractor);
            // prepare the appropriate matcher for SURF 
            cv::Ptr<cv::DescriptorMatcher> matcher = new cv::BFMatcher(cv::NORM_L2, false);
            rmatcher.setDescriptorMatcher(matcher);
            
        } else if (m_feAlgorithmName == "ORB")
        {
            // prepare feature extractor
            cv::Ptr<cv::DescriptorExtractor> extractor = new cv::OrbDescriptorExtractor;
            rmatcher.setDescriptorExtractor(extractor);
            // prepare the appropriate matcher for ORB
            cv::Ptr<cv::DescriptorMatcher> matcher = new cv::BFMatcher(cv::NORM_HAMMING, false);
            rmatcher.setDescriptorMatcher(matcher);
            
        } else if (m_feAlgorithmName == "FREAK")
        {
            // prepare feature extractor
            //cv::Ptr<cv::DescriptorExtractor> extractor = new cv::FREAK;
            //fd.setDescriptorExtractor(extractor);
            // prepare the appropriate matcher for FREAK
            //cv::Ptr<cv::DescriptorMatcher> matcher = new cv::BFMatcher(cv::NORM_HAMMING, false);
            //fd.setDescriptorMatcher(matcher);
        }
        else {
            std::cerr << "Unsupported algorithm:" << m_feAlgorithmName << std::endl;
            assert(false);
        }
        
        // call the RobustMatcher to match the object keypoints with the scene keypoints
        cv::vector<cv::Point2f> objectKeypoints2f, sceneKeypoints2f;
        std::vector<cv::DMatch> matches;
        cv::Mat fundamentalMat = rmatcher.match(grayImage, // input scene image
                                                objectKeypoints, // input computed object image keypoints
                                                objectDescriptors, // input computed object image descriptors
                                                matches, // output matches
                                                objectKeypoints2f, // output object keypoints (Point2f)
                                                sceneKeypoints2f); // output scene keypoints (Point2f)
        
        if ( matches.size() >= m_minMatches ) {
            
            // draw perspetcive lines (box object in the frame)
            if (m_drawPerspective)
                rmatcher.drawPerspective(outputFrame,
                                        objectImage,
                                         objectKeypoints2f,
                                         sceneKeypoints2f);
            
            // draw keypoint matches as yellow points on the output frame
            if (m_drawMatches)
                rmatcher.drawMatches(outputFrame, 
                                     matches,
                                     sceneKeypoints2f);
            
            // draw epipolar lines
            if (m_drawEpipolarLines)
                rmatcher.drawEpipolarLines(outputFrame,
                                           objectImage,
                                           grayImage,
                                           objectKeypoints2f,
                                           sceneKeypoints2f, 1);
        }
    }
    
    // compute object image keypoints and descriptors
    if (computeObject) {
        
        // select feature detection mechanism
        if ( m_fdAlgorithmName == "SURF" )
        {
            // prepare keypoints detector
            cv::Ptr<cv::FeatureDetector> detector = new cv::SurfFeatureDetector(m_hessianThreshold);
            // Compute object keypoints
            detector->detect(objectImage,objectKeypoints);
            
        }
        else if ( m_fdAlgorithmName == "ORB" )
        {
            // prepare feature detector and detect the object keypoints
            cv::Ptr<cv::FeatureDetector> detector = new cv::OrbFeatureDetector(m_nFeatures);
            // Compute object keypoints
            detector->detect(objectImage,objectKeypoints);
        }
        else {
            std::cerr << "Unsupported algorithm:" << m_fdAlgorithmName << std::endl;
            assert(false);
        }
        
        // select feature extraction mechanism
        if ( m_feAlgorithmName == "SURF" )
        {
            cv::Ptr<cv::DescriptorExtractor> extractor = new cv::SurfDescriptorExtractor;
            // Compute object feature descriptors
            extractor->compute(objectImage,objectKeypoints,objectDescriptors);
        }
        else if ( m_feAlgorithmName == "ORB" )
        {
            cv::Ptr<cv::DescriptorExtractor> extractor = new cv::OrbDescriptorExtractor;
            // Compute object feature descriptors
            extractor->compute(objectImage,objectKeypoints,objectDescriptors);
        }
        else if ( m_feAlgorithmName == "FREAK" )
        {
            //cv::Ptr<cv::DescriptorExtractor> extractor = new cv::FREAK;
            // Compute object feature descriptors
            //extractor->compute(objectImage,objectKeypoints,objectDescriptors);
        }
        else {
            std::cerr << "Unsupported algorithm:" << m_feAlgorithmName << std::endl;
            assert(false);
        }
        
        // set flags
        computeObject = false;
        detectObject = true;
    }
    return true;
}