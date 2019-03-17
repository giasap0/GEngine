#include "GMatrix3x3.h"

using namespace GMath;

GVector3 GMath::operator*(const GVector3& v, const GMatrix3x3& mat)		//faster then using AMP
{
	GVector3 r;
	r[0] = (v[0]*mat._m[0]) + (v[1]*mat._m[3]) + (v[2]*mat._m[6] );
	r[1] = (v[0]*mat._m[1]) + (v[1]*mat._m[4]) + (v[2]*mat._m[7] );
	r[2] = (v[0]*mat._m[2]) + (v[1]*mat._m[5]) + (v[2]*mat._m[8] );
	return r;
}

std::ostream& GMath::operator<<(std::ostream& os, const GMatrix3x3& mat)
{
	os << "| ";
	for(int i=0; i<9; ++i)
	{
		os << mat._m[i] << " ";
		if(i==2 || i==5)
			os <<"|\n| ";
	}
	os <<"|\n";
	return os;
}

GMatrix3x3 GMatrix3x3::operator*(const GMatrix3x3& other) const
{
	GMatrix3x3 result;
	for(int rc=0; rc<9; rc++)
	{
		for(int inner=0; inner<3; ++inner)
		{
			result._m[rc] += _m[(rc/3)*(3)+inner] * other._m[(inner*3)+(rc%3)];
		}
	}
	return result;
}

