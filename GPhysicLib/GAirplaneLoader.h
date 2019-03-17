#pragma once
#ifndef GAIRPLANE_LOADER_H
#define GAIRPLANE_LOADER_H

#include "GAirplaneBuildInfo.h"

namespace GPhysics
{
	class GAirplaneLoader
	{
	public:
		static GString fileName;
		GAirplaneLoader()						{}
		~GAirplaneLoader()						{}

		static bool loadBuildingInfo(_Out_ GAirplaneBuildInfo& buildingInfo);
	private:
		static bool loadFromFile(_Out_ GAirplaneBuildInfo& buildingInfo);
		static bool readData(std::ifstream* fin, _Out_ GAirplaneBuildInfo& buildingInfo);
		static bool readCoefficients(std::ifstream* fin, _Out_ GAirplaneBuildInfo& buildingInfo);
	private:
		//prevent copy
		GAirplaneLoader(const GAirplaneLoader&);
		GAirplaneLoader& operator=(const GAirplaneLoader&);
	};
}

#endif		//GAIRPLANE_LOADER_H