#include "SoundManager.h"

std::map<std::string, HSAMPLE> SoundManager::soundsLoaded;

void SoundManager::init() {
	//Inicializamos BASS  (id_del_device, muestras por segundo, ...)
	BASS_Init(1, 44100, BASS_DEVICE_STEREO, 0, NULL);
}

HSAMPLE SoundManager::loadSound(std::string sound, int flags) {

	std::map<std::string, HSAMPLE>::iterator it = soundsLoaded.find(sound);
	if (it != soundsLoaded.end()) {
		return it->second;
	}

	std::string basePath = "data/sounds/";
	std::string fileSound = basePath + sound;

	//Cargamos un sample (memoria, filename, offset, length, max, flags)
	//use BASS_SAMPLE_LOOP in the last param to have a looped sound
	HSAMPLE hSample = BASS_SampleLoad(false, fileSound.c_str(), 0, 0, 3, flags);

	soundsLoaded[sound] = hSample;
	return hSample;
}

void SoundManager::reproduceSound(std::string sound, int volume, int flags) {
	SoundManager::init();
	HSAMPLE hSample = SoundManager::loadSound(sound, flags);

	//El handler para un canal
	HCHANNEL hSampleChannel;

	//Creamos un canal para el sample
	hSampleChannel = BASS_SampleGetChannel(hSample, false);
	
	BASS_ChannelSetAttribute(hSampleChannel, BASS_ATTRIB_MUSIC_VOL_CHAN, volume);

	//Lanzamos un sample
	BASS_ChannelPlay(hSampleChannel, true);

}