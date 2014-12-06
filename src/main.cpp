/** ****************************************************************************
 *  @file    main.cpp
 *  @brief   Generate the characters database.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ----------------------- INCLUDES --------------------------------------------
#include <MyFreetype.hpp>
#include <Constants.hpp>
#include <trace.hpp>

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <opencv/cv.h>

namespace fs = boost::filesystem;

// -----------------------------------------------------------------------------
//
// Purpose and Method:
// Inputs:
// Outputs:
// Dependencies:
// Restrictions and Caveats:
//
// -----------------------------------------------------------------------------
int
main
  (
  int argc,
  char **argv
  )
{
  PRINT("Program started ...");

  // Open the fonts directory
  fs::path fonts_dir(urjc::Constants::FONTS_DIR);
  if (fs::exists(fonts_dir) && fs::is_directory(fonts_dir))
  {
    double ticks = static_cast<double>(cv::getTickCount());
    #ifndef DNIe
    /// Generate the synthetic images for each true type font using Freetype
    unsigned DIGIT[10] = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };
    unsigned UPPER[26] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                           50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61 };
    unsigned LOWER[26] = { 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                           82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93 };
    std::vector<unsigned> characters;
    characters.insert(characters.end(), DIGIT, DIGIT + sizeof(DIGIT)/sizeof(DIGIT[0]));
    characters.insert(characters.end(), UPPER, UPPER + sizeof(UPPER)/sizeof(UPPER[0]));
    characters.insert(characters.end(), LOWER, LOWER + sizeof(LOWER)/sizeof(LOWER[0]));
    urjc::MyFreetype freetype(characters);

    fs::directory_iterator end_it;
    for (fs::directory_iterator it(fonts_dir) ; it != end_it ; it++)
    {
      if (fs::is_regular_file(it->status()))
      {
        // Check each true type font
        PRINT("  Open true type font: " << it->path().filename().c_str());
        std::string extension = it->path().extension().c_str();
        if (!extension.compare(".ttf"))
        {
          freetype.generateImagesFromTrueTypeFont(it->path().c_str());
        }
        else
        {
          ERROR("  Error. File '" << it->path().c_str() << "' can't be opened");
          continue;
        }
      }
    }
    #else
    /// Generate the synthetic images for the OCRB true type font using Freetype
    unsigned DIGIT[10] = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };
    unsigned UPPER[27] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                           50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 102 };
    unsigned DELIM[1] = { 31 };
    std::vector<unsigned> characters;
    characters.insert(characters.end(), DIGIT, DIGIT + sizeof(DIGIT)/sizeof(DIGIT[0]));
    characters.insert(characters.end(), UPPER, UPPER + sizeof(UPPER)/sizeof(UPPER[0]));
    characters.insert(characters.end(), DELIM, DELIM + sizeof(DELIM)/sizeof(DELIM[0]));
    urjc::MyFreetype freetype(characters);

    // Check the OCRB true type font
    std::string ocrb_ttf = "Ocrb.ttf";
    PRINT("  Open true type font: " << ocrb_ttf);
    std::string input_dir = fonts_dir.c_str() + ocrb_ttf;
    freetype.generateImagesFromTrueTypeFont(input_dir.c_str());
    #endif

    /// Apply random transformations and save images
    freetype.transformImages();
    freetype.saveImages(urjc::Constants::CHARS_DIR);

    ticks = static_cast<double>(cv::getTickCount() - ticks);
    PRINT("Freetype time: " << (ticks/cv::getTickFrequency())*1000 << " ms");
    return EXIT_SUCCESS;
  }
  else
  {
    ERROR("Error. Directory '" << urjc::Constants::FONTS_DIR << "' doesn't exist");
    return EXIT_FAILURE;
  }
}
