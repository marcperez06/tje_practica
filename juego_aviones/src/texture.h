/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This contains a texture wrapper to use textures. It allow to load TGAs.
*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "includes.h"
#include <map>
#include <string>

class Shader;

// TEXTURE CLASS
class Texture
{
	//a general struct to store all the information about a TGA file
	typedef struct ImageInfo 
	{
		unsigned int width;
		unsigned int height;
		unsigned int bpp; //bits per pixel
		bool origin_topleft;
		Uint8* data; //bytes with the pixel information
		bool BGR; //colors encoded as BGR instead of RGB (TGA work like this)
	} TGAInfo;

public:
	static std::map<std::string, Texture*> Texture::sTexturesLoaded;
	GLuint texture_id; // GL id to identify the texture in opengl, every texture must have its own id
	float width;
	float height;
	std::string filename;

	unsigned int format;
	unsigned int type;
	bool mipmaps;

	ImageInfo info;

	Texture();
	Texture(unsigned int width, unsigned int height, int format = GL_RGB, int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL, int data_format = -1);
	~Texture();

	void create(unsigned int width, unsigned int height, int format = GL_RGB, int type = GL_UNSIGNED_BYTE, bool mipmaps = true, Uint8* data = NULL, int data_format = -1);

	void bind();
	void unbind();
	static void UnbindAll();

	bool load(const char* filename, bool mipmaps = true, bool upload_to_vram = true);

	void generateMipmaps();

	void toViewport( Shader* shader = NULL );

	static Texture* Load(const char* filename);

protected:
	ImageInfo* loadTGA(const char* filename);
	ImageInfo* loadPNG(const char* filename);
};

bool isPowerOfTwo(int n);

#endif