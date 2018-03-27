#include "stdafx.h"
#include "Vector.h"

float Vector2::Dist(const Vector2 &V1, const Vector2 &V2)
{
	return sqrtf(DistSquared(V1, V2));
}

float Vector2::DistSquared(const Vector2 &V1, const Vector2 &V2)
{
	return (V2.X - V1.X) * (V2.X - V1.X) + (V2.Y - V1.Y) * (V2.Y - V1.Y);
}

void Vector2::Normalize()
{
	float l = sqrt(X * X + Y * Y);
	X = X / l;
	Y = Y / l;
}

Vector2 Vector2::operator*(const Matrix2 Mat) const
{
	Vector2 Result;

	Result.X = X * Mat._11 + Y * Mat._21;
	Result.Y = X * Mat._12 + Y * Mat._22;

	return Result;
}

Vector2 Vector2::operator*(const Vector2 V) const
{
	return Vector2(X * V.X, Y * V.Y);
}

Vector2 Vector2::operator+(const Vector2 V) const
{
	return Vector2(X+V.X, Y+V.Y);
}

Vector2 Vector2::operator-(const Vector2 V) const
{
	return Vector2(X - V.X, Y - V.Y);
}

Vector2 Vector2::operator^(const Vector2 V) const
{
	return Vector2();
}

float Vector2::operator|(const Vector2 V) const
{
	return X * V.X + Y * V.Y;
}