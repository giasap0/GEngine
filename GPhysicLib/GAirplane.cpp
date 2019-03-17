#include "GAirplane.h"
#include "GPhysicConstants.h"
#include <Windows.h>

using namespace GPhysics;

void GAirplane::initialize(const GAirplaneBuildInfo& buildingInfo)
{
	_vNorm = _wSpan_over_2v = _chord_over_2v = .0f;
	_versorUP = GVector3(.0f, .0f, +1.0f);
	_versorL = GVector3(.0f, .0f, +1.0f);
	_versorWings = GVector3(.0f, +1.0f, .0f);
	_versorV = GVector3(.0f, .0f, .0f);
	_engine.max_power() = buildingInfo.max_power();
	DBG_OUTPT("max power == " << _engine.max_power() );
	_buildInfo = buildingInfo;
	_oldAlfa = Radians ( .0f );
}

//		DBG_OUTPT("cl0 == " << _cL0);

void GAirplane::readCommands(Real dt, const GPlaneCommand& planeCommand)
{
	static const Real omegaCommands = GMath::Constants::pi_double / 120.0f;
	static const Real omegaTrim = GMath::Constants::pi_double / 3600.0f;
	static Real elevatorDeflection = .0f;

	_engine.throttle() += planeCommand.throttle() * 0.2f*dt;
	_airplaneStatus.commands.throttle = _engine.throttle();

	_airplaneStatus.commands.trimElevator += Radians( (planeCommand.trimElevator()*omegaTrim*dt) / _buildInfo.deflection_limit_elevator().toReal() );

	if (planeCommand.isAnalogicCommand())
	{
		_airplaneStatus.commands.pitch = _airplaneStatus.commands.trimElevator + (_buildInfo.deflection_limit_elevator() * planeCommand.pitch());
		clamp<Radians>(_airplaneStatus.commands.pitch, -_buildInfo.deflection_limit_elevator(), _buildInfo.deflection_limit_elevator());

		_airplaneStatus.commands.yaw = _buildInfo.deflection_limit_rudder() * planeCommand.yaw();
		_airplaneStatus.commands.roll = _buildInfo.deflection_limit_aileron() * planeCommand.roll();
	}
	else
	{
		if (isZero(planeCommand.pitch())){
			_airplaneStatus.commands.pitch = _airplaneStatus.commands.trimElevator;
			elevatorDeflection = .0f;
		}
		else
		{
			elevatorDeflection += planeCommand.pitch() *omegaCommands*dt;
			_airplaneStatus.commands.pitch = _airplaneStatus.commands.trimElevator + elevatorDeflection;
			clamp<Radians>(_airplaneStatus.commands.pitch, -_buildInfo.deflection_limit_elevator(), _buildInfo.deflection_limit_elevator());
		}
		if (isZero(planeCommand.yaw()))
			_airplaneStatus.commands.yaw = .0f;
		else
		{
			_airplaneStatus.commands.yaw += planeCommand.yaw() *omegaCommands*dt;
			clamp<Radians>(_airplaneStatus.commands.yaw, -_buildInfo.deflection_limit_rudder(), _buildInfo.deflection_limit_rudder());
		}
		if (isZero(planeCommand.roll()))
			_airplaneStatus.commands.roll = .0f;
		else
		{
			_airplaneStatus.commands.roll += planeCommand.roll() *omegaCommands*dt;
			clamp<Radians>(_airplaneStatus.commands.roll, -_buildInfo.deflection_limit_aileron(), _buildInfo.deflection_limit_aileron());
		}
	}
	/*
	DBG_OUTPT("######################################################################################### ");
	DBG_OUTPT("comando crudo trim == " << planeCommand.trimElevator() );
	DBG_OUTPT("comando ridimensionato trim == " << planeCommand.trimElevator()*omegaTrim*dt);
	DBG_OUTPT("limite elevatore == " << Degree(_buildInfo.deflection_limit_elevator()).toReal());
	DBG_OUTPT("comando elevatore == " << Degree( _airplaneStatus.commands.pitch).toReal() );
	*/

}//EO readCommands

