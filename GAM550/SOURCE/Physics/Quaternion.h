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
};
#endif