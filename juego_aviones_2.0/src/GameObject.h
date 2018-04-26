#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "mesh.h"
#include "Material.h"
#include "camera.h"
#include "Entity.h"

class GameObject : public Entity {

	protected:
		Mesh* mesh;
		Material* material; // Texture + Shader + Vector4 color + como se pinta booleanas (blend_alpha, depth_test, cull_face)

	public:
		
		GameObject(const Vector3 position) : Entity(position) {
			this->mesh = NULL;
			this->material = NULL;
		}

		GameObject(const Vector3 position, Mesh * mesh) : Entity(position) {
			this->mesh = mesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * mesh, const Quaternion rotation) : Entity(position, rotation) {
			this->mesh = mesh;
			this->material = new Material();
		}

		GameObject(const Vector3 position, Mesh * mesh, Texture* texture, Shader* shader, Vector4 color) : Entity(position) {
			this->mesh = mesh;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * mesh, 
					Texture* texture, Shader* shader, Vector4 color) : Entity(position, rotation) {
	
			this->mesh = mesh;
			this->material = new Material(texture, shader, color);
		}

		GameObject(const Vector3 position, const Quaternion rotation, Mesh * mesh,
					Material * material) : Entity(position, rotation) {

			this->mesh = mesh;
			this->material = material;
		}

		GameObject(const Transform transform, Mesh * mesh, Material * material) : Entity(transform ) {

			this->mesh = mesh;
			this->material = material;
		}

		Mesh* getMesh() { return this->mesh; }
		Material* getMaterial() { return this->material; }

		void setMesh(Mesh * mesh) { this->mesh = mesh; }
		void setMaterial(Material* material) { this->material =  material; }

		void render() { render(Camera::current); }

		void render(Camera * camera) {

			Shader* shader = this->material->getShader();
			Camera* camera = camera;

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