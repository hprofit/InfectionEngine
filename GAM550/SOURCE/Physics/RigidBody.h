#ifndef RigidBody_H
#define RigidBody_H

#include "physics_math.h"
namespace physics {

    /**
     * A rigid body is the basic simulation object in the physics
     * core.
     */
   /*
    RigidBody.halfSize = Vector3(4,4,4);
    RigidBody.body->setPosition(0, 7, 0);
    RigidBody.body->setOrientation(1,0,0,0);
    RigidBody.body->setVelocity(0,0,0);
    RigidBody.body->setRotation(0,0,0);
    RigidBody.body->setMass(100.0f);
    Matrix3 it;
    it.setBlockInertiaTensor(RigidBody.halfSize, 100.0f);
    RigidBody.body->setInertiaTensor(it);
    RigidBody.body->setDamping(0.9f, 0.9f);
    RigidBody.body->calculateDerivedData();
    RigidBody.calculateInternals();

    RigidBody.body->setAcceleration(Vector3::GRAVITY);
    RigidBody.body->setAwake(true);
    RigidBody.body->setCanSleep(true);
   */
    class RigidBody
    {
    public:

      enum BodyType
      {
        dynamic = 0,
        statics = 1,
        //kinematic = 2
      }bodytype;

      
    protected:
        /**
         * Holds the inverse of the mass of the rigid body.
           setting mass: use setMass()
         */
        real inverseMass;

        /**
         * Holds the inverse of the body's inertia tensor. The
         * inertia tensor provided must not be degenerate
         */
        Matrix3x3 inverseInertiaTensor = Matrix3x3(0.f, 0.f, 0.f,
          0.f, 0.f, 0.f,
          0.f, 0.f, 0.f);;;

        /**
         * Holds the amount of damping applied to linear
         * motion.
         */
        real linearDamping;

        /**
         * Holds the amount of damping applied to angular
         * motion.
         */
        real angularDamping;


        Vector3D position;

        Quaternion orientation;

        Vector3D velocity;

        Vector3D rotation;

        /**
         * Holds the inverse inertia tensor of the body in world
         * space. The inverse inertia tensor member is specified in
         * the body's local space.
         *
         * use for inverseInertiaTensor
         */
        Matrix3x3 inverseInertiaTensorWorld = Matrix3x3(0.f, 0.f, 0.f,
          0.f, 0.f, 0.f,
          0.f, 0.f, 0.f);

        /**
         * Holds the amount of motion of the body. This is a recency
         * weighted mean that can be used to put a body to sleap.
         */
        real motion;

        bool isAwake;

        bool canSleep;

        /**
         * Holds a transform matrix for converting body space into
         * world space and vice versa. This can be achieved by calling
         * the getPointIn*Space functions.
         *
         * use for getPointInLocalSpace
         * use for getPointInWorldSpace
         * use for getTransform
         */
        Matrix4x4 transformMatrix = Matrix4x4(1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 0.f);;

        /**
         * These data members store the current force, torque and
         * acceleration of the rigid body.
         */

        Vector3D forceAccum;
        Vector3D torqueAccum;
        Vector3D acceleration;

        /**
         * Holds the linear acceleration of the rigid body, for the
         * previous frame.
         */
        Vector3D lastFrameAcceleration;

        /*@}*/

    public:

        /**
         * Calculates internal data from state data. This should be called
         * after the body's state is altered directly (it is called
         * automatically during integration). If you change the body's state
         * and then intend to integrate before querying any data (such as
         * the transform matrix), then you can ommit this step.
         */
        void calculateDerivedData();

        /**
         * Integrates the rigid body forward in time by the given amount.
         * This function uses a Newton-Euler integration method, which is a
         * linear approximation to the correct integral. For this reason it
         * may be inaccurate in some cases.
         */
        void integrate(real duration);




        void setMass(const real mass);
        real getMass() const;


        void setInverseMass(const real inverseMass);

        real getInverseMass() const;

        /**
         * Returns true if the mass of the body is not-infinite.
         */
        bool hasFiniteMass() const;

        /*
         * Sets the intertia tensor for the rigid body.
         *
         * param must be a full rank matrix and must be
         * invertible.
         */
        void setInertiaTensor(const Matrix3x3 &inertiaTensor);

        void getInertiaTensor(Matrix3x3 *inertiaTensor) const;

        Matrix3x3 getInertiaTensor() const;

        /**
         * Copies the current inertia tensor of the rigid body into
         * the given matrix.
         */
        void getInertiaTensorWorld(Matrix3x3 *inertiaTensor) const;
        Matrix3x3 getInertiaTensorWorld() const;

        //Inverse Inertia Tensor
        void setInverseInertiaTensor(const Matrix3x3 &inverseInertiaTensor);
        void getInverseInertiaTensor(Matrix3x3 *inverseInertiaTensor) const;
        Matrix3x3 getInverseInertiaTensor() const;

