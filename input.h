////////////////////////////////////////////////////////////
//
// File:  input.h
// Authors:  G. Fotiades, A. Koutmos
// Contributors:
// Last modified: 2/2/11
//
// Description:  This file holds the declarations for some input functions
//               that will be used in the input.cpp file.
//
////////////////////////////////////////////////////////////

#ifndef __INPUT_H__
#define __INPUT_H__

void mouseMovement(int x, int y);

void mouseInput(int button, int state, int x, int y);

void specialInput(int key, int x, int y);

void input(unsigned char key, int x, int y);

void UpdateTextEntry(char key);

void ShowHelp();

#endif
