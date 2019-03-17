#pragma once
#ifndef GMATRIX3x3_H
#define GMATRIX3x3_H

#include "GVector3.h"
#include "GEquationSolver.h"
#include <GVector.h>

namespace GMath{

//first element == 11
class GMatrix3x3
{
	friend GVector3 operator*(const GVector3& v, const GMatrix3x3& mat);
	inline friend GMatrix3x3 operator*(const Real& d, const GMatrix3x3& mat)		{return mat*d;}
	friend std::ostream& operator<<(std::ostream& os, const GMatrix3x3& mat);
	
public:
	GMatrix3x3(void)										{for(int i=0; i<9; i++) _m[i]=0;}
	explicit GMatrix3x3(const Real m[9] )					{for(int i=0; i<9; i++) _m[i]=m[i];}
	GMatrix3x3(const GMatrix3x3& other)						{for(int i=0; i<9; i++) _m[i]=other._m[i];}
	~GMatrix3x3(void)										{}

	//operators
	inline GMatrix3x3& operator=(const GMatrix3x3& mat)		{for(int i=0; i<9; i++) _m[i]=mat._m[i]; return *this;}
	inline bool operator ==(const GMatrix3x3& m) const		{for(int i=0; i<9; i++) if(!areEqual(_m[i], m._m[i]))return false; return true;}
	inline bool operator !=(const GMatrix3x3& other) const	{return ! this->operator==(other);}
	inline GMatrix3x3 operator +(const GMatrix3x3& x) const	{GMatrix3x3 m(*this);for(int i=0; i<9; i++) m._m[i]+=x._m[i];return m;}
    inline GMatrix3x3& operator +=(const GMatrix3x3& x)		{for(int i=0; i<9; i++) _m[i]+=x._m[i];return *this;}
    inline GMatrix3x3 operator -(const GMatrix3x3& x) const	{GMatrix3x3 m(*this);for(int i=0; i<9; i++) m._m[i]-=x._m[i];return m;}
	inline GMatrix3x3 operator-() const						{GMatrix3x3 m(*this); for(int i=0; i<9; i++) m._m[i]*=-1; return m;}
    inline GMatrix3x3& operator -=(const GMatrix3x3& x)		{for(int i=0; i<9; i++) _m[i]-=x._m[i];return *this;}
	GMatrix3x3 operator * (const Real& d) const				{GMatrix3x3 m(*this);for(int i=0; i<9; i++) m._m[i]*=d;return m;}
    inline GMatrix3x3& operator *=(const Real& d)			{for(int i=0; i<9; i++) _m[i]*=d; return *this;}
    GMatrix3x3 operator *(const GMatrix3x3& other) const;
	GVector3 operator*(const GVector3& v) const;
	GMatrix3x3 operator /(const Real& d) const				{GMatrix3x3 m(*this);for(int i=0; i<9; i++) m._m[i]/=d;return m;}
    inline GMatrix3x3& operator /=(const Real& d)			{for(int i=0; i<9; i++) _m[i]/=d; return *this;}

	//access (first element==11 not 00)
	inline Real& operator()(uint row, uint column)				{ row -= 1; column -= 1; if (row == -1 || column == -1) return _m[0]; return _m[column + (row * 3)]; }
	//(first element==11 not 00)
	inline const Real& operator()(uint row, uint column) const	{ row -= 1; column -= 1; if (row == -1 || column == -1) return _m[0]; return _m[column + (row * 3)]; }
	//return Real _m[9]
	inline Real* data()											{return &_m[0];}
	inline const Real* const_data() const						{return &_m[0];}
	//matrix informations

