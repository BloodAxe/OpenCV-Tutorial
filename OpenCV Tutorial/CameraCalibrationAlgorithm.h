#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
#include <array>


enum PatternType {
    CHESSBOARD = 0,
    CIRCLES_GRID = 1,
    ACIRCLES_GRID = 2
};

class CameraCalibrationAlgorithm
{
public:
    typedef std::vector<cv::Point3f>               VectorOf3DPoints;
    typedef std::vector<cv::Point2f>               VectorOf2DPoints;
    typedef std::vector<std::vector<cv::Point3f> > VectorOfVectorOf3DPoints;
    typedef std::vector<std::vector<cv::Point2f> > VectorOfVectorOf2DPoints;
    typedef std::vector<cv::Mat>                   VectorOfMat;
    
    CameraCalibrationAlgorithm(cv::Size patternSize, PatternType type);
    
    bool detectCorners(const cv::Mat& frame, VectorOf2DPoints& corners2d) const;
    
    bool calibrateCamera(
                         const VectorOfVectorOf2DPoints& gridCorners,
                         const cv::Size imageSize,
                         cv::Mat& cameraMatrix,
                         cv::Mat& distCoeffs
                         ) const;
    
protected:
    
    
    static void calcChessboardCorners
    (
     cv::Size boardSize,
     float squareSize,
     VectorOf3DPoints& corners,
     PatternType patternType
     );
    
    static bool runCalibration
    (
     VectorOfVectorOf2DPoints imagePoints,
     cv::Size imageSize,
     cv::Size boardSize,
     PatternType patternType,
     float squareSize,
     float aspectRatio,
     int flags,
     cv::Mat& cameraMatrix,
     cv::Mat& distCoeffs,
     VectorOfMat& rvecs,
     VectorOfMat& tvecs,
     std::vector<float>& reprojErrs,
     double& totalAvgErr
     );
    
    static double computeReprojectionErrors(
                                            const VectorOfVectorOf3DPoints& objectPoints,
                                            const VectorOfVectorOf2DPoints& imagePoints,
                                            const VectorOfMat& rvecs,
                                            const VectorOfMat& tvecs,
                                            const cv::Mat& cameraMatrix,
                                            const cv::Mat& distCoeffs,
                                            std::vector<float>& perViewErrors
                                            );
    
    
private:
    cv::Size                 m_patternSize;
    PatternType              m_pattern;
};
