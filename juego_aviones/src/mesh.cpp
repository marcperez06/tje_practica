#include "mesh.h"
#include "extra/textparser.h"
#include "utils.h"
#include "shader.h"
#include "includes.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <sys/stat.h>

#include "camera.h"
#include "extra/coldet/coldet.h"

std::map<std::string, Mesh*> Mesh::sMeshesLoaded;
bool Mesh::use_binary = true;
long Mesh::num_meshes_rendered = 0;
long Mesh::num_triangles_rendered = 0;

#define FORMAT_ASE 1
#define FORMAT_OBJ 2
#define FORMAT_BIN 3

Mesh::Mesh()
{
	radius = 0;
	vertices_vbo_id = uvs_vbo_id = normals_vbo_id = colors_vbo_id = 0;

	primitive = GL_TRIANGLES;
	collision_model = NULL;
	clear();
}

Mesh::~Mesh()
{
	clear();
}


void Mesh::clear()
{
	//Free VBOs
	if (vertices_vbo_id) 
		glDeleteBuffersARB(1,&vertices_vbo_id);
	if (uvs_vbo_id)
		glDeleteBuffersARB(1,&uvs_vbo_id);
	if (normals_vbo_id) 
		glDeleteBuffersARB(1,&normals_vbo_id);
	if (colors_vbo_id) 
		glDeleteBuffersARB(1,&colors_vbo_id);
	if (interleaved_vbo_id)
		glDeleteBuffersARB(1, &interleaved_vbo_id);

	//VBOs ids
	vertices_vbo_id = uvs_vbo_id = normals_vbo_id = colors_vbo_id = interleaved_vbo_id = 0;

	//buffers
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	interleaved.clear();

	if (collision_model)
		delete collision_model;
}

void Mesh::render(unsigned int primitive, Shader* sh, int submesh_id)
{
	if (!sh || !sh->compiled)
	{
		assert(0 && "no shader or shader not compiled");
		return;
	}

	assert( (interleaved.size() || vertices.size()) && "No vertices in this mesh");

	int vertex_location = sh->getAttribLocation("a_vertex");
	assert(vertex_location != -1 && "No a_vertex found in shader");

	if (vertex_location == -1)
		return;

	int interleave_offset = interleaved.size() ? sizeof(tInterleaved) : 0;
	int offset_normal = sizeof(Vector3);
	int offset_uv = sizeof(Vector3) + sizeof(Vector3);

	glEnableVertexAttribArray(vertex_location);

	if (vertices_vbo_id || interleaved_vbo_id)
	{
		glBindBuffer( GL_ARRAY_BUFFER, interleave_offset ? interleaved_vbo_id : vertices_vbo_id );
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, interleave_offset, 0);
	}
	else
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, interleave_offset, interleave_offset ? &interleaved[0].vertex : &vertices[0] );

	int normal_location = -1;
	if (normals.size() || interleave_offset)
	{
		normal_location = sh->getAttribLocation("a_normal");
		if (normal_location != -1)
		{
			glEnableVertexAttribArray(normal_location);
			if (normals_vbo_id || interleaved_vbo_id)
			{
				glBindBuffer(GL_ARRAY_BUFFER, interleaved_vbo_id ? interleaved_vbo_id : normals_vbo_id);
				glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, interleave_offset, (void*)offset_normal );
			}
			else
				glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, interleave_offset, interleave_offset ? &interleaved[0].normal : &normals[0]);
		}
	}

	int uv_location = -1;
	if (uvs.size() || interleave_offset)
	{
		uv_location = sh->getAttribLocation("a_uv");
		if (uv_location != -1)
		{
			glEnableVertexAttribArray(uv_location);
			if (uvs_vbo_id || interleaved_vbo_id)
			{
				glBindBuffer(GL_ARRAY_BUFFER, interleaved_vbo_id ? interleaved_vbo_id : uvs_vbo_id);
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, interleave_offset, (void*)offset_uv );
			}
			else
				glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, interleave_offset, interleave_offset ? &interleaved[0].uv : &uvs[0]);
		}
	}

	int color_location = -1;
	if (colors.size())
	{
		color_location = sh->getAttribLocation("a_color");
		if (color_location != -1)
		{
			glEnableVertexAttribArray(color_location);
			if (colors_vbo_id)
			{
				glBindBuffer(GL_ARRAY_BUFFER, colors_vbo_id);
				glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);
			}
			else
				glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0]);
		}
	}

	int start = 0;
	int size = vertices.size();
	if (interleaved.size())
		size = interleaved.size();

	if (submesh_id > 0)
		start = material_range[submesh_id - 1] * 3;
	if (!material_range.empty())
		size = material_range[submesh_id] * 3 - start;

	glDrawArrays(primitive, start, size);

	num_triangles_rendered += size / 3;
	num_meshes_rendered++;

	glDisableVertexAttribArray(vertex_location);
	if (normal_location != -1) glDisableVertexAttribArray(normal_location);
	if (uv_location != -1) glDisableVertexAttribArray(uv_location);
	if (color_location != -1) glDisableVertexAttribArray(color_location);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //if crashes here, comment this line
}

