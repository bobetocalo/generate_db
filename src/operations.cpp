/** ****************************************************************************
 *  @file    operations.cpp
 *  @brief   Apply random transformation algorithms.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ----------------------- INCLUDES --------------------------------------------
#include <operations.hpp>
#include <opencv/highgui.h>

namespace urjc {

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
affineTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  )
{
  float angle = 0.0; // rotate about origin
  float scale = rng.uniform(0.9f, 0.95f); // scale about origin
  float tx = rng.uniform(-1.0f, 2.0f); // translate
  float ty = rng.uniform(-1.0f, 2.0f); // translate

  // 2x3 transformation matrix (2D rotation + 2D translation + scale)
  cv::Matx23f M( scale*cos(angle), sin(angle), tx,
                -sin(angle), scale*cos(angle), ty);

  cv::Mat output;
  cv::warpAffine(img, output, M, output.size());
  img = output.clone();
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
smoothTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  )
{
  int option = rng.uniform(0, 2);
  if (option == 1)
  {
    cv::Mat output;
    int kernel_size = rng.uniform(2, 4);
    cv::blur(img, output, cv::Size(kernel_size,kernel_size), cv::Point(-1,-1));
    img = output.clone();
  }
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
morphologicTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  )
{
  cv::Mat output, kernel = cv::Mat(3, 3, CV_8U);
  cv::Point anchor = cv::Point(-1,-1);
  int iters = 1, border_type = cv::BORDER_REPLICATE;
  int option = rng.uniform(0, 3);
  switch (option)
  {
    case 0:
      cv::erode(img, output, kernel, anchor, iters, border_type);
      img = output.clone();
      break;
    case 1:
      cv::dilate(img, output, kernel, anchor, iters, border_type);
      img = output.clone();
      break;
    case 2:
      break;
  }
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
modifyPixelsIntensity
  (
  cv::RNG &rng,
  cv::Mat &img
  )
{
  // Modify pixel intensity in background and character
  for (int row=0; row < img.rows; row++)
  {
    for (int col=0; col < img.cols; col++)
    {
      uchar *pixel = &img.at<uchar>(row, col);
      if (static_cast<int>(*pixel) < 128)
        *pixel = static_cast<uchar>(rng.uniform(128, 256)); // White background
      else
        *pixel = static_cast<uchar>(rng.uniform(0, 128)); // Black character
    }
  }
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
anisotropicFilter
  (
  cv::RNG &rng,
  cv::Mat &img
  )
{
  int option = rng.uniform(0, 2);
  if (option == 1)
  {
    // Preprocess the generated images
    float std = (static_cast<float>(11)-1.0)/(7.0*2.0);
    cv::Mat smoothed = cv::Mat(img.rows, img.cols, img.type());
    cv::Mat conv_mask = createGaussianMask(11, std);
    anisotropicSmooth(img, smoothed, conv_mask);

    cv::Mat output;
    cv::add(smoothed, cv::Scalar(0.000001), smoothed); // CV_32FC1
    img.convertTo(img, CV_32FC1);
    cv::divide(img, smoothed, output);
    output.convertTo(output, CV_8UC1); // CV_8UC1
    cv::equalizeHist(output, output);
    img = output.clone();
  }
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
cv::Mat
createGaussianMask
  (
  size_t size,
  float std
  )
{
  cv::Mat kernel = cv::Mat(size, size, CV_32FC1);
  double EPS = std::numeric_limits<double>::epsilon();

  // Mask size should be odd
  if (size % 2 == 0)
    size = size + 1;
  int half_size = (size - 1)/2;
  
  float sum = 0.0;
  float x, y, gauss;
  float gauss_max = exp(-(0.0)/(2.0*std*std)) * EPS;
  for (int row=0; row < size; row++)
  {
    for (int col=0; col < size; col++)
    {
      x = static_cast<float>(col - half_size);
      y = static_cast<float>(row - half_size);
      gauss = exp( -(x*x + y*y)/(2.0*std*std) );
      if (gauss < gauss_max)
        gauss = 0.0;

      kernel.at<float>(row, col) = gauss;
      sum += gauss;
    }
  }
  kernel *= (1.0f/sum);
  
  return kernel;
}

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
void
anisotropicSmooth
  (
  cv::Mat &src,
  cv::Mat &dst,
  cv::Mat &kernel
  )
{
  // Add convolution borders to apply anisotropic filter
  float offset_j = floor(kernel.cols / 2.0f);
  float offset_i = floor(kernel.rows / 2.0f);
  cv::Mat src_border = cv::Mat(cv::Size(src.cols+(offset_j*2),src.rows+(offset_i*2)), src.depth(), src.channels());
  cv::copyMakeBorder(src, src_border, offset_i, offset_i, offset_j, offset_j, cv::BORDER_REPLICATE);

  dst.convertTo(dst, CV_32FC1);
  const float num_pixels = kernel.cols*kernel.rows;
  for (int i=offset_i; i < src_border.rows-offset_i; i++)
  {
    for (int j=offset_j; j < src_border.cols-offset_j; j++)
    {
      // Set in a source image the kernel ROI patch centered on (i,j) pixel
      cv::Mat src_aux = src_border(cv::Rect(j-offset_j, i-offset_i, kernel.cols, kernel.rows));

      // Find which of the input image elements of the current patch are bigger
      // than the mean
      cv::Scalar mean = cv::mean(src_aux);
      cv::Mat big_than_mean;
      cv::inRange(src_aux, mean, cv::Scalar(256), big_than_mean);

      // Set to zero the kernel elements with corresponding source elements not
      // bigger than mean.
      big_than_mean *= (1.0f/255.0f);
      big_than_mean.convertTo(big_than_mean, CV_32FC1);
      cv::Mat kernel_aux;
      cv::multiply(kernel, big_than_mean, kernel_aux);

      // Correlation of source patch and kernel (division by the kernel sum of
      // non zero elements)
      cv::Scalar scalar;
      scalar = cv::sum(kernel_aux);
      src_aux.convertTo(src_aux, CV_32FC1);
      cv::multiply(src_aux, kernel_aux, kernel_aux, 1.0/scalar.val[0]);
      scalar = cv::sum(kernel_aux);

      // Set the final value in the destination image
      double value = scalar.val[0]/num_pixels;
      dst.at<float>(i-offset_i,j-offset_j) = static_cast<float>(value);
    }
  }
};

}; // close namespace urjc
