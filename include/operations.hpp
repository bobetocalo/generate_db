/** ****************************************************************************
 *  @file    operations.hpp
 *  @brief   Apply random transformation algorithms.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ------------------ RECURSION PROTECTION -------------------------------------
#ifndef OPERATIONS_HPP
#define OPERATIONS_HPP

// ----------------------- INCLUDES --------------------------------------------
#include <opencv/cv.h>

namespace urjc {

/**
 * @brief Applies an affine transformation to an image.
 */
void
affineTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  );

/**
 * @brief Applies a smooth blur noise.
 */
void
smoothTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  );

/**
 * @brief Applies an erasion or dilation operator.
 */
void
morphologicTransform
  (
  cv::RNG &rng,
  cv::Mat &img
  );

/**
 * @brief Changes the background and character color intensity.
 */
void
modifyPixelsIntensity
  (
  cv::RNG &rng,
  cv::Mat &img
  );

/**
 * @brief Applies an anisotropic filter.
 */
void
anisotropicFilter
  (
  cv::RNG &rng,
  cv::Mat &img
  );

cv::Mat
createGaussianMask
  (
  size_t size,
  float std
  );

void
anisotropicSmooth
  (
  cv::Mat &src,
  cv::Mat &dst,
  cv::Mat &kernel
  );
  
}; // close namespace urjc

#endif /* OPERATIONS_HPP */
