#ifndef PATHS_H
#define PATHS_H

#include <string>

class Paths {
	public:
		// Carpetas generales
		static const std::string data;
		static const std::string meshASE;
		static const std::string meshObj;
		static const std::string meshBin;
		static const std::string shaders;
		static const std::string textures;
		static const std::string recursos;

		// Carpetas que contienen las meshes
		static const std::string wildcat;
		static const std::string island;


};

const std::string Paths::data = "data/";
const std::string Paths::meshASE = Paths::data + "mesh_ASE/";
const std::string Paths::meshObj = Paths::data + "mesh_obj/";
const std::string Paths::meshBin = Paths::data + "mesh_bin/";
const std::string Paths::shaders = Paths::data + "shaders/";
const std::string Paths::textures = Paths::data + "textures/";
const std::string Paths::recursos = Paths::data + "recursos_javi_agenjo/";
const std::string Paths::wildcat = Paths::recursos + "wildcat/";
const std::string Paths::island = Paths::recursos + "island/";

#endif