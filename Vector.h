#pragma once

#include "Matrix.h"
#include "stdafx.h"

struct Vector2
{
public:
	float X;
	float Y;

public:
	Vector2() : X(0), Y(0) {}
	Vector2(float InX, float InY) : X(InX), Y(InY) {}

	static float Dist(const Vector2 &V1, const Vector2 &V2);
	static float DistSquared(const Vector2 &V1, const Vector2 &V2);

	void Normalize();
	Vector2 operator *(const Matrix2 Mat) const;
	Vector2 operator *(const Vector2 V) const;
	Vector2 operator +(const Vector2 V) const;
	Vector2 operator -(const Vector2 V) const;
	Vector2 operator ^(const Vector2 V) const;
	float operator |(const Vector2 V) const;

	bool Equals(const Vector2& V, float Tolerance = KINDA_SMALL_NUMBER) const;
};

FORCEINLINE bool Vector2::Equals(const Vector2& V, float Tolerance) const
{
	return fabs(X - V.X) <= Tolerance && fabs(Y - V.Y) <= Tolerance;
}