#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "mesh.h"
#include "Material.h"
#include "camera.h"
#include "Entity.h"

class GameObject : public Entity {

	protected:
		Mesh* highMesh;
		Mesh* lowMesh;
		Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)

	public:
		
		// --- CONSTRUCTORES ---

		GameObject(const Vector3 position) : Entity(position) {
			this->highMesh = NULL;
			this->lowMesh = NULL;
			this->material = NULL;
		}

		GameObject(const Vector3 position, Mesh * highMesh) : Entity(position) {
			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * highMesh, const Quaternion rotation) : Entity(position, rotation) {
			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * highMesh, Texture* texture, Shader* shader, Vector4 color) : Entity(position) {
			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, 
					Texture* texture, Shader* shader, Vector4 color) : Entity(position, rotation) {
	
			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh,
					Material * material) : Entity(position, rotation) {

			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = material;
		}

		GameObject(const Transform transform, Mesh * highMesh, Material * material) : Entity(transform ) {
			this->highMesh = highMesh;
			this->lowMesh = NULL;
			this->material = material;
		}

		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh) : Entity(position) {
			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, const Quaternion rotation) : Entity(position, rotation) {
			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * highMesh, Mesh * lowMesh, Texture* texture, Shader* shader, Vector4 color) : Entity(position) {
			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Texture* texture, Shader* shader, Vector4 color) : Entity(position, rotation) {

			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * highMesh, Mesh * lowMesh,
					Material * material) : Entity(position, rotation) {

			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = material;
		}

		GameObject(const Transform transform, Mesh * highMesh, Mesh * lowMesh, Material * material) : Entity(transform) {
			this->highMesh = highMesh;
			this->lowMesh = lowMesh;
			this->material = material;
		}

		Mesh* getHighMesh() { return this->highMesh; }
		Mesh* getLowMesh() { return this->lowMesh; }
		Material* getMaterial() { return this->material; }

		void setHighMesh(Mesh * highMesh) { this->highMesh = highMesh; }
		void setLowMesh(Mesh * lowMesh) { this->lowMesh = lowMesh; }
		void setMaterial(Material* material) { this->material =  material; }

		void render() { render(Camera::current); }

		void render(Camera * camera) {

			BoundingBox axisBoundingBox;
			Mesh * mesh = this->highMesh;
			Shader* shader = this->material->getShader();

			if (camera->eye.distance(this->transform.getPosition()) > 100) {
				if (this->lowMesh != NULL) { mesh = this->lowMesh; }
			}

			if (mesh != NULL) {

				axisBoundingBox = transformBoundingBox(this->transform.getMatrixModel(), mesh->box);

				if (camera->testBoxInFrustum(axisBoundingBox.center, axisBoundingBox.halfsize) == CLIP_INSIDE) {

					if (shader != NULL) {

						// Activar falgs...

						if (this->material->getBlendAlpha() == true) { glEnable(GL_BLEND); }
						if (this->material->getDepthTest() == true) {}
						if (this->material->getCullFace() == true) {}

						//enable shader
						shader->enable();

						//upload uniforms
						shader->setUniform("u_color", this->material->getColor());
						shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
						shader->setUniform("u_texture", this->material->getTexture());
						shader->setUniform("u_model", this->transform.getMatrixModel());
						shader->setUniform("u_camera_position", camera->eye);
						shader->setUniform("u_time", 1);

						mesh->render(GL_TRIANGLES, shader);

						//disable shader
						shader->disable();

						// Desactivar falgs...

						if (this->material->getBlendAlpha() == true) { glDisable(GL_BLEND); }
						if (this->material->getDepthTest() == true) {}
						if (this->material->getCullFace() == true) {}
					}

				}

			}
		
		}

		virtual void uptade(float deltaTime) {}

};

#endif