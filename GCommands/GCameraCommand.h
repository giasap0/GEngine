#pragma once
#ifndef GCAMERACOMMAND_H
#define GCAMERACOMMAND_H

class GCameraCommand
{
public:
	GCameraCommand()				{}
	virtual ~GCameraCommand()		{}

	bool operator == (const GCameraCommand& other) const;

	inline void resetValues()									 { cameraHorizontal = cameraVertical = .0f; }

	float cameraHorizontal, cameraVertical;
};

#endif	//GCAMERACOMMAND_H
