#ifndef _CONFIG_H_
#define _CONFIG_H

/*
 * Colors are parsed thorugh gdk_rgba_parse
 * accepted string formats are:
 * 
 *      “#rgb”, “#rrggbb”, “#rrrgggbbb” or ”#rrrrggggbbbb”       
 *      “rgb(r,g,b)”  or “rgba(r,g,b,a)”
 * 
 * Where “r”, “g”, “b” and “a” are respectively the red, green, blue and alpha color values. 
 * In the last two cases, “r”, “g”, and “b” are either integers in the range 0 to 255 or percentage values 
 * in the range 0% * to 100%, 
 * and a is a floating point value in the range 0 to 1. 
*/

#define BACKGROUND_COLOR "#000000"

/*
 *  COLOR_1 is default selected color
*/
#define COLOR_1         "#ff0000"  
#define COLOR_2         "#00ff00"
#define COLOR_3         "#0000ff"
#define COLOR_4         "#ffffff"

/*
 *  LINE_WIDTH should be a float number between 0.5 and 15
 *  LINE_VARY should be a float number between 0.1 and 5.0 
 * 
 *  increase/decrease of LINE_WIDTH only works while (LINE_WIDTH + LINE_VARY) is 
 *      > 0.5
 *      < 15.0
*/
#define LINE_WIDTH 4.0
#define LINE_VARY 1.0

#endif