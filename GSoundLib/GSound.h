#pragma once
#ifndef GSOUND_H
#define GSOUND_H

#include "GSoundIncludes.h"


//--------------------------------------------------------------------------------------
//Class GSound: manage the 2D and 3D sound system
//--------------------------------------------------------------------------------------
class GSound
{
private:

	//The WaveHeaderType structure is for reading in the header to determine the required information for loading in the .wav audio data.
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	GSound(void);
	GSound(const GSound& copy)					{}
	virtual ~GSound(void)						{}

	bool initialize(HWND hWnd);
	void shutDown();

	bool playWaveFile();

private:
	bool initializeDirectSound(HWND hWnd);
	void shutDownDirectSound();

	// handles loading in a .wav audio file and then copies the data onto a new secondary buffer
	bool loadWaveFile(char* fileName,IDirectSoundBuffer8** buffer);
	void shutdownWaveFile(IDirectSoundBuffer8**);

	IDirectSound8*			_pDirectSound;
	IDirectSoundBuffer*		_pPrimaryBuffer;
	IDirectSoundBuffer8*	_pSecondaryBuffer;

};

#endif		//GSOUND_H

