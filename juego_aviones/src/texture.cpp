#include "texture.h"
#include "utils.h"

#include <iostream> //to output
#include <cmath>

#include "mesh.h"
#include "shader.h"
#include "extra/picopng.h"
#include <cassert>

std::map<std::string, Texture*> Texture::sTexturesLoaded;

Texture::Texture()
{
	width = 0;
	height = 0;
	texture_id = 0;
	mipmaps = false;
	format = 0;
	type = 0;
	info.width = info.height = info.bpp = 0;
	info.BGR = false;
	info.data = NULL;
}

Texture::Texture(unsigned int width, unsigned int height, int format, int type, bool mipmaps, Uint8* data, int data_format)
{
	texture_id = 0;
	create(width, height, format, type, mipmaps, data, data_format);
}

Texture::~Texture()
{
	if (info.data)
		delete []info.data;
	glDeleteTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::create(unsigned int width, unsigned int height, int format, int type, bool mipmaps, Uint8* data, int data_format)
{
	assert(width && height && "texture must have a size");

	this->width = width;
	this->height = height;
	this->format = format;
	this->type = type;

	glGenTextures(1, &texture_id); //we need to create an unique ID for the texture
	glBindTexture(GL_TEXTURE_2D, texture_id);	//we activate this id to tell opengl we are going to use this texture

	glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, data_format == -1 ? format : data_format, type, data );

	this->mipmaps = mipmaps && isPowerOfTwo(width) && isPowerOfTwo(height) && format != GL_DEPTH_COMPONENT;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//set the min filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);   //set the mag filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mipmaps ? GL_REPEAT : GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mipmaps ? GL_REPEAT : GL_CLAMP_TO_EDGE);

	if(data && this->mipmaps)
		generateMipmaps(); //glGenerateMipmapEXT(GL_TEXTURE_2D); 

	glBindTexture(GL_TEXTURE_2D, 0);
	assert(glGetError() == GL_NO_ERROR && "Error creating texture");
}


bool Texture::load( const char* filename, bool mipmaps, bool upload_to_vram )
{
	std::string str = filename;
	std::string ext = str.substr( str.size() - 4,4 );
	ImageInfo* info = NULL;
	long time = getTime();

	std::cout << " + Texture loading: " << filename << " ... ";

	if (ext == ".tga" || ext == ".TGA")
		info = loadTGA(filename);
	else if (ext == ".png" || ext == ".PNG")
		info = loadPNG(filename);
	else
	{
		std::cout << "[ERROR]: unsupported format" << std::endl;
		return false; //unsupported file type
	}

	if (info == NULL) //file not found
	{
		std::cout << " [ERROR]: Texture not found " << std::endl;
		return false;
	}

	this->filename = filename;

	if (!upload_to_vram) //used to load images without uploading them
	{
		this->info = *info;
		delete info;
		return true;
	}

	//How to store a texture in VRAM
	glGenTextures(1, &texture_id); //we need to create an unique ID for the texture
	glBindTexture(GL_TEXTURE_2D, texture_id);	//we activate this id to tell opengl we are going to use this texture

	this->mipmaps = mipmaps && isPowerOfTwo(width) && isPowerOfTwo(height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );	//set the min filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->mipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR); //set the mag filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->mipmaps ? GL_REPEAT : GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->mipmaps ? GL_REPEAT : GL_CLAMP_TO_EDGE );
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4); //better quality but takes more resources

	type = GL_UNSIGNED_BYTE;
	format = (info->bpp == 24 ? GL_RGB : GL_RGBA);
	width = (float)info->width;
	height = (float)info->height;
	this->info = *info;
	this->info.data = NULL; //data is not keep in RAM

	if(info->BGR)
		glTexImage2D(GL_TEXTURE_2D, 0, format, info->width, info->height, 0, (info->bpp == 24 ? GL_BGR : GL_BGRA), type, info->data); //upload without mipmaps
	else
		glTexImage2D(GL_TEXTURE_2D, 0, format, info->width, info->height, 0, (info->bpp == 24 ? GL_RGB : GL_RGBA), type, info->data); //upload without mipmaps

	generateMipmaps(); 

	delete info->data;
	delete info;

	std::cout << "[OK] Size: " << width << "x" << height << " Time: " << (getTime() - time) * 0.001 << "sec" << std::endl;

	return true;
}

void Texture::bind()
{
	glEnable( GL_TEXTURE_2D ); //enable the textures 
	glBindTexture( GL_TEXTURE_2D, texture_id );	//enable the id of the texture we are going to use
}

void Texture::unbind()
{
	glDisable( GL_TEXTURE_2D ); //disable the textures 
	glBindTexture( GL_TEXTURE_2D, 0 );	//disable the id of the texture we are going to use
}