	//matrix rank (<=3)
	int rank() const;
	int nullRows() const;
    int nullColumns() const;
	inline Real determinant() const							{Real d=0; for(int i=0;i<3;i++) d+=_m[i]*(_m[3+((i+1)%3)]*_m[6+((i+2)%3)] -_m[3+((i+2)%3)]*_m[6+((i+1)%3)]); return d;}
	inline Real trace() const								{Real t=.0f; t+= _m[0]+_m[4]+_m[8]; return t;}
	inline GVector3 diagonal() const						{return GVector3(_m[0], _m[4], _m[8]);}
	inline GVector3 row(uint atRow) const					{atRow-=1; uint i = atRow*3; return GVector3(_m[i], _m[i+1], _m[i+2]);}
	inline GVector3 column(uint atColumn) const				{atColumn-=1; return GVector3(_m[atColumn], _m[atColumn+3], _m[atColumn+6]);}
	inline GVector3 versorX() const							{ return column(1); }
	inline GVector3 versorY() const							{ return column(2); }
	inline GVector3 versorZ() const							{ return column(3); }
	GString toString() const;
	//slow (0.03sec) - fast for special cases (like antiSymmetric)
	GMatrix3x3 transpose() const;
	//slow: general inverting algorithm
	//call this only if rank==3
    GMatrix3x3 inverse() const;
	GVector<GComplex> eigen() const;

	//bools
	//all elements==0
    bool isNull() const;
    bool isIdentity() const;
	//all elements in diagonal!=0 other elements==0
    bool isDiagonal() const;
    bool zeroOnDiagonal() const;
	//all zeros under diagonal
    bool isTriangularUpper() const;
    bool isTriangularLower() const;
    bool isTriangular() const;
	//aij == aji
    bool isSymmetric() const;
	//aij == -aji
    bool isAntiSymmetric() const;
    bool isInvertible() const;
	//represents pure rotation
    bool isOrthogonal() const;

	//transformations

    void makeItIdentity();
	//makes M==M.transpose()
	//very fast
	void transposeMe();
	inline void fill(const Real& newDefaultValue)				{for(int i=0;i<9;++i) _m[i]=newDefaultValue;}
	inline void setDiagonalValues(const Real& diagonalValue)	{_m[0]=_m[4]=_m[8]=diagonalValue;}
	inline void setDiagonalValues(const GVector3& diag)			{_m[0]=diag.x() ;_m[4]=diag.y() ;_m[8]=diag.z();}
	//first row==1
	void replaceRow(const GVector3& row, uint atRow);
	void replaceColumn(const GVector3& col, uint atCol);
    void swapRows(uint row1, uint row2);
    void swapColumns(uint col1, uint col2);

	//utility matrix (static members)

	inline static GMatrix3x3 Identity()							{GMatrix3x3 m; m._m[0]=m._m[4]=m._m[8]=1; return m;}
	inline static GMatrix3x3 ScaleMatrix(Real scaleValue)		{GMatrix3x3 m; m._m[0]=m._m[4]=m._m[8]=scaleValue; return m;}
	inline static GMatrix3x3 ScaleMatrix(const GVector3& scale) {GMatrix3x3 m; m._m[0]=scale.x(); m._m[4]= scale.y(); m._m[8]=scale.z(); return m;}
	static GMatrix3x3 Psi (Radians around_z);
	static GMatrix3x3 Teta(Radians around_y);
	static GMatrix3x3 Fi  (Radians around_x);
	//The orthogonal matrix corresponding to a clockwise/left-handed rotation with Euler angles
	static GMatrix3x3 RotationMatrix(Radians around_x, Radians around_y, Radians around_z);
	//Rotation matrix from axis and angle
	static GMatrix3x3 RotationMatrix(const GVector3& u, Radians teta);
	//Build a skew-symmetric matrix from a Vector. The transpose is also its negative; that is, it satisfies the condition -A = AT
	static GMatrix3x3 AntiSymmetricMatrix(const GVector3& v);

	//usefull operations

	//angle of rotation around rotation-axis
	inline Radians rotationAngle() const						{return arcCos( (trace()-1)/2.0f ); }
private:
	Real _m[9];
};

}		//EO namespace
#endif // GMATRIX3x3_H

