/** ****************************************************************************
 *  @file    utils.cpp
 *  @brief   Converting a character code into a ascii index.
 *  @author  Roberto Valle Fernandez.
 *  @date    2012/01
 *  @copyright All rights reserved.
 *  Escuela Tecnica Superior de Ingenieria Informatica (Computer Science School)
 *  Universidad Rey Juan Carlos (Spain)
 ******************************************************************************/

// ----------------------- INCLUDES --------------------------------------------
#include <utils.hpp>
#include <string>

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
std::string
asciiCode2String
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
string2AsciiCode
  (
  const std::string character
  )
{
  int idx;
  // Digits
  if (character.compare("0")==0) idx = 19;
  else if (character.compare("1")==0) idx = 20;
  else if (character.compare("2")==0) idx = 21;
  else if (character.compare("3")==0) idx = 22;
  else if (character.compare("4")==0) idx = 23;
  else if (character.compare("5")==0) idx = 24;
  else if (character.compare("6")==0) idx = 25;
  else if (character.compare("7")==0) idx = 26;
  else if (character.compare("8")==0) idx = 27;
  else if (character.compare("9")==0) idx = 28;
  // Upper
  else if (character.compare("A")==0) idx = 36;
  else if (character.compare("B")==0) idx = 37;
  else if (character.compare("C")==0) idx = 38;
  else if (character.compare("D")==0) idx = 39;
  else if (character.compare("E")==0) idx = 40;
  else if (character.compare("F")==0) idx = 41;
  else if (character.compare("G")==0) idx = 42;
  else if (character.compare("H")==0) idx = 43;
  else if (character.compare("I")==0) idx = 44;
  else if (character.compare("J")==0) idx = 45;
  else if (character.compare("K")==0) idx = 46;
  else if (character.compare("L")==0) idx = 47;
  else if (character.compare("M")==0) idx = 48;
  else if (character.compare("N")==0) idx = 49;
  else if (character.compare("O")==0) idx = 50;
  else if (character.compare("P")==0) idx = 51;
  else if (character.compare("Q")==0) idx = 52;
  else if (character.compare("R")==0) idx = 53;
  else if (character.compare("S")==0) idx = 54;
  else if (character.compare("T")==0) idx = 55;
  else if (character.compare("U")==0) idx = 56;
  else if (character.compare("V")==0) idx = 57;
  else if (character.compare("W")==0) idx = 58;
  else if (character.compare("X")==0) idx = 59;
  else if (character.compare("Y")==0) idx = 60;
  else if (character.compare("Z")==0) idx = 61;
  else if ((character[0] == 'N') && (character[1] != '\0')) idx = 102;
  // Lower
  else if (character.compare("a_")==0) idx = 68;
  else if (character.compare("b_")==0) idx = 69;
  else if (character.compare("c_")==0) idx = 70;
  else if (character.compare("d_")==0) idx = 71;
  else if (character.compare("e_")==0) idx = 72;
  else if (character.compare("f_")==0) idx = 73;
  else if (character.compare("g_")==0) idx = 74;
  else if (character.compare("h_")==0) idx = 75;
  else if (character.compare("i_")==0) idx = 76;
  else if (character.compare("j_")==0) idx = 77;
  else if (character.compare("k_")==0) idx = 78;
  else if (character.compare("l_")==0) idx = 79;
  else if (character.compare("m_")==0) idx = 80;
  else if (character.compare("n_")==0) idx = 81;
  else if (character.compare("o_")==0) idx = 82;
  else if (character.compare("p_")==0) idx = 83;
  else if (character.compare("q_")==0) idx = 84;
  else if (character.compare("r_")==0) idx = 85;
  else if (character.compare("s_")==0) idx = 86;
  else if (character.compare("t_")==0) idx = 87;
  else if (character.compare("u_")==0) idx = 88;
  else if (character.compare("v_")==0) idx = 89;
  else if (character.compare("w_")==0) idx = 90;
  else if (character.compare("x_")==0) idx = 91;
  else if (character.compare("y_")==0) idx = 92;
  else if (character.compare("z_")==0) idx = 93;
  else if (character.compare("á_")==0) idx = 105;
  else if (character.compare("é_")==0) idx = 112;
  else if (character.compare("í_")==0) idx = 116;
  else if (character.compare("ñ_")==0) idx = 120;
  else if (character.compare("ó_")==0) idx = 121;
  else if (character.compare("ú_")==0) idx = 126;
  // Signs
  else if (character.compare("<")==0) idx = 31;
  else if (character.compare("?")==0) idx = 34;
  else if (character.compare("¿")==0) idx = 162;
  else idx = 0;

  return idx;
}

}; // close namespace urjc
