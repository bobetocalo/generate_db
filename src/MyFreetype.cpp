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
#include <operations.hpp>
#include <trace.hpp>

#include <fstream>
#include <boost/filesystem.hpp>
#include <opencv/highgui.h> // imwrite

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
MyFreetype::MyFreetype
  (
  std::vector<unsigned> &characters
  )
{
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

  /// Generate a set of character images using a true type font
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
  TRACE("Transform images ...");
  cv::RNG rng(time(NULL));
  for (unsigned short i=0; i < m_images.size(); i++)
  {
    // Repeat images
    const std::vector<cv::Mat> aux = m_images[i];
    for (unsigned short j=0; j < Constants::NUM_ITERS; j++)
      m_images[i].insert(m_images[i].end(), aux.begin(), aux.end());

    // Make the transformations
    for (unsigned short j=0; j < m_images[i].size(); j++)
    {
      affineTransform(rng, m_images[i][j]);
      smoothTransform(rng, m_images[i][j]);
      modifyPixelsIntensity(rng, m_images[i][j]);
      morphologicTransform(rng, m_images[i][j]);
      anisotropicFilter(rng, m_images[i][j]);
      //cv::threshold(m_images[i][j], m_images[i][j], 0, 255, cv::THRESH_OTSU);
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
  TRACE("Save images ...");
  std::vector<int> compression_params;
  compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(3);
  for (unsigned short i=0; i < m_images.size(); i++)
  {
    // Create directory
    std::string character = this->freetypeCharCode2String(m_characters[i]);
    std::string mydir(output_dir);
    boost::filesystem::path mypath(mydir + character + "/");
    if (!boost::filesystem::exists(mypath))
      boost::filesystem::create_directory(mypath);

    // Save each image into a file
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

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
std::string
MyFreetype::freetypeCharCode2String
  (
  const int idx
  )
{
  std::string character;
  switch (idx)
  {
    // Digits
    case 19:  character = "0"; break;
    case 20:  character = "1"; break;
    case 21:  character = "2"; break;
    case 22:  character = "3"; break;
    case 23:  character = "4"; break;
    case 24:  character = "5"; break;
    case 25:  character = "6"; break;
    case 26:  character = "7"; break;
    case 27:  character = "8"; break;
    case 28:  character = "9"; break;
    // Upper
    case 36:  character = "A"; break;
    case 37:  character = "B"; break;
    case 38:  character = "C"; break;
    case 39:  character = "D"; break;
    case 40:  character = "E"; break;
    case 41:  character = "F"; break;
    case 42:  character = "G"; break;
    case 43:  character = "H"; break;
    case 44:  character = "I"; break;
    case 45:  character = "J"; break;
    case 46:  character = "K"; break;
    case 47:  character = "L"; break;
    case 48:  character = "M"; break;
    case 49:  character = "N"; break;
    case 50:  character = "O"; break;
    case 51:  character = "P"; break;
    case 52:  character = "Q"; break;
    case 53:  character = "R"; break;
    case 54:  character = "S"; break;
    case 55:  character = "T"; break;
    case 56:  character = "U"; break;
    case 57:  character = "V"; break;
    case 58:  character = "W"; break;
    case 59:  character = "X"; break;
    case 60:  character = "Y"; break;
    case 61:  character = "Z"; break;
    case 102: character = "Ñ"; break;
    // Lower
    case 68:  character = "a_"; break;
    case 69:  character = "b_"; break;
    case 70:  character = "c_"; break;
    case 71:  character = "d_"; break;
    case 72:  character = "e_"; break;
    case 73:  character = "f_"; break;
    case 74:  character = "g_"; break;
    case 75:  character = "h_"; break;
    case 76:  character = "i_"; break;
    case 77:  character = "j_"; break;
    case 78:  character = "k_"; break;
    case 79:  character = "l_"; break;
    case 80:  character = "m_"; break;
    case 81:  character = "n_"; break;
    case 82:  character = "o_"; break;
    case 83:  character = "p_"; break;
    case 84:  character = "q_"; break;
    case 85:  character = "r_"; break;
    case 86:  character = "s_"; break;
    case 87:  character = "t_"; break;
    case 88:  character = "u_"; break;
    case 89:  character = "v_"; break;
    case 90:  character = "w_"; break;
    case 91:  character = "x_"; break;
    case 92:  character = "y_"; break;
    case 93:  character = "z_"; break;
    case 105: character = "á_"; break;
    case 112: character = "é_"; break;
    case 116: character = "í_"; break;
    case 120: character = "ñ_"; break;
    case 121: character = "ó_"; break;
    case 126: character = "ú_"; break;
    // Signs
    case 31:  character = "<"; break;
    case 34:  character = "?"; break;
    case 162: character = "¿"; break;
    default:  character = "-"; break;
  }

  return character;
}

}; // close namespace urjc
