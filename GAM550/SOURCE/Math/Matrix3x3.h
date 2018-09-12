/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Matrix3x3.h
Purpose: Provides a 3 x 3 Matrix and associated functionality
Language: C++
Project: GAM541
Author: Holden Profit
Creation date: 1/17/18
- End Header --------------------------------------------------------*/

//#pragma once

#ifndef MATRIX3X3_H
#define MATRIX3X3_H

//#include "Vector2D.h"
//#include "Vector3D.h"
class Quaternion;
class Matrix3x3
{
private:
	float m_matrix[3][3];

public:
	Matrix3x3();
	Matrix3x3(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
  Matrix3x3(const Vector3D &compOne, const Vector3D &compTwo, const Vector3D &compThree);
	Matrix3x3(const Matrix3x3 &other); // Copy Ctor
	Matrix3x3& operator= (const Matrix3x3 &other); // Assignment Operator
	~Matrix3x3();

	float Get(int row, int col) const;
	void Set(int row, int col, float val);
	void Transpose();
	void Zero();
	void Identity();
	float Determinant() const;
	void Print() const;

	static Matrix3x3 Zero3D();
	static Matrix3x3 Identity3D();
	static float Determinant(const Matrix3x3& other);
	static Matrix3x3 Translate(float x, float y);
	static Matrix3x3 Scale(float x, float y);
	static Matrix3x3 RotateDeg_Z(float degrees);
	static Matrix3x3 RotateRad_Z(float radians);
  //physics
  void setDiagonal(real a, real b, real c);
  //Sets the value of the matrix from inertia tensor values.
  void setInertiaTensorCoeffs(real ix, real iy, real iz, real ixy = 0, real ixz = 0, real iyz = 0);
  /**
    * Sets the value of the matrix as an inertia tensor of
    * a rectangular block aligned with the body's coordinate
    * system with the given axis half-sizes and mass.
    */
  void setBlockInertiaTensor(const Vector3D &halfSizes, real mass);
  /**
    * Sets the matrix to be a skew symmetric matrix based on
    * the given vector. The skew symmetric matrix is the equivalent
    * of the vector product. So if a,b are vectors. a x b = A_s b
    * where A_s is the skew symmetric form of a.
    */
  void setSkewSymmetric(const Vector3D vector);
  /**
    * Sets the matrix values from the given three vector components.
    * These are arranged as the three columns of the vector.
    */
  void setComponents(const Vector3D &compOne, const Vector3D &compTwo, const Vector3D &compThree);
  Vector3D transform(const Vector3D &vector);
  Vector3D transformTranspose(const Vector3D &vector) const;
  Vector3D getRowVector(int i) const;
  Vector3D getAxisVector(int i) const;
  void setInverse(const Matrix3x3 &m);
  Matrix3x3 inverse() const;
  void invert();
  void setTranspose(const Matrix3x3 &m);
  Matrix3x3 transpose() const;
  void setOrientation(const Quaternion &q);
  static Matrix3x3 linearInterpolate(const Matrix3x3& a, const Matrix3x3& b, real prop);

	bool operator== (const  Matrix3x3& other);
	bool operator!= (const  Matrix3x3& other);
	Matrix3x3 operator+ (const Matrix3x3& other);
	Matrix3x3 operator- (const Matrix3x3& other);
	Matrix3x3 operator* (const Matrix3x3& other);
	Matrix3x3 operator* (const float scalar);
	Matrix3x3 operator/ (const float divisor);

	Vector2D operator* (const Vector2D& other);

	Vector3D operator* (const Vector3D& other);
  void operator*=(const Matrix3x3 &o);
  void operator*=(const real scalar);
  void operator+=(const Matrix3x3 &o);

};

#if TEST_MODE
void Matrix3x3Tests();
#endif

#endif