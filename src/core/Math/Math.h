#ifndef __Math_H
#define __Math_H
namespace Math
{
	//constants
	static const double EPSILON  = 4.37114e-05;
	static const double EPSILON2  = EPSILON*EPSILON;
	static const double M_PI     = 3.14159265358979323846;
	static const double M_2PI    = 6.28318530717958707692;
	static const double M_HALFPI = M_PI * 0.5;

	/*
	==========================
	SquareRootFloat by Carmack
	==========================
	*/
	inline float sqrtf_fast(float number) {
		long i;
		float x, y;
		const float f = 1.5F;

		x = number * 0.5F;
		y  = number;
		i  = * ( long * ) &y;
		i  = 0x5f3759df - ( i >> 1 );
		y  = * ( float * ) &i;
		y  = y * ( f - ( x * y * y ) );
		y  = y * ( f - ( x * y * y ) );
		return number * y;
	}


	inline float toRadians( float x )
	{
		return x * 0.017453292519943295769f; // ( x * PI / 180 )
	}

	inline double toRadians( double x )
	{
		return x * 0.017453292519943295769; // ( x * PI / 180 )
	}

	inline float toDegrees( float x )
	{
		return x * 57.295779513082321f; // ( x * 180 / PI )
	}

	inline double toDegrees( double x )
	{
		return x * 57.295779513082321; // ( x * 180 / PI )
	}

	/**
	* Performs a lerp operation against all suported types
	*/
	template<typename T, typename L>
	T lerp( const T &a, const T &b, L factor )
	{
		return a + ( b - a ) * factor;
	}

}

#endif