int GMatrix3x3::rank() const
{
	if( !areEqual( determinant(), 0.0f ) )
		return 3;
	GVector3 r1= row(0);
	GVector3 r2= row(1);
	GVector3 r3 = row(2);
	if(r1.isParallelTo(r2) || r1.isParallelTo(r3) )
	{
		if(r2.isParallelTo(r3) )
			return 1;
		return 2;
	}
	else if(r2.isParallelTo(r1) || r2.isParallelTo(r3) )
	{
		return 2;
	}
	return 0;
}
int GMatrix3x3::nullRows() const
{
	int r,nrows=0;
	for(int row=0; row<3; ++row)
	{
		r= row*3;
		//if( _m[r] == _m[r+1] == _m[r+2] == 0.0f )
		if( areEqual(_m[r],_m[r+1] ) && areEqual(_m[r+1],_m[r+2]) && areEqual(_m[r+2],0.0f)  )
			++nrows;
	}
	return nrows;
}
int GMatrix3x3::nullColumns() const
{
	int ncol=0;
	for(int col=0;col<3;++col)
	{
		//if( _m[col] == _m[col+3] == _m[col+6] == 0.0f )
		if( areEqual( _m[col], _m[col+3] ) && areEqual(_m[col+3],_m[col+6] ) && areEqual( _m[col+6], .0f) )
			++ncol;
	}
	return ncol;
}
GString GMatrix3x3::toString() const
{
	GString str("| ");
	for(int i=0; i<9;++i)
	{
		str+= GString::number(_m[i] ) + " ";
		if(i==2 || i==5)
			str+="|\n| ";
	}
	str+="|\n";
	return str;
}
GMatrix3x3 GMatrix3x3::transpose() const
{
	GMatrix3x3 T;
	//ogni riga di this è una colonna di T
	for(int y = 0; y < 3; ++y)
	{
		for(int x = 0; x < 3; ++x)
		{
			T._m[(x*3)+y]= _m[(y*3)+x];
		}
	}
	return T;
}
void GMatrix3x3::transposeMe()
{
	Real temp;
	for(int y = 0; y < 3; ++y)
	{
		for(int x = y+1; x < 3; ++x)		//swap(vc[x*n + y], vc[y*n + x]);
		{
			temp = _m[(x*3)+y];
			_m[(x*3)+y]=_m[(y*3)+x];
			_m[(y*3)+x]=temp;
		}
	}
}
GMatrix3x3 GMatrix3x3::inverse() const
{
	if(isDiagonal())
    {
        GMatrix3x3 inv;
       inv._m[0] = 1.0f/_m[0];
	   inv._m[4] = 1.0f/_m[4];
	   inv._m[8] = 1.0f/_m[8];
        return inv;
    }
	GMatrix3x3 inv;
	Real k = 1.0f/determinant();
	inv(0,0) = ((operator()(1,1) * operator()(2,2))- ( operator()(2,1)*operator()(1,2)))*k;
	inv(1,0) = -((operator()(1,0) * operator()(2,2))- ( operator()(1,2)*operator()(2,0)))*k;
	inv(2,0) = ((operator()(1,0) * operator()(2,1))- ( operator()(2,0)*operator()(1,1)))*k;

	inv(0,1) = -((operator()(0,1) * operator()(2,2))- ( operator()(0,2)*operator()(2,1)))*k;
	inv(1,1) = ((operator()(0,0) * operator()(2,2))- ( operator()(0,2)*operator()(2,0)))*k;
	inv(2,1) = -((operator()(0,0) * operator()(2,1))- ( operator()(2,0)*operator()(0,1)))*k;

	inv(0,2) = ((operator()(0,1) * operator()(1,2))- ( operator()(0,2)*operator()(1,1)))*k;
	inv(1,2) = -((operator()(0,0) * operator()(1,2))- ( operator()(1,0)*operator()(0,2)))*k;
	inv(2,2) = ((operator()(0,0) * operator()(1,1))- ( operator()(1,0)*operator()(0,1)))*k;

	return inv;
}
GVector<GComplex> GMatrix3x3::eigen() const
{
	if(isDiagonal() || isTriangular() )
	{
		GVector<GComplex> res;
		res.append(_m[0]);
		res.append(_m[4]);
		res.append(_m[8]);
		return res;
	}
	Real B = _m[0] + _m[4] + _m[8];
	Real C = (_m[1]*_m[3])+(_m[6]*_m[2])+(_m[5]*_m[7])-(_m[0]*_m[4])-(_m[0]*_m[8])-(_m[4]*_m[8]);
	Real D = (_m[0]*_m[4]*_m[8])-(_m[0]*_m[5]*_m[7])-(_m[1]*_m[3]*_m[8])+(_m[3]*_m[2]*_m[7])+(_m[6]*_m[1]*_m[5])-(_m[6]*_m[2]*_m[4]);
	GVector<GComplex> s = GEquationSolver::solveCubic(-1.0,B,C,D);
	return s;
}
bool GMatrix3x3::isNull() const
{
	for(int i=0; i<9;++i)
	{
		if(! areEqual(_m[i], 0.0f ) )
			return false;
	}
	return true;
}
bool GMatrix3x3::isIdentity() const
{
	for(int i=0; i<9; ++i)
	{
		if(i==0 || i==4 || i== 8)
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
bool GMatrix3x3::isDiagonal() const
{
	for(int i=0; i<9; ++i)
	{
		if(i==0 || i==4 || i== 8)
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
bool GMatrix3x3::zeroOnDiagonal() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[4],0.0f) ||areEqual(_m[8],0.0f) )
		return true;
	return false;
}
bool GMatrix3x3::isTriangularUpper() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[4],0.0f) ||areEqual(_m[8],0.0f) )
		return false;
	if( areEqual(_m[3],0.0f) && areEqual(_m[6],0.0f) && areEqual(_m[7],0.0f) )
		return true;
	return false;
}
bool GMatrix3x3::isTriangularLower() const
{
	if( areEqual(_m[0],0.0f) || areEqual(_m[4],0.0f) ||areEqual(_m[8],0.0f) )
		return false;
	if( areEqual(_m[1],0.0f) && areEqual(_m[2],0.0f) && areEqual(_m[5],0.0f) )
		return true;
	return false;
}
bool GMatrix3x3::isTriangular() const
{
	if( isTriangularUpper() || isTriangularLower() )
		return true;
	return false;
}
bool GMatrix3x3::isSymmetric() const
{
	if( areEqual(_m[1],_m[3]) && areEqual(_m[2],_m[6]) && areEqual(_m[5],_m[7]) )
		return true;
	return false;
}
bool GMatrix3x3::isAntiSymmetric() const
{
	if( areEqual(_m[1],-_m[3]) && areEqual(_m[2],-_m[6]) && areEqual(_m[5],-_m[7]) )
		return true;
	return false;
}
bool GMatrix3x3::isInvertible() const
{
	if( areEqual( determinant(), 0.0f ) )
		return false;
	else
		return true;
}
bool GMatrix3x3::isOrthogonal() const
{
	//The matrix A is orthogonal if one of this:
    //The transpose is equal to the inverse: [A][A]T = [I]
    //By making the matrix from a set of mutually perpendicular basis vectors.
    //The matrix represents a pure rotation.
	GMatrix3x3 T = transpose();
	if (operator*(T) == Identity() )
		return true;
	return false;
}
void GMatrix3x3::makeItIdentity()
{
	for(int i=0; i<9;++i)
	{
		if(i==0 || i==4 || i==8 )
			_m[i]=1;
		else
			_m[i]=.0f;
	}
}
void GMatrix3x3::replaceRow(const GVector3& row, uint atRow)
{
	if(atRow>3 || atRow<1 )
		return;
	uint r = (atRow-1)*3;
	for(int c=0; c<3; ++c)
	{
		_m[r+c]=row[c];
	}
}
void GMatrix3x3::replaceColumn(const GVector3& col, uint atCol)
{
	if(atCol>3 || atCol <1 )
		return;
	atCol-=1;
	for(uint r=0;r<3;++r)
	{
		_m[atCol +(r*3)]=col[r];
	}
}
void GMatrix3x3::swapRows(uint row1, uint row2)
{
	if(row1==row2 || (row1>3 || row2>3) || ( row1<1 || row2<1 ) )
		return;
	uint r1= (row1-1)*3, r2=(row2-1)*3;
	Real t=0;
	for(int c=0; c<3; ++c)
	{
		t=_m[r1+c];
		_m[r1+c]=_m[r2+c];
		_m[r2+c]=t;
	}
}
void GMatrix3x3::swapColumns(uint col1, uint col2)
{
	if(col1==col2 || (col1>3 || col2>3) || (col1<1 || col2<1) )
		return;
	Real t=0;
	col1-=1;
	col2-=1;
	for(int r=0; r<3;++r)
	{
		t=_m[(r*3)+col1];
		_m[(r*3)+col1] = _m[(r*3)+col2];
		_m[(r*3)+col2] = t;
	}
}

