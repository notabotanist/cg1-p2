////////////////////////////////////////////////////////////
//
// File:  input.cpp
// Authors:  G. Fotiades, A. Koutmos
// Contributors: Sean Strout
// Last modified: 4/18/11
//
// Description:  This file holds the implementations for input receiving,
//               that consists of both keyboard and mouse input, and deciding
//               how to deal with given input.
//
////////////////////////////////////////////////////////////

#include "resources.h"
#include "input.h"
#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern void refreshAll();

void UpdateTextEntry(char key)
{
    //If the key is not a valid key return
    if (key != BACKSPACE && (key < '0' || key > '9') && key != ENTER)
        return;

    //If primary tessellation field is the active field
    if (fields[PRIMARY_TESS_FIELD_INDEX].active)
    {
        //Check for ENTER key
        if (key == ENTER)
        {
            //Convert the string to and integer and set the primary tessellation of the active rendering
            renderings[activeRendering].primaryTessellation = atoi(fields[PRIMARY_TESS_FIELD_INDEX].buttonText.c_str());

            //Check that it is not out of bounds
            if(renderings[activeRendering].primaryTessellation < TESSELLATION_MIN)
                renderings[activeRendering].primaryTessellation = TESSELLATION_MIN;
            if(renderings[activeRendering].primaryTessellation > TESSELLATION_MAX)
                renderings[activeRendering].primaryTessellation = TESSELLATION_MAX;

            //Deactivate the text field and set the flag to recalculate the tessellation
            fields[PRIMARY_TESS_FIELD_INDEX].active = false;
            tessChange = true;
        }
        //Check for BACKSPACE key
        else if (key == BACKSPACE)
        {
            //Check that the text field is not empty
            if (fields[PRIMARY_TESS_FIELD_INDEX].buttonText.length() == 0)
                return;

            //Delete the last typed character
            fields[PRIMARY_TESS_FIELD_INDEX].buttonText.erase(fields[PRIMARY_TESS_FIELD_INDEX].buttonText.length() - 1);
        }

        //If the text field is less than 3 characters then append the key
        else if (fields[PRIMARY_TESS_FIELD_INDEX].buttonText.size() < TEXT_FIELD_MAX_LENGTH)
        {
             fields[PRIMARY_TESS_FIELD_INDEX].buttonText += key;
        }
    }
    //If secondary tessellation field is the active field
    else if (fields[SECONDARY_TESS_FIELD_INDEX].active)
    {
        //Check for ENTER key
        if (key == ENTER)
        {
            //Convert the string to and integer and set the primary tessellation of the active rendering
            renderings[activeRendering].secondaryTessellation = atoi(fields[SECONDARY_TESS_FIELD_INDEX].buttonText.c_str());

            //Check that it is not out of bounds
            if(renderings[activeRendering].secondaryTessellation < TESSELLATION_MIN)
                renderings[activeRendering].secondaryTessellation = TESSELLATION_MIN;
            if(renderings[activeRendering].secondaryTessellation > TESSELLATION_MAX)
                renderings[activeRendering].secondaryTessellation = TESSELLATION_MAX;

            //Deactivate the text field and set the flag to recalculate the tessellation
            fields[SECONDARY_TESS_FIELD_INDEX].active = false ;
            tessChange = true;
        }
        //Check for BACKSPACE key
        else if (key == BACKSPACE)
        {
            //Check that the text field is not empty
            if (fields[SECONDARY_TESS_FIELD_INDEX].buttonText.length() == 0)
                return;

            //Delete the last typed character
            fields[SECONDARY_TESS_FIELD_INDEX].buttonText.erase(fields[SECONDARY_TESS_FIELD_INDEX].buttonText.length() - 1);
        }
        //If the text field is less than 3 characters then append the key
        else if (fields[SECONDARY_TESS_FIELD_INDEX].buttonText.size() < TEXT_FIELD_MAX_LENGTH)
        {
             fields[SECONDARY_TESS_FIELD_INDEX].buttonText += key;
        }
    }
}

