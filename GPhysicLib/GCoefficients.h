#pragma once
#ifndef G_AEREODYNAMIC_COEFFICIENTS_H
#define G_AEREODYNAMIC_COEFFICIENTS_H

#include <GMath.h>
#include <GEngineCore.h>

using namespace GMath;

namespace GPhysics
{
	class GAirplaneLoader;

	namespace GAereodynamic{

		class Cx
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend Cx Lerp(const Cx& A, const Cx& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				Cx out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			Cx()							{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~Cx()					{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			Cx(Real x, Real xQ):Cx()		{ _values[0] = x; _values[1] = xQ; }
			Cx(const Cx& o)					{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = o._values[i]; } }

			Cx & operator= (const Cx& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const Cx& c) const {
				for (int i = 0; i < _size; i++) {
					if (! areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const Cx& c) const							{ return !this->operator==(c); }

			const Real& cx() const												{ return _values[0]; }
			const Real& cxQ() const												{ return _values[1]; }
			const Real& cxA() const												{ return _values[2]; }
			const Real& cxAP() const											{ return _values[3]; }
			const Real& cxU() const												{ return _values[4]; }
			const Real& cxB() const												{ return _values[5]; }
			const Real& cxP() const												{ return _values[6]; }
			const Real& cxR() const												{ return _values[7]; }
			const Real& cx_de() const											{ return _values[8]; }
			const Real& cx_dle() const											{ return _values[9]; }

			Real& cx() 															{ return _values[0]; }
			Real& cxQ() 														{ return _values[1]; }
			Real& cxA()															{ return _values[2]; }
			Real& cxAP()														{ return _values[3]; }
			Real& cxU() 														{ return _values[4]; }
			Real& cxB() 														{ return _values[5]; }
			Real& cxP() 														{ return _values[6]; }
			Real& cxR()															{ return _values[7]; }
			Real& cx_de()														{ return _values[8]; }
			Real& cx_dle()														{ return _values[9]; }

		private:
			static const int _size = 10;
			Real* _values;
		};

		class Cy
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend Cy Lerp(const Cy& A, const Cy& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				Cy out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			Cy()																{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~Cy()														{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			Cy(Real y, Real yB, Real yP, Real yR): Cy()							{ _values[0] = yB; _values[1] = yP; _values[2] = yR; _values[3] = y; }
			Cy(const Cy& c)														{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; } }

			Cy & operator= (const Cy& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const Cy& c) const {
				for (int i = 0; i < _size; i++) {
					if (!areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const Cy& c) const							{ return !this->operator==(c); }

			const Real& cyB() const												{ return _values[0]; }
			const Real& cyP() const												{ return _values[1]; }
			const Real& cyR() const												{ return _values[2]; }
			const Real& cy() const												{ return _values[3]; }
			const Real& cyBP() const											{ return _values[4]; }
			const Real& cyA() const												{ return _values[5]; }
			const Real& cyQ() const												{ return _values[6]; }
			const Real& cy_da() const											{ return _values[7]; }
			const Real& cy_dr() const											{ return _values[8]; }

			Real& cyB() 														{ return _values[0]; }
			Real& cyP() 														{ return _values[1]; }
			Real& cyR()															{ return _values[2]; }
			Real& cy()															{ return _values[3]; }
			Real& cyBP()														{ return _values[4]; }
			Real& cyA()															{ return _values[5]; }
			Real& cyQ()															{ return _values[6]; }
			Real& cy_da()														{ return _values[7]; }
			Real& cy_dr()														{ return _values[8]; }
		private:
			static const int _size = 9;
			Real* _values;
		};

		class Cz
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend Cz Lerp(const Cz& A, const Cz& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				Cz out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			Cz()							{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~Cz()					{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			Cz(Real cz, Real czQ) :Cz()		{ _values[0] = cz; _values[1] = cz; }
			Cz(const Cz& c)					{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; } }

			Cz & operator= (const Cz& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const Cz& c) const {
				for (int i = 0; i < _size; i++) {
					if (!areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const Cz& c) const							{ return !this->operator==(c); }

			const Real& cz() const												{ return _values[0]; }
			const Real& czQ() const												{ return _values[1]; }
			const Real& czA() const												{ return _values[2]; }
			const Real& czAP() const											{ return _values[3]; }
			const Real& czU() const												{ return _values[4]; }
			const Real& czB() const												{ return _values[5]; }
			const Real& czP() const												{ return _values[6]; }
			const Real& czR() const												{ return _values[7]; }
			const Real& cz_de() const											{ return _values[8]; }
			const Real& cz_dle() const											{ return _values[9]; }

			Real& cz() 															{ return _values[0]; }
			Real& czQ() 														{ return _values[1]; }
			Real& czA()															{ return _values[2]; }
			Real& czAP()														{ return _values[3]; }
			Real& czU()															{ return _values[4]; }
			Real& czB()															{ return _values[5]; }
			Real& czP()															{ return _values[6]; }
			Real& czR()															{ return _values[7]; }
			Real& cz_de()														{ return _values[8]; }
			Real& cz_dle()														{ return _values[9]; }
		private:	
			static const int _size = 10;
			Real* _values;
		};

		class CL
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend CL Lerp(const CL& A, const CL& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				CL out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			CL()							{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~CL()					{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			CL(Real cl, Real clB, Real clP, Real clR, Real cl_da, Real cl_dr) : CL()
			{
				_values[0] = clB; _values[1] = clP;
				_values[2] = clR; _values[3] = cl_da;
				_values[4] = cl_dr; _values[5] = cl;
			}
			CL(const CL& c)					{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; } }

			CL & operator= (const CL& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const CL& c) const {
				for (int i = 0; i < _size; i++) {
					if (!areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const CL& c) const							{ return !this->operator==(c); }

			const Real& clB() const												{ return _values[0]; }
			const Real& clP() const												{ return _values[1]; }
			const Real& clR() const												{ return _values[2]; }
			const Real& cl_da() const											{ return _values[3]; }
			const Real& cl_dr() const											{ return _values[4]; }
			const Real& cl() const												{ return _values[5]; }
			const Real& clBP() const											{ return _values[6]; }
			const Real& clA() const												{ return _values[7]; }
			const Real& clQ() const												{ return _values[8]; }

			Real& clB() 														{ return _values[0]; }
			Real& clP() 														{ return _values[1]; }
			Real& clR() 														{ return _values[2]; }
			Real& cl_da() 														{ return _values[3]; }
			Real& cl_dr() 														{ return _values[4]; }
			Real& cl()															{ return _values[5]; }
			Real& clBP()														{ return _values[6]; }
			Real& clA()															{ return _values[7]; }
			Real& clQ()															{ return _values[8]; }
		private:
			static const int _size = 9;
			Real* _values;
		};
		class CM
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend CM Lerp(const CM& A, const CM& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				CM out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			CM()																{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~CM()														{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			CM(Real cm, Real cmQ, Real cmB, Real cm_de) : CM()
			{
				_values[0] = cm; _values[1] = cmQ;
				_values[2] = cmB; _values[3] = cm_de;
			}
			CM(const CM& c)														{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; } }

			CM & operator= (const CM& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const CM& c) const {
				for (int i = 0; i < _size; i++) {
					if (!areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const CM& c) const							{ return !this->operator==(c); }

			const Real& cm() const												{ return _values[0]; }
			const Real& cmQ() const												{ return _values[1]; }
			const Real& cmB() const												{ return _values[2]; }
			const Real& cm_de() const											{ return _values[3]; }
			const Real& cmA() const												{ return _values[4]; }
			const Real& cmAP() const											{ return _values[5]; }
			const Real& cmU() const												{ return _values[6]; }
			const Real& cmP() const												{ return _values[7]; }
			const Real& cmR() const												{ return _values[8]; }
			const Real& cm_dle() const											{ return _values[9]; }

			Real& cm() 															{ return _values[0]; }
			Real& cmQ() 														{ return _values[1]; }
			Real& cmB() 														{ return _values[2]; }
			Real& cm_de() 														{ return _values[3]; }
			Real& cmA()															{ return _values[4]; }
			Real& cmAP()														{ return _values[5]; }
			Real& cmU()															{ return _values[6]; }
			Real& cmP()															{ return _values[7]; }
			Real& cmR()															{ return _values[8]; }
			Real& cm_dle()														{ return _values[9]; }
		private:
			static const int _size = 10;
			Real* _values;
		};

		class CN
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend CN Lerp(const CN& A, const CN& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				CN out;
				for (int i = 0; i < A._size; i++)
				{
					out._values[i] = (((1.0f - beta)*A._values[i]) + (beta*B._values[i]));
				}
				return out;
			}
		public:
			CN()																{ _values = new Real[_size]; for (int i = 0; i < _size; i++) { _values[i] = .0f; } }
			virtual ~CN()														{ if (_values != nullptr) { delete[] _values; _values = nullptr; } }
			CN(Real cn, Real cnB, Real cnP, Real cnR, Real cn_da, Real cn_dr) : CN()
			{
				_values[0] = cnB; _values[1] = cnP;
				_values[2] = cnR; _values[3] = cn_da;
				_values[4] = cn_dr; _values[5] = cn;
			}
			CN(const CN& c)														{ _values = new Real[_size]; for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; } }

			CN & operator= (const CN& c)										{ for (int i = 0; i < _size; i++){ _values[i] = c._values[i]; }; return *this; }
			inline bool operator==(const CN& c) const {
				for (int i = 0; i < _size; i++) {
					if (!areEqual(_values[i], c._values[i]))
						return false;
				}
				return true;
			}
			inline bool operator!=(const CN& c) const							{ return !this->operator==(c); }

			const Real& cnB() const												{ return _values[0]; }
			const Real& cnP() const												{ return _values[1]; }
			const Real& cnR() const												{ return _values[2]; }
			const Real& cn_da() const											{ return _values[3]; }
			const Real& cn_dr() const											{ return _values[4]; }
			const Real& cn() const												{ return _values[5]; }
			const Real& cnBP() const											{ return _values[6]; }
			const Real& cnA() const												{ return _values[7]; }
			const Real& cnQ() const												{ return _values[8]; }

			Real& cnB() 														{ return _values[0]; }
			Real& cnP() 														{ return _values[1]; }
			Real& cnR() 														{ return _values[2]; }
			Real& cn_da() 														{ return _values[3]; }
			Real& cn_dr() 														{ return _values[4]; }
			Real& cn()															{ return _values[5]; }
			Real& cnBP()														{ return _values[6]; }
			Real& cnA()															{ return _values[7]; }
			Real& cnQ()															{ return _values[8]; }
		private:
			static const int _size = 9;
			Real* _values;
		};
		
		class Coefficients
		{
			//linear interpolation
			//lerp(A,B,beta) = (1-beta)*A + beta*B		//0<=beta<=1
			inline friend Coefficients Lerp(const Coefficients& A, const Coefficients& B, Real beta)
			{
				clamp(beta, .0f, 1.0f);
				Coefficients out;
				out._cx = Lerp(A._cx, B._cx, beta);
				out._cy = Lerp(A._cy, B._cy, beta);
				out._cz = Lerp(A._cz, B._cz, beta);
				out._cl = Lerp(A._cl, B._cl, beta);
				out._cm = Lerp(A._cm, B._cm, beta);
				out._cn = Lerp(A._cn, B._cn, beta);
				return out;
			}
			friend class GAirplaneLoader;
		public:
			Coefficients()  : _cx(), _cy(),_cz(), _cl(),_cm(),_cn()			{}
			virtual ~Coefficients()					{}

			Coefficients(const Coefficients& c)
			{
				_cx.operator=(c._cx);
				_cy.operator=(c._cy);
				_cz.operator=(c._cz);
				_cl.operator=(c._cl);
				_cm.operator=(c._cm);
				_cn.operator=(c._cn);
			}

			Coefficients & operator= (const Coefficients& c)
			{
				_cx.operator=(c._cx);
				_cy.operator=(c._cy);
				_cz.operator=(c._cz);
				_cl.operator=(c._cl);
				_cm.operator=(c._cm);
				_cn.operator=(c._cn);

				return *this;
			}
			inline bool operator==(const Coefficients& c) const
			{
				if (_cx.operator!=(c._cx)) return false;
				if (_cy.operator!=(c._cy)) return false;
				if (_cz.operator!=(c._cz)) return false;
				if (_cl.operator!=(c._cl)) return false;
				if (_cm.operator!=(c._cm)) return false;
				if (_cn.operator!=(c._cn)) return false;
				return true;
			}
			inline bool operator !=(const Coefficients& c) const	{ return !this->operator==(c); }

			const Cx& cx() const 				{ return _cx; }
			const Cy& cy() const 				{ return _cy; }
			const Cz& cz() const 				{ return _cz; }
			const CL& cl() const 				{ return _cl; }
			const CM& cm() const 				{ return _cm; }
			const CN& cn() const 				{ return _cn; }

			Cx& cx()  							{ return _cx; }
			Cy& cy()  							{ return _cy; }
			Cz& cz()  							{ return _cz; }
			CL& cl()  							{ return _cl; }
			CM& cm()  							{ return _cm; }
			CN& cn()  							{ return _cn; }

		private:
			Cx _cx;
			Cy _cy;
			Cz _cz;
			CL _cl;
			CM _cm;
			CN _cn;
		};
}
}

#endif			//G_AEREODYNAMIC_COEFFICIENTS_H