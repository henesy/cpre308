/**
 * @file      bash_color.h
 * @author    Jeramie Vens
 * @brief     Bash Color Codes
 */

#ifndef BASH_COLOR_H
#define BASH_COLOR_H

/// @def BASH_RESET
/// Reset the terminal to default colors
#define BASH_RESET "\x1b[0m"

/// @def BASH_FORE_BLACK
/// Set the foreground color to black
#define BASH_FORE_BLACK		"\x1b[30m" 
/// @def BASH_FORE_RED
/// Set the foreground color to red
#define BASH_FORE_RED 		"\x1b[31m"
/// @def BASH_FORE_GREEN
/// Set the foreground color to green
#define BASH_FORE_GREEN 		"\x1b[32m"
/// @def BASH_FORE_YELLOW
/// Set the foreground color to yellow
#define BASH_FORE_YELLOW 	"\x1b[33m"
/// @def BASH_FORE_BLUE
/// Set the foreground color to blue
#define BASH_FORE_BLUE 		"\x1b[34m"
/// @def BASH_FORE_MAGENTA
/// Set the foreground color to magenta
#define BASH_FORE_MAGENTA	"\x1b[35m"
/// @def BASH_FORE_CYAN
/// Set the foreground color to CYAN
#define BASH_FORE_CYAN 		"\x1b[36m"
/// @def BASH_FORE_WHITE
/// Set the foreground color to white
#define BASH_FORE_WHITE		"\x1b[37m"

/// @def BASH_BACK_BLACK
/// Set the background color to black
#define BASH_BACK_BLACK		"\x1b[40m"
/// @def BASH_BACK_RED
/// Set the background color to red
#define BASH_BACK_RED		"\x1b[41m"
/// @def BASH_BACK_GREEN
/// Set the background color to green
#define BASH_BACK_GREEN		"\x1b[42m"
/// @def BASH_BACK_YELLOW
/// Set the background color to yellow
#define BASH_BACK_YELLOW		"\x1b[43m"
/// @def BASH_BACK_BLUE
/// Set the background color to blue
#define BASH_BACK_BLUE		"\x1b[44m"
/// @def BASH_BACK_MAGENTA
/// Set the background color to magenta
#define BASH_BACK_MAGENTA	"\x1b[45m"
/// @def BASH_BACK_CYAN
/// Set the background color to cyan
#define BASH_BACK_CYAN		"\x1b[46m"
/// @def BASH_BACK_WHITE
/// Set the background color to white
#define BASH_BACK_WHITE		"\x1b[47m"

/// @def BASH_ATTR_BRIGHT
/// Set the text to be brightly colored 
#define BASH_ATTR_BRIGHT		"\x1b[1m"
/// @def BASH_ATTR_DIM
/// Set the text to be dimmly colored
#define BASH_ATTR_DIM		"\x1b[2m"
/// @def BASH_ATTR_UNDERSCORE
/// Set the text to have an underscore
#define BASH_ATTR_UNDERSCORE	"\x1b[4m"
/// @def BASH_ATTR_REVERSE
/// Invert the foreground and background colors
#define BASH_ATTR_REVERSE	"\x1b[7m"
/// @def BASH_ATTR_HIDDEN
/// Set the text to not be visible
#define BASH_ATTR_HIDDEN		"\x1b[8m"

#endif



