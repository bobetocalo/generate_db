/** ****************************************************************************
 *  @file    Constants.hpp
 *  @brief   Identifier whose associated value cannot be altered.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ------------------ RECURSION PROTECTION -------------------------------------
#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

namespace urjc {

/** ****************************************************************************
 * @class Constants
 * @brief Class identifiers used by the computer vision algorithms.
 ******************************************************************************/
class Constants
{
public:

  static const char *FONTS_DIR, *CHARS_DIR;
  static const double ROTATION_ANGLE;
  static const unsigned CHAR_SIZE, NUM_ITERS;
};

} // close namespace urjc

#endif /* CONSTANTS_HPP */
