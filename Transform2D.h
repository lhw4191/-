#pragma once

#include "Vector.h"
#include "Matrix.h"

struct Transform2D
{
public :
	Transform2D() {};
	Transform2D(Vector2 inLocation, float inRotation, float scale) : Location(inLocation), Rotation(inRotation), Scale(scale) {}

	void SetTrasform(Vector2 inLocation, float inRotation, float scale)
	{
		Location = inLocation;
		Rotation = inRotation;
		Scale = scale;
	}

	Matrix3 GetTRSMatrix()
	{
		Matrix3 TMat, RMat, SMat;
		TMat.SetTranslation(Location.X, Location.Y);
		RMat.SetRotation(Rotation);
		SMat.SetScale(Scale);

		return TMat * RMat * SMat;
	}

	Matrix3 GetViewMatrix()
	{
		Matrix3 TMat, RMat;
		TMat.SetTranslation(-Location.X, -Location.Y);
		RMat.SetRotation(-Rotation);

		return RMat * TMat;
	}

	Vector2 Location;
	float Rotation;
	float Scale;
};
