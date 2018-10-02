#include <Stdafx.h>

using namespace physics;

void CollisionPrimitive::calculateInternals()
{
    transform = body->getTransform().Phy_multiply_Matrix(offset);
}

bool IntersectionTests::sphereAndHalfSpace(
    const CollisionSphere &sphere,
    const CollisionPlane &plane)
{
    // Find the distance from the origin
    real ballDistance =
        plane.direction *
        sphere.getAxis(3) -
        sphere.radius;

    // Check for the intersection
    return ballDistance <= plane.offset;
}

bool IntersectionTests::sphereAndSphere(
    const CollisionSphere &one,
    const CollisionSphere &two)
{
    // Find the vector between the objects
    Vector3D midline = one.getAxis(3) - two.getAxis(3);

    // See if it is large enough.
    return midline.SquareLength() <
        (one.radius+two.radius)*(one.radius+two.radius);
}

static inline real transformToAxis(
    const CollisionBox &box,
    const Vector3D &axis
    )
{
    return
        box.halfSize.x * real_abs(axis * box.getAxis(0)) +
        box.halfSize.y * real_abs(axis * box.getAxis(1)) +
        box.halfSize.z * real_abs(axis * box.getAxis(2));
}

/**
 * This function checks if the two boxes overlap
 * along the given axis. 
 */
static inline bool overlapOnAxis(
    const CollisionBox &one,
    const CollisionBox &two,
    const Vector3D &axis,
    const Vector3D &toCentre
    )
{
    // Project half-size of one onto axis
    real oneProject = transformToAxis(one, axis);
    real twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    real distance = real_abs(toCentre * axis);

    // Check for overlap
    return (distance < oneProject + twoProject);
}

// This preprocessor definition is only used as a convenience
// in the boxAndBox intersection  method.
#define TEST_OVERLAP(axis) overlapOnAxis(one, two, (axis), toCentre)

bool IntersectionTests::boxAndBox(
    const CollisionBox &one,
    const CollisionBox &two
    )
{
    // Find the vector between the two centres
    Vector3D toCentre = two.getAxis(3) - one.getAxis(3);

    return (
        
        TEST_OVERLAP(one.getAxis(0)) &&
        TEST_OVERLAP(one.getAxis(1)) &&
        TEST_OVERLAP(one.getAxis(2)) &&

        
        TEST_OVERLAP(two.getAxis(0)) &&
        TEST_OVERLAP(two.getAxis(1)) &&
        TEST_OVERLAP(two.getAxis(2)) &&

        
        TEST_OVERLAP(one.getAxis(0) % two.getAxis(0)) &&
        TEST_OVERLAP(one.getAxis(0) % two.getAxis(1)) &&
        TEST_OVERLAP(one.getAxis(0) % two.getAxis(2)) &&
        TEST_OVERLAP(one.getAxis(1) % two.getAxis(0)) &&
        TEST_OVERLAP(one.getAxis(1) % two.getAxis(1)) &&
        TEST_OVERLAP(one.getAxis(1) % two.getAxis(2)) &&
        TEST_OVERLAP(one.getAxis(2) % two.getAxis(0)) &&
        TEST_OVERLAP(one.getAxis(2) % two.getAxis(1)) &&
        TEST_OVERLAP(one.getAxis(2) % two.getAxis(2))
    );
}
#undef TEST_OVERLAP

bool IntersectionTests::boxAndHalfSpace(
    const CollisionBox &box,
    const CollisionPlane &plane
    )
{
    real projectedRadius = transformToAxis(box, plane.direction);

    // how far the box is from the origin
    real boxDistance =
        plane.direction *
        box.getAxis(3) -
        projectedRadius;

    // Check for the intersection
    return boxDistance <= plane.offset;
}

