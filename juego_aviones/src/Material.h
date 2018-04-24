#ifndef MATERIAL_H
#define MATERIAL_H

#include "framework.h"
#include "texture.h"
#include "shader.h"

class Material {

	protected:
		Texture* texture;
		Shader* shader;
		Vector4 color;
		bool blendAlpha;
		bool depthTest;
		bool cullFace;

	public:

		Material(Texture * texture, Shader * shader) {
			this->texture = texture;
			this->shader = shader;
			this->blendAlpha = false;
			this->depthTest = false;
			this->cullFace = false;
		}

		Material(Texture * texture, Shader * shader, bool blendAlpha, bool depthTest, bool cullFace) {
			this->texture = texture;
			this->shader = shader;
			this->blendAlpha = blendAlpha;
			this->depthTest = depthTest;
			this->cullFace = cullFace;
		}

		

};

#endif