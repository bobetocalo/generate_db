/** ****************************************************************************
 *  @file    MyFreetype.cpp
 *  @brief   Use Freetype library to generate images.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ----------------------- INCLUDES --------------------------------------------
#include <MyFreetype.hpp>
#include <Constants.hpp>
#include <utils.hpp>
#include <operations.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
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
MyFreetype::setCharacters
  (
  std::vector<unsigned> &characters
  )
{
  // Initialize members
  m_characters = characters;
  m_images.resize(m_characters.size());
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
MyFreetype::generateImagesFromTrueTypeFont
  (
  const char *input_dir
  )
{
  // Initialize Freetype library
  FT_Library library;
  FT_Init_FreeType(&library);

  // Generate a set of character images using a True Type font directory
  std::ifstream ttf_file;
  ttf_file.open(input_dir, std::ios::binary);
  if (ttf_file.is_open())
  {
    // Get length of file
    ttf_file.seekg(0, std::ios::end);
    std::fstream::pos_type ttf_size = ttf_file.tellg();
    ttf_file.seekg (0, std::ios::beg);
    // Allocate memory
    unsigned char *buffer = new unsigned char[ttf_size];
    // Read data as a block
    ttf_file.read((char*)buffer, ttf_size);
    ttf_file.close();

    // Create a font face object
    FT_Face face;
    FT_New_Memory_Face(library, buffer, ttf_size, 0, &face);

    // Dump out each Glyph to a Bitmap
    for (int idx=0; idx < m_characters.size(); idx++)
      this->writeGlyphAsBitmap(idx, face);

    // Now that we are done it is safe to delete the memory
    delete [] buffer;

    FT_Done_Face(face);
    FT_Done_FreeType(library);
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
MyFreetype::transformImages
  ()
{
  cv::RNG rng(time(NULL));
  for (unsigned short i=0; i < m_images.size(); i++)
  {
    // Repeat images
    const std::vector<cv::Mat> aux = m_images[i];
    for (unsigned short j=0; j < Constants::NUM_ITERS; j++)
      m_images[i].insert(m_images[i].end(), aux.begin(), aux.end());

    // Make the random transformations
    for (unsigned short j=0; j < m_images[i].size(); j++)
    {
      affineTransform(rng, m_images[i][j]);
      smoothTransform(rng, m_images[i][j]);
      modifyPixelsIntensity(rng, m_images[i][j]);
      morphologicTransform(rng, m_images[i][j]);
      anisotropicFilter(rng, m_images[i][j]);
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
MyFreetype::saveImages
  (
  const char *output_dir
  )
{
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(3);
  for (unsigned short i=0; i < m_images.size(); i++)
  {
    // Create directory
    std::string character = asciiCode2String(m_characters[i]);
    std::string mydir(output_dir);
    boost::filesystem::path mypath(mydir + character + "/");
    if (!boost::filesystem::exists(mypath))
      boost::filesystem::create_directory(mypath);

    // Save each image into a new file
    for (unsigned short j=0; j < m_images[i].size(); j++)
    {
      std::string aux(mypath.string() + "char_" + character + "_" + std::to_string(j) + ".png");
      cv::imwrite(aux, m_images[i][j], compression_params);
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
MyFreetype::writeGlyphAsBitmap
  (
  const int idx,
  FT_Face &face
  )
{
  // Set the size to use at 200dpi
  FT_Set_Char_Size(face, Constants::CHAR_SIZE*64, Constants::CHAR_SIZE*64, 200, 200);

  // For each character create a lot of images with different rotations
  for (double r=-Constants::ROTATION_ANGLE; r <= Constants::ROTATION_ANGLE; r++)
  {
    // Set up a transformation matrix
    FT_Matrix matrix;
    double angle = r * (2.0*M_PI)/360.0;
    matrix.xx = (FT_Fixed)( cos(angle) * 0x10000L);
    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
    matrix.yx = (FT_Fixed)( sin(angle) * 0x10000L);
    matrix.yy = (FT_Fixed)( cos(angle) * 0x10000L);

    FT_Vector pen;
    pen.x = 0;
    pen.y = 0;

    FT_Set_Transform(face, &matrix, &pen);

    // Load the glyph we are looking for
    FT_UInt glyph_index = m_characters[idx];
    FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

    FT_Glyph glyph;
    FT_Get_Glyph(face->glyph, &glyph);

    // Convert The Glyph To A Bitmap
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

    // This reference will make accessing the bitmap easier
    FT_Bitmap &bitmap = bitmap_glyph->bitmap;

    // Create the grayscale image
    int width = bitmap.width;
    int height = bitmap.rows;
    cv::Mat image = cv::Mat(height, width, CV_8UC1);
    for (int row = 0; row < height; row++)
    {
      for (int col = 0; col < width; col++)
      {
    	uchar pixel = static_cast<uchar>(bitmap.buffer[col + bitmap.width*row]);
    	image.at<uchar>(cv::Point(col, row)) = pixel;
      }
    }

    // Store the image
    m_images[idx].push_back(image);

    // Clean up afterwards
    FT_Done_Glyph(glyph);
  }
}

}; // close namespace urjc