unsigned CollisionDetector::sphereAndTruePlane(
    const CollisionSphere &sphere,
    const CollisionPlane &plane,
    CollisionData *data
    )
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    // Cache the sphere position
    Vector3D position = sphere.getAxis(3);

    // Find the distance from the plane
    real centreDistance = plane.direction * position - plane.offset;

    // Check if we're within radius
    if (centreDistance*centreDistance > sphere.radius*sphere.radius)
    {
        return 0;
    }

    // Check which side of the plane
    Vector3D normal = plane.direction;
    real penetration = -centreDistance;
    if (centreDistance < 0)
    {
        normal *= -1;
        penetration = -penetration;
    }
    penetration += sphere.radius;

    // Create the contact - it has a normal in the plane direction.
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->penetration = penetration;
    contact->contactPoint = position - plane.direction * centreDistance;
    contact->setBodyData(sphere.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::sphereAndHalfSpace(
    const CollisionSphere &sphere,
    const CollisionPlane &plane,
    CollisionData *data
    )
{
    // have contacts
    if (data->contactsLeft <= 0) return 0;

    Vector3D position = sphere.getAxis(3);

    // distance from the plane
    real ballDistance =
        plane.direction * position -
        sphere.radius - plane.offset;

    if (ballDistance >= 0) return 0;

    //has a normal in the plane direction.
    Contact* contact = data->contacts;
    contact->contactNormal = plane.direction;
    contact->penetration = -ballDistance;
    contact->contactPoint =
        position - plane.direction * (ballDistance + sphere.radius);
    contact->setBodyData(sphere.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::sphereAndSphere(
    const CollisionSphere &one,
    const CollisionSphere &two,
    CollisionData *data
    )
{
    //have contacts
    if (data->contactsLeft <= 0) return 0;

    Vector3D positionOne = one.getAxis(3);
    Vector3D positionTwo = two.getAxis(3);

    // Find the vector between the objects
    Vector3D midline = positionOne - positionTwo;
    real size = midline.Length();

    // See if it is large enough.
    if (size <= 0.0f || size >= one.radius+two.radius)
    {
        return 0;
    }

    Vector3D normal = midline * (((real)1.0)/size);

    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = positionOne + midline * (real)0.5;
    contact->penetration = (one.radius+two.radius - size);
    contact->setBodyData(one.body, two.body,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}




/*
 * checks if the two boxes overlap
 * along the given axis
 */
static inline real penetrationOnAxis(
    const CollisionBox &one,
    const CollisionBox &two,
    const Vector3D &axis,
    const Vector3D &toCentre
    )
{
    // Project the half-size of one onto axis
    real oneProject = transformToAxis(one, axis);
    real twoProject = transformToAxis(two, axis);

    // Project this onto the axis
    real distance = real_abs(toCentre * axis);

    // overlap
    return oneProject + twoProject - distance;
}


static inline bool tryAxis(
    const CollisionBox &one,
    const CollisionBox &two,
    Vector3D axis,
    const Vector3D& toCentre,
    unsigned index,

    // values may be updated
    real& smallestPenetration,
    unsigned &smallestCase
    )
{
    // Make sure we have a normalized axis, and don't check almost parallel axes
    if (axis.SquareLength() < 0.0001) return true;
    axis.normalise();

    real penetration = penetrationOnAxis(one, two, axis, toCentre);

    if (penetration < 0) return false;
    if (penetration < smallestPenetration) {
        smallestPenetration = penetration;
        smallestCase = index;
    }
    return true;
}

void fillPointFaceBoxBox(
    const CollisionBox &one,
    const CollisionBox &two,
    const Vector3D &toCentre,
    CollisionData *data,
    unsigned best,
    real pen
    )
{
    Contact* contact = data->contacts;

    Vector3D normal = one.getAxis(best);
    if (one.getAxis(best) * toCentre > 0)
    {
        normal = normal * -1.0f;
    }

    Vector3D vertex = two.halfSize;
    if (two.getAxis(0) * normal < 0) vertex.x = -vertex.x;
    if (two.getAxis(1) * normal < 0) vertex.y = -vertex.y;
    if (two.getAxis(2) * normal < 0) vertex.z = -vertex.z;

    contact->contactNormal = normal;
    contact->penetration = pen;
    contact->contactPoint = two.getTransform().Phy_multiply_Vector3(vertex);
    contact->setBodyData(one.body, two.body,
        data->friction, data->restitution);
}

static inline Vector3D contactPoint(
    const Vector3D &pOne,
    const Vector3D &dOne,
    real oneSize,
    const Vector3D &pTwo,
    const Vector3D &dTwo,
    real twoSize,

 
    bool useOne)
{
    Vector3D toSt, cOne, cTwo;
    real dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    real denom, mua, mub;

    smOne = dOne.SquareLength();
    smTwo = dTwo.SquareLength();
    dpOneTwo = dTwo * dOne;

    toSt = pOne - pTwo;
    dpStaOne = dOne * toSt;
    dpStaTwo = dTwo * toSt;

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    // Zero denominator indicates parrallel lines
    if (real_abs(denom) < 0.0001f) {
        return useOne?pOne:pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    if (mua > oneSize ||
        mua < -oneSize ||
        mub > twoSize ||
        mub < -twoSize)
    {
        return useOne?pOne:pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}

#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;

unsigned CollisionDetector::boxAndBox(
    const CollisionBox &one,
    const CollisionBox &two,
    CollisionData *data
    )
{

    // Find the vector between the two centres
    Vector3D toCentre = two.getAxis(3) - one.getAxis(3);

    real pen = REAL_MAX;
    unsigned best = 0xffffff;

    CHECK_OVERLAP(one.getAxis(0), 0);
    CHECK_OVERLAP(one.getAxis(1), 1);
    CHECK_OVERLAP(one.getAxis(2), 2);

    CHECK_OVERLAP(two.getAxis(0), 3);
    CHECK_OVERLAP(two.getAxis(1), 4);
    CHECK_OVERLAP(two.getAxis(2), 5);

    // Store the best axis-major
    unsigned bestSingleAxis = best;

    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(0), 6);
    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(1), 7);
    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(2), 8);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(0), 9);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(1), 10);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(2), 11);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(0), 12);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(1), 13);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(2), 14);

    // Make sure we've got a result.
    assert(best != 0xffffff);

    if (best < 3)
    {
        fillPointFaceBoxBox(one, two, toCentre, data, best, pen);
        data->addContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        fillPointFaceBoxBox(two, one, toCentre*-1.0f, data, best-3, pen);
        data->addContacts(1);
        return 1;
    }
    else
    {
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        Vector3D oneAxis = one.getAxis(oneAxisIndex);
        Vector3D twoAxis = two.getAxis(twoAxisIndex);
        Vector3D axis = oneAxis % twoAxis;
        axis.normalise();

        // The axis should point from box one to box two.
        if (axis * toCentre > 0) axis = axis * -1.0f;

        Vector3D ptOnOneEdge = one.halfSize;
        Vector3D ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; i++)
        {
            if (i == oneAxisIndex) ptOnOneEdge[i] = 0;
            else if (one.getAxis(i) * axis > 0) ptOnOneEdge[i] = -ptOnOneEdge[i];

            if (i == twoAxisIndex) ptOnTwoEdge[i] = 0;
            else if (two.getAxis(i) * axis < 0) ptOnTwoEdge[i] = -ptOnTwoEdge[i];
        }

        ptOnOneEdge = one.transform.Phy_multiply_Vector3(ptOnOneEdge);
        ptOnTwoEdge = two.transform.Phy_multiply_Vector3(ptOnTwoEdge);

        // So we have a point and a direction for the colliding edges.
        // We need to find out point of closest approach of the two
        // line-segments.
        Vector3D vertex = contactPoint(
            ptOnOneEdge, oneAxis, one.halfSize[oneAxisIndex],
            ptOnTwoEdge, twoAxis, two.halfSize[twoAxisIndex],
            bestSingleAxis > 2
            );

        // We can fill the contact.
        Contact* contact = data->contacts;

        contact->penetration = pen;
        contact->contactNormal = axis;
        contact->contactPoint = vertex;
        contact->setBodyData(one.body, two.body,
            data->friction, data->restitution);
        data->addContacts(1);
        return 1;
    }
    return 0;
}
#undef CHECK_OVERLAP




