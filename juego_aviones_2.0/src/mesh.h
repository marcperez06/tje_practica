#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"

#include <map>
#include <string>

class Shader;

class Mesh
{
public:
	static std::map<std::string, Mesh*> sMeshesLoaded;
	static bool use_binary; //always load the binary version of a mesh when possible
	static bool interleave_meshes; //loaded meshes will me automatically interleaved
	static bool auto_upload_to_vram; //loaded meshes will be stored in the VRAM
	static long num_meshes_rendered;
	static long num_triangles_rendered;

	struct tInterleaved {
		Vector3 vertex;
		Vector3 normal;
		Vector2 uv;
	};

	std::string name;

	std::vector<std::string> material_name; 
	std::vector<unsigned int> material_range; 

	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors; //here we store the colors

	std::vector< tInterleaved > interleaved; //to render interleaved

	Vector3 aabb_min;
	Vector3	aabb_max;
	BoundingBox box;

	float radius;

	unsigned int vertices_vbo_id;
	unsigned int uvs_vbo_id;
	unsigned int normals_vbo_id;
	unsigned int colors_vbo_id;

	unsigned int interleaved_vbo_id;

	Mesh();
	~Mesh();

	void clear();

	void render( unsigned int primitive, Shader* shader, int submesh_id = 0, int num_instances = 0 );
	void renderInstanced(unsigned int primitive, Shader* shader, const Matrix44* instanced_models, int number);
	void renderBounding( const Matrix44& model, bool world_bounding = true );
	void renderFixedPipeline(int primitive); //sloooooooow

	void enableBuffers(Shader* shader);
	void disableBuffers(Shader* shader);

	bool readBin(const char* filename);
	bool writeBin(const char* filename);

	unsigned int getNumSubmaterials() { return material_name.size(); }
	unsigned int getNumSubmeshes() { return material_range.size(); }

	void* collision_model;
	void createCollisionModel();
	bool testRayCollision( Matrix44 model, Vector3 ray_origin, Vector3 ray_direction, Vector3& collision, Vector3& normal );

	static Mesh* Load(const char* filename);

	void createQuad(float center_x, float center_y, float w, float h, bool flip_uvs);
	void createPlane(float size);
	void createCube();
	void createWireBox();
	void createGrid(float dist);

	unsigned int getNumVertices() { return interleaved.size() ? interleaved.size() : vertices.size(); }

	static Mesh* getQuad();

	void uploadToVRAM();
	bool interleaveBuffers();

private:
	bool loadASE(const char* filename);
	bool loadOBJ(const char* filename);
};

bool testMeshesCollision(Mesh* a, Matrix44 model_a, Mesh* b, Matrix44 model_b);

#endif