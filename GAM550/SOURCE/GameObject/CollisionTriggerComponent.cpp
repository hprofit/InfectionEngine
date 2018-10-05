/*#include <Stdafx.h>


CollisionTriggerComponent::CollisionTriggerComponent(InfectGUID guid) :
  Component(guid)
{
  SetCollisionData();
}

void CollisionTriggerComponent::Deactivate() {
  mp_Parent = nullptr;
}

void CollisionTriggerComponent::Serialize(const json& j)
{

}

void CollisionTriggerComponent::HandleEvent(Event * pEvent)
{
  switch (pEvent->Type()) {
  default:
    break;
  }
}

void CollisionTriggerComponent::Update(float dt)
{
  for (std::list<CollisionComponent::Box *>::iterator it = m_BoxPool.begin(); it != m_BoxPool.end(); ++it)
  {
    (*it)->body->integrate(INFECT_FRAMERATE.GetFrameTime());
    (*it)->calculateInternals();
  }

  for (std::list<CollisionComponent::Sphere *>::iterator it = m_SpherePool.begin(); it != m_SpherePool.end(); ++it)
  {
    (*it)->body->integrate(INFECT_FRAMERATE.GetFrameTime());
    (*it)->calculateInternals();
  }
  // Set up the collision data structure
  cData.reset(maxContacts);
  cData.friction = m_CollisionData[0];
  cData.restitution = m_CollisionData[1];
  cData.tolerance = m_CollisionData[2];

  // loading collision
  CollisionTrigger();

  // solve collision
  resolver.resolveContacts(
    cData.contactArray,
    cData.contactCount,
    INFECT_FRAMERATE.GetFrameTime()
  );
}

void CollisionTriggerComponent::CollisionTrigger()
{
  for (std::list<CollisionComponent::Box *>::iterator it = m_BoxPool.begin(); it != m_BoxPool.end(); ++it)
  {
    // check box and Sphere
    for (std::list<CollisionComponent::Sphere *>::iterator it_other = m_SpherePool.begin(); it_other != m_SpherePool.end(); ++it_other)
    {
      // Check ground box-sphere collisions
      if (!cData.hasMoreContacts()) return;
      physics::CollisionDetector::boxAndSphere(*(*it), *(*it_other), &cData);
    }

    // check box and box
    if (!cData.hasMoreContacts()) return;
    for (std::list<CollisionComponent::Box *>::iterator it_other = m_BoxPool.begin(); it_other != m_BoxPool.end(); ++it_other)
    {
      if (*it != *it_other)
      {
        if (!cData.hasMoreContacts()) return;
        physics::CollisionDetector::boxAndBox(*(*it), *(*it_other), &cData);
      }
    }

    // check box and plane 
    for (std::list<CollisionComponent::Plane *>::iterator it_other = m_PlanePool.begin(); it_other != m_PlanePool.end(); ++it_other)
    {
      if (!cData.hasMoreContacts()) return;
      physics::CollisionDetector::boxAndHalfSpace(*(*it), *(*it_other), &cData);
    }
  }

  for (std::list<CollisionComponent::Sphere *>::iterator it = m_SpherePool.begin(); it != m_SpherePool.end(); ++it)
  {
    for (std::list<CollisionComponent::Sphere *>::iterator it_other = m_SpherePool.begin(); it_other != m_SpherePool.end(); ++it_other)
    {
      if (!cData.hasMoreContacts()) return;
      // Check ground sphere-sphere collisions
      if (*it != *it_other)
        physics::CollisionDetector::sphereAndSphere(*(*it), *(*it_other), &cData);
    }

    // check sphere and plane 
    for (std::list<CollisionComponent::Plane *>::iterator it_other = m_PlanePool.begin(); it_other != m_PlanePool.end(); ++it_other)
    {
      if (!cData.hasMoreContacts()) return;
      physics::CollisionDetector::sphereAndHalfSpace(*(*it), *(*it_other), &cData);
    }
  }

}

void CollisionTriggerComponent::AddToBoxPool(CollisionComponent::Box *box)
{
  m_BoxPool.push_back(box);
}

void CollisionTriggerComponent::AddToSpherePool(CollisionComponent::Sphere *sphere)
{
  m_SpherePool.push_back(sphere);
}

void CollisionTriggerComponent::AddToPlanePool(CollisionComponent::Plane *plane)
{
  m_PlanePool.push_back(plane);
}

void CollisionTriggerComponent::SetCollisionData(real friction_, real restitution_, real tolerance_)
{
  m_CollisionData[0] = friction_;
  m_CollisionData[1] = restitution_;
  m_CollisionData[2] = tolerance_;
}
*/