unsigned CollisionDetector::boxAndPoint(
    const CollisionBox &box,
    const Vector3D &point,
    CollisionData *data
    )
{
    // Transform the point into box coordinates
    Vector3D relPt = box.transform.transformInverse(point);

    Vector3D normal;

    // Check each axis, looking for the axis on which the
    // penetration is least deep.
    real min_depth = box.halfSize.x - real_abs(relPt.x);
    if (min_depth < 0) return 0;
    normal = box.getAxis(0) * (real)((relPt.x < 0)?-1:1);

    real depth = box.halfSize.y - real_abs(relPt.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * (real)((relPt.y < 0) ? -1 : 1);
    }

    depth = box.halfSize.z - real_abs(relPt.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * (real)((relPt.z < 0) ? -1 : 1);
    }

    // Compile the contact
    Contact* contact = data->contacts;
    contact->contactNormal = normal;
    contact->contactPoint = point;
    contact->penetration = min_depth;

    // Where this is called this value can be left, or filled in.
    contact->setBodyData(box.body, NULL,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndSphere(
    const CollisionBox &box,
    const CollisionSphere &sphere,
    CollisionData *data
    )
{
    // Transform the centre of the sphere into box coordinates
    Vector3D centre = sphere.getAxis(3);
    Vector3D relCentre = box.transform.transformInverse(centre);

    // Early out check to see if we can exclude the contact
    if (real_abs(relCentre.x) - sphere.radius > box.halfSize.x ||
        real_abs(relCentre.y) - sphere.radius > box.halfSize.y ||
        real_abs(relCentre.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    Vector3D closestPt(0,0,0);
    real dist;

    // Clamp each coordinate to the box.
    dist = relCentre.x;
    if (dist > box.halfSize.x) dist = box.halfSize.x;
    if (dist < -box.halfSize.x) dist = -box.halfSize.x;
    closestPt.x = dist;

    dist = relCentre.y;
    if (dist > box.halfSize.y) dist = box.halfSize.y;
    if (dist < -box.halfSize.y) dist = -box.halfSize.y;
    closestPt.y = dist;

    dist = relCentre.z;
    if (dist > box.halfSize.z) dist = box.halfSize.z;
    if (dist < -box.halfSize.z) dist = -box.halfSize.z;
    closestPt.z = dist;

    dist = (closestPt - relCentre).SquareLength();
    if (dist > sphere.radius * sphere.radius) return 0;

    Vector3D closestPtWorld = box.transform.transform(closestPt);

    Contact* contact = data->contacts;
    contact->contactNormal = (closestPtWorld - centre);
    contact->contactNormal.normalise();
    contact->contactPoint = closestPtWorld;
    contact->penetration = sphere.radius - real_sqrt(dist);
    contact->setBodyData(box.body, sphere.body,
        data->friction, data->restitution);

    data->addContacts(1);
    return 1;
}

unsigned CollisionDetector::boxAndHalfSpace(
    const CollisionBox &box,
    const CollisionPlane &plane,
    CollisionData *data
    )
{
    // Make sure we have contacts
    if (data->contactsLeft <= 0) return 0;

    // Check for intersection
    if (!IntersectionTests::boxAndHalfSpace(box, plane))
    {
        return 0;
    }

    static real mults[8][3] = {{1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
                               {1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1}};

    Contact* contact = data->contacts;
    unsigned contactsUsed = 0;
    for (unsigned i = 0; i < 8; i++) {

        // Calculate the position of each vertex
        Vector3D vertexPos(mults[i][0], mults[i][1], mults[i][2]);
        vertexPos.componentProductUpdate(box.halfSize);
        vertexPos = box.transform.transform(vertexPos);

        // Calculate the distance from the plane
        real vertexDistance = vertexPos * plane.direction;

        // Compare this to the plane's distance
        if (vertexDistance <= plane.offset)
        {
            contact->contactPoint = plane.direction;
            contact->contactPoint *= (vertexDistance-plane.offset);
            contact->contactPoint += vertexPos;
            contact->contactNormal = plane.direction;
            contact->penetration = plane.offset - vertexDistance;

            // Write the appropriate data
            contact->setBodyData(box.body, NULL,
                data->friction, data->restitution);

            // Move onto the next contact
            contact++;
            contactsUsed++;
            if (contactsUsed == (unsigned)data->contactsLeft) return contactsUsed;
        }
    }

    data->addContacts(contactsUsed);
    return contactsUsed;
}
