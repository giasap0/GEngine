#pragma once
#ifndef G_AEREODYNAMIC_COEFFICIENT_MAP_H
#define G_AEREODYNAMIC_COEFFICIENT_MAP_H

#include "GCoefficients.h"
using namespace GMath;

namespace GPhysics
{
	class GAirplaneLoader;

	namespace GAereodynamic{

		class GCoefficientsMap
		{
			friend class GAirplaneLoader;
		public:
			GCoefficientsMap(uint size);
			GCoefficientsMap(const GCoefficientsMap&);
			virtual ~GCoefficientsMap();
			
			GCoefficientsMap& operator=(const GCoefficientsMap&);

			//get coefficients - use linear interpolation
			//not mapped == all zero
			inline Coefficients getCoefficients(Radians angle) const
			{
				if (_size == 0)
					return Coefficients();
				Coefficients onlyCX;
				onlyCX.cx().cx() = _coefficients[_size-1].cx().cx();
				for (uint i = 0; i < _size; i++)
				{
					if (angle == _angles[i])
						return _coefficients[i];
					if (i == _size - 1)
						return onlyCX;
					if (angle >= _angles[i] && angle <= _angles[i + 1])
					{
						Real denom = (_angles[i + 1] - _angles[i]).toReal();
						if (isZero(denom))
							denom = GMath::Constants::epsilon;
						Real beta = (angle - _angles[i]).toReal() / denom;
						return Lerp(_coefficients[i], _coefficients[i + 1], beta);
					}
				}
				return onlyCX;
			}
			inline bool operator==(const GCoefficientsMap& map) const
			{
				if (_size != map._size)				return false;
				if (_size == 0 && map._size == 0)	return true;
				for (uint i = 0; i < _size; i++)
				{
					if (_angles[i] != map._angles[i]) return false;
					if (_coefficients[i] != map._coefficients[i]) return false;
				}
				return true;
			}
		private:
			uint _size;
			Radians* _angles;
			Coefficients* _coefficients;
		};
	}
}
#endif			//G_AEREODYNAMIC_COEFFICIENT_MAP_H
