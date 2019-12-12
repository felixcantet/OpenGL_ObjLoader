#pragma once
struct Vector2 {
private:
	float x;
	float y;

public:

	Vector2() {
		this->x = 0.f;
		this->y = 0.f;
	}

	Vector2(float defaultValue) {
		this->x = defaultValue;
		this->y = defaultValue;
	}

	Vector2(float xValue, float yValue) {
		this->x = xValue;
		this->y = yValue;
	}

	float get_x() const 
	{
		return this->x;
	}

	float get_y() const
	{
		return this->y;
	}

	void set_x(float value)
	{
		this->x = value;
	}

	void set_y(float value)
	{
		this->y = value;
	}

	void set(float xValue, float yValue) {
		this->x = xValue;
		this->y = yValue;
	}

	Vector2& operator + (const Vector2 v) {
		return Vector2(this->x + v.get_x(), this->y + v.get_y());
	}

	Vector2& operator - (const Vector2 v) {
		return Vector2(this->x - v.get_x(), this->y - v.get_y());
	}
};
