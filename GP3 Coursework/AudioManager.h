#pragma once
#include "pch.h"
#include "common.h"
#include "AssetManager.h"

class AudioManager
{
public:
	static AudioManager* GetInstance();
	~AudioManager();
	void LoadSound(const char* filename, const char* soundName);
	void DeleteSound(const char* soundName);
	void PlaySound(const char* soundName);
	void StopSound(const char* soundName);

	void Quit();

private:
	AudioManager();
	static AudioManager* instance;

	AssetManager* assetMan = AssetManager::GetInstance();

	struct data {
		int sourceID, bufferID;
		char* buffer;
		std::string name;
		data(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->sourceID = sI;
			this->bufferID = bI;
			this->buffer = b;
			this->name = n;
		}

	};

	std::vector<data> datas;
	ALCcontext* context;
	ALCdevice* device;
	int ConvertToInt(char* buffer, int length);
	bool IsBigEndian();
	char* LoadWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);
};
