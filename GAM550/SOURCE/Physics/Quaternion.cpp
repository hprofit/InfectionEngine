#include <Stdafx.h>

void Quaternion::normalise()
{
  real d = r * r + i * i + j * j + k * k;

  // Check for zero length quaternion, and use the no-rotation
  // quaternion in that case.
  if (d < real_epsilon) {
    r = 1;
    return;
  }

  d = ((real)1.0) / real_sqrt(d);
  r *= d;
  i *= d;
  j *= d;
  k *= d;
}


void Quaternion::operator *=(const Quaternion &multiplier)
{
  Quaternion q = *this;
  r = q.r*multiplier.r - q.i*multiplier.i -
    q.j*multiplier.j - q.k*multiplier.k;
  i = q.r*multiplier.i + q.i*multiplier.r +
    q.j*multiplier.k - q.k*multiplier.j;
  j = q.r*multiplier.j + q.j*multiplier.r +
    q.k*multiplier.i - q.i*multiplier.k;
  k = q.r*multiplier.k + q.k*multiplier.r +
    q.i*multiplier.j - q.j*multiplier.i;
}


void Quaternion::addScaledVector(const Vector3D& vector, real scale)
{
  Quaternion q(0,
    vector.x * scale,
    vector.y * scale,
    vector.z * scale);
  q *= *this;
  r += q.r * ((real)0.5);
  i += q.i * ((real)0.5);
  j += q.j * ((real)0.5);
  k += q.k * ((real)0.5);
}

void Quaternion::rotateByVector(const Vector3D& vector)
{
  Quaternion q(0, vector.x, vector.y, vector.z);
  (*this) *= q;
}

Quaternion Quaternion::operator+(const Quaternion & Q2)
{
	Quaternion Q1 = *this;

	Quaternion result;

	result.r = Q1.r + Q2.r;
	result.i = Q1.i + Q2.i;
	result.j = Q1.j + Q2.j;
	result.k = Q1.k + Q2.k;

	return result;
}

Quaternion Quaternion::operator-(const Quaternion & Q2)
{
	Quaternion Q1 = *this;

	Quaternion result;

	result.r = Q1.r - Q2.r;
	result.i = Q1.i - Q2.i;
	result.j = Q1.j - Q2.j;
	result.k = Q1.k - Q2.k;

	return result;
}

Quaternion Quaternion::operator*(const Quaternion & Q2)
{
	Quaternion Q1 = *this;

	Quaternion result;

	result.r = Q1.r * Q2.r;
	result.i = Q1.i * Q2.i;
	result.j = Q1.j * Q2.j;
	result.k = Q1.k * Q2.k;

	return result;
}

Quaternion Quaternion::QuatScalarMult(Quaternion Q, real value)
{
	Quaternion result;

	result.r = value * Q.r;
	result.i = value * Q.i;
	result.j = value * Q.j;
	result.k = value * Q.k;

	return result;
}

Quaternion Quaternion::QuatScalarDiv(Quaternion Q, real value)
{
	Quaternion result;

	result.r = Q.r / value;
	result.i = Q.i / value;
	result.j = Q.j / value;
	result.k = Q.k / value;

	return result;
}

void Quaternion::QuatSetIdentity()
{
	Quaternion &Q = *this;

	Q.r = 1;
	Q.i = 0;
	Q.j = 0;
	Q.k = 0;

}

Quaternion Quaternion::QuatSetIdentity(Quaternion & Q)
{
	Q.r = 1.0f;
	Q.i = 0.0f;
	Q.j = 0.0f;
	Q.k = 0.0f;


	return Q;
}

Quaternion Quaternion::QuatInverse(Quaternion Q)
{
	real divided = Q.r * Q.r + Q.i * Q.i + Q.j * Q.j + Q.k * Q.k;

	Q.r = Q.r / divided;
	Q.i = -Q.i / divided;
	Q.j = -Q.j / divided;
	Q.k = -Q.k / divided;

	return Q;
}

