//
//  totalvariation.h
//  OpenCV Tutorial
//
//  Created by Emmanuel d'Angelo on 01.08.12.
//
//

#ifndef OpenCV_Tutorial_totalvariation_h
#define OpenCV_Tutorial_totalvariation_h

//! Horizontal gradient with forward scheme
void horizontal_gradient_forward(cv::Mat const &X, cv::Mat &Dx);
//! Horizontal gradient with backward scheme
void horizontal_gradient_backward(cv::Mat const &X, cv::Mat &Dx);

//! Vertical gradient with forward scheme
void vertical_gradient_forward(cv::Mat const &X, cv::Mat &Dx);
//! Vertical gradient with backward scheme
void vertical_gradient_backward(cv::Mat const &X, cv::Mat &Dx);

//! 2D field (X1,X2) divergence with backward scheme
void divergence_backward(cv::Mat const &X1, cv::Mat const &X2, cv::Mat &divX);

//! Total-Variation DeNoising using [FISTA]
void tvdn_fista(cv::Mat const &input, float lambda, int iterations, cv::Mat &result);


//------------------------------//
//          References          //
//------------------------------//

// [FISTA]
// Beck, A., & Teboulle, M. (2009).
// Fast Gradient-Based Algorithms for Constrained Total Variation Image Denoising and Deblurring Problems.
// Image Processing, IEEE Transactions on, 18(11), 2419–2434. doi:10.1109/TIP.2009.2028250


#endif
