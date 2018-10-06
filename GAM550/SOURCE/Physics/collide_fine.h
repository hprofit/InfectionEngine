
#ifndef COLLISION_FINE_H
#define COLLISION_FINE_H


namespace physics {

    // Forward declarations of primitive friends
    class IntersectionTests;
    class CollisionDetector;

    /**
     * Represents a primitive to detect collisions against.
     */
    class CollisionPrimitive
    {
    public:
        friend class IntersectionTests;
        friend class CollisionDetector;

        /**
         * The rigid body that is represented by this primitive.
         */
        RigidBody * body;

        Matrix4x4 offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
          0.f, 1.f, 0.f, 0.f,
          0.f, 0.f, 1.f, 0.f,
          0.f, 0.f, 0.f, 0.f);


        void calculateInternals();


        Vector3D getAxis(unsigned index) const
        {
            return transform.getAxisVector(index);
        }

       
        const Matrix4x4& getTransform() const
        {
            return transform;
        }


    protected:
       
      Matrix4x4 transform = Matrix4x4(1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f);
    };

    /**
     * Represents a rigid body that can be treated as a sphere
     * for collision detection.
     */
    class CollisionSphere : public CollisionPrimitive
    {
    public:
        /**
         * The radius of the sphere.
         */
        real radius;
    };

    class CollisionPlane
    {
    public:
        /**
         * The plane normal
         */
        Vector3D direction;

        /**
         * The distance of the plane from the origin.
         */
        real offset;
    };

    
    class CollisionBox : public CollisionPrimitive
    {
    public:
        /**
         * Holds the half-sizes of the box along each of its local axes.
         */
        Vector3D halfSize;
    };

    /**
     * A wrapper class that holds fast intersection tests. These
     * can be used to drive the coarse collision detection system or
     * as an early out in the full collision tests below.
     */
    class IntersectionTests
    {
    public:

        static bool sphereAndHalfSpace(
            const CollisionSphere &sphere,
            const CollisionPlane &plane);

        static bool sphereAndSphere(
            const CollisionSphere &one,
            const CollisionSphere &two);

        static bool boxAndBox(
            const CollisionBox &one,
            const CollisionBox &two);

        /**
         * The half-space is given as a direction (unit) vector and the
         * offset of the limiting plane from the origin, along the given
         * direction.
         */
        static bool boxAndHalfSpace(
            const CollisionBox &box,
            const CollisionPlane &plane);
    };


    /**
     * A helper structure
     */
    class Contact;
    struct CollisionData
    {
       
        Contact *contactArray;

      
        Contact *contacts;

        /** Holds the maximum number of contacts the array can take. */
        int contactsLeft;

        /** Holds the number of contacts found so far. */
        unsigned contactCount;

        
        real friction;
        
        real restitution;

        real tolerance;

        bool hasMoreContacts()
        {
            return contactsLeft > 0;
        }

      
        void reset(unsigned maxContacts)
        {
            contactsLeft = maxContacts;
            contactCount = 0;
            contacts = contactArray;
        }

       
        void addContacts(unsigned count)
        {
            // Reduce the number of contacts remaining, add number used
            contactsLeft -= count;
            contactCount += count;

            // Move the array forward
            contacts += count;
        }
    };

    /**
     * A wrapper class that holds the fine grained collision detection
     * routines.
     */
    class CollisionDetector
    {
    public:
        /*
        it takes the details
        of two objects, and a pointer to a contact array to fill. It
        returns the number of contacts it wrote into the array.*/
        static unsigned sphereAndHalfSpace(
            const CollisionSphere &sphere,
            const CollisionPlane &plane,
            CollisionData *data
            );

        static unsigned sphereAndTruePlane(
            const CollisionSphere &sphere,
            const CollisionPlane &plane,
            CollisionData *data
            );

        static unsigned sphereAndSphere(
            const CollisionSphere &one,
            const CollisionSphere &two,
            CollisionData *data
            );

        /**
         * Does a collision test on a collision box and a plane representing
         * a half-space
         */
        static unsigned boxAndHalfSpace(
            const CollisionBox &box,
            const CollisionPlane &plane,
            CollisionData *data
            );

        static unsigned boxAndBox(
            const CollisionBox &one,
            const CollisionBox &two,
            CollisionData *data
            );

        static unsigned boxAndPoint(
            const CollisionBox &box,
            const Vector3D &point,
            CollisionData *data
            );

        static unsigned boxAndSphere(
            const CollisionBox &box,
            const CollisionSphere &sphere,
            CollisionData *data
            );
    };



} 

#endif 
