#include "EntityMesh.h"
#include "../../game.h"

// --- CONSTRUCTORES ---
EntityMesh::EntityMesh() : Entity() {
	this->highMesh = NULL;
	this->lowMesh = NULL;
	this->material = NULL;
	this->type = ENTITY_MESH;
}

EntityMesh::EntityMesh(const Transform transform, Mesh * highMesh, Material * material) : Entity(transform) {
	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = material;
	this->type = ENTITY_MESH;
}

EntityMesh::EntityMesh(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material) : Entity(transform) {
	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = material;
	this->type = ENTITY_MESH;
}

// --- FIN CONSTRUCTORES ---

EntityMesh::~EntityMesh() {}

//void EntityMesh::render() { render(Camera::current); }

void EntityMesh::render(Camera* camera) {
	Matrix44 globalMatrix = this->getGlobalMatrix();

	this->renderMesh(camera, globalMatrix);

	for (int i = 0; i < this->children.size(); i++) {
		this->children[i]->render(camera);
	}
}

Mesh* EntityMesh::getCorrectMeshRespectCameraDistance(Camera* camera) {
	Mesh * mesh = this->highMesh;
	if (camera->eye.distance(this->transform.position) > 60) {
		if (this->lowMesh != NULL) { mesh = this->lowMesh; }
	}
	return mesh;
}

void EntityMesh::renderMesh(Camera * camera, Matrix44 globalMatrix) {

	BoundingBox axisBoundingBox;
	Shader* shader = this->material->shader;
	Mesh* mesh = this->getCorrectMeshRespectCameraDistance(camera);

	if (mesh == NULL || shader == NULL) {
		return;
	}

	axisBoundingBox = transformBoundingBox(globalMatrix, mesh->box);

	if (camera->testBoxInFrustum(axisBoundingBox.center, axisBoundingBox.halfsize) != CLIP_OUTSIDE) {

		//this->activeGlFlags();

		//enable shader
		shader->enable();

		//upload uniforms
		shader->setUniform("u_color", this->material->color);
		shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
		shader->setUniform("u_texture", this->material->texture);
		if (this->material->extraTexture != NULL) {
			shader->setUniform("u_extra_texture", this->material->extraTexture);
		}

		if (this->material->extraTexture != NULL) {
			shader->setUniform("u_normal_texture", this->material->normalTexture);
		}

		shader->setUniform("u_model", globalMatrix);
		shader->setUniform("u_camera_position", camera->eye);
		shader->setUniform("u_time", Game::instance->time);

		mesh->render(GL_TRIANGLES, shader);

		//disable shader
		shader->disable();

		//this->desactiveGlFlags();

	}

}

void EntityMesh::activeGlFlags() {
	if (this->material->blendAlpha == true) { glEnable(GL_BLEND); }
	if (this->material->depthTest == true) { glEnable(GL_DEPTH_TEST); }
	if (this->material->cullFace == true) { glEnable(GL_CULL_FACE); }
}

void EntityMesh::desactiveGlFlags() {
	if (this->material->blendAlpha == true) { glDisable(GL_BLEND); }
	if (this->material->depthTest == true) { glDisable(GL_DEPTH_TEST); }
	if (this->material->cullFace == true) { glDisable(GL_CULL_FACE); }
}

void EntityMesh::update(float deltaTime) { 
	Entity::update(deltaTime);
}