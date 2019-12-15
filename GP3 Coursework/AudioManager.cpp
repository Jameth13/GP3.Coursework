#include "AudioManager.h"

AudioManager* AudioManager::instance = nullptr;

AudioManager* AudioManager::GetInstance()
{
	//Return instance, create an instance if we don't already have one.
	if (instance == nullptr)
	{
		instance = new AudioManager();
	}

	return instance;
}

AudioManager::AudioManager()
{
	device = alcOpenDevice(NULL); //open sound card
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
	}

	alcMakeContextCurrent(context);
}

AudioManager::~AudioManager() {}

void AudioManager::LoadSound(const char* filename, const char* soundName)
{
	bool found = false;
	unsigned int sourceID, bufferID;
	char* soundData = NULL;
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		if (datas[i].name == filename && datas[i].bufferID != -1)
		{
			bufferID = datas[i].bufferID;
			found = true;
			break;
		}
	}
	if (!found)
	{
		int channel, sampleRate, bps, size;
		soundData = LoadWAV(filename, channel, sampleRate, bps, size);
		unsigned int format;
		alGenBuffers(1, &bufferID);

		if (channel == 1)
		{
			if (bps == 8)
				format = AL_FORMAT_MONO8;
			else
				format = AL_FORMAT_MONO16;
		}
		else
		{
			if (bps == 8)
				format = AL_FORMAT_STEREO8;
			else
				format = AL_FORMAT_STEREO16;
		}

		alBufferData(bufferID, format, soundData, size, sampleRate);
	}
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceID, (!found ? bufferID : -1), soundData, filename));

	assetMan->sounds[soundName] = sourceID;
}

char* AudioManager::LoadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4); //WAV
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4); //16 bit
	in.read(buffer, 2); //1
	in.read(buffer, 2);
	chan = ConvertToInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = ConvertToInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = ConvertToInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = ConvertToInt(buffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

int AudioManager::ConvertToInt(char* buffer, int length)
{
	int a = 0;
	if (!IsBigEndian())
		for (int i = 0; i < length; i++)
			((char*)& a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)& a)[3 - i] = buffer[i];
	return a;
}

bool AudioManager::IsBigEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}


void AudioManager::DeleteSound(const char* soundName)
{}

void AudioManager::PlaySound(const char* soundName)
{
	alSourcePlay(assetMan->sounds[soundName]);
}
void AudioManager::StopSound(const char* soundName)
{
	alSourceStop(assetMan->sounds[soundName]);
}

void AudioManager::Quit()
{
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)& datas[i].sourceID);
		if (datas[i].bufferID != -1)
		{
			alDeleteBuffers(1, (unsigned int*)& datas[i].bufferID);
			delete[] datas[i].buffer;
		}
	}
	alcDestroyContext(context);
	alcCloseDevice(device);

	instance = nullptr;
	delete this;
}
