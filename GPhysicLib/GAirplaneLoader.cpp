#include "GAirplaneLoader.h"
#include "GPhysicIncludes.h"
#include <fstream>
#include <Windows.h>

using namespace GPhysics;
using namespace GMath;

GString GAirplaneLoader::fileName = GString("input/airplaneInput.txt");

//load from file
bool GAirplaneLoader::loadBuildingInfo(_Out_ GAirplaneBuildInfo& info)
{
	info._inertia = GMatrix3x3::Identity();
	if (!GAirplaneLoader::loadFromFile(info))
		return false;
	info._inertiaInverse = info._inertia.inverse();
	return true;
}

bool GAirplaneLoader::loadFromFile(_Out_ GAirplaneBuildInfo& info)
{
	std::ifstream fin;
	fin.open(fileName.const_data(), std::ios_base::in);

	if (fin.fail() == true)
	{
		return false;
	}
	if (!GAirplaneLoader::readData(&fin, info))
	{
		return false;
	}
	if (!GAirplaneLoader::readCoefficients(&fin, info))
	{
		return false;
	}
	fin.close();
	return true;
}

bool GAirplaneLoader::readCoefficients(std::ifstream* fin, _Out_ GAirplaneBuildInfo& info)
{
	GStringList numbers;
	std::string line;
	GString ln = GString();
	//block represent a table. For example: block == 1 means i'm loading X Force Derivatives (CXA, CXU, CXQ and so on)
	int indx = -1, block = 0, num_angles = info._coefficientsMap._size;
	while (!fin->eof())
	{
		if (line.size() == 0){
			std::getline(*fin, line);
			continue;
		}
		
		//if in the line ww have a '*' it's a comment
		if (line[0] == '*'){
			std::getline(*fin, line);
			continue;
		}
		indx = line.find_first_of("ALPHA");
		if (indx == std::string::npos)		//if this we have data to load
		{ 
			ln = line.c_str();
			ln.cleanSpaces();			//replace all spaces with one space		

			//we have to read num_angles lines
			for (int i = 0; i < num_angles; i++)
			{
				numbers = ln.split(' ');
				std::list<GString>::const_iterator itr = numbers.const_begin();
				if (block == 0)			// then STEADY STATE COEFFICIENTS	
				{					
					info._coefficientsMap._angles[i] = Degree((*itr).toFloat());
					++itr;
					int column = 0;
					for (; itr != numbers.end(); itr++)
					{
						if (column == 0)
							info._coefficientsMap._coefficients[i]._cx.cx() = (*itr).toFloat();
						else if (column == 1)
							info._coefficientsMap._coefficients[i]._cy.cy() = (*itr).toFloat();
						else if (column == 2)
							info._coefficientsMap._coefficients[i]._cz.cz() = (*itr).toFloat();
						else if (column == 3)
							info._coefficientsMap._coefficients[i]._cl.cl() = (*itr).toFloat();
						else if (column == 4)
							info._coefficientsMap._coefficients[i]._cm.cm() = (*itr).toFloat();
						else if (column == 5)
							info._coefficientsMap._coefficients[i]._cn.cn() = (*itr).toFloat();
						++column;
					}
				}
				else if (block > 0)
				{
					++itr;
					int column = 0;
					for (; itr != numbers.end(); itr++)
					{
						if (block == 1) // X	FORCE	DERIVATIVES
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cx.cxA() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cx.cxAP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cx.cxU() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cx.cxQ() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cx.cxB() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cx.cxP() = (*itr).toFloat();
							else if (column == 6)
								info._coefficientsMap._coefficients[i]._cx.cxR() = (*itr).toFloat();
						}
						else if (block == 2) // Y	FORCE	DERIVATIVES
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cy.cyB() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cy.cyBP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cy.cyP() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cy.cyR() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cy.cyA() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cy.cyQ() = (*itr).toFloat();
						}
						else if (block == 3) // Z	FORCE	DERIVATIVES
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cz.czA() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cz.czAP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cz.czU() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cz.czQ() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cz.czB() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cz.czP() = (*itr).toFloat();
							else if (column == 6)
								info._coefficientsMap._coefficients[i]._cz.czR() = (*itr).toFloat();
						}
						else if (block == 4) // ROLLING	MOMENT	DERIVATIVES	
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cl.clB() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cl.clBP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cl.clP() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cl.clR() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cl.clA() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cl.clQ() = (*itr).toFloat();
						}
						else if (block == 5) // PITCHING MOMENT DERIVATIVES
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cm.cmA() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cm.cmAP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cm.cmU() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cm.cmQ() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cm.cmB() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cm.cmP() = (*itr).toFloat();
							else if (column == 6)
								info._coefficientsMap._coefficients[i]._cm.cmR() = (*itr).toFloat();
						}
						else if (block == 6) // YAWING	MOMENT	DERIVATIVES	
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cn.cnB() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cn.cnBP() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cn.cnP() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cn.cnR() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cn.cnA() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cn.cnQ() = (*itr).toFloat();
						}
						else if (block == 7) // CONTROL FORCE DERIVATIVES	
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cx.cx_de() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cx.cx_dle() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cz.cz_de() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cz.cz_dle() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cy.cy_da() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cy.cy_dr() = (*itr).toFloat();
						}
						else if (block == 8) // CONTROL	MOMENT DERIVATIVES		
						{
							if (column == 0)
								info._coefficientsMap._coefficients[i]._cl.cl_da() = (*itr).toFloat();
							else if (column == 1)
								info._coefficientsMap._coefficients[i]._cl.cl_dr() = (*itr).toFloat();
							else if (column == 2)
								info._coefficientsMap._coefficients[i]._cm.cm_de() = (*itr).toFloat();
							else if (column == 3)
								info._coefficientsMap._coefficients[i]._cm.cm_dle() = (*itr).toFloat();
							else if (column == 4)
								info._coefficientsMap._coefficients[i]._cn.cn_da() = (*itr).toFloat();
							else if (column == 5)
								info._coefficientsMap._coefficients[i]._cn.cn_dr() = (*itr).toFloat();

						}
						++column;
					}//EO for columns					
				}
				std::getline(*fin, line);
				ln = line.c_str();
				ln.cleanSpaces();
			}//EO for each ALPHA
			++block;

		}//EO we are reading a table of data
		std::getline(*fin, line);
	}//EO while(EOF)
	return true;
}//EO readCoefficients

