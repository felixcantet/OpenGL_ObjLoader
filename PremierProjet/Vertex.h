#include "Vector2.h"
#include "Vector3.h"

//struct Vertex {
//	Vector3 position;
//	Vector3 color;
//	Vector2 texcoord;
//
//	Vertex() {
//		position = Vector3();
//		color = Vector3();
//		texcoord = Vector2();
//	}
//
//	Vertex(Vector3 &pos, Vector3 &col, Vector2 &coord) {
//		position = pos;
//		color = col;
//		texcoord = coord;
//	}
//};


struct Vertex {
	
	float x;
	float y;
	float z;

	float nx;
	float ny;
	float nz;

	float r;
	float g;
	float b;
	float a;

	float u;
	float v;
};