real Quaternion::QuatLength()
{
	real result;
	Quaternion &Q = *this;


	real length = Q.r * Q.r + Q.i * Q.i + Q.j * Q.j + Q.k * Q.k;
	result = sqrtf(length);

	return result;
}

real Quaternion::QuatLength(Quaternion Q)
{
	real result;

	real length = Q.r * Q.r + Q.i * Q.i + Q.j * Q.j + Q.k * Q.k;
	result = sqrtf(length);

	return result;
}

real Quaternion::QuatDotProduct(Quaternion Q2)
{
	Quaternion Q1 = *this;

	real result;

	result = Q1.r * Q2.r + Q1.i * Q2.i + Q1.j * Q2.j + Q1.k * Q2.k;

	return result;
}

real Quaternion::QuatDotProduct(Quaternion Q1, Quaternion Q2)
{
	real result;

	result = Q1.r * Q2.r + Q1.i * Q2.i + Q1.j * Q2.j + Q1.k * Q2.k;

	return result;
}

void Quaternion::QuatConjugate()
{
	Quaternion &Q = *this;

	Q.i = -Q.i;
	Q.j = -Q.j;
	Q.k = -Q.k;
}

void Quaternion::QuatConjugate(Quaternion& Q)
{
	Q.i = -Q.i;
	Q.j = -Q.j;
	Q.k = -Q.k;
}

void Quaternion::QuatNegation()
{
	Quaternion &Q = *this;

	Q.r = -Q.r;
	Q.i = -Q.i;
	Q.j = -Q.j;
	Q.k = -Q.k;

}

Matrix3x3 Quaternion::GetMatrixRepresentation()
{
	Quaternion& Q = *this;
	Matrix3x3 result(
		(1.0) - (2.0) * (Q.j * Q.j + Q.k * Q.k), (2.0) * (Q.i * Q.j - Q.k * Q.r),			(2.0) * (Q.i * Q.k + Q.j * Q.r),
		(2.0) * (Q.i * Q.j + Q.k * Q.r),		 (1.0) - (2.0) * (Q.i * Q.i + Q.k * Q.k),	(2.0) * (Q.j * Q.k - Q.i * Q.r),
		(2.0) * (Q.i * Q.k - Q.j * Q.r),		 (2.0) * (Q.j * Q.k + Q.i * Q.r),			(1.0) - (2.0) * (Q.i * Q.i + Q.j * Q.j) );

	return result;
}

Quaternion Quaternion::DegreeToQuaternion(Vector3D angle)
{
  float x = sin(angle.y / 2.f * DEG_TO_RAD) * sin(angle.z / 2.f * DEG_TO_RAD) * cos(angle.x / 2.f * DEG_TO_RAD) + cos(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float y = sin(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) + cos(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float z = cos(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) - sin(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);
  float w = cos(angle.y / 2.f * DEG_TO_RAD)* cos(angle.z / 2.f * DEG_TO_RAD)* cos(angle.x / 2.f * DEG_TO_RAD) - sin(angle.y / 2.f * DEG_TO_RAD)* sin(angle.z / 2.f * DEG_TO_RAD)* sin(angle.x / 2.f * DEG_TO_RAD);

  return Quaternion(w, x, y, z); //r(1), i(0), j(0), k(0) 
}

Vector3D Quaternion::QuaternionToDegree(Quaternion q)
{
  Vector3D result;
  float ysqr = q.j * q.j;
  float t0 = -2.0f * (ysqr + q.k * q.k) + 1.0f;
  float t1 = +2.0f * (q.i * q.j - q.r * q.k);
  float t2 = -2.0f * (q.i * q.k + q.r * q.j);
  float t3 = +2.0f * (q.j * q.k - q.r * q.i);
  float t4 = -2.0f * (q.i * q.i + ysqr) + 1.0f;

  t2 = t2 > 1.0f ? 1.0f : t2;
  t2 = t2 < -1.0f ? -1.0f : t2;

  result.y = std::asin(t2) * RAD_TO_DEG;
  result.x = std::atan2(t3, t4) * RAD_TO_DEG;
  result.z = std::atan2(t1, t0) * RAD_TO_DEG;
  return result;
}