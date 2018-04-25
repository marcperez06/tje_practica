#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "framework.h"
#include "Transform.h"
#include "mesh.h"
#include "Material.h"
#include "camera.h"

class GameObject {

	protected:
		Transform transform;
		Mesh* mesh;
		Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)
		std::string name;
		int uuid; // universal unique identifier

	public:
		
		GameObject(const Vector3 position) {
			this->transform = Transform(position, Quaternion());
			this->mesh = NULL;
			this->material = NULL;
		}

		GameObject(const Vector3 position, Mesh * mesh) {
			this->transform = Transform(position, Quaternion());
			this->mesh = mesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * mesh, Quaternion rotation) {
			this->transform = Transform(position, rotation);
			this->mesh = mesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * mesh, Texture* texture, Shader* shader, Vector4 color) {
			this->transform = Transform(position, Quaternion());
			this->mesh = mesh;
			this->material = new Material(texture, shader, color);
		}

		Transform* getTransform() { return &this->transform; }
		Mesh* getMesh() { return this->mesh; }
		Material* getMaterial() { return this->material; }
		std::string getName() { return this->name; }
		int getUuid() { return this->uuid; }

		void setPosition(Vector3 position) { this->transform.setPosition(position); }
	
		void setMesh(Mesh * mesh) { this->mesh = mesh; }
		void setMaterial(Material* material) { this->material =  material; }
		void setTransform(Transform transform) { this->transform = transform; }

		void setName(std::string name) { this->name = name; }
		void setUuid(int uuid) { this->uuid = uuid; }

		void move(const Vector3 pos) {
			Matrix44 newModel;
			Vector3 newPos = newModel * pos;
			this->transform.setPosition(newPos);
		}

		void render() {

			Shader* shader = this->material->getShader();
			Camera* camera = Camera::current;

			if (shader != NULL) {

				// Activar falgs...

				//enable shader
				shader->enable();

				//upload uniforms
				shader->setUniform("u_color", this->material->getColor());
				shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
				shader->setUniform("u_texture", this->material->getTexture());
				shader->setUniform("u_model", this->transform.getMatrixModel());
				shader->setUniform("u_time", 1);

				mesh->render(GL_TRIANGLES, shader);

				//disable shader
				shader->disable();
			}

		}

		virtual void uptade(float deltaTime) {}

};

#endif