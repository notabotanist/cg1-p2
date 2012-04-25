////////////////////////////////////////////////////////////
//
// File:  tessellation.cpp
// Authors:  G. Fotiades, A. Koutmos
// Contributors: Sean Strout
// Last modified: 4/18/11
//
// Description:  This file holds the implementation for the framework startup,
//               drawing the GUI, setup, adjusting to resizing, etc.  The functions
//               it uses to draw the cone/sphere/cylinder/cube will be incorporated
//               at link time.
//
////////////////////////////////////////////////////////////

#include "resources.h"
#include "input.h"
#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Function prototypes
void statusWindowDisplay();
void refreshAll();

// Following rendering functions will be defined
// in the renderings.o object at link time
extern void Cone(int n, int m);
extern void Cube(int n);
extern void Cylinder(int n, int m);
extern void Sphere(int n);
extern const char* PROJECT_NAME;

// Actual declarations for extern'ed shared variables
// within the resources.h file - these are shared between
// multiple files as globals.
int mainWindow;
int statusWindow;
int tessWindow;
int windowSizex;
int windowSizey;
int lastx;
int lasty;
textField fields[2];
shapeState renderings[4];
std::vector<Point3> vertices;
bool tessChange;
bool mouseDown;
short activeRendering;
bool helpActive;

///////////////////////////////////////////////////////////
//Convert numbers to strings
///////////////////////////////////////////////////////////
std::string numToString(int num)
{
    //Temp variables used for the conversion
    std::string returnString = "";
    std::string currentCharacter;
    int current;

    //Loop while the number is greater than 0
    while(num > 0)
    {
        //Strip off the 10's spot number
        current = num % 10;
        num /= 10;

        //Appened it to the front of the return string
        currentCharacter = (current + '0');
        returnString.insert(0, currentCharacter);
    }

    return returnString;
}

///////////////////////////////////////////////////////////
//Function used to draw text labels
///////////////////////////////////////////////////////////
void drawLabel(double r, double g, double b, int x, int y, std::string text)
{
    //Set the color and position of the text
    glColor3f(r, g, b);
    glRasterPos2i(x, y);

    //Draw the text 1 character at a time
    for(unsigned int count = 0 ; count < text.size() ; ++count)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[count]);
}

///////////////////////////////////////////////////////////
//Redraw all windows and sub-windows
///////////////////////////////////////////////////////////
void refreshAll()
{
    //Refresh the main window
    glutSetWindow(mainWindow);
    glutPostRedisplay();

    //Refresh the tessellation window
    glutSetWindow(tessWindow);
    glutPostRedisplay();

    //Refresh the status window
    glutSetWindow(statusWindow);
    glutPostRedisplay();
}

///////////////////////////////////////////////////////////
//Initialize state machine and global settings
///////////////////////////////////////////////////////////
void initialize()
{
    // Initialize help display variable
    helpActive = false;

    //Set the initial window size
    windowSizex = INIT_WINDOW_SIZE_X;
    windowSizey = INIT_WINDOW_SIZE_Y;

    //Set the active rendering to 0 (i.e cube)
    activeRendering = RENDERING_CUBE;

    //Set up the primary tessellation field
    fields[PRIMARY_TESS_FIELD_INDEX].buttonText = "";
    fields[PRIMARY_TESS_FIELD_INDEX].x = TESS_FIELD_X;
    fields[PRIMARY_TESS_FIELD_INDEX].y = int(windowSizey * EIGHTH_WINDOW);
    fields[PRIMARY_TESS_FIELD_INDEX].active = false ;

    //Set up the secondary tessellation field
    fields[SECONDARY_TESS_FIELD_INDEX].buttonText = "";
    fields[SECONDARY_TESS_FIELD_INDEX].x = TESS_FIELD_X;
    fields[SECONDARY_TESS_FIELD_INDEX].y = int(windowSizey * ONE_SIXTEENTH_WINDOW);
    fields[SECONDARY_TESS_FIELD_INDEX].active = false;

    //Set up all the states of all the shapes
    for(int count = 0 ; count < 4 ; count ++)
    {
	// cylinder and cone have minimum primary tessellation, N=3
	if (count == 1 || count == 2) {
        	renderings[count].primaryTessellation = 3 ;
	} else {	 // cube and sphere have primary tessellation, N=1
        	renderings[count].primaryTessellation = 1 ;
	}
		
        renderings[count].secondaryTessellation = 1 ;
        renderings[count].xRotation = 0 ;
        renderings[count].yRotation = 0 ;
        renderings[count].zRotation = 0 ;
    }

    //Tessellation needs to be calculated initially
    tessChange = true;
}

