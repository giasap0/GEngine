#pragma once
#ifndef GMATRIX4x4_H
#define GMATRIX4x4_H

#include <DirectXMath.h>
#include "GMatrix3x3.h"
#include "GVector4.h"

/*
 *  TO DO:
 *  - normalMatrix() //si prende la 3x3 che parte da (0,0) si fa l'inversa, e poi la trasporta
 *  -
 ***/

namespace GMath{
//Affine Transformation Matrix	--	first element == 11
class GMatrix4x4
{
	friend GVector4 operator*(const GVector4& v, const GMatrix4x4& mat);
	friend GVector3 operator*(const GVector3& v, const GMatrix4x4& mat);
	inline friend GMatrix4x4 operator*(const Real& d, const GMatrix4x4& mat)		{return mat*d;}
	friend std::ostream& operator<<(std::ostream& os, const GMatrix4x4& mat);
	friend void storeFromXMMatrix(_Out_ GMatrix4x4& outMat, const DirectX::XMMATRIX& readMat);
	friend void storeXMMatrix(const GMatrix4x4& readMat, _Out_ DirectX::XMMATRIX& outMat);

public:
	GMatrix4x4(void)										{for(int i=0; i<16; i++) _m[i]=0;}
	explicit GMatrix4x4(const Real m[16])					{for(int i=0; i<16; i++) _m[i]=m[i];}
	GMatrix4x4(const GMatrix4x4& other)						{for(int i=0; i<16; i++) _m[i]=other._m[i];}
	GMatrix4x4(const GMatrix3x3& subMatrix);
	GMatrix4x4(const GMatrix3x3& subMatrix, const GVector3& translation);
	~GMatrix4x4(void)										{}

	//operators
	inline GMatrix4x4& operator=(const GMatrix4x4& mat)		{for(int i=0; i<16; i++) _m[i]=mat._m[i]; return *this;}
	GMatrix4x4& operator=(const GMatrix3x3& mat);
	inline bool operator ==(const GMatrix4x4& m) const		{for(int i=0; i<16; i++) if(!areEqual(_m[i], m._m[i]))return false; return true;}
	inline bool operator !=(const GMatrix4x4& other) const	{return ! this->operator==(other);}
	inline GMatrix4x4 operator +(const GMatrix4x4& x) const	{GMatrix4x4 m(*this);for(int i=0; i<16; i++) m._m[i]+=x._m[i];return m;}
    inline GMatrix4x4& operator +=(const GMatrix4x4& x)		{for(int i=0; i<16; i++) _m[i]+=x._m[i];return *this;}
    inline GMatrix4x4 operator -(const GMatrix4x4& x) const	{GMatrix4x4 m(*this);for(int i=0; i<16; i++) m._m[i]-=x._m[i];return m;}
	inline GMatrix4x4 operator-() const						{GMatrix4x4 m(*this); for(int i=0; i<16; i++) m._m[i]*=-1.0f; return m;}
    inline GMatrix4x4& operator -=(const GMatrix4x4& x)		{for(int i=0; i<16; i++) _m[i]-=x._m[i];return *this;}
	GMatrix4x4 operator * (const Real& d) const				{GMatrix4x4 m(*this);for(int i=0; i<16; i++) m._m[i]*=d;return m;}
    inline GMatrix4x4& operator *=(const Real& d)			{for(int i=0; i<16; i++) _m[i]*=d; return *this;}
    GMatrix4x4 operator *(const GMatrix4x4& other) const;
	GMatrix4x4 operator /(const Real& d) const				{GMatrix4x4 m(*this);for(int i=0; i<16; i++) m._m[i]/=d;return m;}
    inline GMatrix4x4& operator /=(const Real& d)			{for(int i=0; i<16; i++) _m[i]/=d; return *this;}

	//access
	inline Real& operator()(uint row, uint column)				{row-=1;column-=1;return _m[column + (row*4) ];}
	inline const Real& operator()(uint row, uint column) const	{row-=1;column-=1;return _m[column + (row*4) ];}
	//return Real _m[16];
	inline Real* data()											{return &_m[0];}
	inline const Real* const_data() const						{return &_m[0];}

	//matrix informations

