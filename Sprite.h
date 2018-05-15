#pragma once
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureShader.h"

class Sprite
{
private:
	TextureShader * shader;
	int drawLayer;

public:
	Vector2 position;
	float angle;
	float scale;
	
	Sprite() {
		shader = new TextureShader; 
		position = Vector2();
		angle = 0;
		scale = 1.0f;
		drawLayer = 0;
	}

	void Init(Vector2 position, float angle, float scale, int layer) {
		this->position = position;
		this->angle = angle;
		this->scale = scale;
		this->drawLayer = layer;
	}

	void Render(Mesh* mesh)
	{
		shader->Render(mesh);
	}

	void SetDrawLayer(int layer)
	{
		this->drawLayer = layer;
	}
	void SetMatrix(Matrix3 worldMatrix, Matrix3 viewMatrix)
	{
		shader->SetMatrix(worldMatrix, viewMatrix);
	}
	void SetTexture(Texture* texture)
	{
		shader->SetTexture(texture);
	}
	
	static bool comp(const Sprite* a, const Sprite* b)
	{
		return (a->drawLayer > b->drawLayer);
	}
};