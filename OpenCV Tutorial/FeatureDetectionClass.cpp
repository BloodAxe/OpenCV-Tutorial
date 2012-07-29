//
//  FeatureDetectionClass.cpp
//  OpenCV Tutorial
//
//  Created by Anton Belodedenko on 25/07/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "FeatureDetectionClass.h"

// Set the feature detector
void FeatureDetectionClass::setFeatureDetector(cv::Ptr<cv::FeatureDetector>& detect) {
    detector= detect;
}

// Set the descriptor extractor
void FeatureDetectionClass::setDescriptorExtractor(cv::Ptr<cv::DescriptorExtractor>& desc) {
    extractor= desc;
}

// Set the matcher
void FeatureDetectionClass::setDescriptorMatcher(cv::Ptr<cv::DescriptorMatcher>& match) {
    matcher= match;
}

// Set confidence level
void FeatureDetectionClass::setConfidenceLevel(double conf) {
    confidence= conf;
}

//Set MinDistanceToEpipolar
void FeatureDetectionClass::setMinDistanceToEpipolar(double dist) {
    distance= dist;
}

//Set ratio
void FeatureDetectionClass::setRatio(float rat) {
    ratio= rat;
}

// draw epipolar lines
void FeatureDetectionClass::drawEpipolarLines(cv::Mat& image_out, // output image
                                              cv::Mat& image1, // image 1
                                              cv::Mat& image2, // image 2
                                              std::vector<cv::Point2f>& points1, // keypoints 1
                                              std::vector<cv::Point2f>& points2, // keypoints 2)
                                              int whichImage) // image to compute epipolar lines in
{
    // Compute F matrix from 7 matches
    cv::Mat F = cv::findFundamentalMat(cv::Mat(points1), // points in object image
                                   cv::Mat(points2), // points in scene image
                                   CV_FM_7POINT); // 7-point method
    
    std::vector<cv::Vec3f> lines1;
    
    // Compute corresponding epipolar lines
    cv::computeCorrespondEpilines(cv::Mat(points1), // image points
                                  whichImage, // in image 1 (can also be 2)
                                  F, // F matrix
                                  lines1); // vector of epipolar lines
    
    // for all epipolar lines
    for (cv::vector<cv::Vec3f>::const_iterator it = lines1.begin(); it!=lines1.end(); ++it)
    {
        // Draw the line between first and last column
        cv::line(image_out,
                 cv::Point(0,-(*it)[2]/(*it)[1]),
                 cv::Point(image2.cols,-((*it)[2]+
                                              (*it)[0]*image2.cols)/(*it)[1]),
                 cv::Scalar(255,255,255));
    }

}

// draw matched keypoints
void FeatureDetectionClass::drawMatches(cv::Mat& image, // output image
                                        std::vector<cv::DMatch>& matches, // matches
                                        std::vector<cv::Point2f>& points) // keypoints
{
    for (size_t it= 0; it < matches.size(); ++it) {
        cv::circle( image, points[it], 3, cv::Scalar(0,0,255), -1, 8);
    }

}

// compute homography (RANSAC)
void FeatureDetectionClass::drawPerspective(cv::Mat& image, // output image
                                            cv::Mat& image1, // image 1
                                            std::vector<cv::Point2f>& points1, // keypoints 1
                                            std::vector<cv::Point2f>& points2) // keypoints 2
{
    cv::Mat H = cv::findHomography( points1, points2, CV_RANSAC );
    if( !H.empty() ) 
    {
        // get the corners from the object image
        std::vector<cv::Point2f> obj_corners(4), scene_corners(4);
        
        obj_corners[0] = cvPoint(0,0);
        obj_corners[1] = cvPoint( image1.cols, 0 );
        obj_corners[2] = cvPoint( image1.cols, image1.rows );
        obj_corners[3] = cvPoint( 0, image1.rows );
        
        // calculate perspective transformation of object corners within scene
        cv::perspectiveTransform( obj_corners, scene_corners, H);
        
        // draw lines between the corners (the mapped object in the scene)
        cv::line( image, scene_corners[0], scene_corners[1], cv::Scalar( 0, 0, 255 ), 4 );
        cv::line( image, scene_corners[1], scene_corners[2], cv::Scalar( 0, 0, 255 ), 4 );
        cv::line( image, scene_corners[2], scene_corners[3], cv::Scalar( 0, 0, 255 ), 4 );
        cv::line( image, scene_corners[3], scene_corners[0], cv::Scalar( 0, 0, 255 ), 4 );
    }
}