bool GAirplaneLoader::readData(std::ifstream* fin, _Out_ GAirplaneBuildInfo& info)
{
	std::string line;
	int indx = 0;
	int recorded = 0;
	Real Jx, Jy, Jz, Jxz, Jxy, Jyz;
	Jx = Jy = Jz = Jxz = Jxy = Jyz = 0;
	int checkEmptyFile = 0;
	std::getline(*fin, line);
	while (!fin->eof() || recorded < 15)
	{
		if (checkEmptyFile > 1000)			//I Found a case in wich EOF didn't worked for an empty file (don't know why)
			return false;
		if (line.size() == 0){
			++checkEmptyFile;
			std::getline(*fin, line);
			continue;
		}
		//if the line starts with '*' it's a comment
		if (line[0] == '*'){
			std::getline(*fin, line);
			continue;
		}
		indx = line.find(';');
		if (indx <= 0){
			std::getline(*fin, line);
			continue;
		}
		if (recorded == 0){			
			info._mass = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 1){
			info._wingSpan = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 2){
			info._surface = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 3){
			info._chord = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 4){
			info._max_power = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 5){
			Jx = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 6){
			Jy = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 7){
			Jz = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 8){
			Jxz = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 9){
			Jxy = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 10){
			Jyz = std::stof(line.substr(0, indx).c_str());
			++recorded;
		}
		else if (recorded == 11){
			info._deflection_limit_elevator = Degree( std::stof(line.substr(0, indx).c_str()) );
			++recorded;
		}
		else if (recorded == 12){
			info._deflection_limit_aileron = Degree(std::stof(line.substr(0, indx).c_str()) );
			++recorded;
		}
		else if (recorded == 13){
			info._deflection_limit_rudder = Degree( std::stof(line.substr(0, indx).c_str()) );
			++recorded;
		}
		else if (recorded == 14){
			int size = std::atoi(line.substr(0, indx).c_str());
			if (size <= 0)
				return false;
			info._coefficientsMap = GAereodynamic::GCoefficientsMap(size);
			++recorded;
			std::getline(*fin, line);
			break;
		}
		std::getline(*fin, line);
	}//EO while recorded < 15

	info._inertia(1, 1) = Jx;
	info._inertia(2, 2) = Jy;
	info._inertia(3, 3) = Jz;

	info._inertia(1, 3) = Jxz;
	info._inertia(3, 1) = -Jxz;

	info._inertia(1, 2) = Jxy;
	info._inertia(2, 1) = -Jxy;

	info._inertia(2, 3) = Jyz;
	info._inertia(3, 2) = -Jyz;

	return true;
}//EO readData


