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
 * @brief A class that stores images information.
 ******************************************************************************/
class MyFreetype
{
public:

  // Constructor
  MyFreetype
    (
    std::vector<unsigned> &characters
    );

  // Destroyer
  ~MyFreetype
    () {};

  /**
   * @brief Generate a set of character images using a true type font.
   */
  void
  generateImagesFromTrueTypeFont
    (
    const char *input_dir
    );

  /**
   * @brief Apply image random algorithm operations.
   */
  void
  transformImages
    ();

  /**
   * @brief Save synthetic character images to the specific directory.
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

  /**
   * @brief Returns the character associated to this index.
   */
  std::string
  freetypeCharCode2String
    (
    const int idx
    );

  // 10 digits + 26 uppers + 26 lowers
  std::vector<unsigned> m_characters;

  // For each character store images with different fonts and rotations
  std::vector< std::vector<cv::Mat> > m_images;
};

}; // close namespace urjc

#endif /* MYFREETYPE_HPP */
