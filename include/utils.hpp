/** ****************************************************************************
 *  @file    utils.hpp
 *  @brief   Converting a character code into a ascii index.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ------------------ RECURSION PROTECTION -------------------------------------
#ifndef UTILS_HPP
#define UTILS_HPP

// ----------------------- INCLUDES --------------------------------------------
#include <string>

namespace urjc {

/**
 *  @brief Returns the character associated to this ascii index.
 */
std::string
asciiCode2String
  (
  const int idx
  );

/**
 *  @brief Returns the ascii index associated to this character.
 */
int
string2AsciiCode
  (
  const std::string character
  );

}; // close namespace urjc

#endif /* UTILS_HPP */
