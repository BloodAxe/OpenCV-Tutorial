//
//  ROFSample.cpp
//  OpenCV Tutorial
//
//  Created by Emmanuel d'Angelo on 20.07.12.
//  Copyright (c) 2012 EPFL/STI/IEL/LTS2. All rights reserved.
//

#include <iostream>
#include <vector>
#include "totalvariation.h"
#include "ROFSample.h"

//! Init and create options
ROFSample::ROFSample() : m_iterations(10), m_lambdaExponent(0), m_lambda(1.0)
{
    registerOption("Iterations",  "", &m_iterations, 1, 100);
    registerOption("Data weight exponent", "", &m_lambdaExponent, -4, 4);
    registerOption("Process color", "", &m_processColor);
}

//! Gets a sample name
std::string ROFSample::getName() const
{
    return "Rudin-Osher-Fatemi (TV)";
}

//! Returns a detailed sample description
std::string ROFSample::getDescription() const
{
    return "The Rudin-Osher-Fatemi functional aims at minimizing the Total Variation of an image. It is an anisotropic diffusion algorithm, where (unlike heat diffusion) grey levels hardly diffuse through image edges, hence preserving piecewise flat areas and avoiding undesired blur.\nThe ROF functional is solved here using the FISTA fast algorithm by Beck and Teboulle (2009).";
}

std::string ROFSample::getSampleIcon() const
{
    return "ROFSampleIcon.png";
}

//! Processes a frame and returns output image 
bool ROFSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    m_lambda = std::pow(10.0, m_lambdaExponent);
    
    std::vector<cv::Mat> inChannels;
    if (m_processColor)
        cv::split(inputFrame, inChannels);
    else {
        inChannels.resize(1);
        getGray(inputFrame, inChannels[0]);
    }
    
    std::vector<cv::Mat> outChannels(inChannels.size());
    cv::Mat tmp32f_in, tmp32f_out;
    
    for (int i = 0; i < MIN(3, inChannels.size()); ++i) {
        if (inChannels[i].type() != CV_32F)
            inChannels[i].convertTo(tmp32f_in, CV_32F, 1.0/255.0);
        else
            tmp32f_in = inChannels[i];
        
        tvdn_fista(tmp32f_in, m_lambda, m_iterations, tmp32f_out);
        tmp32f_out.convertTo(outChannels[i], CV_8U, 255.0);
    }
    
    if (m_processColor) {
        outChannels[3] = inChannels[3];
        cv::merge(outChannels, outputFrame);
    }
    else cv::cvtColor(outChannels[0], outputFrame, CV_GRAY2BGRA);
    
    return true;
}