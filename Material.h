#pragma once
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureShader.h"

class Material
{
private:
	TextureShader * shader;

public:
	Material() { shader = new TextureShader; }

	int drawLayer;

	void Render(Mesh* mesh)
	{
		shader->Render(mesh);
	}
	void SetMatrix(Matrix3 matrix)
	{
		shader->SetWorldMatrix(matrix);
	}
	void SetTexture(Texture* texture)
	{
		shader->SetTexture(texture);
	}

	static bool comp(const Material* a, const Material* b)
	{
		return (a->drawLayer > b->drawLayer);
	}
};