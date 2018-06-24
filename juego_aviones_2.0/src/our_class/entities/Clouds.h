#ifndef CLOUDS_H
#define CLOUDS_H

#include "EntityMesh.h"

//class Camera;

class Clouds : EntityMesh {
	public:
		struct CloudProperties {
			int id;
			Vector3 pos;
			float size;
			float distance;
		};

		std::vector<CloudProperties> clouds;

		Clouds();
		~Clouds();
		virtual void render(Camera* camera);
};

#endif
