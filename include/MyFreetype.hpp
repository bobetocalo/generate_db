/** ****************************************************************************
 *  @file    MyFreetype.hpp
 *  @brief   Use Freetype library to generate images.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ------------------ RECURSION PROTECTION -------------------------------------
#ifndef MYFREETYPE_HPP
#define MYFREETYPE_HPP

// ----------------------- INCLUDES --------------------------------------------
#include <string>
#include <vector>
#include <opencv/cv.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

namespace urjc {

/** ****************************************************************************
 * @class MyFreetype
 * @brief A class that use Freetype library to generate images.
 ******************************************************************************/
class MyFreetype
{
public:

  // Constructor
  MyFreetype
    () {};

  // Destroyer
  ~MyFreetype
    () {};

  /**
   * @brief Set characters of interest.
   */
  void
  setCharacters
    (
    std::vector<unsigned> &characters
    );

  /**
   * @brief Generate a list of synthetic images using a True Type font.
   */
  void
  generateImagesFromTrueTypeFont
    (
    const char *input_dir
    );

  /**
   * @brief Repeat images and apply random algorithm operations.
   */
  void
  transformImages
    ();

  /**
   * @brief Save synthetic images in the output directory.
   */
  void
  saveImages
    (
    const char *output_dir
    );

private:

  /**
   * @brief Create a set of images with different rotations from the
   * character associated to this index.
   */
  void
  writeGlyphAsBitmap
    (
    const int idx,
    FT_Face &face
    );

  // Set digits + uppers + lowers + delimiter
  std::vector<unsigned> m_characters;

  // For each character store images with different fonts and rotations
  std::vector< std::vector<cv::Mat> > m_images;
};

}; // close namespace urjc

#endif /* MYFREETYPE_HPP */
