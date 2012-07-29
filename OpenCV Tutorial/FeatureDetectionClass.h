//
//  FeatureDetectionClass.h
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef OpenCV_Tutorial_FeatureDetectionClass_h
#define OpenCV_Tutorial_FeatureDetectionClass_h

// FeatureDetectionClass class
// (RobustMatcher) from OpenCV2 Computer Vision Application Programming Cookbook, Chapter 9
class FeatureDetectionClass
{
private:
    // pointer to the feature point detector object
    cv::Ptr<cv::FeatureDetector> detector;
    // pointer to the feature descriptor extractor object
    cv::Ptr<cv::DescriptorExtractor> extractor;
    // pointer to the matcher object
    cv::Ptr<cv::DescriptorMatcher > matcher;
    float ratio; // max ratio between 1st and 2nd NN
    bool refineF; // if true will refine the F matrix
    double distance; // min distance to epipolar
    double confidence; // confidence level (probability)
    
public:
    FeatureDetectionClass(): ratio(0.65f), refineF(true), confidence(0.99), distance(3.0) {
        // SURF is the default feature detector and extractor
        detector= new cv::SurfFeatureDetector(400);
        extractor= new cv::SurfDescriptorExtractor();
        // BruteForce matcher is the default matcher
        matcher= new cv::BFMatcher(cv::NORM_L1, false);   
    }

    // Set the feature detector
    void setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect);
    
    // Set the descriptor extractor
    void setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc);
    
    // Set the matcher
    void setDescriptorMatcher(cv::Ptr<cv::DescriptorMatcher>& match);
    
    // Set confidence level
    void setConfidenceLevel(double conf);
    
    //Set MinDistanceToEpipolar
    void setMinDistanceToEpipolar(double dist);
    
    //Set ratio
    void setRatio(float rat);
    
    // Clear matches for which NN ratio is > than threshold
    // return the number of removed points
    // (corresponding entries being cleared,
    // i.e. size will be 0)
    int ratioTest(std::vector<std::vector<cv::DMatch> >
                  &matches);
    
    // Insert symmetrical matches in symMatches vector
    void symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
                      const std::vector<std::vector<cv::DMatch> >& matches2,
                      std::vector<cv::DMatch>& symMatches);
    
    // Identify good matches using RANSAC
    // Return fundamental matrix
    cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
                       const std::vector<cv::KeyPoint>& keypoints1,
                       const std::vector<cv::KeyPoint>& keypoints2,
                       std::vector<cv::DMatch>& outMatches,
                       std::vector<cv::Point2f>& points1,
                       std::vector<cv::Point2f>& points2);
    
    // Match feature points using symmetry test and RANSAC
    // returns fundamental matrix
    cv::Mat match(cv::Mat& image2, // input scene image
                  std::vector<cv::KeyPoint>& keypoints1, // input computed object keypoints
                  cv::Mat& descriptors1, // input computed object descriptors
                  std::vector<cv::DMatch>& matches, // output matches
                  std::vector<cv::Point2f>& points1, // output object keypoints (Point2f)
                  std::vector<cv::Point2f>& points2); // output scene keypoints (Point2f)
    
    // draw matched keypoints
    void drawMatches(cv::Mat& image, // output image
                     std::vector<cv::DMatch>& matches, // matches
                     std::vector<cv::Point2f>& scene); // scene keypoints (Point2f)
    
    // draw epipolar lines
    void drawEpipolarLines(cv::Mat& image_out, // output image
                           cv::Mat& image1, // image 1
                           cv::Mat& image2, // image 2
                           std::vector<cv::Point2f>& points1, // keypoints 1
                           std::vector<cv::Point2f>& points2, // keypoints 2)
                           int whichImage); // image to compute epipolar lines in
    
    void drawPerspective(cv::Mat& image, // output image
                        cv::Mat& image1, // input (object) image
                        std::vector<cv::Point2f>& points1, // keypoints 1
                        std::vector<cv::Point2f>& points2); // keypoints 2
};

#endif


