/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>
#include "MathFunctions.h"

std::unordered_map<unsigned short, std::vector<unsigned int> > pascalTriangle;

static unsigned int _Factorial(unsigned int num)
{
	if (num == 0)	return 1;
	return (num == 1 ? num : num * _Factorial(num - 1));
}

static void _GeneratePascalRow(unsigned short degree) {
	std::vector<unsigned int> row;
	row.reserve(degree + 1);
	row.push_back(1);
	unsigned int dF = _Factorial(degree);
	// (d, i) = (i!) / (d! * (i - d)!)
	for (int i = 1; i < degree + 1; ++i) {
		row.push_back(dF / (_Factorial(i) * _Factorial(degree - i)));
	}
	pascalTriangle[degree] = row;
}

Quaternion QuatLerp(Quaternion Q1, Quaternion Q2, real interpolation_value)
{
	Quaternion sub = Q2 - Q1;
	Quaternion mul;
	mul.QuatScalarMult(sub, interpolation_value);
	return (Q1 + mul);
}

Quaternion QuatSlerp(Quaternion Q1, Quaternion Q2, real interpolation_value)
{
	real d;
	d = Q1.QuatDotProduct(Q2);
	Quaternion res;

	if (d > 0.9995)
	{
		//Do lerp
		res = QuatLerp(Q1, Q2, interpolation_value);
		res.normalise();

		return res;
	}

	if (d < 0)
	{
		//negate everything
		Q2.QuatNegation();
		d = -d;
	}

	//Cos inverse of the d
	real angle = acos(d);

	Quaternion parta = Q1.QuatScalarMult(Q1, sinf((1 - interpolation_value)*angle));
	Quaternion partb = Q2.QuatScalarMult(Q2, sinf(interpolation_value * angle));
	Quaternion partab = parta - partb;

	return	partab.QuatScalarDiv(partab, sinf(angle));

}


Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t, bool isEaseIn, bool isEaseOut) {
	return Vector3D(Lerp(vectorA.x, vectorB.x, t, isEaseIn, isEaseOut), Lerp(vectorA.y, vectorB.y, t, isEaseIn, isEaseOut), Lerp(vectorA.z, vectorB.z, t, isEaseIn, isEaseOut), Lerp(vectorA.w, vectorB.w, t, isEaseIn, isEaseOut));
}

Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t, bool isEaseIn, bool isEaseOut) {
	return Vector2D(Lerp(vectorA.x, vectorB.x, t, isEaseIn, isEaseOut), Lerp(vectorA.y, vectorB.y, t, isEaseIn, isEaseOut), Lerp(vectorA.w, vectorB.w, t, isEaseIn, isEaseOut));
}

float Lerp(float valueA, float valueB, float t, bool isEaseIn, bool isEaseOut) {
	if (isEaseIn && isEaseOut) {
		t = t*t*(3.0f - 2.0f*t);
	}
	else if (isEaseIn) {
		t = 1 - cosf(t*PI*0.5f);
	}
	else if (isEaseOut) {
		t = sinf(t*PI*0.5f);
	}

	if (t > 1)
		t = 1;
	else if (t < 0)
		t = 0;

	return valueA*(1.f-t) + (t)* valueB;
}

float SinWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*sinf(frequency*time + phaseShift);
}

float CosWave(float amplitude, float frequency, float phaseShift, float time) {
	return amplitude*cosf(frequency*time + phaseShift);
}

float RandomFloat(float min, float max) {
	return (static_cast<float>(Rand()) / static_cast<float>(INFECT_RAND_MAX)) * (max-min) + min;
}

int RandomInt(int min, int max) {
	if (min == 0 && max == 0)
		return 0;

	return Rand() % (max-min) + min;
}

unsigned int GetPascalEntry(unsigned short degree, unsigned short index)
{
	if (pascalTriangle.find(degree) == pascalTriangle.end())
		_GeneratePascalRow(degree);

	return pascalTriangle[degree][index];
}

static float BernsteinFormula(const unsigned short& d, const float& t, const float& tPow, const unsigned short& i) {
	float dci = float(GetPascalEntry(d, i));
	return std::powf((1.f - t), float(d - i)) * tPow * dci;
}

float BezierInterpolation(const std::vector<Point>& points, const float& t) {
	if (points.size() == 0)	return 0.f;

	unsigned short d = (unsigned short)points.size() - 1;
	float tPow = 1.f;
	float returnValue = points[0].y * BernsteinFormula(d, t, tPow, 0);
	for (unsigned short i = 1; i < points.size(); i++) {
		tPow *= t;
		returnValue += points[i].y * BernsteinFormula(d, t, tPow, i);
	}
	return returnValue;
}

float ClampAngleTo180(float angle)
{
	if (angle > 180.f)
		return angle - float(int(angle) % 180) * 180.f;
	else if (angle < -180.f)
		return angle + float(int(-angle) % 180) * 180.f;
	return angle;
}

bool IsSimilar(float a, float b)
{
	return double(abs(a - b)) < EPSILON;
}

long holdrand = 0;

void SeedRand(unsigned int seed)
{
	//srand(seed);
	holdrand = (long)seed;
}

int Rand()
{
	//return rand();
	holdrand = (holdrand * 214012L + 2531011L) >> 16;
	return holdrand & INFECT_RAND_MAX;
}

float Rand_Zero_One()
{
	float val = (float)Rand();
	return val / (float)INFECT_RAND_MAX;
}

float cot(float val)
{
	return cos(val) / sin(val);
}

#define MATH_E 2.71828f

float GetGaussianWeight(float width, float i)
{
	float s = width / 2.0f;
	float iDivS = i / s;
	float power = -0.5f * (iDivS * iDivS);

	return pow(MATH_E, power);
}
