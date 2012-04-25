////////////////////////////////////////////////////////////
//
// File:  renderings.cpp
// Authors:  G. Fotiades, A. Koutmos
// Contributors: Matthew MacEwan
// Last modified: 2/2/11
//
// Description:  This file holds the implementations cube/sphere/cylinder/cone
//			   functions.  These will be linked to framework at link time,
//			   when the final binary is produced.
//
////////////////////////////////////////////////////////////

#include "resources.h"

// addTriangle definition will be resolved by the tessellation.o object at link time
extern void addTriangle(Point3 p1, Point3 p2, Point3 p3);

// Window title
const char* PROJECT_NAME = "Project 2 - Tessellation (Matthew MacEwan)";

void Cube(int n){
	// Your Cube code goes here
	float step = 1.0 / n;
	// iterate over rows
	for (int i = 0; i < n; i++) {
		// iterate over columns
		for (int j = 0; j < n; j++) {
			// front face
			addTriangle(Point3(-0.5 + j * step, 0.5 - i * step, 0.5),
						Point3(-0.5 + j * step, 0.5 - (i+1) * step, 0.5),
						Point3(-0.5 + (j+1) * step, 0.5 - i * step, 0.5));
			addTriangle(Point3(-0.5 + (j+1) * step, 0.5 - i * step, 0.5),
						Point3(-0.5 + j * step, 0.5 - (i+1) * step, 0.5),
						Point3(-0.5 + (j+1) * step, 0.5 - (i+1) * step, 0.5));

			// rear face
			addTriangle(Point3(0.5 - j * step, 0.5 - i * step, -0.5),
						Point3(0.5 - j * step, 0.5 - (i+1) * step, -0.5),
						Point3(0.5 - (j+1) * step, 0.5 - i * step, -0.5));
			addTriangle(Point3(0.5 - (j+1) * step, 0.5 - i * step, -0.5),
						Point3(0.5 - j * step, 0.5 - (i+1) * step, -0.5),
						Point3(0.5 - (j+1) * step, 0.5 - (i+1) * step, -0.5));
		}
	}
	return;
}

void Cone(int n, int m){
	// Your Cone code goes here
	return;
}

void Cylinder(int n, int m){
	// Your Cylinder code goes here
	return;
}

void Sphere(int n){
	// Your Sphere code goes here
	return;
}
