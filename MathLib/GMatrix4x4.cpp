#include "GMatrix4x4.h"

using namespace GMath;

GVector4 GMath::operator*(const GVector4& v, const GMatrix4x4& mat)
{
	GVector4 r;
	r[0] = (v[0]*mat._m[0]) + (v[1]*mat._m[4]) + (v[2]*mat._m[8] )+ (v[3]*mat._m[12] );
	r[1] = (v[0]*mat._m[1]) + (v[1]*mat._m[5]) + (v[2]*mat._m[9] )+ (v[3]*mat._m[13] );
	r[2] = (v[0]*mat._m[2]) + (v[1]*mat._m[6]) + (v[2]*mat._m[10] )+ (v[3]*mat._m[14] );
	r[3] = (v[0]*mat._m[3]) + (v[1]*mat._m[7]) + (v[2]*mat._m[11] )+ (v[3]*mat._m[15] );
	return r;
}
GVector3 GMath::operator*(const GVector3& v, const GMatrix4x4& mat)
{
	GVector3 r;
	r[0] = (v[0]*mat._m[0]) + (v[1]*mat._m[4]) + (v[2]*mat._m[8] );
	r[1] = (v[0]*mat._m[1]) + (v[1]*mat._m[5]) + (v[2]*mat._m[9] );
	r[2] = (v[0]*mat._m[2]) + (v[1]*mat._m[6]) + (v[2]*mat._m[10] );
	return r;
}
std::ostream& GMath::operator<<(std::ostream& os, const GMatrix4x4& mat)
{
	os << "| ";
	for(int i=0;i<16; ++i)
	{
		os << mat._m[i] << " ";
		if(i==3 || i==7 || i==11)
			os<<"|\n| ";
	}
	os <<"|\n";
	return os;
}

void  GMath::storeFromXMMatrix(_Out_ GMatrix4x4& outMat, const DirectX::XMMATRIX& readMat)
{
	using namespace DirectX;
	XMFLOAT4X4 copy;
	XMStoreFloat4x4(&copy,readMat);

	outMat._m[0] = copy._11;							outMat._m[4] = copy._21;
	outMat._m[1] = copy._12;							outMat._m[5] = copy._22;
	outMat._m[2] = copy._13;							outMat._m[6] = copy._23;
	outMat._m[3] = copy._14;							outMat._m[7] = copy._24;

	outMat._m[8] = copy._31;							outMat._m[12] =copy._41;
	outMat._m[9] = copy._32;							outMat._m[13] =copy._42;
	outMat._m[10] =copy._33;							outMat._m[14] =copy._43;
	outMat._m[11] =copy._34;							outMat._m[15] =copy._44;
}

void GMath::storeXMMatrix(const GMatrix4x4& readMat, _Out_ DirectX::XMMATRIX& outMat)
{
	using namespace DirectX;
	outMat = XMMATRIX (readMat.const_data() );
}


GMatrix4x4::GMatrix4x4(const GMatrix3x3& subMatrix)
{
	int row=0;
	for(int r=0; r<3;++r)
	{
		row=r*4;
		for(int c=0;c<3;++c)
		{
			_m[row+c]=subMatrix(r+1,c+1);
		}

	}
	_m[3]=0.0f;
	_m[7]=0.0f;
	_m[11]=0.0f;

	_m[12]=0.0f;
	_m[13]=0.0f;
	_m[14]=0.0f;

	_m[15]=1.0f;
}
GMatrix4x4::GMatrix4x4(const GMatrix3x3& subMatrix, const GVector3& translation)
{
	int row=0;
	for(int r=0; r<3;++r)
	{
		row=r*4;
		for(int c=0;c<3;++c)
		{
			_m[row+c]=subMatrix(r+1,c+1);
		}

	}
	_m[3]=0.0f;
	_m[7]=0.0f;
	_m[11]=0.0f;

	_m[12]=translation.x();
	_m[13]=translation.y();
	_m[14]=translation.z();
	_m[15]=1.0f;
}

inline GMatrix4x4& GMatrix4x4::operator=(const GMatrix3x3& mat)
{
	int row=0;
	for(int r=0; r<3;++r)
	{
		row=r*4;
		for(int c=0;c<3;++c)
		{
			_m[row+c]=mat(r+1,c+1);
		}

	}
	_m[3]=0.0f;
	_m[7]=0.0f;
	_m[11]=0.0f;

	_m[12]=0.0f;
	_m[13]=0.0f;
	_m[14]=0.0f;

	_m[15]=1.0f;
}