// Clear matches for which NN ratio is > than threshold
// return the number of removed points
// (corresponding entries being cleared,
// i.e. size will be 0)
int FeatureDetectionClass::ratioTest(std::vector<std::vector<cv::DMatch> >
              &matches) {
    int removed=0;
    // for all matches
    for (std::vector<std::vector<cv::DMatch> >::iterator
         matchIterator= matches.begin();
         matchIterator!= matches.end(); ++matchIterator) {
        // if 2 NN has been identified
        if (matchIterator->size() > 1) {
            // check distance ratio
            if ((*matchIterator)[0].distance/
                (*matchIterator)[1].distance > ratio) {
                matchIterator->clear(); // remove match
                removed++;
            }
        } else { // does not have 2 neighbours
            matchIterator->clear(); // remove match
            removed++;
        }
    }
    return removed;
}

// Insert symmetrical matches in symMatches vector
void FeatureDetectionClass::symmetryTest(const std::vector<std::vector<cv::DMatch> >& matches1,
                  const std::vector<std::vector<cv::DMatch> >& matches2,
                  std::vector<cv::DMatch>& symMatches) {
    // for all matches image 1 -> image 2
    for (std::vector<std::vector<cv::DMatch> >::
         const_iterator matchIterator1= matches1.begin();
         matchIterator1!= matches1.end(); ++matchIterator1) {
        // ignore deleted matches
        if (matchIterator1->size() < 2)
            continue;
        // for all matches image 2 -> image 1
        for (std::vector<std::vector<cv::DMatch> >::
             const_iterator matchIterator2= matches2.begin();
             matchIterator2!= matches2.end();
             ++matchIterator2) {
            // ignore deleted matches
            if (matchIterator2->size() < 2)
                continue;
            // Match symmetry test
            if ((*matchIterator1)[0].queryIdx ==
                (*matchIterator2)[0].trainIdx &&
                (*matchIterator2)[0].queryIdx ==
                (*matchIterator1)[0].trainIdx) {
                // add symmetrical match
                symMatches.push_back(cv::DMatch((*matchIterator1)[0].queryIdx,
                                                (*matchIterator1)[0].trainIdx,
                                                (*matchIterator1)[0].distance));
                break; // next match in image 1 -> image 2
            }
        }
    }
}