void Mesh::uploadToVRAM()
{
	assert(vertices.size() || interleaved.size());

	if (glGenBuffersARB == 0)
	{
		std::cout << "Error: your graphics cards dont support VBOs. Sorry." << std::endl;
		exit(0);
	}

	if (interleaved.size())
	{
		// Vertex,Normal,UV
		if (interleaved_vbo_id == 0)
			glGenBuffersARB(1, &interleaved_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, interleaved_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, interleaved.size() * sizeof(tInterleaved), &interleaved[0], GL_STATIC_DRAW_ARB);
	}
	else
	{
		// Vertices
		if (vertices_vbo_id == 0)
			glGenBuffersARB(1, &vertices_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vertices_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW_ARB);

		// UVs
		if (uvs.size())
		{
			if (uvs_vbo_id == 0)
				glGenBuffersARB(1, &uvs_vbo_id);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, uvs_vbo_id);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW_ARB);
		}

		// Normals
		if (normals.size())
		{
			if (normals_vbo_id == 0)
				glGenBuffersARB(1, &normals_vbo_id);
			glBindBufferARB(GL_ARRAY_BUFFER_ARB, normals_vbo_id);
			glBufferDataARB(GL_ARRAY_BUFFER_ARB, normals.size() * sizeof(Vector3), &normals[0], GL_STATIC_DRAW_ARB);
		}
	}

	// Colors
	if (colors.size())
	{
		if (colors_vbo_id == 0)
			glGenBuffersARB(1, &colors_vbo_id);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, colors_vbo_id);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, colors.size() * sizeof(Vector4), &colors[0], GL_STATIC_DRAW_ARB);
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	checkGLErrors();

	//clear buffers to save memory
}

void Mesh::createCollisionModel()
{
	CollisionModel3D* collision_model = newCollisionModel3D();
	//collision_model->setTriangleNumber(vStrip.size() / 3);
	for (int count = 0; count < vertices.size()/3; count++)
		collision_model->addTriangle( vertices[count*3].x, vertices[count*3].y, vertices[count*3].z,
									vertices[count*3+1].x, vertices[count*3+1].y, vertices[count*3+1].z,
									vertices[count*3+2].x, vertices[count*3+2].y, vertices[count*3+2].z);
	collision_model->finalize();
	this->collision_model = collision_model;
}

bool Mesh::testRayCollision(Matrix44 model, Vector3 start, Vector3 front, Vector3& collision, Vector3& normal)
{
	CollisionModel3D* collision_model = (CollisionModel3D*)this->collision_model;
	assert(collision_model && "CollisionModel3D must be created before using it, call createCollisionModel");

	collision_model->setTransform( model.m );
	if (collision_model->rayCollision( start.v , front.v, true) == false)
		return false;

	collision_model->getCollisionPoint( collision.v, true );

	float t1[9],t2[9];
	collision_model->getCollidingTriangles(t1,t2,false);

	Vector3 v1;
	Vector3 v2;
	v1=Vector3(t1[3]-t1[0],t1[4]-t1[1],t1[5]-t1[2]);
	v2=Vector3(t1[6]-t1[0],t1[7]-t1[1],t1[8]-t1[2]);
	v1.normalize();
	v2.normalize();
	normal = v1.cross(v2);

	return true;
}

bool Mesh::interleaveBuffers()
{
	if (!vertices.size() || !normals.size() || !uvs.size())
		return false;

	assert(vertices.size() == normals.size() && normals.size() == uvs.size());

	interleaved.resize(vertices.size());

	for (int i = 0; i < vertices.size(); ++i)
	{
		interleaved[i].vertex = vertices[i];
		interleaved[i].normal = normals[i];
		interleaved[i].uv = uvs[i];
	}

	vertices.resize(0);
	normals.resize(0);
	uvs.resize(0);
}

