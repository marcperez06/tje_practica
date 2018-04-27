#ifndef MATERIAL_H
#define MATERIAL_H

#include "../framework.h"
#include "../texture.h"
#include "../shader.h"

class Material {

	public:

		Texture* texture;
		Shader* shader;
		Vector4 color;
		bool blendAlpha;
		bool depthTest;
		bool cullFace;

		Material();

		Material(Texture * texture, Shader * shader, Vector4 color);

		Material(Texture * texture, Shader * shader, Vector4 color, bool blendAlpha, bool depthTest, bool cullFace);

};

#endif