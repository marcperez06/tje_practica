#include "EntityMesh.h"

// --- CONSTRUCTORES ---

EntityMesh::EntityMesh(const Vector3 position) : Entity(position) {
	this->highMesh = NULL;
	this->lowMesh = NULL;
	this->material = NULL;
}

EntityMesh::EntityMesh(const Vector3 position, Mesh * highMesh) : Entity(position) {
	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = new Material();
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh) : Entity(position, rotation) {
	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = new Material();
}

EntityMesh::EntityMesh(const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color) : Entity(position) {
	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = new Material(texture, shader, color);
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh,
						Texture* texture, Shader* shader, Vector4 color) : Entity(position, rotation) {

	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = new Material(texture, shader, color);
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh,
						Material * material) : Entity(position, rotation) {

	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = material;
}

EntityMesh::EntityMesh(const Transform transform, Mesh * highMesh, Material * material) : Entity(transform) {
	this->highMesh = highMesh;
	this->lowMesh = NULL;
	this->material = material;
}

EntityMesh::EntityMesh(const Vector3 position, Mesh * highMesh, Mesh * lowMesh) : Entity(position) {
	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = new Material();
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh) : Entity(position, rotation) {
	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = new Material();
}

EntityMesh::EntityMesh(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color) : Entity(position) {
	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = new Material(texture, shader, color);
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
						Texture* texture, Shader* shader, Vector4 color) : Entity(position, rotation) {

	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = new Material(texture, shader, color);
}

EntityMesh::EntityMesh(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
						Material * material) : Entity(position, rotation) {

	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = material;
}

EntityMesh::EntityMesh(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material) : Entity(transform) {
	this->highMesh = highMesh;
	this->lowMesh = lowMesh;
	this->material = material;
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

void EntityMesh::renderMesh(Camera * camera, Matrix44 globalMatrix) {

	BoundingBox axisBoundingBox;
	Mesh * mesh = this->highMesh;
	Shader* shader = this->material->shader;

	if (camera->eye.distance(this->transform.position) > 60) {
		if (this->lowMesh != NULL) { mesh = this->lowMesh; }
	}

	if (mesh != NULL) {

		axisBoundingBox = transformBoundingBox(globalMatrix, mesh->box);

		if (1) { //camera->testBoxInFrustum(axisBoundingBox.center, axisBoundingBox.halfsize) == CLIP_INSIDE) {

			if (shader != NULL) {

				// Activar falgs...

				if (this->material->blendAlpha == true) { glEnable(GL_BLEND); }
				if (this->material->depthTest == true) { glEnable(GL_DEPTH_TEST); }
				if (this->material->cullFace == true) { glEnable(GL_CULL_FACE); }

				//enable shader
				shader->enable();

				//upload uniforms
				shader->setUniform("u_color", this->material->color);
				shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
				shader->setUniform("u_texture", this->material->texture);
				shader->setUniform("u_model", globalMatrix);
				shader->setUniform("u_camera_position", camera->eye);
				shader->setUniform("u_time", 1);

				mesh->render(GL_TRIANGLES, shader);

				//disable shader
				shader->disable();

				// Desactivar falgs...

				if (this->material->blendAlpha == true) { glDisable(GL_BLEND); }
				if (this->material->depthTest == true) { glDisable(GL_DEPTH_TEST); }
				if (this->material->cullFace == true) { glDisable(GL_CULL_FACE); }
			}

		}

	}

}

void EntityMesh::update(float deltaTime) {}