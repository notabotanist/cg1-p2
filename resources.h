////////////////////////////////////////////////////////////
//
// File:  resources.h
// Authors:  G. Fotiades, A. Koutmos
// Contributors: Sean Strout
// Last modified: 4/1/11
//
// Description:  This file holds man common values that are used to set
//               up the GUI as well as declaring some variables that other
//               files need to be incorporated at link time.
//
////////////////////////////////////////////////////////////


#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include <string>
#include <vector>
#include "vecmath.h"


//******************************************
// Stuff that might want to be modified (non static, elements)
//******************************************

#define ARROW_MOVE_FACTOR 1.5

#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 700

#define MIN_WINDOW_RENDERING_X 560
#define MIN_WINDOW_RENDERING_Y 675

#define INIT_LOCATION_X 200
#define INIT_LOCATION_Y 100

// Tracking notations
#define RENDERING_CUBE 0
#define RENDERING_CYL 1
#define RENDERING_CONE 2
#define RENDERING_SPH 3
#define PRIMARY_TESS_FIELD_INDEX 0
#define SECONDARY_TESS_FIELD_INDEX 1

// Misc needed numerics.
#define TESSELLATION_MAX 150
#define TESSELLATION_MIN 1

// Rendering selection button attributes
#define RENDERING_BUTT_TOP_RED   .58
#define RENDERING_BUTT_TOP_GREEN .87
#define RENDERING_BUTT_TOP_BLUE  .96
#define RENDERING_BUTT_TOP_COLOR RENDERING_BUTT_TOP_RED, RENDERING_BUTT_TOP_GREEN, RENDERING_BUTT_TOP_BLUE
#define RENDERING_BUTT_BOT_RED   .22
#define RENDERING_BUTT_BOT_GREEN .24
#define RENDERING_BUTT_BOT_BLUE  .64
#define RENDERING_BUTT_BOT_COLOR RENDERING_BUTT_BOT_RED, RENDERING_BUTT_BOT_GREEN, RENDERING_BUTT_BOT_BLUE

//******************************************
// Tweaking variables that you probably won't want to modify
//******************************************

// Keys
#define BACKSPACE 8
#define ENTER 13
#define ESCAPE 0x1b

// Element (buttons, fields, etc.) spacings, offsets, etc.
#define TEXT_FIELD_MAX_LENGTH 3
#define RENDERING_SELECTION_ROW_MAX 50
#define RENDERING_SELECTION_ROW_MIN 10
#define RENDERING_BUTTON_ADJUST_X 10
#define RENDERING_BUTTON_ADJUST_Y RENDERING_BUTTON_ADJUST_X
#define TESS_FIELD_EDGE_LEFT_OFFSET 5
#define TESS_FIELD_EDGE_RIGHT_OFFSET 42
#define TESS_FIELD_EDGE_TOP_OFFSET 5
#define TESS_FIELD_EDGE_BOT_OFFSET 25
#define TESS_INC_LEFT_OFFSET 52
#define TESS_INC_RIGHT_OFFSET 87
#define TESS_DEC_LEFT_OFFSET 97
#define TESS_DEC_RIGHT_OFFSET 132
#define TESS_INC_TEXT_X_OFFSET 63
#define TESS_INC_TEXT_Y_OFFSET 3
#define TESS_DEC_TEXT_X_OFFSET 108
#define TESS_DEC_TEXT_Y_OFFSET 3
#define HELP_BUTTON_LEFT_OFFSET 80
#define HELP_BUTTON_RIGHT_OFFSET 40
#define HELP_BUTTON_BOT_OFFSET 70
#define HELP_BUTTON_TOP_OFFSET 20
#define HELP_BUTTON_TEXT_X_OFFSET 64
#define HELP_BUTTON_TEXT_Y_OFFSET 35
#define HELP_TEXT_WORD_OFFSET 5
#define HELP_TEXT_DEF_OFFSET  150
#define TESS_FIELD_X 225

// Field boarders (effects)
#define TESS_FIELD_BORDER_BOT_OFFSET 10
#define TESS_FIELD_BORDER_TOP_OFFSET 30
#define TESS_FIELD_BORDER_LEFT_OFFSET 10
#define TESS_FIELD_BORDER_RIGHT_OFFSET 47

// Window geometry & attributes
#define QUARTER_WINDOW .25
#define HALF_WINDOW .50
#define THREE_QUARTER_WINDOW .75
#define EIGHTH_WINDOW .125
#define THREE_EIGHTS_WINDOW .375
#define FIVE_EIGTHS_WINDOW .625
#define SEVEN_EIGHTS_WINDOW .875
#define ONE_SIXTEENTH_WINDOW .0625
#define QUARTER_CIRCLE 45

// Misc. needed numerics
#define DEGREE_RADIAN_FACTOR .0174532925
#define HALF_CIRCLE 180
#define BLACK_D 0.0, 0.0, 0.0
#define WHITE_D 1.0, 1.0, 1.0
#define GRAY_D 0.7, 0.7, 0.7
#define GRAY_BUTT_TOP_D 0.6, 0.6, 0.6
#define YELLOW_D 1.0, 1.0, 0.0

// Some structs to define common 'objects' to use, and keep track
// of the active state
struct textField
{
    std::string buttonText;
    bool active;
    int x;
    int y;
};

struct shapeState
{
    int primaryTessellation;
    int secondaryTessellation;
    double xRotation;
    double yRotation;
    double zRotation;
};

// Handles for the GLUT windows
extern int mainWindow;
extern int statusWindow;
extern int tessWindow;

// Sizes for the GLUT windows
extern int windowSizex;
extern int windowSizey;

// Keeps track of where the mouse was clicked
extern int lastx;
extern int lasty;

// Keeps track of the text fields
extern textField fields[2];

// Keeps track of the current rendering state
extern shapeState renderings[4];

// Keeps track of the active verticies for what is in the tessellation window
extern std::vector<Point3> vertices;

// Flag as to whether or not the active rendering needs to be redrawn
extern bool tessChange;

// If mouse button 1 is down
extern bool mouseDown;

// Numerical representation of the active rendering (0-3)
extern short activeRendering;

// Is true if the help screen is up, false otherwise
extern bool helpActive;

#endif
