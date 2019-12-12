#pragma once
#include <cmath>

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

	/*void perspective(float near, float far, float left, float right, float top, float bottom) 
	{
		data[0] = (2 * near) / (right - left);
		data[2] = (right + left) / (right - left);
		data[5] = (2 * near) / (top - bottom);
		data[6] = (top + bottom) / (top - bottom);
		data[10] = -((far + near) / (far - near));
		data[11] = -((2 * near * far) / (far - near));
		data[14] = -1;
		data[15] = 0;
	}*/

	void orthographique(float left, float right, float bottom, float top, float near, float far) 
	{
		data[0] = 2 / (right - left);
		data[12] = -((right + left) / (right - left));
		data[5] = 2 / (top - bottom);
		data[13] = -((top + bottom) / (top - bottom));
		data[10] = (-2)/(far - near);
		data[14] = -((far + near) / (far - near));

	}

	

	mat4() 
	{
		identity();
	}

};
