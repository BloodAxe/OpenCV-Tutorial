//
//  ROFSample.cpp
//  OpenCV Tutorial
//
//  Created by Emmanuel d'Angelo on 20.07.12.
//  Copyright (c) 2012 EPFL/STI/IEL/LTS2. All rights reserved.
//

#include <iostream>
#include "ROFSample.h"

ROFSample::ROFSample() : m_iterations(10), m_lambda(1.0) 
{
    registerOption("Iterations",  "", &m_iterations, 1, 100);
    registerOption("Data weight", "", &m_lambda, 0.01, 100.0);
}

//! Gets a sample name
std::string ROFSample::getName() const
{
    return "Rudin-Osher-Fatemi (TV)";
}

//! Returns a detailed sample description
std::string ROFSample::getDescription() const
{
    return "The Rudin-Osher-Fatemi functional aims at minimizing the Total Variation of an image. It is an anisotropic diffusion algorithm, where (unlike heat diffusion) grey levels hardly diffuse through image edges, hence preserving piecewise flat areas and avoiding blur.";
}

std::string ROFSample::getSampleIcon() const
{
    return "";
}

//! Processes a frame and returns output image 
bool ROFSample::processFrame(const cv::Mat& inputFrame, cv::Mat& outputFrame)
{
    getGray(inputFrame, grayImage);
    inputFrame.copyTo(outputFrame);
    return true;
}