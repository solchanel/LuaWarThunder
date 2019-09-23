#pragma once
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include <DirectXMath.h>

#define CHECK_VALID(_v ) 0
#define Assert( _exp ) ((void)0)

#define M_PI		3.14159265358979323846f
#define M_RADPI		57.295779513082f
#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG( x )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x )  ( (float)(x) * (float)(M_PI_F / 180.f) )

class Vector;
class Angle;

namespace M
{
	bool DirectXWorldToScreen(const Vector& point, Vector &screen);
	bool ScreenTransform(const Vector& point, ImVec2& screen);
	bool WorldToScreen(const Vector& origin, ImVec2& screen);
	bool WS2(const Vector &point, ImVec2 &screen);
	//Vector VectorTransform(const Vector in, float *matrix);
	void VectorTransform(Vector& in1, Vector *in2, Vector& out);
	void MatrixAngles(const Vector matrix[3], Angle &Angles);

	void VectorAngles(const Vector& forward, Angle& angles);
	float GetFOV(const Angle &viewangles, const Angle &aimangles);

	void AngleVectors(const Angle& angles, Vector* forward);
	Angle CalcAngle(Vector src, Vector dst);

	void NormalizeAngles(Angle& angles);
}

class AngleByValue;
class Angle
{
public:
	float x, y, z;

	Angle(void);
	Angle(float X, float Y, float Z);

	operator AngleByValue &()
	{
		return *((AngleByValue *)(this));
	}

	operator const AngleByValue &() const
	{
		return *((const AngleByValue *)(this));
	}

	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	bool IsValid() const;
	void Invalidate();

	bool IsZero()
	{
		CHECK_VALID(*this);
		if (this->x == 0.f && this->y == 0.f && this->z == 0.f)
			return true;

		return false;
	}

	float operator[](int i) const;
	float& operator[](int i);

	float* Base();
	float const* Base() const;

	bool operator==(const Angle& v) const;
	bool operator!=(const Angle& v) const;

	Angle& operator+=(const Angle& v);
	Angle& operator-=(const Angle& v);
	Angle& operator*=(float s);
	Angle& operator/=(float s);

	float Length() const;
	float LengthSqr() const;

	Angle& operator=(const Angle& src);

	Angle operator-(void) const;

	Angle operator+(const Angle& v) const;
	Angle operator-(const Angle& v) const;
	Angle operator*(float fl) const;
	Angle operator/(float fl) const;

	Angle Clamp();
	Angle Mod(float N);

	inline auto Normalize()
	{
		auto x_rev = this->x / 360.f;
		if (this->x > 180.f || this->x < -180.f)
		{
			x_rev = std::abs(x_rev);
			x_rev = std::round(x_rev);

			if (this->x < 0.f)
				this->x = (this->x + 360.f * x_rev);

			else
				this->x = (this->x - 360.f * x_rev);
		}

		auto y_rev = this->y / 360.f;
		if (this->y > 180.f || this->y < -180.f)
		{
			y_rev = std::abs(y_rev);
			y_rev = std::round(y_rev);

			if (this->y < 0.f)
				this->y = (this->y + 360.f * y_rev);

			else
				this->y = (this->y - 360.f * y_rev);
		}

		auto z_rev = this->z / 360.f;
		if (this->z > 180.f || this->z < -180.f)
		{
			z_rev = std::abs(z_rev);
			z_rev = std::round(z_rev);

			if (this->z < 0.f)
				this->z = (this->z + 360.f * z_rev);

			else
				this->z = (this->z - 360.f * z_rev);
		}
	}

	Angle Normalized()
	{
		if (this->x != this->x)
			this->x = 0;
		if (this->y != this->y)
			this->y = 0;
		if (this->z != this->z)
			this->z = 0;

		if (this->x > 89.f)
			this->x = 89.f;
		if (this->x < -89.f)
			this->x = -89.f;

		while (this->y > 180)
			this->y -= 360;
		while (this->y <= -180)
			this->y += 360;

		if (this->y > 180.f)
			this->y = 180.f;
		if (this->y < -180.f)
			this->y = -180.f;

		this->z = 0;

		return *this;
	}

	std::string ToString(std::string per, unsigned prec = 2);
};

inline std::string Angle::ToString(std::string per, unsigned prec)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(prec) << x << per << y << per << z;
	return ss.str();
}

