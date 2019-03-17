#include "GSound.h"

GSound::GSound() : _pDirectSound(nullptr), _pPrimaryBuffer(nullptr), _pSecondaryBuffer(nullptr)
{
}

void GSound::shutDown()
{
	shutdownWaveFile(&_pSecondaryBuffer);
	shutDownDirectSound();
}

bool GSound::initialize(HWND hWnd)
{
	bool result = initializeDirectSound(hWnd);
	if(!result)				return false;

	result = loadWaveFile("sounds/sound01.wav",&_pSecondaryBuffer);
	if(!result)				return false;

	return true;
}

//Note that you can query the system for all the sound devices and then grab the pointer to the primary sound buffer for a specific device,
bool GSound::initializeDirectSound(HWND hWnd)
{
	HRESULT result = DirectSoundCreate8(nullptr,&_pDirectSound,nullptr);
	if( FAILED (result) )
	{
		DBG_OUTPT("GSound::initializaDirectSound error creating direct sound instance");
		DBG_OUTPT("error HRESULT = " << result);
		return false;
	}
	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = _pDirectSound->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
	if ( FAILED(result) )		return false;

	//set up the description of how to access the primary buffer
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	result = _pDirectSound->CreateSoundBuffer(&bufferDesc,&_pPrimaryBuffer,nullptr);
	if ( FAILED(result) )
	{
		DBG_OUTPT("GSound::initializaDirectSound error creating primary buffer on the default sound device");
		DBG_OUTPT("error HRESULT = " << result);
		return false;
	}

	// Setup the format of the primary sound bufffer.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;
	// Set the primary buffer to be the wave format specified.
	result = _pPrimaryBuffer->SetFormat(&waveFormat);
	if ( FAILED(result) )
	{
		DBG_OUTPT("GSound::initializaDirectSound cannot set the  primary buffer to the sound format specified");
		DBG_OUTPT("error HRESULT = " << result);
		return false;
	}

	return true;
}

void GSound::shutDownDirectSound()
{
	if(_pPrimaryBuffer != nullptr)
	{
		_pPrimaryBuffer->Release();
		_pPrimaryBuffer = nullptr;
	}

	if(_pDirectSound != nullptr)
	{
		_pDirectSound->Release();
		_pDirectSound = nullptr;
	}
	return;
}

bool GSound::loadWaveFile(char* fileName, IDirectSoundBuffer8** secondaryBuffer)
{
	FILE* filePtr;
	int error = fopen_s(&filePtr,fileName,"rb");
	if(error != 0 )
	{
		DBG_OUTPT("GSound::loadWaveFile cannot load file: " << fileName);
		DBG_OUTPT("fopen_s(...) error n " << error);
		return false;
	}

	// Read the wave file header
	WaveHeaderType waveFileHeader;
	unsigned int count = fread(&waveFileHeader, sizeof(waveFileHeader),1,filePtr);
	if(count != 1)
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
	   (waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}
 
	// Check that the file format is the WAVE format.
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
	   (waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}
 
	// Check that the sub chunk ID is the fmt format.
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
	   (waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}
 
	// Check that the audio format is WAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}
 
	// Check that the wave file was recorded in stereo format.
	if(waveFileHeader.numChannels != 2)
	{
		return false;
	}
 
	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}
 
	// Ensure that the wave file was recorded in 16 bit format.
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}
 
	// Check for the data chunk header.
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
	   (waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	IDirectSoundBuffer* tempBuffer;
	HRESULT result = _pDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if(FAILED(result))						{return false;}
 
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if(FAILED(result))						{return false;}
 
	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = nullptr;

	//******************************************************************************************
	//Now that the secondary buffer is ready we can load in the wave data from the audio file.
	//******************************************************************************************

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if(waveData == nullptr)
	{
		DBG_OUTPT("GSound::LoadWaveFile cannot allocate memory for temporary buffer");
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)				{return false;}
 
	// Close the file once done reading.
	error = fclose(filePtr);
	if(error != 0)										{return false;}
 
	// Lock the secondary buffer to write wave data into it.
	unsigned char *bufferPtr;
	unsigned long bufferSize;
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if(FAILED(result))									
	{
		DBG_OUTPT("GSound::LoadWaveFile cannot lock secondary buffer. error HRESULT = " << result);
		return false;
	}
 
	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if(FAILED(result))
	{
		DBG_OUTPT("GSound::LoadWaveFile cannot unlock secondary buffer. error HRESULT = " << result);
		return false;
	}
 
	// Release the wave data since it was copied into the secondary buffer.
	delete [] waveData;
	waveData = nullptr;
 
	DBG_OUTPT("loaded file: " << fileName);
	return true;
}

void GSound::shutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if(*secondaryBuffer!=nullptr)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

/*
The PlayWaveFile function will play the audio file stored in the secondary buffer.
The moment you use the Play function it will automatically mix the audio into the primary buffer and start it playing if it wasn't already.
Also note that we set the position to start playing at the beginning of the secondary sound buffer otherwise it will continue from where it last stopped playing.
*/
bool GSound::playWaveFile()
{
	HRESULT result = _pSecondaryBuffer->SetCurrentPosition(0);
	if(FAILED(result))						{return false;}

	result = _pSecondaryBuffer->SetVolume(DSBVOLUME_MAX);
	if(FAILED(result))						{return false;}

	result = _pSecondaryBuffer->Play(0,0,0);
	if(FAILED(result))
	{
		DBG_OUTPT("GSound::playWaveFile cannot play secondary buffer. error HRESULT = " << result);
		return false;
	}

	return true;
}


