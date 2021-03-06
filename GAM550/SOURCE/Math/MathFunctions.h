/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

Quaternion QuatLerp(Quaternion Q1, Quaternion Q2, real interpolation_value);
Quaternion QuatSlerp(Quaternion Q1, Quaternion Q2, real interpolation_value);

Vector3D Lerp(const Vector3D& vectorA, const Vector3D& vectorB, float t, bool isEaseIn = false, bool isEaseOut = false);
Vector2D Lerp(const Vector2D& vectorA, const Vector2D& vectorB, float t, bool isEaseIn = false, bool isEaseOut = false);
float Lerp(float valueA, float valueB, float t, bool isEaseIn = false, bool isEaseOut = false);
float SinWave(float amplitude, float frequency, float phaseShift, float time);
float CosWave(float amplitude, float frequency, float phaseShift, float time);
float RandomFloat(float min, float max);
int RandomInt(int min, int max);

unsigned int GetPascalEntry(unsigned short degree, unsigned short index);
float BezierInterpolation(const std::vector<Point>& points, const float& t);

// Given an angle, returns a value between -180 and +180
float ClampAngleTo180(float angle);

bool IsSimilar(float a, float b);

void SeedRand(unsigned int seed);
int Rand();
float Rand_Zero_One();

float cot(float val);

float GetGaussianWeight(float width, float i);


#endif