//
//  cvneon.cpp
//  OpenCV Tutorial
//
//  Created by BloodAxe on 8/12/12.
//
//

#include "cvneon.h"


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
    asm volatile
    (
     // Store m & v - avoiding q4-q7 which need to be preserved - q0 = result
     "vldmia %1, { q8-q11 } \n\t"   // q8-q11 = m
     "vldmia %2, { q1 }     \n\t"   // q1     = v
     
     // Convert v to floats
     "vcvt.f32.s32 q1, q1 \n\t"
     
     // result = first column of A x V.x
     "vmul.f32 q0, q8, d2[0] \n\t"
     
     // result += second column of A x V.y
     "vmla.f32 q0, q9, d2[1] \n\t"
     
     // result += third column of A x V.z
     "vmla.f32 q0, q10, d3[0] \n\t"
     
     // result += last column of A x V.w
     "vmla.f32 q0, q11, d3[1] \n\t"
     
     // convert to integer
     "vcvt.s32.f32 q0, q0 \n\t"
     
     // output = result registers
     "vstmia %0, { q0 }  \n\t"
     
     : // no output
     : "r" (output), "r" (m), "r" (v)      // input - note *value* of pointer doesn't change
     : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11" //clobber
     );
  }
  
  static void neon_asm_convert(uint8_t * __restrict dest, uint8_t * __restrict src, int numPixels)
  {
    __asm__ volatile("lsr          %2, %2, #3      \n"
                     "# build the three constants: \n"
                     "mov         r4, #28          \n" // Blue channel multiplier
                     "mov         r5, #151         \n" // Green channel multiplier
                     "mov         r6, #77          \n" // Red channel multiplier
                     "vdup.8      d4, r4           \n"
                     "vdup.8      d5, r5           \n"
                     "vdup.8      d6, r6           \n"
                     "0:						   \n"
                     "# load 8 pixels:             \n"
                     "vld4.8      {d0-d3}, [%1]!   \n"
                     "# do the weight average:     \n"
                     "vmull.u8    q7, d0, d4       \n"
                     "vmlal.u8    q7, d1, d5       \n"
                     "vmlal.u8    q7, d2, d6       \n"
                     "# shift and store:           \n"
                     "vshrn.u16   d7, q7, #8       \n" // Divide q3 by 256 and store in the d7
                     "vst1.8      {d7}, [%0]!      \n"
                     "subs        %2, %2, #1       \n" // Decrement iteration count
                     "bne         0b            \n" // Repeat unil iteration count is not zero
                     :
                     : "r"(dest), "r"(src), "r"(numPixels)
                     : "r4", "r5", "r6"
                     );
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