typedef struct 
{
	int size;
	Vector3 aabb_min;
	Vector3	aabb_max;
	Vector3	center;
	Vector3	halfsize;
	float radius;
	int material_range[4];
	char streams[4]; //Normal|Uvs|Color|Extra
} sMeshInfo;

bool Mesh::readBin(const char* filename)
{
	FILE *f;
	assert(filename);

	struct stat stbuffer;

	stat(filename,&stbuffer);
	f = fopen(filename,"rb");
	if (f == NULL) return false;

	unsigned int size = stbuffer.st_size;
	char* data = new char[size];
	fread(data,size,1,f);
	fclose(f);

	//watermark
	if ( memcmp(data,"MBIN",4) != 0 )
	{
		std::cout << "[ERROR] loading BIN: invalid content: " << filename << std::endl;
		return false;
	}

	char* pos = data + 4;
	sMeshInfo info;
	memcpy(&info,pos,sizeof(sMeshInfo));
	pos += sizeof(sMeshInfo);

	if (info.streams[0] == 'I')
	{
		interleaved.resize(info.size);
		memcpy((void*)&interleaved[0], pos, sizeof(tInterleaved) * info.size);
		pos += sizeof(tInterleaved) * info.size;
	}

	if (info.streams[0] == 'V')
	{
		vertices.resize(info.size);
		memcpy((void*)&vertices[0], pos, sizeof(Vector3) * info.size);
		pos += sizeof(Vector3) * info.size;
	}

	if (info.streams[1] == 'N')
	{
		normals.resize(info.size);
		memcpy((void*)&normals[0],pos,sizeof(Vector3) * info.size);
		pos += sizeof(Vector3) * info.size;
	}

	if (info.streams[2] == 'U')
	{
		uvs.resize(info.size);
		memcpy((void*)&uvs[0],pos,sizeof(Vector2) * info.size);
		pos += sizeof(Vector2) * info.size;
	}

	if (info.streams[3] == 'C')
	{
		colors.resize(info.size);
		memcpy((void*)&colors[0],pos,sizeof(Vector4) * info.size);
		pos += sizeof(Vector4) * info.size;
	}

	aabb_max = info.aabb_max;
	aabb_min = info.aabb_min;
	center = info.center;
	halfsize = info.halfsize;
	radius = info.radius;

	for (int i = 0; i < 4; i++)
		if (info.material_range[i] != -1)
			material_range.push_back( info.material_range[i] );
		else
			break;

	delete[] data;

	createCollisionModel();
	return true;
}

bool Mesh::writeBin(const char* filename)
{
	assert( vertices.size() || interleaved.size() );
	std::string s_filename = filename;
	s_filename += ".bin";

	FILE* f = fopen(s_filename.c_str(),"wb");
	if (f == NULL)
	{
		std::cout << "[ERROR] cannot write mesh BIN: " << s_filename.c_str() << std::endl;
		return false;
	}

	//watermark
	fwrite("MBIN",sizeof(char),4,f);

	sMeshInfo info;
	info.size = interleaved.size() ? interleaved.size() : vertices.size();
	info.aabb_max = aabb_max;
	info.aabb_min = aabb_min;
	info.center = center;
	info.halfsize = halfsize;
	info.radius = radius;

	info.streams[0] = interleaved.size() ? 'I' : 'V';
	info.streams[1] = normals.size() ? 'N' : ' ';
	info.streams[2] = uvs.size() ? 'U' : ' ';
	info.streams[3] = colors.size() ? 'C' : ' ';

	for (unsigned int i = 0; i < 4; i++)
		info.material_range[i] = material_range.size() > i ? material_range[i] : -1;

	//write info
	fwrite((void*)&info, sizeof(sMeshInfo),1, f);

	//write streams
	if (interleaved.size())
		fwrite((void*)&interleaved[0], interleaved.size() * sizeof(tInterleaved), 1, f);
	else
	{
		fwrite((void*)&vertices[0], vertices.size() * sizeof(Vector3), 1, f);
		if (normals.size())
			fwrite((void*)&normals[0], normals.size() * sizeof(Vector3), 1, f);
		if (uvs.size())
			fwrite((void*)&uvs[0], uvs.size() * sizeof(Vector2), 1, f);
	}

	if (colors.size())
		fwrite((void*)&colors[0], colors.size() * sizeof(Vector4), 1, f);

	fclose(f);
	return true;
}

