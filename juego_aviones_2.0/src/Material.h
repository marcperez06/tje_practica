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

		Material() {
			this->texture = NULL;
			this->shader = NULL;
			this->color = Vector4(0, 0, 0, 1);
			this->blendAlpha = false;
			this->depthTest = false;
			this->cullFace = false;
		}

		Material(Texture * texture, Shader * shader, Vector4 color ) {
			this->texture = texture;
			this->shader = shader;
			this->color = color;
			this->blendAlpha = false;
			this->depthTest = false;
			this->cullFace = false;
		}

		Material(Texture * texture, Shader * shader, Vector4 color, bool blendAlpha, bool depthTest, bool cullFace) {
			this->texture = texture;
			this->shader = shader;
			this->color = color;
			this->blendAlpha = blendAlpha;
			this->depthTest = depthTest;
			this->cullFace = cullFace;
		}
		
		Texture* getTexture() const { return this->texture; }
		Shader* getShader() const { return this->shader; }
		Vector4 getColor() const { return this->color; }

		bool getBlendAlpha() const { return this->blendAlpha; }
		bool getDepthTest() const { return this->depthTest; }
		bool getCullFace() const { return this->cullFace; }

		void setTexture(Texture* texture) { this->texture = texture; }
		void setShader(Shader* shader) { this->shader = shader; }
		void setColor(Vector4 color) { this->color = color; }

		void setBlendAlpha(bool value) { this->blendAlpha = value; }
		void setDepthTest(bool value) { this->depthTest = value; }
		void setCullFace(bool value) { this->cullFace = value; }

};

#endif