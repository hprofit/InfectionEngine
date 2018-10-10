#ifndef Quaternion_H
#define Quaternion_H

class Quaternion
{
public:
  union {
    struct {
      real r;
      real i;
      real j;
      real k;
    };
    real data[4];
  };
  Quaternion() : r(1), i(0), j(0), k(0) {}
  Quaternion(const real r, const real i, const real j, const real k)
    : r(r), i(i), j(j), k(k) {}


  void normalise();


  void operator *=(const Quaternion &multiplier);


  void addScaledVector(const Vector3D& vector, real scale);

  void rotateByVector(const Vector3D& vector);

  Quaternion DegreeToQuaternion(Vector3D angle);

  Vector3D QuaternionToDegree(Quaternion q);
  //========================================================
  //Additional Quaternion Functions
  //========================================================


  Quaternion operator+ (const Quaternion& Q2);

  Quaternion operator- (const Quaternion& Q2);

  Quaternion operator* (const Quaternion& Q2);

  Quaternion QuatScalarMult(Quaternion Q, real value);

  Quaternion QuatScalarDiv(Quaternion Q, real value);

  void QuatSetIdentity();

  Quaternion QuatSetIdentity(Quaternion &Q);

  Quaternion QuatInverse(Quaternion Q);

  real QuatLength();

  real QuatLength(Quaternion Q);

  real QuatDotProduct(Quaternion Q2);

  real QuatDotProduct(Quaternion Q1, Quaternion Q2);

  void QuatConjugate(Quaternion & Q);

  void QuatConjugate();

  void QuatNegation();

  Matrix3x3 GetMatrixRepresentation();


};
#endif