        /**
         * Copies the current inverse inertia tensor of the rigid body
         * into the given matrix.
         *
         * @param inverseInertiaTensor A pointer to a matrix to hold
         * the current inverse inertia tensor of the rigid body. The
         * inertia tensor is expressed in world space.
         */
        void getInverseInertiaTensorWorld(Matrix3x3 *inverseInertiaTensor) const;

        /**
         * Gets a copy of the current inverse inertia tensor of the
         * rigid body.
         *
         * @return A new matrix containing the current inverse
         * intertia tensor. The inertia tensor is expressed in world
         * space.
         */
        Matrix3x3 getInverseInertiaTensorWorld() const;

        /* Damping
         * Sets both linear and angular damping in one function call.
         */
        void setDamping(const real linearDamping, const real angularDamping);
        void setLinearDamping(const real linearDamping);
        real getLinearDamping() const;
        void setAngularDamping(const real angularDamping);
        real getAngularDamping() const;

        // Position
        void setPosition(const Vector3D &position);
        void setPosition(const real x, const real y, const real z);
        void getPosition(Vector3D *position) const;
        Vector3D getPosition() const;

        /**
         * Sets the orientation of the rigid body.
         */
        void setOrientation(const Quaternion &orientation);
        void setOrientation(const real r, const real i,
            const real j, const real k);
        void getOrientation(Quaternion *orientation) const;
        Quaternion getOrientation() const;
        void getOrientation(Matrix3x3 *matrix) const;
        void getOrientation(real matrix[3][3]) const;

        /**
         * Fills the given matrix with a transformation representing
         * the rigid body's position and orientation.
         */
        void getTransform(Matrix4x4 *transform) const;
        void getTransform(real matrix[16]) const;
        void getGLTransform(float matrix[16]) const;

        /**
         * Gets a transformation representing the rigid body's
         * position and orientation.
         */
        Matrix4x4 getTransform() const;

        /**
         * Converts the given point from world space into the body's
         * local space.
         *
         * @param point The point to covert, given in world space.
         *
         * @return The converted point, in local space.
         */
        Vector3D getPointInLocalSpace(const Vector3D &point) const;

        /**
         * Converts the given point from world space into the body's
         * local space.
         *
         * @param point The point to covert, given in local space.
         *
         * @return The converted point, in world space.
         */
        Vector3D getPointInWorldSpace(const Vector3D &point) const;

        /**
         * Converts the given direction from world space into the
         * body's local space.
         *
         * @note When a direction is converted between frames of
         * reference, there is no translation required.
         *
         * @param direction The direction to covert, given in world
         * space.
         *
         * @return The converted direction, in local space.
         */
        Vector3D getDirectionInLocalSpace(const Vector3D &direction) const;

        /**
         * Converts the given direction from world space into the
         * body's local space.
         *
         * @note When a direction is converted between frames of
         * reference, there is no translation required.
         *
         * @param direction The direction to covert, given in local
         * space.
         *
         * @return The converted direction, in world space.
         */
        Vector3D getDirectionInWorldSpace(const Vector3D &direction) const;

        /**
         * Sets the velocity of the rigid body.
         *
         * @param velocity The new velocity of the rigid body. The
         * velocity is given in world space.
         */
        void setVelocity(const Vector3D &velocity);

        /**
         * Sets the velocity of the rigid body by component. The
         * velocity is given in world space.
         *
         * @param x The x coordinate of the new velocity of the rigid
         * body.
         *
         * @param y The y coordinate of the new velocity of the rigid
         * body.
         *
         * @param z The z coordinate of the new velocity of the rigid
         * body.
         */
        void setVelocity(const real x, const real y, const real z);

        /**
         * Fills the given vector with the velocity of the rigid body.
         *
         * @param velocity A pointer to a vector into which to write
         * the velocity. The velocity is given in world local space.
         */
        void getVelocity(Vector3D *velocity) const;

        /**
         * Gets the velocity of the rigid body.
         *
         * @return The velocity of the rigid body. The velocity is
         * given in world local space.
         */
        Vector3D getVelocity() const;

        /**
         * Applies the given change in velocity.
         */
        void addVelocity(const Vector3D &deltaVelocity);

        /**
         * Sets the rotation of the rigid body.
         *
         * @param rotation The new rotation of the rigid body. The
         * rotation is given in world space.
         */
        void setRotation(const Vector3D &rotation);

        /**
         * Sets the rotation of the rigid body by component. The
         * rotation is given in world space.
         *
         * @param x The x coordinate of the new rotation of the rigid
         * body.
         *
         * @param y The y coordinate of the new rotation of the rigid
         * body.
         *
         * @param z The z coordinate of the new rotation of the rigid
         * body.
         */
        void setRotation(const real x, const real y, const real z);

        /**
         * Fills the given vector with the rotation of the rigid body.
         *
         * @param rotation A pointer to a vector into which to write
         * the rotation. The rotation is given in world local space.
         */
        void getRotation(Vector3D *rotation) const;