/*
bool GAirplaneLoader::loadBuildingInfo(_Out_ GAirplaneBuildInfo& info)
{
	//assign values
	info._max_thrust = max_thrust;
	info._surface = surface;
	info._wingSpan = wingSpan;
	info._chord = chord;
	info._mass = mass;
	info._inertia = GMatrix3x3::Identity();
	Real Jx = 12875;						//x body cel'ho su asse z
	Real Jy = 75674;						// y body cel'ho su asse x
	Real Jz = 85552;						//z body cel'ho su asse y
	Real Jxz = 1331;
	info._inertia(1, 1) = Jy;
	info._inertia(2, 2) = Jz;
	info._inertia(3, 3) = Jx;
	info._inertia(2, 3) = -Jxz;
	info._inertia(3, 2) = Jxz;
	info._inertiaInverse = info._inertia.inverse();
	info._deflection_limit_aileron = info._deflection_limit_elevator 
									= info._deflection_limit_rudder = DEFLECTION_LIMIT;
	uint numAngles = 2;
	info._coefficientsMap = GCoefficientsMap(numAngles);
	
	info._coefficientsMap._angles[0] = Degree(-20.0f);
	info._coefficientsMap._angles[1] = Degree(25.0f);

	info._coefficientsMap._coefficients[0].cx().cx() = -0.09f;
	info._coefficientsMap._coefficients[1].cx().cx() = 0.13f;

	info._coefficientsMap._coefficients[0].cz().cz() = 1.116f;
	info._coefficientsMap._coefficients[1].cz().cz() = -1.658f;

	//info._coefficientsMap._coefficients[0].cl().clB() = -0.1f;
	info._coefficientsMap._coefficients[0].cl().cl_da() = -0.6f;
	//info._coefficientsMap._coefficients[0].cl().cl_dr() = 0.038f;
	//info._coefficientsMap._coefficients[1].cl().clB() = -0.2f;
	info._coefficientsMap._coefficients[1].cl().cl_da() = -0.10f;
	//info._coefficientsMap._coefficients[1].cl().cl_dr() = 0.002f;

	info._coefficientsMap._coefficients[0].cm().cm() = 0.0f;// 0.127f;
	info._coefficientsMap._coefficients[0].cm().cm_de() = -0.48f;
	info._coefficientsMap._coefficients[1].cm().cm() = 0.0f; // -0.01f;
	info._coefficientsMap._coefficients[1].cm().cm_de() = -0.87f;

	//info._coefficientsMap._coefficients[0].cn().cnB() = -0.01f;
	//info._coefficientsMap._coefficients[0].cn().cn_da() = -0.0255f;
	info._coefficientsMap._coefficients[0].cn().cn_dr() = -0.1f;
	//info._coefficientsMap._coefficients[1].cn().cnB() = -0.08f;
	//info._coefficientsMap._coefficients[1].cn().cn_da() = 0.012f;
	info._coefficientsMap._coefficients[1].cn().cn_dr() = -0.09f;
}
*/