void GAirplane::updateState(Real dt, const GPlaneCommand& planeCommand)
{
	readCommands(dt, planeCommand);
	
	GVector3 position = _airplaneStatus.position.translationVector();
	Real altitude = position.z();
	//body axes
	GVector3 versor_bodyX = _versorUP ^ _versorWings * -1.0f;
	GPlane bodyZX = GPlane(-_versorWings, GPoint(position));
	GPlane bodyXY = GPlane(_versorUP, GPoint(position));

	_versorV = _airplaneStatus.v.normalized();

	GVector3 relativeV = bodyZX.projectVector(_versorV);

	if (isZero(relativeV.normSquare() ))
		_airplaneStatus.alfa = _airplaneStatus.aroundY;
	else
		_airplaneStatus.alfa = versor_bodyX.angleRadians(relativeV);
	if (relativeV.normalized().z() > versor_bodyX.z() )
		_airplaneStatus.alfa *= -1.0f;


	if ( isZero( _vNorm ) )
		_airplaneStatus.beta = .0f;
	else
	{
		relativeV = bodyXY.projectVector(_versorV);
		_airplaneStatus.beta = GMath::arcSin(relativeV.y() / relativeV.norm()) *-1.0f;
	}
	
	//calcualte alfa rate
	if( !isZero ( dt ) )
	{
		_airplaneStatus.alfaRate = ( _airplaneStatus.alfa - _oldAlfa ).toReal() / dt;
	}
	_oldAlfa = _airplaneStatus.alfa;

	_vNorm = _airplaneStatus.v.norm();
	_wSpan_over_2v = isZero(_vNorm) ? .0f : _buildInfo.wingSpan() / (2 * _vNorm);
	_chord_over_2v = isZero(_vNorm) ? .0f : _buildInfo.chord() / (2 * _vNorm);
	//dynamic forces
	_airplaneStatus.thrust = versor_bodyX * _engine.thrust(altitude,_vNorm);
	_airplaneStatus.weight = GVector3(.0f, .0f, _buildInfo.mass()* GPhysicConstants::g*-1.0f);
	//aereodynamic forces and moments
	Real dynPressureS = 0.5f * GAtmosphere::getAirDensity(altitude) * _airplaneStatus.v.normSquare() * _buildInfo.surface();	
	Coefficients coeff = _buildInfo.coefficientsMap().getCoefficients(_airplaneStatus.alfa);

	_airplaneStatus.drag = (_versorV)*(dynPressureS * cx(coeff))* -1.0f;

	_versorL = ((_versorV) ^ _versorWings)* -1.0f;
	_airplaneStatus.lift = _versorL * (dynPressureS * cz(coeff));
	
	_airplaneStatus.yAereodyn = -_versorWings * (dynPressureS * cy( coeff ) );

	_airplaneStatus.mx = (versor_bodyX) * dynPressureS * _buildInfo.wingSpan() * cL(coeff);	//roll
//	_airplaneStatus.my = _versorWings * dynPressureS * _buildInfo.chord() * cM(coeff);		// pitch - beccheggio
	_airplaneStatus.my = GVector3(.0f, +1.0f, .0f) * dynPressureS * _buildInfo.chord() * cM(coeff);		// pitch - beccheggio
	_airplaneStatus.mz = (_versorUP)  * dynPressureS * _buildInfo.wingSpan() *  cN(coeff);

	GMatrix3x3 antiSym_Omega = GMatrix3x3::AntiSymmetricMatrix(_airplaneStatus.w);
	//acceleration (vectorial equations )

	_airplaneStatus.a = (_airplaneStatus.thrust + _airplaneStatus.drag + _airplaneStatus.lift + _airplaneStatus.yAereodyn + _airplaneStatus.weight) / _buildInfo.mass();
	_airplaneStatus.a += antiSym_Omega* _airplaneStatus.v;

	_airplaneStatus.w_dot = _buildInfo.inertiaInverse() * (
															(_airplaneStatus.mx + _airplaneStatus.my + _airplaneStatus.mz)
															- antiSym_Omega* ( _buildInfo.inertia ()*_airplaneStatus.w )
														);

	//integration
	_airplaneStatus.v += _airplaneStatus.a*dt;
	_airplaneStatus.position.addTranslation(_airplaneStatus.v*dt);

	_airplaneStatus.w += _airplaneStatus.w_dot*dt;
	_airplaneStatus.position.applyRotation(Radians(_airplaneStatus.w.x()*dt*-1.0f), Radians(_airplaneStatus.w.y()*dt*-1.0f), Radians(_airplaneStatus.w.z()*dt*-1.0f));

	//those are just for logs
	_airplaneStatus.aroundX = GMath::reduceAngle(_airplaneStatus.aroundX + Radians(_airplaneStatus.w.x()*dt));
	_airplaneStatus.aroundY = GMath::reduceAngle(_airplaneStatus.aroundY + Radians(_airplaneStatus.w.y()*dt));
	_airplaneStatus.aroundZ = GMath::reduceAngle(_airplaneStatus.aroundZ + Radians(_airplaneStatus.w.z()*dt));

	//check ground
	if (_airplaneStatus.position.translationVector().z() < .0f)
	{
		_airplaneStatus.position.setTranlsationZ(.0f);
		if (_airplaneStatus.v.z() < -GMath::Constants::approssimation)
		{
			_airplaneStatus.v.z() = .0f;
		}

		if (_airplaneStatus.aroundY < -GMath::Constants::approssimation )
			_airplaneStatus.w.y() = .03f;
		_airplaneStatus.w.x() = .0f;
	}

	//update airplane versors
	_versorWings = GVector3(.0f, +1.0f, .0f) * _airplaneStatus.position;
	_versorUP = GVector3(.0f, .0f, +1.0f) * _airplaneStatus.position;
}