        /**
         * Gets the rotation of the rigid body.
         *
         * @return The rotation of the rigid body. The rotation is
         * given in world local space.
         */
        Vector3D getRotation() const;

        /**
         * Applies the given change in rotation.
         */
        void addRotation(const Vector3D &deltaRotation);

        /**
         * Returns true if the body is awake and responding to
         * integration.
         *
         * @return The awake state of the body.
         */
        bool getAwake() const
        {
            return isAwake;
        }

        /**
         * Sets the awake state of the body. If the body is set to be
         * not awake, then its velocities are also cancelled, since
         * a moving body that is not awake can cause problems in the
         * simulation.
         *
         * @param awake The new awake state of the body.
         */
        void setAwake(const bool awake=true);

        /**
         * Returns true if the body is allowed to go to sleep at
         * any time.
         */
        bool getCanSleep() const
        {
            return canSleep;
        }

        /**
         * Sets whether the body is ever allowed to go to sleep. Bodies
         * under the player's control, or for which the set of
         * transient forces applied each frame are not predictable,
         * should be kept awake.
         *
         * @param canSleep Whether the body can now be put to sleep.
         */
        void setCanSleep(const bool canSleep=true);

        /*@}*/


        /**
         * @name Retrieval Functions for Dynamic Quantities
         *
         * These functions provide access to the acceleration
         * properties of the body. The acceleration is generated by
         * the simulation from the forces and torques applied to the
         * rigid body. Acceleration cannot be directly influenced, it
         * is set during integration, and represent the acceleration
         * experienced by the body of the previous simulation step.
         */
        /*@{*/

        /**
         * Fills the given vector with the current accumulated value
         * for linear acceleration. The acceleration accumulators
         * are set during the integration step. They can be read to
         * determine the rigid body's acceleration over the last
         * integration step. The linear acceleration is given in world
         * space.
         *
         * @param linearAcceleration A pointer to a vector to receive
         * the linear acceleration data.
         */
        void getLastFrameAcceleration(Vector3D *linearAcceleration) const;

        /**
         * Gets the current accumulated value for linear
         * acceleration. The acceleration accumulators are set during
         * the integration step. They can be read to determine the
         * rigid body's acceleration over the last integration
         * step. The linear acceleration is given in world space.
         *
         * @return The rigid body's linear acceleration.
         */
        Vector3D getLastFrameAcceleration() const;

        /*@}*/


        /**
         * @name Force, Torque and Acceleration Set-up Functions
         *
         * These functions set up forces and torques to apply to the
         * rigid body.
         */
        /*@{*/

        /**
         * Clears the forces and torques in the accumulators. This will
         * be called automatically after each intergration step.
         */
        void clearAccumulators();

        /**
         * Adds the given force to centre of mass of the rigid body.
         * The force is expressed in world-coordinates.
         *
         * @param force The force to apply.
         */
        void addForce(const Vector3D &force);

        /**
         * Adds the given force to the given point on the rigid body.
         * Both the force and the
         * application point are given in world space. Because the
         * force is not applied at the centre of mass, it may be split
         * into both a force and torque.
         *
         * @param force The force to apply.
         *
         * @param point The location at which to apply the force, in
         * world-coordinates.
         */
        void addForceAtPoint(const Vector3D &force, const Vector3D &point);

        /**
         * Adds the given force to the given point on the rigid body.
         * The direction of the force is given in world coordinates,
         * but the application point is given in body space. This is
         * useful for spring forces, or other forces fixed to the
         * body.
         *
         * @param force The force to apply.
         *
         * @param point The location at which to apply the force, in
         * body-coordinates.
         */
        void addForceAtBodyPoint(const Vector3D &force, const Vector3D &point);

        /**
         * Adds the given torque to the rigid body.
         * The force is expressed in world-coordinates.
         *
         * @param torque The torque to apply.
         */
        void addTorque(const Vector3D &torque);

        /**
         * Sets the constant acceleration of the rigid body.
         *
         * @param acceleration The new acceleration of the rigid body.
         */
        void setAcceleration(const Vector3D &acceleration);

        /**
         * Sets the constant acceleration of the rigid body by component.
         *
         * @param x The x coordinate of the new acceleration of the rigid
         * body.
         *
         * @param y The y coordinate of the new acceleration of the rigid
         * body.
         *
         * @param z The z coordinate of the new acceleration of the rigid
         * body.
         */
        void setAcceleration(const real x, const real y, const real z);

        /**
         * Fills the given vector with the acceleration of the rigid body.
         *
         * @param acceleration A pointer to a vector into which to write
         * the acceleration. The acceleration is given in world local space.
         */
        void getAcceleration(Vector3D *acceleration) const;

        /**
         * Gets the acceleration of the rigid body.
         *
         * @return The acceleration of the rigid body. The acceleration is
         * given in world local space.
         */
        Vector3D getAcceleration() const;

        /*@}*/

    };

} 


void PrintMatrix4x4(const Matrix4x4 & mat);
Vector3D RotationMatrixToEulerAngles(Matrix4x4 R);

#endif 