void Texture::UnbindAll()
{
	glDisable( GL_TEXTURE_CUBE_MAP );
	glDisable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void Texture::generateMipmaps()
{
	if(!glGenerateMipmapEXT)
		return;

	glBindTexture( GL_TEXTURE_2D, texture_id );	//enable the id of the texture we are going to use
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR ); //set the mag filter
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}


void Texture::toViewport(Shader* shader)
{
	Mesh* quad = Mesh::getQuad();
	if (!shader)
		shader = Shader::getDefaultShader("screen");
	shader->enable();
	shader->setUniform("u_texture", this);
	quad->render(GL_TRIANGLES, shader);
}


//TGA format from: http://www.paulbourke.net/dataformats/tga/
Texture::ImageInfo* Texture::loadTGA(const char* filename)
{
    GLubyte TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    GLubyte TGAcompare[12];
    GLubyte header[6];
    GLuint bytesPerPixel;
    GLuint imageSize;
    //GLuint type = GL_RGBA;

    FILE * file = fopen(filename, "rb");
    
    if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
        memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
        fread(header, 1, sizeof(header), file) != sizeof(header))
    {
        if (file == NULL)
            return NULL;
        else
        {
            fclose(file);
            return NULL;
        }
    }

	ImageInfo* tgainfo = new ImageInfo;
    
    tgainfo->width = header[1] * 256 + header[0];
    tgainfo->height = header[3] * 256 + header[2];
	tgainfo->bpp = header[4];

	bool error = false;

	if (tgainfo->bpp != 24 && tgainfo->bpp != 32)
	{
		error = true;
		std::cerr << "File format not supported: " << tgainfo->bpp << "bpps" << std::endl;
	}
    
    if (tgainfo->width <= 0 || tgainfo->height <= 0)
	{
		error = true;
		std::cerr << "Wrong texture size: " << tgainfo->width << "x" << tgainfo->height << " pixels" << std::endl;
	}

	if(error)
    {
        fclose(file);
		delete tgainfo;
        return NULL;
    }

    bytesPerPixel = tgainfo->bpp / 8;
    imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
    tgainfo->data = (GLubyte*)malloc(imageSize);
    
    if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
    {
        if (tgainfo->data != NULL)
            free(tgainfo->data);
            
        fclose(file);
		delete tgainfo;
        return NULL;
    }

	if (header[5] & (1 << 4)) //flip
		tgainfo->origin_topleft = true;

	tgainfo->BGR = true;
    
	/* swap?
    for (GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
    {
        temp = tgainfo->data[i];
        tgainfo->data[i] = tgainfo->data[i + 2];
        tgainfo->data[i + 2] = temp;
    }
	//*/
    
    fclose(file);
	return tgainfo;
}

#include <iostream>
#include <fstream>

Texture::ImageInfo* Texture::loadPNG(const char* filename)
{
	std::ifstream file( filename, std::ios::in | std::ios::binary | std::ios::ate);

	//get filesize
	std::streamsize size = 0;
	if (file.seekg(0, std::ios::end).good()) size = file.tellg();
	if (file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	if (!size)
		return NULL;

	std::vector<unsigned char> buffer;

	//read contents of the file into the vector
	if (size > 0)
	{
		buffer.resize((size_t)size);
		file.read((char*)(&buffer[0]), size);
	}
	else 
		buffer.clear();

	ImageInfo* info = new ImageInfo;
	std::vector<unsigned char> out_image;

	if (decodePNG( out_image, info->width, info->height, buffer.empty() ? 0 : &buffer[0], (unsigned long)buffer.size(), true) != 0)
	{
		delete info;
		return NULL;
	}

	info->data = new Uint8[ out_image.size() ];
	memcpy( info->data, &out_image[0], out_image.size() );
	info->BGR = false;
	info->bpp = 32;

	return info;
}

bool isPowerOfTwo( int n )
{
	return (n & (n - 1)) == 0;
}

Texture* Texture::Load(const char * filename) {
	assert(filename);
	std::map<std::string, Texture*>::iterator it = sTexturesLoaded.find(filename);
	if (it != sTexturesLoaded.end())
		return it->second;

	Texture* t = new Texture();
	t->filename = filename;

	char file_format = 0;
	std::string ext = t->filename.substr(t->filename.size() - 4, 4);

	if ((ext != ".tga") || (ext != ".png")) {
		std::cerr << "Unknown mesh format: " << filename << std::endl;
		return NULL;
	}

	long time = getTime();
	std::cout << " + Texture loading: " << filename << " ... ";
	std::string binfilename = filename;

	bool loaded = t->load(filename);

	if (!loaded)
	{
		delete t;
		std::cout << "[ERROR]: Mesh not found" << std::endl;
		return NULL;
	}

	sTexturesLoaded[filename] = t;
	return t;
}