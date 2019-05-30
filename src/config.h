#ifndef _CONFIG_H_
#define _CONFIG_H


/**
 * The default window WIDTH and HEIGHT in pixel.
*/
static const int WINDOW_WIDTH = 1440;
static const int WINDOW_HEIGHT = 900;

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

#define BACKGROUND_COLOR "#212121"

/*
 *  COLOR_1 is default selected color
*/
#define COLOR_1         "#3B7A88"  
#define COLOR_2         "#7DC27A"
#define COLOR_3         "#FC736D"
#define COLOR_4         "#BDCDBF"

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