// Identify good matches using RANSAC
// Return fundamental matrix
cv::Mat FeatureDetectionClass::ransacTest(const std::vector<cv::DMatch>& matches,
                   const std::vector<cv::KeyPoint>& keypoints1,
                   const std::vector<cv::KeyPoint>& keypoints2,
                   std::vector<cv::DMatch>& outMatches,
                   std::vector<cv::Point2f>& points1,
                   std::vector<cv::Point2f>& points2) {
    // Convert keypoints into Point2f
    //std::vector<cv::Point2f> points1, points2;
    cv::Mat fundamental;
    for (std::vector<cv::DMatch>::
         const_iterator it= matches.begin();
         it!= matches.end(); ++it) {
        // Get the position of left keypoints
        float x= keypoints1[it->queryIdx].pt.x;
        float y= keypoints1[it->queryIdx].pt.y;
        points1.push_back(cv::Point2f(x,y));
        // Get the position of right keypoints
        x= keypoints2[it->trainIdx].pt.x;
        y= keypoints2[it->trainIdx].pt.y;
        points2.push_back(cv::Point2f(x,y));
    }
    // Compute F matrix using RANSAC
    std::vector<uchar> inliers(points1.size(),0);
    if (points1.size()>0&&points2.size()>0){
        cv::Mat fundamental= cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2), // matching points
                                                    inliers,       // match status (inlier or outlier)
                                                    CV_FM_RANSAC, // RANSAC method
                                                    distance,      // distance to epipolar line
                                                    confidence); // confidence probability
        // extract the surviving (inliers) matches
        std::vector<uchar>::const_iterator
        itIn= inliers.begin();
        std::vector<cv::DMatch>::const_iterator
        itM= matches.begin();
        // for all matches
        for ( ;itIn!= inliers.end(); ++itIn, ++itM) {
            if (*itIn) { // it is a valid match
                outMatches.push_back(*itM);
            }
        }
        if (refineF) {
            // The F matrix will be recomputed with
            // all accepted matches
            // Convert keypoints into Point2f
            // for final F computation
            points1.clear();
            points2.clear();
            for (std::vector<cv::DMatch>::
                 const_iterator it= outMatches.begin();
                 it!= outMatches.end(); ++it) {
                // Get the position of left keypoints
                float x= keypoints1[it->queryIdx].pt.x;
                float y= keypoints1[it->queryIdx].pt.y;
                points1.push_back(cv::Point2f(x,y));
                // Get the position of right keypoints
                x= keypoints2[it->trainIdx].pt.x;
                y= keypoints2[it->trainIdx].pt.y;
                points2.push_back(cv::Point2f(x,y));
            }
            // Compute 8-point F from all accepted matches
            if (points1.size()>0&&points2.size()>0){
                fundamental= cv::findFundamentalMat(cv::Mat(points1),cv::Mat(points2), // matches
                                                    CV_FM_8POINT); // 8-point method
            }
        }
    }
    return fundamental;
}

// Match feature points using symmetry test and RANSAC
// returns fundamental matrix
cv::Mat FeatureDetectionClass::match(cv::Mat& image2, // input scene image
              std::vector<cv::KeyPoint>& keypoints1, // input computed object keypoints
              cv::Mat& descriptors1, // input computed object descriptors
              std::vector<cv::DMatch>& matches, // output matches
              std::vector<cv::Point2f>& points1, // output object keypoints (Point2f)
              std::vector<cv::Point2f>& points2) // output scene keypoints (Point2f)
{
    // 1a. Detection of the features  
    std::vector<cv::KeyPoint> keypoints2;
    detector->detect(image2,keypoints2);
    // 1b. Extraction of the descriptors
    cv::Mat descriptors2;
    extractor->compute(image2,keypoints2,descriptors2);
    // 2. Match the two image descriptors
    // from object image to scene image
    // based on k nearest neighbours (with k=2)
    std::vector<std::vector<cv::DMatch> > matches1;
    matcher->knnMatch(descriptors1,
                      descriptors2,
                      matches1, // vector of matches (up to 2 per entry)
                      2);        // return 2 nearest neighbours
    // from scene image to object image
    // based on k nearest neighbours (with k=2)
    std::vector<std::vector<cv::DMatch> > matches2;
    matcher->knnMatch(descriptors2,
                      descriptors1,
                      matches2, // vector of matches (up to 2 per entry)
                      2);        // return 2 nearest neighbours
    // 3. Remove matches for which NN ratio is
    // > than threshold
    // clean object image -> scene image matches
    int removed= ratioTest(matches1);
    // clean scene image -> object image matches
    removed= ratioTest(matches2);
    // 4. Remove non-symmetrical matches
    std::vector<cv::DMatch> symMatches;
    symmetryTest(matches1,matches2,symMatches);
    // 5. Validate matches using RANSAC
    cv::Mat fundamental= ransacTest(symMatches,
                                    keypoints1,
                                    keypoints2,
                                    matches,
                                    points1,
                                    points2);
    // return the found fundamental matrix
    return fundamental;
}