
#ifndef COLLISION_COARSE_H
#define COLLISION_COARSE_H

namespace physics {

    struct BoundingSphere
    {
        Vector3D centre;
        real radius;

    public:
        /**
         * Creates a new bounding sphere at the given centre and radius.
         */
        BoundingSphere(const Vector3D &centre, real radius);

        /**
         * Creates a bounding sphere to enclose the two given bounding
         * spheres.
         */
        BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

        /**
         * Checks if the bounding sphere overlaps 
         */
        bool overlaps(const BoundingSphere *other) const;

        /**
         * Reports how much this bounding sphere would have to grow
         * by to incorporate the given bounding sphere. 
         */
        real getGrowth(const BoundingSphere &other) const;

        real getSize() const
        {
          return (real)((real)1.333333f) * 3.1415f * radius * radius * radius;
        }
    };

    /**
     * Stores a potential contact to check later.
     */
    struct PotentialContact
    {
        RigidBody* body[2];
    };

    /**
     * A base class for nodes in a bounding volume hierarchy.
     */
    template<class BoundingVolumeClass>
    class BVHNode
    {
    public:
        BVHNode * children[2];

        /**
         * Holds a single bounding volume encompassing all the
         * descendents of this node.
         */
        BoundingVolumeClass volume;

        RigidBody * body;

        BVHNode * parent;


        BVHNode(BVHNode *parent, const BoundingVolumeClass &volume,
            RigidBody* body=NULL)
            : parent(parent), volume(volume), body(body)
        {
            children[0] = children[1] = NULL;
        }


        bool isLeaf() const
        {
            return (body != NULL);
        }

        unsigned getPotentialContacts(PotentialContact* contacts,
                                      unsigned limit) const;

        void insert(RigidBody* body, const BoundingVolumeClass &volume);

        ~BVHNode();

    protected:


        bool overlaps(const BVHNode<BoundingVolumeClass> *other) const;


        unsigned getPotentialContactsWith(
            const BVHNode<BoundingVolumeClass> *other,
            PotentialContact* contacts,
            unsigned limit) const;

        void recalculateBoundingVolume(bool recurse = true);
    };


    template<class BoundingVolumeClass>
    bool BVHNode<BoundingVolumeClass>::overlaps(
        const BVHNode<BoundingVolumeClass> * other
        ) const
    {
        return volume->overlaps(other->volume);
    }

    template<class BoundingVolumeClass>
    void BVHNode<BoundingVolumeClass>::insert(
        RigidBody* newBody, const BoundingVolumeClass &newVolume
        )
    {
        if (isLeaf())
        {
            // Child one is a copy of us.
            children[0] = new BVHNode<BoundingVolumeClass>(
                this, volume, body
                );

            // Child two holds the new body
            children[1] = new BVHNode<BoundingVolumeClass>(
                this, newVolume, newBody
                );

            this->body = NULL;


            recalculateBoundingVolume();
        }

        else
        {
            if (children[0]->volume.getGrowth(newVolume) <
                children[1]->volume.getGrowth(newVolume))
            {
                children[0]->insert(newBody, newVolume);
            }
            else
            {
                children[1]->insert(newBody, newVolume);
            }
        }
    }

    template<class BoundingVolumeClass>
    BVHNode<BoundingVolumeClass>::~BVHNode()
    {
        if (parent)
        {
            BVHNode<BoundingVolumeClass> *sibling;
            if (parent->children[0] == this) sibling = parent->children[1];
            else sibling = parent->children[0];

            parent->volume = sibling->volume;
            parent->body = sibling->body;
            parent->children[0] = sibling->children[0];
            parent->children[1] = sibling->children[1];

            sibling->parent = NULL;
            sibling->body = NULL;
            sibling->children[0] = NULL;
            sibling->children[1] = NULL;
            delete sibling;

            parent->recalculateBoundingVolume();
        }


        if (children[0]) {
            children[0]->parent = NULL;
            delete children[0];
        }
        if (children[1]) {
            children[1]->parent = NULL;
            delete children[1];
        }
    }

    template<class BoundingVolumeClass>
        void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume(
        bool recurse
        )
    {
        if (isLeaf()) return;

        // Use the bounding volume combining constructor.
        volume = BoundingVolumeClass(
            children[0]->volume,
            children[1]->volume
            );

        // Recurse up the tree
        if (parent) parent->recalculateBoundingVolume(true);
    }

    template<class BoundingVolumeClass>
    unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(
        PotentialContact* contacts, unsigned limit
        ) const
    {
        if (isLeaf() || limit == 0) return 0;

        // Get the potential contacts of one of our children with
        // the other
        return children[0]->getPotentialContactsWith(
            children[1], contacts, limit
            );
    }

    template<class BoundingVolumeClass>
    unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(
        const BVHNode<BoundingVolumeClass> *other,
        PotentialContact* contacts,
        unsigned limit
        ) const
    {

        if (!overlaps(other) || limit == 0) return 0;


        if (isLeaf() && other->isLeaf())
        {
            contacts->body[0] = body;
            contacts->body[1] = other->body;
            return 1;
        }

        // Determine which node to descend into. If either is
        // a leaf, then we descend the other. If both are branches,
        // then use the one with the largest size.
        if (other->isLeaf() ||
            (!isLeaf() && volume->getSize() >= other->volume->getSize()))
        {
            // Recurse into ourself
            unsigned count = children[0]->getPotentialContactsWith(
                other, contacts, limit
                );

            // Check enough slots to do the other side too
            if (limit > count) {
                return count + children[1]->getPotentialContactsWith(
                    other, contacts+count, limit-count
                    );
            } else {
                return count;
            }
        }
        else
        {
            // Recurse into the other node
            unsigned count = getPotentialContactsWith(
                other->children[0], contacts, limit
                );

            // Check nough slots to do the other side too
            if (limit > count) {
                return count + getPotentialContactsWith(
                    other->children[1], contacts+count, limit-count
                    );
            } else {
                return count;
            }
        }
    }

}

#endif 