void input(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'z':
    case 'Z':
        helpActive = !helpActive;
        break;

    case ESCAPE:
    case 'q':
    case 'Q':
        //The key pressed was the ESCAPE key
        exit(0);
        break;

    case '+':
    case '=':
        //Check the bounds of the tessellation
        if (renderings[activeRendering].primaryTessellation < TESSELLATION_MAX)
        {
            renderings[activeRendering].primaryTessellation++;

            //The tessellation sub window needs recalculating
            tessChange = true;
        }
        break;

    case '-':
    case '_':
        //Check the bounds of the tessellation
        if(renderings[activeRendering].primaryTessellation > TESSELLATION_MIN)
        {
            renderings[activeRendering].primaryTessellation--;

            //The tessellation sub window needs recalculating
            tessChange = true;
        }
        break;

    case '[':
    case '{':
        //Check the bounds of the tessellation
        if (renderings[activeRendering].secondaryTessellation > TESSELLATION_MIN)
        {
            renderings[activeRendering].secondaryTessellation--;

            //The tessellation sub window needs recalculating
            tessChange = true;
        }
        break;

    case '}':
    case ']':
        //Check the bounds of the tessellation
        if (renderings[activeRendering].secondaryTessellation < TESSELLATION_MAX)
        {
            renderings[activeRendering].secondaryTessellation++;

            //The tessellation sub window needs recalculating
            tessChange = true;
        }
        break;

    default:
        //If one of the text fields is active then pass on the key pressed
        //to the update text function
        if (fields[PRIMARY_TESS_FIELD_INDEX].active || fields[SECONDARY_TESS_FIELD_INDEX].active)
            UpdateTextEntry(key);
        //If the button was 1-4 and the text fields are inactive.
        //Changes the shape being tessellated
        else if (!fields[PRIMARY_TESS_FIELD_INDEX].active && !fields[SECONDARY_TESS_FIELD_INDEX].active &&
                 key >= '1' && key <= '4')
        {
            //Set the active shape rendering
            activeRendering = key - '0' - 1;

            //The tessellation sub window needs recalculating
            tessChange = true;
        }
    }

    //Refresh all the sub-windows
    refreshAll();
}

///////////////////////////////////////////////////////////
//Special keyboard input callback
///////////////////////////////////////////////////////////
void specialInput(int key, int x, int y)
{
    //Check for arrow keys and adjust the rotations accordingly
    switch(key)
    {
        case GLUT_KEY_DOWN: renderings[activeRendering].xRotation += ARROW_MOVE_FACTOR;  break;
        case GLUT_KEY_UP: renderings[activeRendering].xRotation -= ARROW_MOVE_FACTOR;  break;
        case GLUT_KEY_LEFT: renderings[activeRendering].yRotation -= ARROW_MOVE_FACTOR;  break;
        case GLUT_KEY_RIGHT: renderings[activeRendering].yRotation += ARROW_MOVE_FACTOR;  break;
    }

    //Refresh only the tessellation subwindow
    glutSetWindow(tessWindow);
    glutPostRedisplay();
}


