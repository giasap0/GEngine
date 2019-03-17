#pragma once
#ifndef GAIRPLANE_H
#define GAIRPLANE_H

#include "Engine.h"
#include "GAirplaneBuildInfo.h"

using GPhysics::GAereodynamic::Coefficients;

namespace GPhysics
{
	struct CommandState
	{
		CommandState() : roll(0), yaw(0), pitch(0)
		{}
		inline CommandState& operator=(const CommandState& c)
		{
			roll = c.roll; yaw = c.yaw; pitch = c.pitch; trimElevator = c.trimElevator;
			return *this;
		}
		inline Radians da()				{ return roll; }
		inline Radians dru()			{ return yaw; }
		inline Radians de()				{ return pitch; }

		Radians roll, yaw, pitch;
		Radians trimElevator;
		Real throttle;
	};
	struct GAirplaneStatus
	{
		GAirplaneStatus() : v(0, 0, 0), w(0, 0, 0), a(0, 0, 0), w_dot(0, 0, 0), aroundX(0), aroundY(0), aroundZ(0)
		{
			position = GMatrix4x4::Identity();
		}
		
		inline GAirplaneStatus& operator = (const GAirplaneStatus& p)
		{
			position = p.position;
			v = p.v; w = p.w;
			a = p.a; w_dot = p.w_dot;
			lift = p.lift; drag = p.drag; yAereodyn = p.yAereodyn; thrust = p.thrust; weight = p.weight;
			mx = p.mx; my = p.my; mz = p.mz;
			aroundX = p.aroundX; aroundY = p.aroundY; aroundZ = p.aroundZ;
			alfa = p.alfa; beta = p.beta;
			commands = p.commands;
			return *this;
		}
		
		//position / orientation
		GMatrix4x4 position;
		//speed
		GVector3 v;
		//angular speed
		GVector3 w;
		//acceleration
		GVector3 a;
		//angular acceleration
		GVector3 w_dot;

		//yAereodyn is the lateral-aereodynamic force.
		GVector3 lift, drag, yAereodyn;
		GVector3 thrust, weight;
		GVector3 mx, my, mz;
		GMath::Radians aroundX, aroundY, aroundZ;
		GMath::Radians alfa, beta;
		Real alfaRate;
		CommandState commands;
	};

	class GAirplane
	{
	public:
		GAirplane()
		{
			_versorUP = GVector3(.0f, +1.0f, .0f);
			_versorL = GVector3(.0f, +1.0f, .0f);
			_versorWings = GVector3(-1.0f, .0f, .0f); 
			_versorV = GVector3(0, 0, 0);
			_vNorm = .0f;
		}
		virtual ~GAirplane()												{}

		inline GAirplaneStatus& planeStatus()									{ return _airplaneStatus; }
		inline const GAirplaneStatus& planeStatus() const						{ return _airplaneStatus; }

		void initialize(const GAirplaneBuildInfo& buildingInfo);
		void updateState(Real dt, const GPlaneCommand& planeCommand);
	private:
		void readCommands(Real dt, const GPlaneCommand& planeCommand);

		Real cx(const Coefficients& c) const;		//drag
		Real cy(const Coefficients& c) const;		//y-wind
		Real cz(const Coefficients& c) const;		//lift
		Real cL(const Coefficients& c) const;		//roll
		Real cM(const Coefficients& c) const;		//pitch
		Real cN(const Coefficients& c) const;		//yaw

	private:
		GAirplaneBuildInfo _buildInfo;
		GAirplaneStatus _airplaneStatus;
		Engine _engine;

		//versor Lift
		GVector3 _versorL;
		//versor speed
		GVector3 _versorV;
		//versor wing
		GVector3 _versorWings;
		//pilot's foot-head
		GVector3 _versorUP;

		//variables I need to optimize calculations
		Real _vNorm, _wSpan_over_2v, _chord_over_2v;
		Radians _oldAlfa;
	private:
		//prevent copy
		GAirplane(const GAirplane&);
		GAirplane& operator=(const GAirplane&);
	};
}

#endif		//GAIRPLANE_H
