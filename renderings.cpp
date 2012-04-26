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

static void cubeFace(int n, Point3 ur, Point3 ul, Point3 bl);
void Cube(int n){
	// Your Cube code goes here
	// front face
	cubeFace(n, Point3(0.5,0.5,0.5), Point3(-0.5,0.5,0.5), Point3(-0.5,-0.5,0.5));
	// rear face
	cubeFace(n, Point3(-0.5,0.5,-0.5), Point3(0.5,0.5,-0.5), Point3(0.5,-0.5,-0.5));
	// top face
	cubeFace(n, Point3(-0.5,0.5,-0.5), Point3(-0.5,0.5,0.5), Point3(0.5,0.5,0.5));
	// bottom face
	cubeFace(n, Point3(0.5,-0.5,0.5), Point3(-0.5,-0.5,0.5), Point3(-0.5,-0.5,-0.5));
	// left face
	cubeFace(n, Point3(-0.5,-0.5,0.5), Point3(-0.5,0.5,0.5), Point3(-0.5,0.5,-0.5));
	// right face
	cubeFace(n, Point3(0.5,0.5,-0.5), Point3(0.5,0.5,0.5), Point3(0.5,-0.5,0.5));
	return;
}

// Draw a single face of a cube given boundary points
static void cubeFace(int n, Point3 ur, Point3 ul, Point3 bl) {
	float step = 1.0 / n;
	// iterate over rows
	for (int i = 0; i < n; i++) {
		// iterate over columns
		for (int j = 0; j < n; j++) {
			Point3 a(ul + (j * step) * (ur - ul) + (i * step) * (bl - ul));
			Point3 b(a + step * (bl - ul));
			Point3 d(a + step * (ur - ul));
			Point3 c(d + step * (bl - ul));
			addTriangle(a, b, d);
			addTriangle(b, c, d);
		}
	}
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