///////////////////////////////////////////////////////////
//Call back function for mouse clicking
///////////////////////////////////////////////////////////
void mouseInput(int button, int state, int x, int y)
{
    //STATUS PANE CLICK------------------------------------
    if (glutGetWindow() == statusWindow && state == GLUT_DOWN  && !helpActive)
    {
        //Assume that the text fields are inactive
        fields[PRIMARY_TESS_FIELD_INDEX].active = false;
        fields[SECONDARY_TESS_FIELD_INDEX].active = false;

        if (y >= RENDERING_SELECTION_ROW_MIN && y <= RENDERING_SELECTION_ROW_MAX)
        {
            //Cube button was clicked
            if (x <= ((windowSizex * QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X) &&
                activeRendering != RENDERING_CUBE)
            {
                //Set the new active rendering and set the flag for new tessellation calculation
                activeRendering = 0;
                tessChange = true;
            }
            //Cylinder button was clicked
            else if (x >= ((windowSizex * QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X) &&
                     x <= ((windowSizex * HALF_WINDOW) - RENDERING_BUTTON_ADJUST_X) &&
                     activeRendering != RENDERING_CYL)
            {
                //Set the new active rendering and set the flag for new tessellation calculation
                activeRendering = 1 ;
                tessChange = true;
            }

            //Cone button was clicked
            else if(x >= ((windowSizex * HALF_WINDOW) + RENDERING_BUTTON_ADJUST_X) &&
                    x <= ((windowSizex * THREE_QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X) &&
                    activeRendering != RENDERING_CONE)
            {
                //Set the new active rendering and set the flag for new tessellation calculation
                activeRendering = 2 ;
                tessChange = true;
            }

            //Sphere button was clicked
            else if(x >= ((windowSizex * THREE_QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X) &&
                    x <= ((windowSizex) - RENDERING_BUTTON_ADJUST_X) &&
                    activeRendering != RENDERING_SPH)
            {
                //Set the new active rendering and set the flag for new tessellation calculation
                activeRendering = 3 ;
                tessChange = true;
            }
        }
        //Primary tessellation text field was clicked
        else if(x >= fields[PRIMARY_TESS_FIELD_INDEX].x - TESS_FIELD_EDGE_LEFT_OFFSET &&
                x <= fields[PRIMARY_TESS_FIELD_INDEX].x + TESS_FIELD_EDGE_RIGHT_OFFSET &&
                y <= ((windowSizey * QUARTER_WINDOW) - (fields[PRIMARY_TESS_FIELD_INDEX].y - TESS_FIELD_EDGE_TOP_OFFSET)) &&
                y >= ((windowSizey * QUARTER_WINDOW) - (fields[PRIMARY_TESS_FIELD_INDEX].y + TESS_FIELD_EDGE_BOT_OFFSET)))
        {
            //Set the text field to active and set its text to empty
            fields[PRIMARY_TESS_FIELD_INDEX].active = true ;
            fields[PRIMARY_TESS_FIELD_INDEX].buttonText = "";
        }
        // Secondary tessellation text field was clicked
        else if(x >= fields[SECONDARY_TESS_FIELD_INDEX].x - HALF_WINDOW &&
                x <= fields[SECONDARY_TESS_FIELD_INDEX].x + TESS_FIELD_EDGE_RIGHT_OFFSET &&
                y <= ((windowSizey * QUARTER_WINDOW) - (fields[SECONDARY_TESS_FIELD_INDEX].y - TESS_FIELD_EDGE_TOP_OFFSET)) &&
                y >= ((windowSizey * QUARTER_WINDOW) - (fields[SECONDARY_TESS_FIELD_INDEX].y + TESS_FIELD_EDGE_BOT_OFFSET)))
        {
            //Set the text field to active and set its text to empty
            fields[SECONDARY_TESS_FIELD_INDEX].active = true ;
            fields[SECONDARY_TESS_FIELD_INDEX].buttonText = "";
        }
        // Help window button
        else if(x >= windowSizex - HELP_BUTTON_LEFT_OFFSET &&
                x <= windowSizex - HELP_BUTTON_RIGHT_OFFSET &&
                y <= ((windowSizey * QUARTER_WINDOW) - HELP_BUTTON_TOP_OFFSET) &&
                y >= ((windowSizey * QUARTER_WINDOW) - HELP_BUTTON_BOT_OFFSET))
        {
            helpActive = !helpActive;
        }
        // Primary inc/dec tessellation buttons
        else if (y <= ((windowSizey * QUARTER_WINDOW) - (fields[PRIMARY_TESS_FIELD_INDEX].y - TESS_FIELD_EDGE_TOP_OFFSET)) &&
                 y >= ((windowSizey * QUARTER_WINDOW) - (fields[PRIMARY_TESS_FIELD_INDEX].y + TESS_FIELD_EDGE_BOT_OFFSET)))
        {
            if(x >= fields[PRIMARY_TESS_FIELD_INDEX].x + TESS_INC_LEFT_OFFSET &&
               x <= fields[PRIMARY_TESS_FIELD_INDEX].x + TESS_INC_RIGHT_OFFSET)
            {
                if (renderings[activeRendering].primaryTessellation < TESSELLATION_MAX)
                {
                    renderings[activeRendering].primaryTessellation++;

                    //The tessellation sub window needs recalculating
                    tessChange = true;
                }
            }

            else if(x >= fields[PRIMARY_TESS_FIELD_INDEX].x + TESS_DEC_LEFT_OFFSET &&
                    x <= fields[PRIMARY_TESS_FIELD_INDEX].x + TESS_DEC_RIGHT_OFFSET)
            {
                if(renderings[activeRendering].primaryTessellation > TESSELLATION_MIN)
                {
                    renderings[activeRendering].primaryTessellation--;

                    //The tessellation sub window needs recalculating
                    tessChange = true;
                }
            }
        }
        // Secondary inc/dec tessellation buttons
        else if (y <= ((windowSizey * QUARTER_WINDOW) - (fields[SECONDARY_TESS_FIELD_INDEX].y - TESS_FIELD_EDGE_TOP_OFFSET)) &&
                 y >= ((windowSizey * QUARTER_WINDOW) - (fields[SECONDARY_TESS_FIELD_INDEX].y + TESS_FIELD_EDGE_BOT_OFFSET)))
        {
            if(x >= fields[SECONDARY_TESS_FIELD_INDEX].x + TESS_INC_LEFT_OFFSET &&
               x <= fields[SECONDARY_TESS_FIELD_INDEX].x + TESS_INC_RIGHT_OFFSET)
            {
                if (renderings[activeRendering].secondaryTessellation < TESSELLATION_MAX)
                {
                    renderings[activeRendering].secondaryTessellation++;

                    //The tessellation sub window needs recalculating
                    tessChange = true;
                }
            }

            else if(x >= fields[SECONDARY_TESS_FIELD_INDEX].x + TESS_DEC_LEFT_OFFSET &&
                    x <= fields[SECONDARY_TESS_FIELD_INDEX].x + TESS_DEC_RIGHT_OFFSET)
            {
                if(renderings[activeRendering].secondaryTessellation > TESSELLATION_MIN)
                {
                    renderings[activeRendering].secondaryTessellation--;

                    //The tessellation sub window needs recalculating
                    tessChange = true;
                }
            }
        }

        //Redraw all the
        refreshAll();
    }

    //TESSELLATION PANE CLICK------------------------------
    else if(glutGetWindow() == tessWindow)
    {
        //Set the state of the mouse button (whether down or up)
        mouseDown = (state == GLUT_DOWN) ? true : false;

        //Set the position at which the mouse was clicked
        lastx = x;
        lasty = y;
    }
}

///////////////////////////////////////////////////////////
//Call back function for mouse movement
///////////////////////////////////////////////////////////
void mouseMovement(int x, int y)
{
    //If the last mouse click was a down action (i.e mouse is being held down)
    if(mouseDown)
    {
        //Calculate the change in x and y
        double dx = double(x - lastx) / windowSizex;
        double dy = double(y - lasty) / windowSizey;

        //Adjust the rotations of the active rendering
        renderings[activeRendering].xRotation += (HALF_CIRCLE * dy);
        renderings[activeRendering].yRotation += (HALF_CIRCLE * dx);

        //Set the last x and y to the current x and y
        lastx = x;
        lasty = y;

        //Redraw only the tessellation window as not to flicker the status window
        glutSetWindow(tessWindow);
        glutPostRedisplay();
    }
}

///////////////////////////////////////////////////////////
//Displays the help dialog
///////////////////////////////////////////////////////////
void ShowHelp()
{
    const short num_lines = 10;

    std::string helpStringWor[num_lines];
    std::string helpStringDef[num_lines];
    helpStringWor[0] = "HELP: (shortcut keys)                                        Framwork created by Greg Fotiades & Alex Koutmos";

    helpStringWor[1] = "Q / q / Esc";
    helpStringDef[1] = "- Closes Program";

    helpStringWor[2] = "+ / -";
    helpStringDef[2] = "- Increases/Decreases Primary Tessellation";

    helpStringWor[3] = "[ / ]";
    helpStringDef[3] = "- Increases/Decreases Secondary Tessellation (Cyl/Cone only)";

    helpStringWor[4] = "1-4";
    helpStringDef[4] = "- Change Rendering (Cube, Cylinder, etc.)";

    helpStringWor[5] = "Z / z";
    helpStringDef[5] = "- Toggle The Help Menu";

    helpStringWor[6] = "Text Box Interaction";
    helpStringDef[6] = "- Click on box, type number, press enter.";

    helpStringWor[7] = "Arrows or Mouse drag";
    helpStringDef[7] = "- Rotates currently selected rendering";

    helpStringWor[8] = "";
    helpStringWor[9] = "Press \'z\' to exit this menu....";

    glColor3f(BLACK_D);
    for (int i = 0, offset = 15 ; i < num_lines ; ++i, offset += 15)
    {
        glRasterPos2i(HELP_TEXT_WORD_OFFSET, (int)(windowSizey * QUARTER_WINDOW) - offset);

        for (unsigned int k = 0 ; k < helpStringWor[i].length() ; ++k)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, helpStringWor[i].at(k));

        glRasterPos2i(HELP_TEXT_DEF_OFFSET, (int)(windowSizey * QUARTER_WINDOW) - offset);

        for (unsigned int k = 0 ; k < helpStringDef[i].length() ; ++k)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, helpStringDef[i].at(k));
    }
}