GMatrix4x4 GMatrix4x4::operator*(const GMatrix4x4& other) const
{
	GMatrix4x4 result;
	for(int rc=0; rc<16; ++rc)
	{
		for(int inner=0; inner<4;++inner)
		{
			result._m[rc]+= _m[(rc/4)*(4)+inner] * other._m[(inner*4)+(rc%4)];
		}

	}
	return result;
}

int GMatrix4x4::rank() const
{
	int r = subMatrix().rank();
	if(r<3)
		return r;
	else
	{
		if( areEqual(_m[15],0.0f ) )
		{
			return 3;
		}
		else
		{
			return 4;
		}
	}
}

int GMatrix4x4::nullRows() const
{
	int r,nrows=0;
	for(int row=0;row<4;++row)
	{
		r = row*4;
		//if( _m[r] == _m[r+1] == _m[r+2]== _m[r+3] == 0.0f )
		if( areEqual(_m[r],_m[r+1] ) && areEqual(_m[r+1],_m[r+2])&& areEqual(_m[r+2],_m[r+3]) && areEqual(_m[r+3],0.0f)  )
			++nrows;
	}
	return nrows;
}
int GMatrix4x4::nullColumns() const
{
	int ncol=0;
	for(int col=0;col<4;++col)
	{
		//if( _m[col] == _m[col+4] == _m[col+8] == _m[col+12]== 0.0f )
		if(areEqual(_m[col],_m[col+4])&&areEqual(_m[col+4],_m[col+8])&&areEqual(_m[col+8],_m[col+12])&&areEqual(_m[col+12], .0f))
			++ncol;
	}
	return ncol;
}
GString GMatrix4x4::toString() const
{
	GString str("| ");
	for(int i=0; i<16;++i)
	{
		str+= GString::number(_m[i] ) + " ";
		if(i==3 || i==7 || i==11)
			str+="|\n| ";
	}
	str+="|\n";
	return str;
}
GMatrix4x4 GMatrix4x4::transpose() const
{
	GMatrix4x4 T;
	int y4;
	for(int y = 0; y < 4; ++y)
	{
		y4 = y*4;
		for(int x = 0; x < 4; ++x)
		{
			T._m[(x*4)+y]= _m[(y4)+x];
		}
	}
	return T;
}
GMatrix3x3 GMatrix4x4::subMatrix() const
{
	GMatrix3x3 sub;
	int row=0;
	for(int r=0; r<3;++r)
	{
		row=r*4;
		for(int c=0;c<3;++c)
		{
			sub(r+1,c+1) = _m[row+c];
		}
	}
	return sub;
}

