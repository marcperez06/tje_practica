#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H
#pragma once

#include <map>
#include <string>
#include "bass.h"

class SoundManager {
	public:
		static std::map<std::string, HSAMPLE> soundsLoaded;
		static void init();
		static HSAMPLE loadSound(std::string sound, int flags = 0);
		static void reproduceSound(std::string sound, int volume = 10, int flags = 0);

};

#endif

