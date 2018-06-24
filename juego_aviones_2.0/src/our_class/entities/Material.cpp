#include "Material.h"

Material::Material() {
	this->texture = NULL;
	this->extraTexture = NULL;
	this->shader = NULL;
	this->color = Vector4(0, 0, 0, 1);
	this->blendAlpha = false;
	this->depthTest = false;
	this->cullFace = false;
}

Material::Material(Texture * texture, Shader * shader, Vector4 color) {
	this->texture = texture;
	this->extraTexture = NULL;
	this->shader = shader;
	this->color = color;
	this->blendAlpha = false;
	this->depthTest = false;
	this->cullFace = false;
}

Material::Material(Texture * texture, Shader * shader, Vector4 color, bool blendAlpha, bool depthTest, bool cullFace) {
	this->texture = texture;
	this->extraTexture = NULL;
	this->shader = shader;
	this->color = color;
	this->blendAlpha = blendAlpha;
	this->depthTest = depthTest;
	this->cullFace = cullFace;
}