bool Mesh::loadASE(const char* filename)
{
	int nVtx,nFcs;
	int count;
	int vId,aId,bId,cId;
	float vtxX,vtxY,vtxZ;
	float nX,nY,nZ;
	TextParser t;
	if (t.create(filename) == false)
		return false;

	t.seek("*MESH_NUMVERTEX");
	nVtx = t.getint();
	t.seek("*MESH_NUMFACES");
	nFcs = t.getint();

	normals.resize(nFcs*3);
	vertices.resize(nFcs*3);
	uvs.resize(nFcs*3);

	std::vector<Vector3> unique_vertices;
	unique_vertices.resize(nVtx);

	const float max_float = 10000000;
	const float min_float = -10000000;
	aabb_min.set(max_float,max_float,max_float);
	aabb_max.set(min_float,min_float,min_float);

	//load unique vertices
	for(count=0;count<nVtx;count++)
	{
		t.seek("*MESH_VERTEX");
		vId = t.getint();
		vtxX=t.getfloat();
		vtxY=t.getfloat();
		vtxZ=t.getfloat();
		Vector3 v(-vtxX,vtxZ,vtxY);
		unique_vertices[count] = v;
		aabb_min.setMin( v );
		aabb_max.setMax( v );
	}
	center = (aabb_max + aabb_min) * 0.5;
	halfsize = (aabb_max - center) * 2;
	radius = max( aabb_max.length(), aabb_min.length() );
	
	int prev_mat = 0;

	//load faces
	for(count=0;count<nFcs;count++)
	{
		t.seek("*MESH_FACE");
		t.seek("A:");
		aId = t.getint();
		t.seek("B:");
		bId = t.getint();
		t.seek("C:");
		cId = t.getint();
		vertices[count*3 + 0] = unique_vertices[aId];
		vertices[count*3 + 1] = unique_vertices[bId];
		vertices[count*3 + 2] = unique_vertices[cId];

		t.seek("*MESH_MTLID");
		int current_mat = t.getint();
		if (current_mat != prev_mat)
		{
			material_range.push_back( count );
			prev_mat = current_mat;
		}
	}

	material_range.push_back(nFcs);

	t.seek("*MESH_NUMTVERTEX");
	nVtx = t.getint();
	std::vector<Vector2> unique_uvs;
	unique_uvs.resize(nVtx);

	for(count=0;count<nVtx;count++)
	{
		t.seek("*MESH_TVERT");
		vId = t.getint();
		vtxX=t.getfloat();
		vtxY=t.getfloat();
		unique_uvs[count]=Vector2(vtxX,vtxY);
	}

	t.seek("*MESH_NUMTVFACES");
	nFcs = t.getint();
	for(count=0;count<nFcs;count++)
	{
		t.seek("*MESH_TFACE");
		t.getint(); //num face
		uvs[count*3] = unique_uvs[ t.getint() ];
		uvs[count*3+1] = unique_uvs[ t.getint() ];
		uvs[count*3+2] = unique_uvs[ t.getint() ];
	}

	//normals
	for(count=0;count<nFcs;count++)
	{
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = t.getfloat();
		nY = t.getfloat();
		nZ = t.getfloat();
		normals[count*3]=Vector3(-nX,nZ,nY);
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = t.getfloat();
		nY = t.getfloat();
		nZ = t.getfloat();
		normals[count*3+1]=Vector3(-nX,nZ,nY);
		t.seek("*MESH_VERTEXNORMAL");
		aId = t.getint();
		nX = t.getfloat();
		nY = t.getfloat();
		nZ = t.getfloat();
		normals[count*3+2]=Vector3(-nX,nZ,nY);
	}

	createCollisionModel();

	return true;
}

