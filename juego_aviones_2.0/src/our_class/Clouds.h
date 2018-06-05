#ifndef CLOUDS_H
#define CLOUDS_H

#include "EntityMesh.h"

class Clouds : EntityMesh{
	public:
		struct CloudProperties {
			int id;
			Vector3 pos;
			float size;
		};

		std::vector<CloudProperties> clouds;

		Clouds();
		~Clouds();
		virtual void render();
		bool sortClouds(const CloudProperties cloudA, const CloudProperties cloudB);
};

#endif
