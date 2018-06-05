#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../framework.h"

class Transform {
	
	public:
		// Vectores generales
		static const Vector3 UP;
		static const Vector3 DOWN;
		static const Vector3 LEFT;
		static const Vector3 RIGHT;
		static const Vector3 FRONT;
		static const Vector3 BACK;

		Matrix44 matrixModel;
		Vector3 position; // Posicion Local del objeto, respecto a su eje de cordenadas.
		Quaternion rotation;

		Transform();
		Transform(Vector3 const position, Quaternion rotation);

		void translate(Vector3 const position);
		void rotate(float angleInRadians, const Vector3 & axis);
};

#endif