bool Mesh::loadOBJ(const char* filename)
{
	struct stat stbuffer;

	FILE* f = fopen(filename,"rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename,&stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size+1];
	fread(data,size,1,f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[255];
	int i = 0;

	std::vector<Vector3> indexed_positions;
	std::vector<Vector3> indexed_normals;
	std::vector<Vector2> indexed_uvs;

	const float max_float = 10000000;
	const float min_float = -10000000;
	aabb_min.set(max_float,max_float,max_float);
	aabb_max.set(min_float,min_float,min_float);

	unsigned int vertex_i = 0;

	//parse file
	while(*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while(i < 255 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line,pos,i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

		//tokenize line
		std::vector<std::string> tokens = tokenize(line," ");

		if (tokens.empty()) continue;

		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
			indexed_positions.push_back(v);

			aabb_min.setMin( v );
			aabb_max.setMax( v );
		}
		else if (tokens[0] == "vt" && tokens.size() == 4)
		{
			Vector2 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()) );
			indexed_uvs.push_back(v);
		}
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3 v( atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()) );
			indexed_normals.push_back(v);
		}
		else if (tokens[0] == "s") //surface? it appears one time before the faces
		{
			//process mesh
			if (uvs.size() == 0 && indexed_uvs.size() )
				uvs.resize(1);
		}
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3 v1,v2,v3;
			v1.parseFromText( tokens[1].c_str(), '/' );

			for (unsigned int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2.parseFromText( tokens[iPoly].c_str(), '/' );
				v3.parseFromText( tokens[iPoly+1].c_str(), '/' );

				vertices.push_back( indexed_positions[ unsigned int(v1.x) -1 ] );
				vertices.push_back( indexed_positions[ unsigned int(v2.x) -1] );
				vertices.push_back( indexed_positions[ unsigned int(v3.x) -1] );
				//triangles.push_back( VECTOR_INDICES_TYPE(vertex_i, vertex_i+1, vertex_i+2) ); //not needed
				vertex_i += 3;

				if (indexed_uvs.size() > 0)
				{
					uvs.push_back( indexed_uvs[ unsigned int(v1.y) -1] );
					uvs.push_back( indexed_uvs[ unsigned int(v2.y) -1] );
					uvs.push_back( indexed_uvs[ unsigned int(v3.y) -1] );
				}

				if (indexed_normals.size() > 0)
				{
					normals.push_back( indexed_normals[ unsigned int(v1.z) -1] );
					normals.push_back( indexed_normals[ unsigned int(v2.z) -1] );
					normals.push_back( indexed_normals[ unsigned int(v3.z) -1] );
				}
			}
		}
	}

	center = (aabb_max + aabb_min) * 0.5;
	halfsize = (aabb_max - center) * 2;
	radius = max( aabb_max.length(), aabb_min.length() );

	material_range.push_back(vertices.size() / 3.0);

	createCollisionModel();

	return true;
}

void Mesh::createCube()
{
	const float _verts[] = { -1, 1, -1, -1, -1, +1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1, -1, +1, 1, 1, -1, 1, 1, 1, 1, -1, +1, 1, 1, -1, 1, -1, +1, 1, -1, -1, -1, 1, 1, 1, -1, 1, 1, 1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, -1, -1, -1, -1, -1, 1, -1, 1, 1, 1, 1, 1, -1, -1, 1, -1, -1, 1, 1, 1, 1, 1, -1, -1, -1, 1, -1, -1, 1, -1, 1, -1, -1, -1, 1, -1, 1, -1, -1, 1 };
	const float _uvs[] = { -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0 };
	vertices.resize(6* 2 * 3);
	uvs.resize(6 * 2 * 3);
	memcpy(&vertices[0], _verts, sizeof(Vector3) * vertices.size());
	memcpy(&uvs[0], _uvs, sizeof(Vector2) * uvs.size());
}

void Mesh::createQuad(float center_x, float center_y, float w, float h, bool flip_uvs)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y + h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x - w*0.5f, center_y - h*0.5f, 0.0f));
	vertices.push_back(Vector3(center_x + w*0.5f, center_y + h*0.5f, 0.0f));

	//texture coordinates
	uvs.push_back(Vector2(1.0f, flip_uvs ? 0.0f : 1.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(1.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 0.0f : 1.0f));
	uvs.push_back(Vector2(0.0f, flip_uvs ? 1.0f : 0.0f));
	uvs.push_back(Vector2(1.0f, flip_uvs ? 0.0f : 1.0f));

	//all of them have the same normal
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
	normals.push_back(Vector3(0.0f, 0.0f, 1.0f));
}