bool GMatrix4x4::isNull() const
{
	for(int i=0; i<16;++i)
	{
		if(! areEqual(_m[i], 0.0f ) )
			return false;
	}
	return true;
}
bool GMatrix4x4::isIdentity() const
{
	for(int i=0; i<16; ++i)
	{
		if(i==0 || i==5 || i== 10|| i== 15)
		{
			if(! areEqual(_m[i],1.0f) )
				return false;
		}
		else
		{
			if(! areEqual(_m[i],0.0f) )
				return false;
		}
	}
	return true;
}
bool GMatrix4x4::isDiagonal() const
{
	for(int i=0; i<16; ++i)
	{
		if(i==0 || i==5 || i== 10|| i== 15)
		{
			if(areEqual(_m[i],0.0f) )
				return false;
		}
		else
		{
			if(! areEqual(_m[i],0.0f) )
				return false;
		}
	}
	return true;
}
bool GMatrix4x4::zeroOnDiagonal() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[5],0.0f) ||areEqual(_m[10],0.0f) ||areEqual(_m[15],0.0f))
		return true;
	return false;
}
bool GMatrix4x4::isTriangularUpper() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[5],0.0f) ||areEqual(_m[10],0.0f) ||areEqual(_m[15],0.0f))
		return false;
	if( areEqual(_m[4],0.0f) && areEqual(_m[9],0.0f) && areEqual(_m[14],0.0f) 
		&& areEqual(_m[8],0.0f) && areEqual(_m[12],0.0f) && areEqual(_m[13],0.0f)
		)
		return true;
	return false;
}
bool GMatrix4x4::isTriangularLower() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[5],0.0f) ||areEqual(_m[10],0.0f) ||areEqual(_m[15],0.0f))
		return false;
	if( areEqual(_m[1],0.0f) && areEqual(_m[2],0.0f) && areEqual(_m[3],0.0f) 
		&& areEqual(_m[6],0.0f) && areEqual(_m[7],0.0f) && areEqual(_m[11],0.0f)
		)
		return true;
	return false;
}
bool GMatrix4x4::isTriangular() const
{
	if( isTriangularUpper() || isTriangularLower() )
		return true;
	return false;
}
bool GMatrix4x4::isSymmetric() const			 //aij == aji
{
	int r4;
	for(int r=0; r<4;++r)
	{
		r4=r*4;
		for(int c=r+1;c<4;++c)
		{
			if(! areEqual(_m[r4+c], _m[(c*4)+r] ) )
				return false;
		}
	}
	return true;
}
bool GMatrix4x4::isAntiSymmetric() const	//aij == -aji
{
	int r4;
	for(int r=0; r<4;++r)
	{
		r4=r*4;
		for(int c=r+1;c<4;++c)
		{
			if(! areEqual(_m[r4+c], _m[(c*4)+r]*(-1.0f) ) )
				return false;
		}
	}
	return true;
}
bool GMatrix4x4::isInvertible() const
{
	if( areEqual( determinant(), 0.0f ) )
		return false;
	else
		return true;
}
void GMatrix4x4::makeItIdentity()
{
	for(int i=0; i<16;++i)
	{
		if( i==0 || i==5 ||i==10 ||i==15)
			_m[i]=1.0f;
		else
			_m[i]=.0f;
	}
}
void GMatrix4x4::transposeMe()
{
	Real temp;
	int y4;
	for(int y = 0; y < 4; ++y)
	{
		y4= y*4;
		for(int x = y+1; x < 4; ++x)		//swap(vc[x*n + y], vc[y*n + x]);
		{
			temp = _m[(x*4)+y];
			_m[(x*4)+y]=_m[y4+x];
			_m[(y4)+x]=temp;
		}
	}
}
void GMatrix4x4::transposeSubMatrix()
{
	Real temp;
	int y4;
	for(int y = 0; y < 3; ++y)
	{
		y4= y*4;
		for(int x = y+1; x < 3; ++x)		//swap(vc[x*n + y], vc[y*n + x]);
		{
			temp = _m[(x*4)+y];
			_m[(x*4)+y]=_m[y4+x];
			_m[(y4)+x]=temp;
		}
	}
}
void GMatrix4x4::replaceRow(const GVector4& row, uint atRow)
{
	if(atRow>4 && atRow<1)
		return;
	uint r = (atRow-1)*4;
	for(int c=0; c<4; ++c)
	{
		_m[r+c]=row[c];
	}

}
void GMatrix4x4::replaceSubRow(const GVector3& row, uint atRow)
{
	if(atRow>3 && atRow<1 )
		return;
	uint r = (atRow-1)*4;
	for(int c=0; c<3; ++c)
	{
		_m[r+c]=row[c];
	}
}
void GMatrix4x4::replaceColumn(const GVector4& col, uint atCol)
{
	if(atCol>4 && atCol<1 )
		return;
	atCol-=1;
	for(uint r=0;r<4;++r)
	{
		_m[atCol +(r*4)]=col[r];
	}
}
void GMatrix4x4::replaceSubCol(const GVector3& col, uint atCol)
{
	if(atCol>3 && atCol<1)
		return;
	atCol-=1;
	for(uint r=0;r<3;++r)
	{
		_m[atCol +(r*4)]=col[r];
	}
}
void GMatrix4x4::swapRows(uint row1, uint row2)
{
	if(row1==row2 || (row1>4 || row2>4) || ((row1<1) || (row2<1)) )
		return;
	uint r1= (row1-1)*4, r2=(row2-1)*4;
	Real t=0;
	for(int c=0; c<4; ++c)
	{
		t=_m[r1+c];
		_m[r1+c]=_m[r2+c];
		_m[r2+c]=t;
	}
}
void GMatrix4x4::swapSubRows(uint row1, uint row2)
{
	if(row1==row2 || (row1>3 || row2>3) || ((row1<1) || (row2<1)) )
		return;
	uint r1= (row1-1)*4, r2=(row2-1)*4;
	Real t=0;
	for(int c=0; c<3; ++c)
	{
		t=_m[r1+c];
		_m[r1+c]=_m[r2+c];
		_m[r2+c]=t;
	}
}	
void GMatrix4x4::swapColumns(uint col1, uint col2)
{
	if(col1==col2 || (col1>4 || col2>4) || ( (col1<1) || (col2<1) ) )
		return;
	Real t=0;
	col1-=1;
	col2-=1;
	for(int r=0; r<4;++r)
	{
		t=_m[(r*4)+col1];
		_m[(r*4)+col1] = _m[(r*4)+col2];
		_m[(r*4)+col2] = t;
	}
}
void GMatrix4x4::swapSubColumns(uint col1, uint col2)
{
	if(col1==col2 || (col1>3 || col2>3) || ( (col1<1) || (col2<1) ) )
		return;
	Real t=0;
	col1-=1;
	col2-=1;
	for(int r=0; r<3;++r)
	{
		t=_m[(r*4)+col1];
		_m[(r*4)+col1] = _m[(r*4)+col2];
		_m[(r*4)+col2] = t;
	}
}

