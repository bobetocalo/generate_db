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
void
loadTrueTypeForDNIs
  (
  std::vector<unsigned> &characters
  )
{
  unsigned DIGIT[10] = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };
  unsigned UPPER[27] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                         50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 102 };
  unsigned DELIM[1]  = { 31 };
  characters.insert(characters.end(), DIGIT, DIGIT + sizeof(DIGIT)/sizeof(DIGIT[0]));
  characters.insert(characters.end(), UPPER, UPPER + sizeof(UPPER)/sizeof(UPPER[0]));
  characters.insert(characters.end(), DELIM, DELIM + sizeof(DELIM)/sizeof(DELIM[0]));
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
loadTrueTypeForWildText
  (
  std::vector<unsigned> &characters
  )
{
  unsigned DIGIT[10] = { 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 };
  unsigned UPPER[26] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                         50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61 };
  unsigned LOWER[26] = { 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81,
                         82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93 };
  characters.insert(characters.end(), DIGIT, DIGIT + sizeof(DIGIT)/sizeof(DIGIT[0]));
  characters.insert(characters.end(), UPPER, UPPER + sizeof(UPPER)/sizeof(UPPER[0]));
  characters.insert(characters.end(), LOWER, LOWER + sizeof(LOWER)/sizeof(LOWER[0]));
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
int
main
  (
  int argc,
  char **argv
  )
{
  double ticks = static_cast<double>(cv::getTickCount());
  fs::path fonts_path(urjc::Constants::FONTS_DIR);
  if (!fs::exists(fonts_path) || !fs::is_directory(fonts_path))
  {
    ERROR("Error. Directory " << fonts_path << " doesn't exist");
    return EXIT_FAILURE;
  }

  PRINT("Allowed options");
  PRINT("  1) Use Spanish document identity OCR-B font");
  PRINT("  2) Use all True Type fonts");

  int option;
  std::cout << std::endl << "Enter the option: ";
  std::cin >> option;

  // Generate the synthetic images using Freetype library
  urjc::MyFreetype freetype;
  std::vector<unsigned> characters;
  std::string filename;
  fs::directory_iterator it1_end;
  switch (option)
  {
    case 1:
      loadTrueTypeForDNIs(characters);
      freetype.setCharacters(characters);
      PRINT("Open True Type font: Ocrb.ttf");
      filename = fonts_path.string() + "Ocrb.ttf";
      freetype.generateImagesFromTrueTypeFont(filename.c_str());
      break;
    case 2:
      loadTrueTypeForWildText(characters);
      freetype.setCharacters(characters);
      for (fs::directory_iterator it1(fonts_path) ; it1 != it1_end ; it1++)
      {
        PRINT("Open True Type font: " << it1->path().filename().string());
        if (it1->path().extension().string().compare(".ttf")==0)
          freetype.generateImagesFromTrueTypeFont(it1->path().c_str());
        else
          ERROR("Error. File " << it1->path() << " can't be opened");
      }
      break;
    default:
      break;
  }

  // Apply random transformations and save images
  TRACE("Transform images ...");
  freetype.transformImages();
  TRACE("Save images ...");
  freetype.saveImages(urjc::Constants::CHARS_DIR);

  ticks = static_cast<double>(cv::getTickCount() - ticks);
  PRINT("Elapsed time: " << (ticks/cv::getTickFrequency())*1000 << " ms");
  return EXIT_SUCCESS;
}