void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)

	vertices.push_back(Vector3(size, 0, size));
	vertices.push_back(Vector3(size, 0, -size));
	vertices.push_back(Vector3(-size, 0, -size));
	vertices.push_back(Vector3(-size, 0, size));
	vertices.push_back(Vector3(size, 0, size));
	vertices.push_back(Vector3(-size, 0, -size));

	//all of them have the same normal
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));
	normals.push_back(Vector3(0, 1, 0));

	//texture coordinates
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(1, 0));
	uvs.push_back(Vector2(0, 0));
	uvs.push_back(Vector2(0, 1));
	uvs.push_back(Vector2(1, 1));
	uvs.push_back(Vector2(0, 0));
}

void Mesh::createGrid(float dist)
{
	int num_lines = 2000;
	Vector4 color(0.5, 0.5, 0.5, 1);

	for (int i = num_lines * -0.5; i <= num_lines * 0.5; ++i)
	{
		vertices.push_back(Vector3(i*dist, 0, dist * num_lines * -0.5 ));
		vertices.push_back(Vector3(i*dist, 0, dist * num_lines * +0.5));
		vertices.push_back(Vector3(dist * num_lines * 0.5, 0, i*dist));
		vertices.push_back(Vector3(dist * num_lines * -0.5, 0, i*dist));

		Vector4 color = i % 10 == 0 ? Vector4(1, 1, 1, 1) : Vector4(0.75, 0.75, 0.75, 0.5);
		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);
		colors.push_back(color);
	}
}



void Mesh::renderAABB( const Matrix44& model )
{
	Mesh m;
	m.createCube();
	Shader* sh = Shader::getDefaultShader("flat");
	sh->setUniform("u_color", Vector4(1, 1, 0, 1));
	sh->setUniform("u_model", Camera::current->viewprojection_matrix);
	sh->setUniform("u_viewprojection", Camera::current->viewprojection_matrix );

	/*
	glPushMatrix();
		glTranslatef( center.x, center.y, center.z);
		glScalef( halfsize.x, halfsize.y, halfsize.z );
		//glutWireCube(1);
	glPopMatrix();
	*/
}



Mesh* Mesh::getQuad()
{
	static Mesh* quad = NULL;
	if (!quad)
	{
		quad = new Mesh();
		quad->createQuad(0, 0, 2, 2, false);
		quad->uploadToVRAM();
	}
	return quad;
}

Mesh* Mesh::Load(const char* filename)
{
	assert(filename);
	std::map<std::string, Mesh*>::iterator it = sMeshesLoaded.find(filename);
	if (it != sMeshesLoaded.end())
		return it->second;

	Mesh* m = new Mesh();
	m->name = filename;

	char file_format = 0;
	std::string ext = m->name.substr(m->name.size() - 4, 4);

	if (ext == ".ase" || ext == ".ASE")
		file_format = FORMAT_ASE;
	else if (ext == ".obj" || ext == ".OBJ")
		file_format = FORMAT_OBJ;
	else if (ext == ".bin" || ext == ".BIN")
		file_format = FORMAT_BIN;
	else
	{
		std::cerr << "Unknown mesh format: " << filename << std::endl;
		return NULL;
	}


	long time = getTime();
	std::cout << " + Mesh loading: " << filename << " ... ";
	std::string binfilename = filename;

	if (file_format != FORMAT_BIN)
		binfilename = binfilename + ".bin";

	if (m->readBin(binfilename.c_str()) && use_binary)
	{
		std::cout << "[VRAM] ";
		m->uploadToVRAM();

		std::cout << "[OK BIN]  Faces: " << (m->interleaved.size() ? m->interleaved.size() : m->vertices.size()) / 3 << " Time: " << (getTime() - time) * 0.001 << "sec" << std::endl;
		sMeshesLoaded[filename] = m;
		return m;
	}

	bool loaded = false;
	if (file_format == FORMAT_OBJ)
		loaded = m->loadOBJ(filename);
	else if (file_format == FORMAT_ASE)
		loaded = m->loadASE(filename);

	if (!loaded)
	{
		delete m;
		std::cout << "[ERROR]: Mesh not found" << std::endl;
		return NULL;
	}

	std::cout << "[INTERL] ";
	m->interleaveBuffers();
	std::cout << "[VRAM] ";
	m->uploadToVRAM();

	std::cout << "[OK]  Faces: " << m->vertices.size() / 3 << " Time: " << (getTime() - time) * 0.001 << "sec" << std::endl;
	if (use_binary)
	{
		std::cout << "\t\t Writing .BIN ... ";
		m->writeBin(filename);
		std::cout << "[OK]" << std::endl;
	}
	sMeshesLoaded[filename] = m;
	return m;
}

