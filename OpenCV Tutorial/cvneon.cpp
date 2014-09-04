//
//  cvneon.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 8/12/12.
//
//

#include "cvneon.h"
#include <cassert>

#if (!TARGET_IPHONE_SIMULATOR)
#include <arm_neon.h>
#endif

namespace cv
{
  bool isAligned(const cv::Mat& m)
  {
    return m.step % 16 == 0;
  }
  
  void initSameSizeAlignedIfNecessary(const cv::Mat& source, cv::Mat& aligned)
  {
    const int continuosStep = source.cols * source.elemSize();
    const int alignedStep   = (continuosStep / 16 + (continuosStep % 16 > 0 ? 1 : 0)) * 16;
    
    if (aligned.rows != source.rows || aligned.cols != source.cols || aligned.step != alignedStep)
    {
      aligned = cv::Mat(source.rows, source.cols, source.type(), alignedStep);
    }
  }
  
  void align16(const cv::Mat& m, cv::Mat& aligned)
  {
    assert(!m.empty());
    
    const int continuosStep = m.cols * m.elemSize();
    const int alignedStep   = (continuosStep / 16 + (continuosStep % 16 > 0 ? 1 : 0)) * 16;
    
    if (aligned.rows != m.rows || aligned.cols != m.cols || aligned.step != alignedStep)
    {
      aligned = cv::Mat(m.rows, m.cols, m.type(), alignedStep);
    }
    
    for (int row = 0; row < m.rows; row++)
    {
      void * srcRow = m.row(row).data;
      void * dstRow = aligned.row(row).data;
      memcpy(dstRow, srcRow, continuosStep);
    }
  }
  
#if (!TARGET_IPHONE_SIMULATOR)
  
  static void neon_asm_mat4_vec4_mul(const float* __restrict m, const int* __restrict v, int* __restrict output)
  {
      float32x4x4_t _m = vld4q_f32(m);
      float32x4_t   _v = vcvtq_f32_s32(vld1q_s32(v));
      
      float32x4_t    r = vmulq_n_f32(_m.val[0], _v[0]);
      r = vmlaq_n_f32(r, _m.val[1], _v[1]);
      r = vmlaq_n_f32(r, _m.val[2], _v[2]);
      r = vmlaq_n_f32(r, _m.val[3], _v[3]);
      
      int32x4_t result = vcvtq_s32_f32(r);
      vst1q_s32(output, result);
  }
  
  static void neon_asm_convert(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
  {
      uint8x8_t r = vdup_n_u8(28);
      uint8x8_t g = vdup_n_u8(151);
      uint8x8_t b = vdup_n_u8(77);
      
      for (int i = 0; i < numPixels; i += 8, src += 32, dest += 8)
      {
          uint8x8x4_t _src = vld4_u8(src);
          int16x8_t gray = vmull_u8(r, _src.val[0]);
          gray = vmlal_u8(gray, _src.val[1], g);
          gray = vmlal_u8(gray, _src.val[2], b);
          uint8x8_t res = vshrn_n_s16(gray, 8);
          vst1_u8(dest, res);
      }
      
  }
  
  void neon_cvtColorBGRA2GRAY(const cv::Mat& input, cv::Mat& gray)
  {
    assert(input.type() == CV_8UC4);

    if (gray.rows != input.rows || gray.cols != input.cols || gray.type() != CV_8UC1)
    {
      gray.create(input.rows, input.cols, CV_8UC1);
    }
    
    uint8_t * __restrict dest = gray.data;
    uint8_t * __restrict src  = input.data;
    int numPixels = input.rows * input.cols;
    
    neon_asm_convert(dest, src, numPixels);
  }
  
  
  void neon_transform_bgra(const cv::Mat& input, cv::Mat& result, const cv::Mat_<float>& m)
  {
    assert(input.type() == CV_8UC4);
    
    if (result.rows != input.rows || result.cols != input.cols || result.type() != CV_8UC4)
    {
      result.create(input.rows, input.cols, CV_8UC4);
    }
    //result = input.clone();
    //initSameSizeAlignedIfNecessary(m, result);
    
    //cv::Mat trans;
    //cv::transpose(m, trans);
    
    float * matrix = reinterpret_cast<float*>(m.data);
    
    int v[4];
    int out[4];
    
    for (int row = 0; row < input.rows; row++)
    {
      cv::Vec4b * srcRow = reinterpret_cast<cv::Vec4b*>(input.row(row).data);
      cv::Vec4b * dstRow = reinterpret_cast<cv::Vec4b*>(result.row(row).data);
      
      for (int col = 0; col < input.cols; col++)
      {
        const cv::Vec4b& src = srcRow[col];
        cv:Vec4b& dst        = dstRow[col];
        
        v[0] =  src[0];
        v[1] =  src[1];
        v[2] =  src[2];
        v[3] =  src[3];
        
        neon_asm_mat4_vec4_mul(matrix, v, out);
        
        dst[0] = cv::saturate_cast<unsigned char>(out[0]);
        dst[1] = cv::saturate_cast<unsigned char>(out[1]);
        dst[2] = cv::saturate_cast<unsigned char>(out[2]);
        dst[3] = cv::saturate_cast<unsigned char>(out[3]);
      }
    }
  }
  
#endif
  
  
}