GMatrix3x3 GMatrix3x3::Psi(Radians around_z)
{
	/*
			cos		sin		0
	Rz =	-sin	cos		0
			0		0		1
	*/
	around_z = reduceAngle(around_z);
	if(around_z==.0f )
		return GMatrix3x3::Identity();
	GMatrix3x3 p;
	Real sinz = sin(around_z), cosz= cos(around_z);
	p._m[0]= cosz;
	p._m[1]= sinz;

	p._m[3]= -sinz;
	p._m[4]= cosz;

	p._m[8]= 1.0f;
	return p;
}
GMatrix3x3 GMatrix3x3::Teta(Radians around_y)
{
	/*
		cos		0	-sin
	Ry = 0		1	0
		sin		0	cos
	*/
	around_y = reduceAngle(around_y);
	if( around_y == .0f )
		return GMatrix3x3::Identity();
	GMatrix3x3 T;
	Real siny = sin(around_y), cosy= cos(around_y);
	T._m[0]= cosy;
	T._m[2]= -siny;

	T._m[4]= 1.0f;

	T._m[6]= siny;
	T._m[8]= cosy;

	return T;
}

GMatrix3x3 GMatrix3x3::Fi(Radians around_x)
{
	/*
			1	0	0
	Rx =	0	cos	sin
			0	-sin	cos
	*/
	around_x = reduceAngle(around_x);
	if( around_x==0.0f )
		return GMatrix3x3::Identity();
	GMatrix3x3 F;
	Real sinx = sin(around_x), cosx= cos(around_x);
	F._m[0]= 1.0f;

	F._m[4]= cosx;
	F._m[5]= sinx;

	F._m[7]= -sinx;
	F._m[8]= cosx;

	return F;
}
GMatrix3x3 GMatrix3x3::RotationMatrix(Radians around_x, Radians around_y, Radians around_z)
{
	/*************************************************************************************
	**					cosy*cosz						cosy*sinz			 -siny		**
	**Rxyz = 	-cosx*sinz+sinx*siny*cosz		cosx*cosz+sinx*siny*sinz	sinx*cosy	**
	**			sinx*sinz+cosx*siny*cosz		-sinx*cosz+cosx*siny*sinz	cosx*cosy	**
	**************************************************************************************/
	around_x = reduceAngle(around_x);	around_y = reduceAngle(around_y);	around_z = reduceAngle(around_z);
	if( (around_x== .0f) && (around_y== .0f) && (around_z== .0f) )
		return GMatrix3x3::Identity();

	Real cosx=cos(around_x), sinx=sin(around_x);
	Real cosy=cos(around_y), siny=sin(around_y);
	Real cosz=cos(around_z), sinz=sin(around_z);
	GMatrix3x3 R;
	R._m[0]= cosy*cosz;
	R._m[1]= cosy*sinz;
	R._m[2]=-siny;

	R._m[3]=-(cosx*sinz)+(sinx*siny*cosz);
	R._m[4]= (cosx*cosz)+(sinx*siny*sinz);
	R._m[5]= sinx*cosy;

	R._m[6]= (sinx*sinz)+(cosx*siny*cosz);
	R._m[7]=-(sinx*cosz)+(cosx*siny*sinz);
	R._m[8]= cosx*cosy;
	return R;
}

