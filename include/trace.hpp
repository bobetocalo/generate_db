/** ****************************************************************************
 *  @file    trace.hpp
 *  @brief   Debug macros definition.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ------------------ RECURSION PROTECTION -------------------------------------
#ifndef TRACE_HPP_
#define TRACE_HPP_

#include <iostream>

#define PRINT(...) std::cout << __VA_ARGS__ << std::endl;
#define ERROR(...) std::cerr << __VA_ARGS__ << std::endl;

#ifndef _RELEASE
  #define TRACE(...) std::cout << __VA_ARGS__ << std::endl;
  #define TRACE_INFO(...) std::cout << __FILE__ << "(" << __LINE__ << "):" << __VA_ARGS__ << std::endl;
#else
  #define TRACE(...)
  #define TRACE_INFO(...)
#endif

#endif /* TRACE_HPP_ */
