//
//  totalvariation.cpp
//  OpenCV Tutorial
//
//  Created by Emmanuel d'Angelo on 01.08.12.
//
//

#include <dispatch/dispatch.h>
#include "totalvariation.h"

static dispatch_queue_t opencv_tv_queue;

static inline float compute_tkp1(float const tk)
{
    return 0.5 * (1.0 + std::sqrt(1.0 + 4.0*tk*tk));
}

void horizontal_gradient_forward(cv::Mat const &X, cv::Mat &Dx)
{
    Dx.create(X.size(), CV_32FC1);
 
    dispatch_apply(X.rows, opencv_tv_queue, ^(size_t y) {

        float const *p_x = X.ptr<float>(y);
        float const *p_xp1 = p_x + 1;
        float *p_dx = Dx.ptr<float>(y);
        
        for (int x = 0; x < X.cols-1; ++x) {
            *p_dx++ = *p_xp1 - *p_x;
            p_x++;
            p_xp1++;
        }
        *p_dx = 0.0;

    });
}

void horizontal_gradient_backward(cv::Mat const &X, cv::Mat &Dx)
{
    Dx.create(X.size(), CV_32FC1);
    
    dispatch_apply(X.rows, opencv_tv_queue, ^(size_t y) {
    
        float const *p_xm1 = X.ptr<float>(y);
        float const *p_x = p_xm1 + 1;
        float *p_dx = Dx.ptr<float>(y);
        *p_dx++ += 0.0;
        
        for (int x = 1; x < X.cols; ++x) {
            *p_dx++ += *p_x - *p_xm1;
            p_x++;
            p_xm1++;
        }
    
    });
}

void vertical_gradient_forward(cv::Mat const &X, cv::Mat &Dx)
{
    Dx.create(X.size(), CV_32FC1);
    
    dispatch_apply(X.rows-1, opencv_tv_queue, ^(size_t y) {
        
        float const *p_x = X.ptr<float>(y);
        float const *p_xp1 = X.ptr<float>(y+1);
        float *p_dx = Dx.ptr<float>(y);
        
        for (int x = 0; x < X.cols; ++x) {
            *p_dx++ = *p_xp1 - *p_x;
            ++p_x;
            ++p_xp1;
        }
        
    });

    memset(Dx.ptr<float>(X.rows-1), 0, 4*X.cols);
}

void vertical_gradient_backward(cv::Mat const &X, cv::Mat &Dx)
{
    Dx.create(X.size(), CV_32FC1);
    memset(Dx.ptr<float>(0), 0, 4*X.cols);
    
    dispatch_apply(X.rows-1, opencv_tv_queue, ^(size_t y) {

        y = y + 1;
        
        float const *p_x = X.ptr<float>(y);
        float const *p_xm1 = X.ptr<float>(y-1);
        float *p_dx = Dx.ptr<float>(y);
        
        for (int x = 0; x < X.cols; ++x) {
            *p_dx++ += *p_x - *p_xm1;
            ++p_x;
            ++p_xm1;
        }

    });
}

void divergence_backward(cv::Mat const &X1, cv::Mat const &X2, cv::Mat &divX)
{
    divX.create(X1.size(), CV_32FC1);
    divX.setTo(cv::Scalar(0));
    
    horizontal_gradient_backward(X1, divX);
    vertical_gradient_backward(X2, divX);
}

void threshold(cv::Mat &X, float xmin, float xmax)
{
    dispatch_apply(X.rows, opencv_tv_queue, ^(size_t y) {

        float *p_x = X.ptr<float>(y);
        
        for (int x = 0; x < X.cols; ++x)
            *p_x++ = MAX(MIN(1.0, *p_x), 0.0);

    });
}

void prox_tv_dual(cv::Mat &X1, cv::Mat &X2)
{
    if (X1.size() != X2.size()) return;
    
    dispatch_apply(X1.rows, opencv_tv_queue, ^(size_t y) {

        float *p_x1 = X1.ptr<float>(y);
        float *p_x2 = X2.ptr<float>(y);
        
        for (int x = 0; x < X1.cols; ++x)
        {
            float norm_x1 = MAX(1.0, std::fabs(*p_x1));
            *p_x1 /= norm_x1;
            p_x1++;
            
            float norm_x2 = MAX(1.0, std::fabs(*p_x2));
            *p_x2 /= norm_x2;
            p_x2++;
        }

    });
}

void tvdn_fista(cv::Mat const &input, float lambda, int iterations, cv::Mat &result)
{
    if (input.type() != CV_32F) return;
    
    input.copyTo(result);
    
    float const updateWeight = 0.125/lambda;
    float tk = 1.0;
    float tkp1 = 1.0;
    
    cv::Mat rk, sk, pk, qk, pkm1, qkm1;
    cv::Mat divX;
    
    rk = cv::Mat::zeros(input.size(), CV_32F);
    rk.copyTo(sk);
    rk.copyTo(qk),
    rk.copyTo(pk);
    rk.copyTo(qkm1),
    rk.copyTo(pkm1);
    
    // Create a queue, set its priority to High
    dispatch_queue_t highPrior = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    opencv_tv_queue = dispatch_queue_create("computer-vision-talks.ROF", DISPATCH_QUEUE_CONCURRENT);
    dispatch_set_target_queue(opencv_tv_queue, highPrior);
    
    for (int i = 0; i < iterations; ++i) {
        
        // 1. Proximal problem
        divergence_backward(rk, sk, divX);
        divX *= (-lambda);
        divX += input;
        threshold(divX, 0.0, 1.0);
        horizontal_gradient_forward(divX, pk);
        vertical_gradient_forward(divX, qk);
        pk *= (-updateWeight);
        qk *= (-updateWeight);
        pk += rk;
        qk += sk;
        prox_tv_dual(pk, qk);
        
        // 2. Parameter update
        tkp1 = compute_tkp1(tk);
        
        // 3. Auxiliary point update
        float mixWeight = (tk - 1.0)/tkp1;
        
        pk.copyTo(rk);
        rk -= pkm1;
        rk *= mixWeight;
        rk += pk;
        
        qk.copyTo(sk);
        sk -= qkm1;
        sk *= mixWeight;
        sk += qk;
        
        // 4. Next iterations
        tk = tkp1;
        cv::swap(pk, pkm1);
        cv::swap(qk, qkm1);
    }
    
    divergence_backward(pkm1, qkm1, result);
    result *= (-lambda);
    result += input;
    threshold(result, 0.0, 1.0);
    
    dispatch_release(opencv_tv_queue);
}
