#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Vector3.h"

const float PI_OVER_360 = 0.0087266;
struct mat4 {
	float data[16];

	void identity() {
		data[0] = 1.f; data[4] = 0; data[8] = 0; data[12] = 0;
		data[1] = 0; data[5] = 1; data[9] = 0; data[13] = 0;
		data[2] = 0; data[6] = 0; data[10] = 1; data[14] = 0;
		data[3] = 0; data[7] = 0; data[11] = 0; data[15] = 1;
	}

	void rotate(float angle)
	{
		data[0] = cos(angle);
		data[4] = -sin(angle);
		data[1] = sin(angle);
		data[5] = cos(angle);
	}

	

	void scale(float factor)
	{
		data[0] = factor;
		data[5] = factor;
		data[10] = factor;
	}

	void translate(float x, float y, float z) {
		data[12] = x;
		data[13] = y;
		data[14] = z;
	}


	void orthographique(float left, float right, float bottom, float top, float near, float far)
	{
		data[0] = 2 / (right - left);
		data[12] = -((right + left) / (right - left));
		data[5] = 2 / (top - bottom);
		data[13] = -((top + bottom) / (top - bottom));
		data[10] = (-2) / (far - near);
		data[14] = -((far + near) / (far - near));

	}

	void setUpRotationMatrix(float angle, float u, float v, float w)
	{
		float L = (u * u + v * v + w * w);
		angle = angle * M_PI / 180.0; //converting to radian value
		float u2 = u * u;
		float v2 = v * v;
		float w2 = w * w;

		data[0] = (u2 + (v2 + w2) * cos(angle)) / L;
		data[1] = (u * v * (1 - cos(angle)) - w * sqrt(L) * sin(angle)) / L;
		data[2] = (u * w * (1 - cos(angle)) + v * sqrt(L) * sin(angle)) / L;
		data[3] = 0.0;
		
		data[4] = (u * v * (1 - cos(angle)) + w * sqrt(L) * sin(angle)) / L;
		data[5] = (v2 + (u2 + w2) * cos(angle)) / L;
		data[6] = (v * w * (1 - cos(angle)) - u * sqrt(L) * sin(angle)) / L;
		data[7] = 0.0;
		
		data[8] = (u * w * (1 - cos(angle)) - v * sqrt(L) * sin(angle)) / L;
		data[9] = (v * w * (1 - cos(angle)) + u * sqrt(L) * sin(angle)) / L;
		data[10] = (w2 + (u2 + v2) * cos(angle)) / L;
		data[11] = 0.0;
		
		data[12] = 0.0;
		data[13] = 0.0;
		data[14] = 0.0;
		data[15] = 1.0;
	}
	
	

	void perspective(float fov, float aspect,
		float znear, float zfar)
	{
		float xymax = znear * tan(fov * PI_OVER_360);
		float ymin = -xymax;
		float xmin = -xymax;

		float width = xymax - xmin;
		float height = xymax - ymin;

		float depth = zfar - znear;
		float q = -(zfar + znear) / depth;
		float qn = -2 * (zfar * znear) / depth;

		float w = 2 * znear / width;
		w = w / aspect;
		float h = 2 * znear / height;

		data[0] = w;
		data[1] = 0;
		data[2] = 0;
		data[3] = 0;
		data[4] = 0;
		data[5] = h;
		data[6] = 0;
		data[7] = 0;
		data[8] = 0;
		data[9] = 0;
		data[10] = q;
		data[11] = -1;
		data[12] = 0;
		data[13] = 0;
		data[14] = qn;
		data[15] = 0;
	}


	

	mat4()
	{
		identity();
	}

};
