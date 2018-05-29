#pragma once
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureShader.h"
#include "Transform2D.h"

class Sprite
{
private:
	TextureShader * shader;
	int drawLayer;

public:
	Transform2D transform;
	
	Sprite() {
		shader = new TextureShader; 
		transform.SetTrasform(Vector2(), 0.0f, 1.0f);
		drawLayer = 0;
	}

	void Init(Transform2D transform, int layer) {
		this->transform = transform;
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