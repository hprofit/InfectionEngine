
#ifndef CONTACTS_H
#define CONTACTS_H

namespace physics {

    /*
     * Forward declaration, see full declaration below for complete
     * documentation.
     */
    class ContactResolver;

    /**
     * A contact represents two bodies in contact. 
     */
    class Contact
    {
        friend class ContactResolver;

    public:
        RigidBody* body[2];

        real friction;
        real restitution;

        Vector3D contactPoint;

        Vector3D contactNormal;

        real penetration;

        void setBodyData(RigidBody* one, RigidBody *two,
                         real friction, real restitution);

    protected:

        /**
         * A transform matrix that converts co-ordinates in the contact's
         * frame of reference to world co-ordinates.
         */
        Matrix3x3 contactToWorld = Matrix3x3(0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f);;

        Vector3D contactVelocity;

        /**
         * Holds the required change in velocity for this contact to be
         * resolved.
         */
        real desiredDeltaVelocity;

        /**
         * Holds the world space position of the contact point relative to
         * centre of each body.
         */
        Vector3D relativeContactPosition[2];

    protected:
        void calculateInternals(real duration);

        /**
         * Reverses the contact. This involves swapping the two rigid bodies
         * and reversing the contact normal.
         * automatically).
         */
        void swapBodies();

        /**
         * Updates the awake state of rigid bodies that are taking
         * place in the given contact.
         */
        void matchAwakeState();

        /**
         * Calculates and sets the internal value for the desired delta
         * velocity.
         */
        void calculateDesiredDeltaVelocity(real duration);

        /**
         * Calculates and returns the velocity of the contact
         * point on the given body.
         */
        Vector3D calculateLocalVelocity(unsigned bodyIndex, real duration);

        void calculateContactBasis();

        /**
         * 
         */
        void applyImpulse(const Vector3D &impulse, RigidBody *body,
                          Vector3D *velocityChange, Vector3D *rotationChange);


        void applyVelocityChange(Vector3D velocityChange[2],
                                 Vector3D rotationChange[2]);

        void applyPositionChange(Vector3D linearChange[2],
                                 Vector3D angularChange[2],
                                 real penetration);

      
        Vector3D calculateFrictionlessImpulse(Matrix3x3 *inverseInertiaTensor);

      
        Vector3D calculateFrictionImpulse(Matrix3x3 *inverseInertiaTensor);
    };

  
    class ContactResolver
    {
    protected:
        /**
         * Holds the number of iterations to perform when resolving
         * velocity.
         */
        unsigned velocityIterations;

        /**
         * Holds the number of iterations to perform when resolving
         * position.
         */
        unsigned positionIterations;

        /**
         * To avoid instability velocities smaller
         * than this value are considered to be zero. Too small and the
         * simulation may be unstable, too large and the bodies may
         * interpenetrate visually. A good starting point is the default
         * of 0.01.
         */
        real velocityEpsilon;

        /**
         * To avoid instability penetrations
         * smaller than this value are considered to be not interpenetrating.
         * Too small and the simulation may be unstable, too large and the
         * bodies may interpenetrate visually. A good starting point is
         * the default of0.01.
         */
        real positionEpsilon;

    public:
        /**
         * Stores the number of velocity iterations used in the
         * last call to resolve contacts.
         */
        unsigned velocityIterationsUsed;

        /**
         * Stores the number of position iterations used in the
         * last call to resolve contacts.
         */
        unsigned positionIterationsUsed;

    private:
        /**
         * Keeps track of whether the internal settings are valid.
         */
        bool validSettings;

    public:
        /**
         * Creates a new contact resolver with the given number of iterations
         * per resolution call, and optional epsilon values.
         */
        ContactResolver(unsigned iterations,
            real velocityEpsilon=(real)0.01,
            real positionEpsilon=(real)0.01);
        ContactResolver(unsigned velocityIterations,
            unsigned positionIterations,
            real velocityEpsilon=(real)0.01,
            real positionEpsilon=(real)0.01);
        bool isValid()
        {
            return (velocityIterations > 0) &&
                   (positionIterations > 0) &&
                   (positionEpsilon >= 0.0f) &&
                   (positionEpsilon >= 0.0f);
        }
        void setIterations(unsigned velocityIterations,
                           unsigned positionIterations);

        void setIterations(unsigned iterations);

        void setEpsilon(real velocityEpsilon,
                        real positionEpsilon);
        void resolveContacts(Contact *contactArray,
            unsigned numContacts,
            real duration);

    protected:
       
        void prepareContacts(Contact *contactArray, unsigned numContacts,
            real duration);

        void adjustVelocities(Contact *contactArray,
            unsigned numContacts,
            real duration);

        void adjustPositions(Contact *contacts,
            unsigned numContacts,
            real duration);
    };

 
    class ContactGenerator
    {
    public:
        
        virtual unsigned addContact(Contact *contact, unsigned limit) const = 0;
    };

} 

#endif 
