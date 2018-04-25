#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "framework.h"

class Transform {
	private:

		Matrix44 matrixModel;
		Vector3 position; // Posicion Local del objeto, respecto a su eje de cordenadas.
		Quaternion rotation;

	public:
		// Carpetas generales
		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;

		Transform() {}
		Transform(Vector3 const position, Quaternion rotation) {
			this->rotation = rotation;
			this->position = position;
			this->matrixModel.translate(position.x, position.y, position.z);
		}

		Vector3 getLocalPosition() const { return this->position; }
		Quaternion getRotation() const { return this->rotation; }
		Matrix44 getMatrixModel() const { return this->matrixModel; }

		void setLocalPosition(Vector3 const position) {
			this->position = position;
			this->matrixModel.translate(this->position.x, this->position.y, this->position.z);
		}

		void setRotationAngle(Quaternion rotation) { this->rotation = rotation; }
		void setMatrixModel(Matrix44 model) { this->matrixModel = model; }

		void traslate(Vector3 const traslation) {
			Matrix44 T;
			
			this->position = this->position + traslation;

			T.setTranslation(this->position.x, this->position.y, this->position.z);
			this->matrixModel = T * this->matrixModel;
		}

};

const  Vector3 Transform::UP = Vector3(0, 1, 0);
const  Vector3 Transform::DOWN = Vector3(0, -1, 0);
const  Vector3 Transform::LEFT = Vector3(-1, 0, 0);
const  Vector3 Transform::RIGHT = Vector3(1, 1, 0);

#endif