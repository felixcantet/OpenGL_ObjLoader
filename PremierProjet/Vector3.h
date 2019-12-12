#pragma once
#pragma once
struct Vector3 {
private:
	float x;
	float y;
	float z;

public:

	Vector3() {
		this->x = 0.f;
		this->y = 0.f;
		this->z = 0.f;
	}

	Vector3(float defaultValue) {
		this->x = defaultValue;
		this->y = defaultValue;
		this->z = defaultValue;
	}

	Vector3(float xValue, float yValue, float zValue) {
		this->x = xValue;
		this->y = yValue;
		this->z = zValue;
	}

	float get_x() const
	{
		return this->x;
	}

	float get_y() const
	{
		return this->y;
	}

	float get_z() const
	{
		return this->z;
	}

	void set_x(float value)
	{
		this->x = value;
	}

	void set_y(float value)
	{
		this->y = value;
	}

	void set_z(float value)
	{
		this->z = value;
	}

	void set(float xValue, float yValue, float zValue) {
		this->x = xValue;
		this->y = yValue;
		this->z = zValue;
	}

	Vector3& operator + (const Vector3 v) {
		return Vector3(this->x + v.get_x(), this->y + v.get_y(), this->z + v.get_z());
	}

	Vector3& operator - (const Vector3 v) {
		return Vector3(this->x - v.get_x(), this->y - v.get_y(), this->z - v.get_z());
	}
};