inline Angle::Angle(void)
{
#ifdef _DEBUG
#ifdef VECTOR_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = float_NAN;
#endif
#endif
}

inline Angle::Angle(float X, float Y, float Z)
{
	x = X;
	y = Y;
	z = Z;
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void Angle::Init(float ix, float iy, float iz)
{
	x = ix;
	y = iy;
	z = iz;
	CHECK_VALID(*this);
}

inline void Angle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline Angle& Angle::operator=(const Angle& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x;
	y = vOther.y;
	z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool Angle::operator==(const Angle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Angle::operator!=(const Angle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline Angle& Angle::operator+=(const Angle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Angle& Angle::operator-=(const Angle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Angle& Angle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}

inline Angle& Angle::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* Angle::Base()
{
	return (float*)this;
}

inline float const* Angle::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& Angle::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

inline float Angle::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float Angle::Length() const
{
	CHECK_VALID(*this);
	return (float)sqrt(LengthSqr()); //todo replace with fastsqrt
}

inline float Angle::LengthSqr() const
{
	CHECK_VALID(*this);
	return x * x + y * y;
}

inline Angle Angle::operator-(void) const
{
	return Angle(-x, -y, -z);
}

inline Angle Angle::operator+(const Angle& v) const
{
	Angle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline Angle Angle::operator-(const Angle& v) const
{
	Angle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline Angle Angle::operator*(float fl) const
{
	Angle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline Angle Angle::operator/(float fl) const
{
	Angle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline Angle Angle::Clamp()
{
	CHECK_VALID(*this);

	if (this->y < -89.0f)
		this->y = -89.0f;

	if (this->y > 89.0f)
		this->y = 89.0f;

	while (this->x < -180.0f)
		this->x += 360.0f;

	while (this->x > 180.0f)
		this->x -= 360.0f;

	this->z = 0.0f;

	return *this;
}

inline Angle Angle::Mod(float N)
{
	CHECK_VALID(*this);
	this->x = fmod(x, N);
	this->y = fmod(y, N);
	this->z = fmod(z, N);

	return *this;
}

class Vector
{
public:
	float x, y, z;
	Vector(void);
	Vector(float X, float Y, float Z);
	Vector(ImVec2 &v);
	Vector(float v[3])
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}
	Vector(DirectX::XMVECTOR &v)
	{
		x = DirectX::XMVectorGetX(v);
		y = DirectX::XMVectorGetY(v);
		z = DirectX::XMVectorGetZ(v);
	}
	void Init(float ix = 0.f, float iy = 0.f, float iz = 0.f);
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	__forceinline Vector&	operator+=(const Vector &v);
	__forceinline Vector&	operator-=(const Vector &v);
	__forceinline Vector&	operator*=(const Vector &v);
	__forceinline Vector&	operator*=(float s);
	__forceinline Vector&	operator/=(const Vector &v);
	__forceinline Vector&	operator/=(float s);
	__forceinline Vector&	operator+=(float fl);
	__forceinline Vector&	operator-=(float fl);
	inline float	Length() const;
	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMVectorSet(x, y, z, 0.f);
	}
	operator ImVec2() const
	{
		return ImVec2{ x, y };
	}
	__forceinline float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (x*x + y * y + z * z);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}
	Vector	NormalizeInPlace();
	Vector fNormalize(const float ir_frac);
	Vector  Normalize(float coeff);
	__forceinline float	DistToSqr(const Vector &vOther) const;
	float	Dot(const Vector& vOther) const;
	float	Length2D(void) const;
	float	Length2DSqr(void) const;
	Vector& operator=(const Vector &vOther);
	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;

	void Rotate2D(const float &f);
	void Rotate(const Angle &);
	std::string ToString(unsigned prec = 2);
};

inline std::string Vector::ToString(unsigned prec)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(prec) << x << "," << y << "," << z;
	return ss.str();
}

inline void SinCos(const float &r, float &s, float &c)
{
	s = sin(r);
	c = cos(r);
}

inline void Vector::Rotate(const Angle &e)
{
	float _x, _y, _z;

	float s, c;

	SinCos(DEG2RAD(e.x), s, c);

	_y = y;
	_z = z;

	y = (_y * c) - (_z * s);
	z = (_y * s) + (_z * c);

	SinCos(DEG2RAD(e.y), s, c);

	_x = x;
	_z = z;

	x = (_x * c) + (_z * s);
	z = (-_x * s) + (_z * c);

	SinCos(DEG2RAD(e.z), s, c);

	_x = x;
	_y = y;

	x = (_x * c) - (_y * s);
	y = (_x * s) + (_y * c);
}

inline void Vector::Rotate2D(const float &f)
{
	float _x, _y;

	float s, c;

	SinCos(DEG2RAD(f), s, c);

	_x = x;
	_y = y;

	x = (_x * c) - (_y * s);
	y = (_x * s) + (_y * c);
}
inline void Vector::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}
inline Vector::Vector(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

inline Vector::Vector(ImVec2 &v)
{
	x = v.x;
	x = v.y;
	z = 0.f;
}
inline Vector::Vector(void)
{
}
inline void Vector::Zero()
{
	x = y = z = 0.f;
}
inline void VectorClear(Vector& a)
{
	a.x = a.y = a.z = 0.f;
}
inline Vector& Vector::operator=(const Vector &vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
inline float& Vector::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
inline float Vector::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
inline bool Vector::operator==(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}
inline bool Vector::operator!=(const Vector& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}
__forceinline void VectorCopy(const Vector& src, Vector& dst)
{
	CHECK_VALID(src);
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
__forceinline  Vector& Vector::operator+=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}
__forceinline  Vector& Vector::operator-=(const Vector& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
__forceinline  Vector& Vector::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}
__forceinline  Vector& Vector::operator*=(const Vector& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}

__forceinline Vector&	Vector::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID(*this);
	return *this;
}

__forceinline Vector&	Vector::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID(*this);
	return *this;
}

__forceinline  Vector& Vector::operator/=(float fl)
{
	Assert(fl != 0.f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

__forceinline  Vector& Vector::operator/=(const Vector& v)
{
	CHECK_VALID(v);
	Assert(v.x != 0.f && v.y != 0.f && v.z != 0.f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}

inline float Vector::Length(void) const
{
	CHECK_VALID(*this);
	return std::sqrtf(x * x + y * y + z * z);
}

inline float Vector::Length2D(void) const
{
	CHECK_VALID(*this);
	return std::sqrtf(x * x + z * z);
	//return std::sqrtf(x * x + y * y);
}

inline float Vector::Length2DSqr(void) const
{
	return (x*x + y * y);
}

inline Vector CrossProduct(const Vector& a, const Vector& b)
{
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float Vector::DistToSqr(const Vector &vOther) const
{
	Vector delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}

//Vector Vector::NormalizeInPlace(const float ir_frac)
//{
//	Vector& v = *this;
//
//	float iradius = ir_frac / (this->Length() + 1.192092896e-07F); //FLT_EPSILON
//
//	v.x *= iradius;
//	v.y *= iradius;
//	v.z *= iradius;
//	return v;
//}

inline Vector Vector::NormalizeInPlace()
{
	Vector& v = *this;

	float iradius = 1.0f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

	v.x *= iradius;
	v.y *= iradius;
	v.z *= iradius;
	return v;
}

inline Vector Vector::fNormalize(const float ir_frac)
{
	Vector vector;
	float length = this->Length();

	if (length != 0)
	{
		vector.x = x * ir_frac / length;
		vector.y = y * ir_frac / length;
		vector.z = z * ir_frac / length;
	}
	else
		vector.x = vector.y = 0.0f;
	vector.z = 1.0f;

	return vector;
}

inline Vector Vector::Normalize(float coeff = 1.0f)
{
	Vector vector;
	float length = this->Length();

	if (length <= 0.000000001)
	{
		vector.x = x * (coeff / length);
		vector.y = y * (coeff / length);
		vector.z = z * (coeff / length);
	}
	else
	{
		vector.x = vector.z = 0.0f;
		vector.y = 1.0f;
	}

	return vector;
}

inline Vector Vector::operator+(const Vector& v) const
{
	Vector res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline Vector Vector::operator-(const Vector& v) const
{
	Vector res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline Vector Vector::operator*(float fl) const
{
	Vector res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline Vector Vector::operator*(const Vector& v) const
{
	Vector res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

inline Vector Vector::operator/(float fl) const
{
	Vector res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline Vector Vector::operator/(const Vector& v) const
{
	Vector res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
inline float Vector::Dot(const Vector& vOther) const
{
	const Vector& a = *this;

	return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}