	//matrix rank (<=4)
	int rank() const;
	int nullRows() const;
    int nullColumns() const;
	//WARNING: it assumes a03=a07=a011= 0
	inline Real determinant() const							{if(areEqual(_m[15],.0f)) return .0f; return subMatrix().determinant()*_m[15];}
	inline Real trace() const								{Real t=.0f; t+= _m[0]+_m[5]+_m[10]+_m[15]; return t;}
	inline GVector4 diagonal() const						{return GVector4(_m[0], _m[5], _m[10], _m[15]);}
	inline GVector4 row(uint atRow) const					{atRow-=1; uint i = atRow*4; return GVector4(_m[i], _m[i+1], _m[i+2], _m[i+3]);}
	inline GVector4 column(uint atColumn) const				{atColumn-=1; return GVector4(_m[atColumn], _m[atColumn+4], _m[atColumn+8], _m[atColumn+12]);}
	GString toString() const;
	inline DirectX::XMMATRIX toXMMatrix() const				{return DirectX::XMMATRIX( const_data() );}
	//slow (0.03sec)
	GMatrix4x4 transpose() const;
	//return upper 3x3 matrix
	GMatrix3x3 subMatrix() const;
	inline GVector3 translationVector() const				{return GVector3(_m[12],_m[13],_m[14]);}
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

	//transformations

    void makeItIdentity();
	//makes M==M.transpose()
	//very fast
	void transposeMe();
	//very fast: transpose the upper 3x3submatrix
	void transposeSubMatrix();
	inline void fill(const Real& newDefaultValue)				{for(int i=0;i<16;++i) _m[i]=newDefaultValue;}
	inline void setDiagonalValues(const Real& diagonalValue)	{_m[0]=_m[5]=_m[10]=_m[15]=diagonalValue;}
	inline void setDiagonalValues(const GVector4& diag)			{_m[0]=diag.x() ;_m[5]=diag.y() ;_m[10]=diag.z();_m[15]=diag.w();}
	inline void setDiagonalValues(const GVector3& diag)			{_m[0]=diag.x() ;_m[5]=diag.y() ;_m[10]=diag.z();}
	void replaceRow(const GVector4& row, uint atRow);
	//replace row of submatrix 3z3
	void replaceSubRow(const GVector3& row, uint atRow);
	void replaceColumn(const GVector4& col, uint atCol);
	//replace column of submatrix 3z3
	void replaceSubCol(const GVector3& col, uint atRow);
    void swapRows(uint row1, uint row2);
	//swap rows of submatrix 3z3
	void swapSubRows(uint row1, uint row2);
    void swapColumns(uint col1, uint col2);
	//swap columns of submatrix 3z3
	void swapSubColumns(uint col1, uint col2);
	void setSubMatrix(const GMatrix3x3& newSubMatrix);
	void setTranslation(const GVector3& newTransl);
	inline void setTranslation(Real x, Real y, Real z)					{setTranslation( GVector3(x,y,z) );}

	//usefull operations

	inline void scale(const Real& uniformScale)							{_m[0]*=uniformScale;_m[5]*=uniformScale;_m[10]*=uniformScale;}
	inline void scale(const GVector3& scaleVector)						{_m[0]*=scaleVector.x();_m[5]*=scaleVector.y();_m[10]*=scaleVector.z();}
	//add this vector to translations elements
	inline void addTranslation(const GVector3& delta)					{_m[12]+=delta.x();_m[13]+=delta.y();_m[14]+=delta.z();}
	//set rotation(it will substitute the upper 3x3 submatrix)
	void setRotation(Radians around_x, Radians around_y, Radians around_z);
	//apply this rotations (involves in matrix multiplications)
	void applyRotation(Radians around_x, Radians around_y, Radians around_z);
	//apply this rotation
	//(matrix multiplication between rotationMatrix and upper 3x3 submatrix)
	void applyRotation(const GMatrix3x3& rotationMatrix);
	//apply teta rotation around axis u
	void applyRotation(const GVector3& u, Radians teta);
	//angle of rotation around rotation-axis
	inline Radians rotationAngle() const								{ Real tr = _m[0]+_m[5]+_m[10];return arcCos( (tr-1)/2.0f );}

	//statics

	inline static GMatrix4x4 Identity()									{GMatrix4x4 M; M._m[0]=M._m[5]=M._m[10]=M._m[15]=1.0f; return M;}
	inline static GMatrix4x4 TranslationMatrix(const GVector3& t)		{GMatrix4x4 M = GMatrix4x4::Identity(); M.setTranslation(t); return M;}

	//brings VERSORS i,j,k in parent-space coordinates and t= position of the new basis in parent-space
	static GMatrix4x4 ChangeOfBasis(const GVector3& i,const GVector3& j,const GVector3& k,const GVector3& t);
	static GMatrix4x4 LookAtLH(const GVector3& position, const GVector3& lookAt, const GVector3& up);

	
private:
	Real _m[16];
};

}		//EO namespace
#endif // GMATRIX4x4_H