Real GAirplane::cx(const Coefficients& c) const
{	
	return GMath::absV(
						c.cx().cx() 
						+ c.cx().cxA() * _airplaneStatus.alfa.toReal()
						+  (c.cx().cxQ() * _airplaneStatus.w.y()*_chord_over_2v) 
						+GMath::absV(c.cx().cx_de() * _airplaneStatus.commands.pitch.toReal())
					);
}

Real GAirplane::cy(const Coefficients& c) const
{
	return (c.cy().cyB() * _airplaneStatus.beta.toReal()) + (c.cy().cyP() * _airplaneStatus.w.x() * _wSpan_over_2v)
		+ (c.cy().cyR() * _airplaneStatus.w.z() * _wSpan_over_2v);
}

Real GAirplane::cz(const Coefficients& c) const
{
	return  c.cz().cz() 
			+ c.cz().czA() * _airplaneStatus.alfa.toReal()
			+ ( c.cz ().czAP () * _airplaneStatus.alfaRate*_chord_over_2v )
			+ (c.cz().czQ() * _airplaneStatus.w.y() * _chord_over_2v)
			- ( c.cz ().cz_de () * _airplaneStatus.commands.pitch.toReal() )
			;
}

Real GAirplane::cL(const Coefficients& c) const
{
	return 0.0f
		- (c.cl().clB() * _airplaneStatus.beta.toReal())						//<= stability, he follows beta == 0.0f 
		+ (c.cl().clP() * _airplaneStatus.w.x() * _wSpan_over_2v)				//<= stability, oppose to roll
		+ (c.cl().clR() * _airplaneStatus.w.z()* _wSpan_over_2v)				//<= stability, oppose to yaw
		+ (c.cl().cl_da() * _airplaneStatus.commands.roll.toReal()) 
	//	+ (c.cl().cl_dr() * _airplaneStatus.commands.yaw.toReal())
		;
}

Real GAirplane::cM(const Coefficients& c) const
{	

	return 0.0f
		+ c.cm().cm()
		+ (c.cm().cmA() * _airplaneStatus.alfa.toReal() )						//<= stability, he follows alfa == 0.0f 
		+ ( c.cm ().cmAP () * _airplaneStatus.alfaRate *_chord_over_2v )		//<= follows alfa constant
		+ ( c.cm ().cmQ () *  _airplaneStatus.w.y () *_chord_over_2v )			//<= stability, oppose to pitch
		+ ( c.cm().cmB() * _airplaneStatus.beta.toReal() )
		+(c.cm().cm_de() * _airplaneStatus.commands.pitch.toReal())
		;
}

Real GAirplane::cN(const Coefficients& c) const
{
	return  0.0f
		+ (c.cn().cnB() * _airplaneStatus.beta.toReal())
		+ (c.cn().cnP() * _airplaneStatus.w.x() * _wSpan_over_2v )			//<= stability, oppose to roll
		+ (c.cn().cnR() * _airplaneStatus.w.z() * _wSpan_over_2v )			//<= stability, oppose to yaw
	//	+ (c.cn().cn_da() * _airplaneStatus.commands.roll.toReal())
		+ (c.cn().cn_dr() * _airplaneStatus.commands.yaw.toReal())
		;
}