GMatrix3x3 GMatrix3x3::RotationMatrix(const GVector3& u, Radians teta)
{
	/*********************************************************************************************
	**			cost+ux*ux*(1-cost)			ux*uy*(1-cost)+ uz*sint		ux*uz*(1-cost)-uy*sint	**
	**	R = 	ux*uy*(1-cost)-uz*sint		cost+uy*uy*(1-cost)			uy*uz*(1-cost)+ux*sint	**
	**			ux*uz*(1-cost)+uy*sint		uz*uy*(1-cost)-ux*sint		cost+uz*uz*(1-cost)		**
	**********************************************************************************************/
	teta = reduceAngle(teta);
	if( teta == 0.0f )
		return GMatrix3x3::Identity();
	Real cost= cos(teta), sint=sin(teta);
	GMatrix3x3 R;
	R._m[0]= cost+(u.x()*u.x()*(1-cost));
	R._m[1]= (u.x()*u.y()*(1-cost))+ (u.z()*sint);
	R._m[2]= (u.x()*u.z()*(1-cost))-(u.y()*sint);

	R._m[3]= (u.x()*u.y()*(1-cost))-(u.z()*sint);
	R._m[4]= cost+(u.y()*u.y()*(1-cost));
	R._m[5]= (u.y()*u.z()*(1-cost) )+ (u.x()*sint);

	R._m[6]= (u.x()*u.z()*(1-cost))+(u.y()*sint);
	R._m[7]= (u.z()*u.y()*(1-cost))-(u.x()*sint);
	R._m[8]= cost+ (u.z()*u.z()*(1-cost));
	return R;
}

/*
GMatrix3x3 GMatrix3x3::operator*(const GMatrix3x3& other) const
{
Real result[9]={0,0,0,0,0,0,0,0,0};
	// Create C++ AMP objects.
	concurrency::extent<2> e(3,3);
	concurrency::array_view<const Real,2> A(e,_m), B(e,other._m);
	concurrency::array_view<Real,2> product(e,result);
	product.discard_data();
	// Define the code to run on each thread on the accelerator.
	concurrency::parallel_for_each(e, [=](concurrency::index<2> idx) restrict(amp)
	{
            int row = idx[0];    int col = idx[1];
			Real sum = .0f;
            for (int inner = 0; inner < B.extent[0]; ++inner) 
			{
                sum += A(row, inner) * B(inner, col);
            }
			product[idx]= sum;
    });
	//back to CPU
	product.synchronize();
	return GMatrix3x3(result);
}

*/