///////////////////////////////////////////////////////////
//Displays the status subwindow
///////////////////////////////////////////////////////////
void statusWindowDisplay()
{
    //Set status sub window to the active glut window
    glutSetWindow(statusWindow);

    //Clear the sub window
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(WHITE_D, 1.0);

    //Set up the drawing and viewing area of the window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowSizex, 0, int(windowSizey * QUARTER_WINDOW));
    glViewport(0, 0, windowSizex, int(windowSizey * QUARTER_WINDOW));

    //Create subwindow divider at the top of the window
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex2i(0, int(windowSizey * QUARTER_WINDOW) - 1);
        glVertex2i(windowSizex, int(windowSizey * QUARTER_WINDOW) - 1);
    glEnd();

    if (helpActive)
    {
        ShowHelp();
        glutSwapBuffers();
        return;
    }

    //Draw Buttons-----------------------------------------
    //***************Cube***************
    glBegin(GL_POLYGON);
        glColor3f(RENDERING_BUTT_TOP_COLOR);
        glVertex2i(RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);
        glVertex2i(int(windowSizex * QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);

        glColor3f(RENDERING_BUTT_BOT_COLOR);
        glVertex2i(int(windowSizex * QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
        glVertex2i(RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
    glEnd();

    //Make the text white if it is the active rending
    if(activeRendering == RENDERING_CUBE)
        drawLabel(WHITE_D, int(windowSizex * EIGHTH_WINDOW) - 22, int(windowSizey * QUARTER_WINDOW) - 38, "Cube");
    else
        drawLabel(BLACK_D, int(windowSizex * EIGHTH_WINDOW) - 22, int(windowSizey * QUARTER_WINDOW) - 38, "Cube");

    //***************Cylinder***************
    glBegin(GL_POLYGON);
        glColor3f(RENDERING_BUTT_TOP_COLOR);
        glVertex2i(int(windowSizex * QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);
        glVertex2i(int(windowSizex * HALF_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);

        glColor3f(RENDERING_BUTT_BOT_COLOR);
        glVertex2i(int(windowSizex * HALF_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
        glVertex2i(int(windowSizex * QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
    glEnd();

    //Make the text white if it is the active rending
    if(activeRendering == RENDERING_CYL)
        drawLabel(WHITE_D, int(windowSizex * THREE_EIGHTS_WINDOW) - 35, int(windowSizey * QUARTER_WINDOW) - 38, "Cylinder");
    else
        drawLabel(BLACK_D, int(windowSizex * THREE_EIGHTS_WINDOW) - 35, int(windowSizey * QUARTER_WINDOW) - 38, "Cylinder");

    //***************Cone***************
    glBegin(GL_POLYGON);
        glColor3f(RENDERING_BUTT_TOP_COLOR);
        glVertex2i(int(windowSizex * HALF_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);
        glVertex2i(int(windowSizex * THREE_QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);

        glColor3f(RENDERING_BUTT_BOT_COLOR);
        glVertex2i(int(windowSizex * THREE_QUARTER_WINDOW) - RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
        glVertex2i(int(windowSizex * HALF_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
    glEnd();

    //Make the text white if it is the active rending
    if(activeRendering == RENDERING_CONE)
        drawLabel(WHITE_D, int(windowSizex * FIVE_EIGTHS_WINDOW) - 22, int(windowSizey * QUARTER_WINDOW) - 38, "Cone");
    else
        drawLabel(BLACK_D, int(windowSizex * FIVE_EIGTHS_WINDOW) - 22, int(windowSizey * QUARTER_WINDOW) - 38, "Cone");

    //***************Sphere***************
    glBegin(GL_POLYGON);
        glColor3f(RENDERING_BUTT_TOP_COLOR);
        glVertex2i(int(windowSizex * THREE_QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);
        glVertex2i(int(windowSizex - RENDERING_BUTTON_ADJUST_X), int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MAX);

        glColor3f(RENDERING_BUTT_BOT_COLOR);
        glVertex2i(int(windowSizex - RENDERING_BUTTON_ADJUST_X), int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
        glVertex2i(int(windowSizex * THREE_QUARTER_WINDOW) + RENDERING_BUTTON_ADJUST_X, int(windowSizey * QUARTER_WINDOW) - RENDERING_SELECTION_ROW_MIN);
    glEnd();

    //Make the text white if it is the active rending
    if(activeRendering == RENDERING_SPH)
        drawLabel(WHITE_D, int(windowSizex * SEVEN_EIGHTS_WINDOW) - 30, int(windowSizey * QUARTER_WINDOW) - 38, "Sphere");
    else
        drawLabel(BLACK_D, int(windowSizex * SEVEN_EIGHTS_WINDOW) - 30, int(windowSizey * QUARTER_WINDOW) - 38, "Sphere");
    //Draw Buttons-----------------------------------------


    //Text Boxes-------------------------------------------
    //Draw the text labels
    drawLabel(BLACK_D, RENDERING_BUTTON_ADJUST_X, int(windowSizey * EIGHTH_WINDOW), "Primary Tessellation:");
    drawLabel(BLACK_D, RENDERING_BUTTON_ADJUST_X, int(windowSizey * ONE_SIXTEENTH_WINDOW), "Secondary Tessellation:");

    //Check to see that fields are currently not active and convert the current renderings
    //tessellation factors to strings
    if(!fields[PRIMARY_TESS_FIELD_INDEX].active)
        fields[PRIMARY_TESS_FIELD_INDEX].buttonText = numToString(renderings[activeRendering].primaryTessellation);
    if(!fields[SECONDARY_TESS_FIELD_INDEX].active)
        fields[SECONDARY_TESS_FIELD_INDEX].buttonText = numToString(renderings[activeRendering].secondaryTessellation);

    //Output the text fields and their values
    for (int i = 0 ; i < 2 ; ++i)
    {
        //If the current text field is active
        if(fields[i].active)
        {
            //Set the color to black
            glColor3f(BLACK_D);

            //Draw a polygon biger than the actual field to simulate a border highlight
            glBegin(GL_POLYGON);
                glVertex2i(fields[i].x - TESS_FIELD_BORDER_LEFT_OFFSET, fields[i].y - TESS_FIELD_BORDER_BOT_OFFSET);
                glVertex2i(fields[i].x + TESS_FIELD_BORDER_RIGHT_OFFSET, fields[i].y - TESS_FIELD_BORDER_BOT_OFFSET);

                glVertex2i(fields[i].x + TESS_FIELD_BORDER_RIGHT_OFFSET, fields[i].y + TESS_FIELD_BORDER_TOP_OFFSET);
                glVertex2i(fields[i].x - TESS_FIELD_BORDER_LEFT_OFFSET, fields[i].y + TESS_FIELD_BORDER_TOP_OFFSET);
            glEnd();
        }

        //Set the color to grey and draw the actual text box
        glColor3f(GRAY_D);
        glBegin(GL_POLYGON);
            glVertex2i(fields[i].x - TESS_FIELD_EDGE_LEFT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);
            glVertex2i(fields[i].x + TESS_FIELD_EDGE_RIGHT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);

            glVertex2i(fields[i].x + TESS_FIELD_EDGE_RIGHT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
            glVertex2i(fields[i].x - TESS_FIELD_EDGE_LEFT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
        glEnd();

        //Set the color and the position of the text in the text field
        glColor3f(BLACK_D);
        glRasterPos2i(fields[i].x, fields[i].y);

        //Output the tessellation factor character by character
        int len = fields[i].buttonText.length();
        for (int j = 0 ; j < len ; ++j)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, fields[i].buttonText[j]);

        //Draw +/- buttons for tessellation
        glBegin(GL_POLYGON);
            glColor3f(GRAY_BUTT_TOP_D);
            glVertex2i(fields[i].x + TESS_INC_LEFT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);
            glVertex2i(fields[i].x + TESS_INC_RIGHT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);

            glColor3f(GRAY_D);
            glVertex2i(fields[i].x + TESS_INC_RIGHT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
            glVertex2i(fields[i].x + TESS_INC_LEFT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
        glEnd();

        glColor3f(WHITE_D);
        glRasterPos2i(fields[i].x + TESS_INC_TEXT_X_OFFSET, fields[i].y + TESS_INC_TEXT_Y_OFFSET);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '+');

        glBegin(GL_POLYGON);
            glColor3f(GRAY_BUTT_TOP_D);
            glVertex2i(fields[i].x + TESS_DEC_LEFT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);
            glVertex2i(fields[i].x + TESS_DEC_RIGHT_OFFSET, fields[i].y - TESS_FIELD_EDGE_TOP_OFFSET);

            glColor3f(GRAY_D);
            glVertex2i(fields[i].x + TESS_DEC_RIGHT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
            glVertex2i(fields[i].x + TESS_DEC_LEFT_OFFSET, fields[i].y + TESS_FIELD_EDGE_BOT_OFFSET);
        glEnd();

        glColor3f(WHITE_D);
        glRasterPos2i(fields[i].x + TESS_DEC_TEXT_X_OFFSET, fields[i].y + TESS_DEC_TEXT_Y_OFFSET);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '-');
    }
    //Text Boxes-------------------------------------------
    glColor3f(YELLOW_D);
    glBegin(GL_POLYGON);
        glVertex2i(windowSizex - HELP_BUTTON_RIGHT_OFFSET, HELP_BUTTON_BOT_OFFSET);
        glVertex2i(windowSizex - HELP_BUTTON_LEFT_OFFSET, HELP_BUTTON_BOT_OFFSET);
        glVertex2i(windowSizex - HELP_BUTTON_LEFT_OFFSET, HELP_BUTTON_TOP_OFFSET);
        glVertex2i(windowSizex - HELP_BUTTON_RIGHT_OFFSET, HELP_BUTTON_TOP_OFFSET);
    glEnd();
    glColor3f(BLACK_D);
    glRasterPos2i(windowSizex - HELP_BUTTON_TEXT_X_OFFSET, HELP_BUTTON_TEXT_Y_OFFSET);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '?');

    //Swap the buffers
    glutSwapBuffers();
}

///////////////////////////////////////////////////////////
//Add a triangle to the vertices vector
///////////////////////////////////////////////////////////
void addTriangle(Point3 p1, Point3 p2, Point3 p3)
{
    //Push back all the points
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
}

///////////////////////////////////////////////////////////
//Displays the shape rending window
///////////////////////////////////////////////////////////
void tessWindowDisplay(void)
{
    //Set the active window to the tessellation window
    glutSetWindow(tessWindow);

    //Set up the display window for the 3d drawing
    float ratio = (1.0 * windowSizex) / windowSizey;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport( (int)(0.125 * windowSizex), 0,
		int(windowSizex * .75),
		int(windowSizey * .75) );
    gluPerspective(QUARTER_CIRCLE, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 2.75,
              0.0, 0.0, -1.0,
              0.0f, 1.0f, 0.0f);

    //Set OpenGL state variables for proper rendering
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glPolygonMode(GL_FRONT, GL_LINE);

    //Clear the sub window
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(WHITE_D, 1.0);

    //If the tessChange flag is set then recalculate the tessellation of the figure
    if(tessChange){
        //Delete all old entries in the vector
        vertices.clear();

        //Depending on which is the current active rendering call the appropriote recalculate function
        switch (activeRendering)
        {
        default:
            activeRendering = 0;
        case 0:
            Cube(renderings[activeRendering].primaryTessellation);  break;
        case 1:
            Cylinder(renderings[activeRendering].primaryTessellation, renderings[activeRendering].secondaryTessellation);  break;
        case 2:
            Cone(renderings[activeRendering].primaryTessellation, renderings[activeRendering].secondaryTessellation);  break;
        case 3:
            Sphere(renderings[activeRendering].primaryTessellation);  break;
        }

        //Tessellation now does not have to be recalculated
        tessChange = false;
    }

    //Draw all the coordinates within the vertices vector
    //Se the color to black
    glColor3f(BLACK_D);

    //Set the rotation at which the shape will be draw based on the active rendering
    glRotatef(renderings[activeRendering].xRotation, 1.0, 0.0, 0.0);
    glRotatef(renderings[activeRendering].yRotation, 0.0, 1.0, 0.0);
    glRotatef(renderings[activeRendering].zRotation, 0.0, 0.0, 1.0);

    //Loop through the vector and draw all the trianlges
    for( unsigned int i = 2; i < vertices.size(); i += 3 ){
        glBegin(GL_TRIANGLES);
            glVertex3d(vertices[i - 2].x, vertices[i - 2].y, vertices[i - 2].z);
            glVertex3d(vertices[i - 1].x, vertices[i - 1].y, vertices[i - 1].z);
            glVertex3d(vertices[i].x, vertices[i].y, vertices[i].z);
        glEnd();
    }

    //Swap the buffers
    glutSwapBuffers();
}

///////////////////////////////////////////////////////////
//Displays the main window
///////////////////////////////////////////////////////////
void mainWindowDisplay(void)
{
    //Set the active window to the main window
    glutSetWindow(mainWindow);

    //Clear the window
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(WHITE_D, 1.0);
}

///////////////////////////////////////////////////////////
//Call back function for window size change
///////////////////////////////////////////////////////////
void changeSize(int w, int h)
{
    //Prevent the size of the window from getting too small
    if(h <= MIN_WINDOW_RENDERING_Y)
        h = MIN_WINDOW_RENDERING_Y;
    if(w <= MIN_WINDOW_RENDERING_X)
        w = MIN_WINDOW_RENDERING_X;

    //Set the size of the window
    windowSizex = w;
    windowSizey = h;

    //Set the size of the tessellation sub window and redraw
    glutSetWindow(tessWindow);
    glutPositionWindow(0, 0);
    glutReshapeWindow(windowSizex, int(windowSizey * THREE_QUARTER_WINDOW));
    glutPostRedisplay();

    //Set the size of the status sub window and redraw
    glutSetWindow(statusWindow);
    glutPositionWindow(0, int(windowSizey * THREE_QUARTER_WINDOW));
    glutReshapeWindow(windowSizex, int(windowSizey * QUARTER_WINDOW));
    glutPostRedisplay();

    //Set the y axis positions of the text fields
    fields[PRIMARY_TESS_FIELD_INDEX].y = int(windowSizey * EIGHTH_WINDOW);
    fields[SECONDARY_TESS_FIELD_INDEX].y = int(windowSizey * ONE_SIXTEENTH_WINDOW);
}

///////////////////////////////////////////////////////////
//Main function
///////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    //Glut initialization
    glutInit(&argc, argv);

    //Set the display mode to use double buffering and depth buffer
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    //Position the window
    glutInitWindowPosition(INIT_LOCATION_X, INIT_LOCATION_Y);

    //Initialize the state machine and other variables
    initialize();

    //Set the window size
    glutInitWindowSize(windowSizex, windowSizey);

    //CREATE THE MAIN WINDOW----------------------------------
    //Set the identifier for the main window
    mainWindow = glutCreateWindow(PROJECT_NAME);

    //Register the callback functions
    glutDisplayFunc(mainWindowDisplay);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(input);
    glutSpecialFunc(specialInput);
    //CREATE THE MAIN WINDOW----------------------------------

    //CREATE DISPLAY SUBWINDOW--------------------------------
    //Set the identifier for the display sub window
    tessWindow = glutCreateSubWindow(mainWindow, 0, 0, windowSizex, int(windowSizey * THREE_QUARTER_WINDOW));

    //Register the callback functions
    glutDisplayFunc(tessWindowDisplay);
    glutKeyboardFunc(input);
    glutSpecialFunc(specialInput);
    glutMouseFunc(mouseInput);
    glutMotionFunc(mouseMovement);
    //CREATE DISPLAY SUBWINDOW--------------------------------

    //CREATE STATUS SUBWINDOW--------------------------------
    //Set the identifier for the status sub window
    statusWindow = glutCreateSubWindow(mainWindow, 0, int(windowSizey * THREE_QUARTER_WINDOW), windowSizex, int(windowSizey * QUARTER_WINDOW)) ;

    //Register the callback functions
    glutDisplayFunc(statusWindowDisplay);
    glutKeyboardFunc(input);
    glutSpecialFunc(specialInput);
    glutMouseFunc(mouseInput);
    //CREATE STATUS SUBWINDOW--------------------------------

    //Enter the glut main loop
    glutMainLoop();

    return 0;
}
