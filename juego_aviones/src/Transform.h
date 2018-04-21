#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "framework.h"

class Transform {
	private:

		Matrix44 matrixModel;
		Vector3 globalPosition;
		Vector3 localPosition;
		float rotationAngle;

	public:
		// Carpetas generales
		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;

		Transform() {}
		Transform(Vector3 const position, float rotationAngle) {
			this->rotationAngle = rotationAngle;
			this->globalPosition = this->matrixModel * position;
			this->localPosition = position;
			this->matrixModel.traslate(position.x, position.y, position.z);
		}

		Vector3 getLocalPosition() const { return this->localPosition; }
		float getRotationAngle() const { return this->rotationAngle; }
		Matrix44 getMatrixModel() const { return this->matrixModel; }

		void setLocalPosition(Vector3 const position) {
			this->localPosition = position;

			std::cout << "X: " + std::to_string(this->localPosition.x) << std::endl;
			std::cout << "Y: " + std::to_string(this->localPosition.y) << std::endl;
			std::cout << "Z: " + std::to_string(this->localPosition.z) << std::endl;

			this->matrixModel.traslate(this->localPosition.x, this->localPosition.y, this->localPosition.z);
		}

		void setRotationAngle(float rotationAngle) { this->rotationAngle = rotationAngle; }
		void setMatrixModel(Matrix44 model) { this->matrixModel = model; }

		void traslate(Vector3 const traslation) {
			Matrix44 T;
			
			this->localPosition = this->localPosition + traslation;

			std::cout << "X: " + std::to_string(this->localPosition.x) << std::endl;
			std::cout << "Y: " + std::to_string(this->localPosition.y) << std::endl;
			std::cout << "Z: " + std::to_string(this->localPosition.z) << std::endl;

			T.setTranslation(this->localPosition.x, this->localPosition.y, this->localPosition.z);
			this->matrixModel = T * this->matrixModel;
		}

};

const  Vector3 Transform::UP = Vector3(0, 1, 0);
const  Vector3 Transform::DOWN = Vector3(0, -1, 0);
const  Vector3 Transform::LEFT = Vector3(-1, 0, 0);
const  Vector3 Transform::RIGHT = Vector3(1, 1, 0);

#endif