void GMatrix4x4::setSubMatrix(const GMatrix3x3& newSubMatrix)
{
	int row=0;
	for(int r=0; r<3;++r)
	{
		row=r*4;
		for(int c=0;c<3;++c)
		{
			_m[row+c]=newSubMatrix(r+1,c+1);
		}

	}
}
void GMatrix4x4::setTranslation(const GVector3& newTransl)
{
	_m[12]=newTransl.x();
	_m[13]=newTransl.y();
	_m[14]=newTransl.z();
}

void GMatrix4x4::setRotation(Radians around_x, Radians around_y, Radians around_z)
{
	around_x = reduceAngle(around_x);
	around_y = reduceAngle(around_y);
	around_z = reduceAngle(around_z);
	if( (around_x== .0f) && (around_y== .0f) && (around_z== .0f) )
	{
		setSubMatrix( GMatrix3x3::Identity() );
		return;
	}
	GMatrix3x3 sub = GMatrix3x3::RotationMatrix(around_x,around_y,around_z);
	setSubMatrix(sub);
}

void GMatrix4x4::applyRotation(Radians around_x, Radians around_y, Radians around_z)
{
	around_x = reduceAngle(around_x);
	around_y = reduceAngle(around_y);
	around_z = reduceAngle(around_z);
	if( (around_x== .0f) && (around_y== .0f) && (around_z== .0f) )
		return ;
	GMatrix3x3 sub = subMatrix();
	sub = sub*GMatrix3x3::RotationMatrix(around_x,around_y,around_z);
	setSubMatrix(sub);
}

void GMatrix4x4::applyRotation(const GMatrix3x3& rotationMatrix)
{
	if(rotationMatrix.isIdentity() )
		return;
	//i have to hard copy
	GMatrix3x3 sub = subMatrix();
	sub = sub*rotationMatrix;
	setSubMatrix(sub);
}

void GMatrix4x4::applyRotation(const GVector3&u, Radians teta)
{
	reduceAngle(teta);
	if(teta==0.0f)
		return;
	GMatrix3x3 sub = subMatrix();
	sub = sub*GMatrix3x3::RotationMatrix(u,teta);
	setSubMatrix(sub);
}

////pass VERSORS i,j,k in parent-space coordinates and t= position of the new basis in parent-space
//	static GMatrix4x4 ChangeOfBasis(const GVector3& i,const GVector3& j,const GVector3& k,const GVector3& t);

GMatrix4x4 GMatrix4x4::ChangeOfBasis(const GVector3& i,const GVector3& j,const GVector3& k,const GVector3& t)
{
	GMatrix4x4 M;
	M._m[0]=i.x();							M._m[4]=i.y();
	M._m[1]=j.x();							M._m[5]=j.y();
	M._m[2]=k.x();							M._m[6]=k.y();

	M._m[8]= i.z();							M._m[12]=t.x();
	M._m[9]= j.z();							M._m[13]=t.y();
	M._m[10]=k.z();							M._m[14]=t.z();

	M._m[15]=1;
	return M;
}


GMatrix4x4 GMatrix4x4::LookAtLH(const GVector3& position, const GVector3& lookAt, const GVector3& up)
{
	GVector3 zaxis = lookAt-position;
	zaxis = zaxis.normalized();
	GVector3 xaxis = up^zaxis;
	xaxis = xaxis.normalized();
	GVector3 yaxis = zaxis^xaxis;
	yaxis = yaxis.normalized();

	GVector3 t;
	t.x() = (xaxis*position);
	t.y() = (yaxis*position);
	t.z() = (zaxis*position);

	return ChangeOfBasis(xaxis,yaxis,zaxis,-t);
}
