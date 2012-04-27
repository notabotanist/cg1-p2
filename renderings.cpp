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

#include<cmath> // for trig
#define PI 3.14159
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
	if (n < 3) {
		// This is nonsense
		return;
	}
	float circlestep = 2.0 * PI / n;
	float edgestep = 1.0 / m;
	for (float a=0; a < n; a++) {
		float pX = 0.5 * cos(a*circlestep);
		float pZ = 0.5 * sin(a*circlestep);
		float qX = 0.5 * cos((a+1)*circlestep);
		float qZ = 0.5 * sin((a+1)*circlestep);
		Point3 apex(0,0.5,0);
		Point3 botP(pX, -0.5, pZ);
		Point3 botQ(qX, -0.5, qZ);

		addTriangle(apex,apex + edgestep * (botQ-apex), apex + edgestep * (botP-apex));
		addTriangle(Point3(0,-0.5,0),botP,botQ);
		for (int i=1; i < m; i++) {
			Point3 a(apex + (i * edgestep) * (botQ - apex));
			Point3 b(a + edgestep * (botQ - apex));
			Point3 d(apex + (i * edgestep) * (botP - apex));
			Point3 c(d + edgestep * (botP - apex));
			addTriangle(a, c, d);
			addTriangle(a, b, c);
		}
	}
	return;
}

void Cylinder(int n, int m){
	// Your Cylinder code goes here
	if (n < 3) {
		// This is nonsense
		return;
	}
	float circlestep = 2.0 * PI / n;
	float edgestep = 1.0 / m;
	for (float a=0; a < n; a++) {
		float pX = 0.5 * cos(a*circlestep);
		float pZ = 0.5 * sin(a*circlestep);
		float qX = 0.5 * cos((a+1)*circlestep);
		float qZ = 0.5 * sin((a+1)*circlestep);
		Point3 topP(pX, 0.5, pZ);
		Point3 topQ(qX, 0.5, qZ);
		Point3 botP(pX, -0.5, pZ);
		Point3 botQ(qX, -0.5, qZ);

		addTriangle(Point3(0,0.5,0),topQ,topP);
		addTriangle(Point3(0,-0.5,0),botP,botQ);
		for (int i=0; i < m; i++) {
			Point3 a(topQ + (i * edgestep) * (botQ - topQ));
			Point3 b(a + edgestep * (botQ - topQ));
			Point3 d(a + (topP - topQ));
			Point3 c(d + edgestep * (botQ - topQ));
			addTriangle(a, b, c);
			addTriangle(a, c, d);
		}
	}
	return;
}

void Sphere(int n){
	// Your Sphere code goes here
	return;
}
