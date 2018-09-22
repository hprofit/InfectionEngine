
#ifndef PARTICLE_H
#define PARTICLE_H

namespace physics {

    /**
     * A particle is the simplest object that can be simulated in the
     * physics system.
     */
    class Particle
    {
    public:

    protected:
        /**
         * Holds the inverse of the mass of the particle. It
         * is more useful to hold the inverse mass because
         * integration is simpler.
         */
        real inverseMass;

        /**
         * Holds the amount of damping applied to linear
         * motion. 
         */
        real damping;


        Vector3D position;

        Vector3D velocity;


        /**
         * Holds the accumulated force to be applied at the next
         * simulation iteration only.
         */
        Vector3D forceAccum;

        Vector3D acceleration;

    public:

        /**
         * Integrates the particle forward in time by the given amount.
         */
        void integrate(real duration);

        void setMass(const real mass);


        real getMass() const;

     
        void setInverseMass(const real inverseMass);


        real getInverseMass() const;

        /**
         * Returns true if the mass of the particle is not-infinite.
         */
        bool hasFiniteMass() const;


        void setDamping(const real damping);


        real getDamping() const;


        void setPosition(const Vector3D &position);

        void setPosition(const real x, const real y, const real z);


        void getPosition(Vector3D *position) const;

        Vector3D getPosition() const;

        void setVelocity(const Vector3D &velocity);


        void setVelocity(const real x, const real y, const real z);

        void getVelocity(Vector3D *velocity) const;


        Vector3D getVelocity() const;

        void setAcceleration(const Vector3D &acceleration);

        void setAcceleration(const real x, const real y, const real z);

        void getAcceleration(Vector3D *acceleration) const;


        Vector3D getAcceleration() const;


        void clearAccumulator();

        void addForce(const Vector3D &force);


    };
}

#endif 