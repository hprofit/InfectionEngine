
#include <Stdafx.h>



void PhyTest::PhyTest_Setup()
{
	// render a ball
	GameObject* pGO = INFECT_GOM.SpawnGameObject();
	MeshComponent* pMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
	pMeshComp->SetScene(SPHERE_PRIMITIVE);


	pTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
	pTransComp->SetPosition(bullet.body->getPosition());
	pTransComp->SetScale(2.0f, 2.0f, 2.0f);
	/*pTransComp->SetPosition(Vector3D(0, 0, 0, 1));
	pTransComp->SetScale(2.0f, 2.0f, 2.0f);*/

	pGO->AddComponent(pMeshComp);
	pGO->AddComponent(pTransComp);
	pGO->LateInitialize();

	// render a plane
	GameObject* pplane = INFECT_GOM.SpawnGameObject();
	MeshComponent* planeMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
	planeMeshComp->SetScene(PLANE_PRIMITIVE);

	TransformComponent* planeTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);

	planeTransComp->SetPosition(Vector3D(0, -10, 0, 1));
	planeTransComp->SetScale(100.0f, 100.0f, 100.0f);
	planeTransComp->RotateX(180+90);

	pplane->AddComponent(planeMeshComp);
	pplane->AddComponent(planeTransComp);
	pplane->LateInitialize();

  //set = plane
  plane.direction = Vector3D(0, 1, 0);
  plane.offset = -10; // SetPosition(Vector3D(0, -10, 0, 1));

  // cube
  box[0].setState(0, -9, 20); box[1].setState(0, -7, 20); box[2].setState(0, -5, 20);


  GameObject* cube[3];

  for (int i = 0; i < 3; i++)
  {
    cube[i]= INFECT_GOM.SpawnGameObject();
    MeshComponent* cubeMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
    cubeMeshComp->SetScene(CUBE_PRIMITIVE);
    cubeMeshComp->SetIsLit(false);
    cubeTransComp[i] = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
    cubeTransComp[i]->SetPosition(box[i].body->getPosition());
    cubeTransComp[i]->SetScale(1.0f, 1.0f, 1.0f);

    cube[i]->AddComponent(cubeMeshComp);
    cube[i]->AddComponent(cubeTransComp[i]);
    cube[i]->LateInitialize();
  }
  

  // set collision
  cData.contactArray = contacts;
}


void PhyTest::PhyTest_Update()
{
  // first active rigibody
  updateObjects();
	pTransComp->SetPosition(bullet.body->getPosition());
	pTransComp->SetScale(2.0f, 2.0f, 2.0f);
  for (int i = 0; i < 3; i++)
  {
    cubeTransComp[i]->SetPosition(box[i].body->getPosition());
  }
  

  // second loading collision
  generateContacts();

  // solve collision
  resolver.resolveContacts(
    cData.contactArray,
    cData.contactCount,
    INFECT_FRAMERATE.GetFrameTime()
  );
}

void PhyTest::updateObjects()
{
	bullet.body->integrate(INFECT_FRAMERATE.GetFrameTime());
	bullet.calculateInternals();

  for (int i = 0; i < 3; i++)
  {
    box[i].body->integrate(INFECT_FRAMERATE.GetFrameTime());
    box[i].calculateInternals();
  }
  
}

void PhyTest::generateContacts()
{

  // Set up the collision data structure
  cData.reset(maxContacts);
  cData.friction = (real)0.9;
  cData.restitution = (real)0.1;
  cData.tolerance = (real)0.1;

  // Check ground plane-box collisions
  if (!cData.hasMoreContacts()) return;
  for (Box *itr = box; itr < box + 3; itr++)
  {
    physics::CollisionDetector::boxAndHalfSpace(*itr, plane, &cData);
    // Check ground box-box collisions
    for (Box *itr2 = box; itr2 < box + 3; itr2++)
    {
      if (itr != itr2)
        physics::CollisionDetector::boxAndBox(*itr, *itr2, &cData);
    }
    // Check ground box-sphere collisions
    physics::CollisionDetector::boxAndSphere(*itr, bullet, &cData);
  }
    

  // Check ground plane-sphere collisions
  if (!cData.hasMoreContacts()) 
    return;
  physics::CollisionDetector::sphereAndHalfSpace(bullet, plane, &cData);


}

void PhyTest::AmmoRound::setState()
{
	body->setMass(200.0f); // 200.0kg
	body->setVelocity(0.0f, 9.0f, 10.0f); // 50m/s
	body->setAcceleration(0.0f, -21.0f, 0.0f);
	body->setDamping(0.99f, 0.8f);
	radius = 2.f;

	body->setCanSleep(false);
	body->setAwake();

	Matrix3x3 tensor;
	real coeff = 0.4f*body->getMass()*radius*radius;
	tensor.setInertiaTensorCoeffs(coeff, coeff, coeff);
	body->setInertiaTensor(tensor);

	// Set the data common to all particle types
	body->setPosition(0.0f, 1.5f, 0.0f);
	//startTime = TimingData::get().lastFrameTimestamp;
	// Clear the force accumulators
	body->calculateDerivedData();
	calculateInternals();

}

void PhyTest::Box::setState(real x, real y, real z)
{
  body->setPosition(x, y, z); // 0 6 20
  body->setOrientation(1, 0, 0, 0);
  body->setVelocity(0, 0, 0);
  body->setRotation(Vector3D(0, 0, 0));
  halfSize = Vector3D(1, 1, 1);

  real mass = halfSize.x * halfSize.y * halfSize.z * 8.0f;
  body->setMass(mass);

  Matrix3x3 tensor;
  tensor.setBlockInertiaTensor(halfSize, mass);
  body->setInertiaTensor(tensor);

  body->setLinearDamping(0.95f);
  body->setAngularDamping(0.8f);
  body->clearAccumulators();
  body->setAcceleration(0, -10.0f, 0);

  body->setCanSleep(false);
  body->setAwake();

  body->calculateDerivedData();
  calculateInternals();
}

void PhyFractureTest::Block::calculateMassProperties(real invDensity)
{
  // Check for infinite mass
  if (invDensity <= 0)
  {
    // Just set zeros for both mass and inertia tensor
    body->setInverseMass(0);
    body->setInverseInertiaTensor(Matrix3x3(0.f, 0.f, 0.f,
      0.f, 0.f, 0.f,
      0.f, 0.f, 0.f));
  }
  else
  {
    // Otherwise we need to calculate the mass
    real volume = halfSize.Length() * 2.0;
    real mass = volume / invDensity;

    body->setMass(mass);

    // And calculate the inertia tensor from the mass and size
    mass *= 0.333f;
    Matrix3x3 tensor;
    tensor.setInertiaTensorCoeffs(
      mass * halfSize.y*halfSize.y + halfSize.z*halfSize.z,
      mass * halfSize.y*halfSize.x + halfSize.z*halfSize.z,
      mass * halfSize.y*halfSize.x + halfSize.z*halfSize.y
    );
    body->setInertiaTensor(tensor);
  }

}

void PhyFractureTest::Block::setState(const Vector3D &position,
  const Quaternion &orientation,
  const Vector3D &extents,
  const Vector3D &velocity)
{
  body->setPosition(position);
  body->setOrientation(orientation);
  body->setVelocity(velocity);
  body->setRotation(::Vector3D(0, 0, 0));
  halfSize = extents;

  ::real mass = halfSize.x * halfSize.y * halfSize.z * 8.0f;
  body->setMass(mass);

  Matrix3x3 tensor;
  tensor.setBlockInertiaTensor(halfSize, mass);
  body->setInertiaTensor(tensor);

  body->setLinearDamping(0.95f);
  body->setAngularDamping(0.8f);
  body->clearAccumulators();
  body->setAcceleration(0, -10.0f, 0);

  //body->setCanSleep(false);
  body->setAwake();

  body->calculateDerivedData();
}

void PhyFractureTest::Block::divideBlock(const physics::Contact& contact,
  Block* target, Block* blocks)
{
  // Find out if we're block one or two in the contact structure, and
  // therefore what the contact normal is.
  Vector3D normal = contact.contactNormal;
  physics::RigidBody *body = contact.body[0];
  if (body != target->body)
  {
    normal.Negate();
    body = contact.body[1];
  }

  // Work out where on the body (in body coordinates) the contact is
  // and its direction.
  Vector3D point = body->getPointInLocalSpace(contact.contactPoint);
  normal = body->getDirectionInLocalSpace(normal);

  // Work out the centre of the split: this is the point coordinates
  // for each of the axes perpendicular to the normal, and 0 for the
  // axis along the normal.
  point = point - normal * (point * normal);

  // Take a copy of the half size, so we can create the new blocks.
  Vector3D size = target->halfSize;

  // Take a copy also of the body's other data.
  physics::RigidBody tempBody;
  tempBody.setPosition(body->getPosition());
  tempBody.setOrientation(body->getOrientation());
  tempBody.setVelocity(body->getVelocity());
  tempBody.setRotation(body->getRotation());
  tempBody.setLinearDamping(body->getLinearDamping());
  tempBody.setAngularDamping(body->getAngularDamping());
  tempBody.setInverseInertiaTensor(body->getInverseInertiaTensor());
  tempBody.calculateDerivedData();

  // Remove the old block
  target->exists = false;

  // Work out the inverse density of the old block
  real invDensity =
    halfSize.Length() * 8 * body->getInverseMass();

  // Now split the block into eight.
  for (unsigned i = 0; i < 8; i++)
  {
    // Find the minimum and maximum extents of the new block
    // in old-block coordinates
    Vector3D min, max;
    if ((i & 1) == 0) {
      min.x = -size.x;
      max.x = point.x;
    }
    else {
      min.x = point.x;
      max.x = size.x;
    }
    if ((i & 2) == 0) {
      min.y = -size.y;
      max.y = point.y;
    }
    else {
      min.y = point.y;
      max.y = size.y;
    }
    if ((i & 4) == 0) {
      min.z = -size.z;
      max.z = point.z;
    }
    else {
      min.z = point.z;
      max.z = size.z;
    }

    // Get the origin and half size of the block, in old-body
    // local coordinates.
    Vector3D halfSize = (max - min) * 0.5f;
    Vector3D newPos = halfSize + min;

    // Convert the origin to world coordinates.
    newPos = tempBody.getPointInWorldSpace(newPos);

    // Work out the direction to the impact.
    Vector3D direction = newPos - contact.contactPoint;
    direction.normalise();

    // Set the body's properties (we assume the block has a body
    // already that we're going to overwrite).
    blocks[i].body->setPosition(newPos);
    blocks[i].body->setVelocity(tempBody.getVelocity() + direction * 10.0f);
    blocks[i].body->setOrientation(tempBody.getOrientation());
    blocks[i].body->setRotation(tempBody.getRotation());
    blocks[i].body->setLinearDamping(tempBody.getLinearDamping());
    blocks[i].body->setAngularDamping(tempBody.getAngularDamping());
    blocks[i].body->setAwake(true);
    blocks[i].body->setAcceleration(Vector3D::GRAVITY);
    blocks[i].body->clearAccumulators();
    blocks[i].body->calculateDerivedData();
    blocks[i].offset = Matrix4x4(1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 0.f);;
    blocks[i].exists = true;
    blocks[i].halfSize = halfSize;

    // Finally calculate the mass and inertia tensor of the new block
    blocks[i].calculateMassProperties(invDensity);
  }
}

void PhyFractureTest::SetBall()
{
  ball.body = new physics::RigidBody();
  ball.radius = 0.25f;
  ball.body->setMass(5.0f);
  ball.body->setDamping(0.9f, 0.9f);
  Matrix3x3 it;
  it.setDiagonal(5.0f, 5.0f, 5.0f);
  ball.body->setInertiaTensor(it);
  ball.body->setAcceleration(Vector3D::GRAVITY);

  ball.body->setCanSleep(false);
  ball.body->setAwake(true);

  // Set up the ball
  ball.body->setPosition(0, 5.0f, 20.0f);
  ball.body->setOrientation(1, 0, 0, 0);
  ball.body->setVelocity(
    2,
    4,
    -20.0f
  );
  ball.body->setRotation(0, 0, 0);
  ball.body->calculateDerivedData();
  ball.body->setAwake(true);
  ball.calculateInternals();

  hit = false;
}

void PhyFractureTest::SetBox()
{
  blocks[0].exists = true;
  for (Block *block = blocks + 1; block < blocks + MAX_BLOCKS; block++)
  {
    block->exists = false;
  }

  // Set the first block
  blocks[0].halfSize = Vector3D(4, 4, 4);
  blocks[0].body->setPosition(0, 7, 0);
  blocks[0].body->setOrientation(1, 0, 0, 0);
  blocks[0].body->setVelocity(0, 0, 0);
  blocks[0].body->setRotation(0, 0, 0);
  blocks[0].body->setMass(100.0f);
  Matrix3x3 it;
  it.setBlockInertiaTensor(blocks[0].halfSize, 100.0f);
  blocks[0].body->setInertiaTensor(it);
  blocks[0].body->setDamping(0.9f, 0.9f);
  blocks[0].body->calculateDerivedData();
  blocks[0].calculateInternals();

  blocks[0].body->setAcceleration(Vector3D::GRAVITY);
  blocks[0].body->setAwake(true);
  blocks[0].body->setCanSleep(true);


  ball_active = true;
}

void PhyFractureTest::generateContacts()
{
  hit = false;

  // Create the ground plane data
  physics::CollisionPlane plane;
  plane.direction = Vector3D(0, 1, 0);
  plane.offset = 0;

  // Set up the collision data structure
  cData.reset(maxContacts);
  cData.friction = (real)0.9;
  cData.restitution = (real)0.2;
  cData.tolerance = (real)0.1;

  // Perform collision detection
  Matrix4x4 transform, otherTransform;
  Vector3D position, otherPosition;
  for (Block *block = blocks; block < blocks + MAX_BLOCKS; block++)
  {
    if (!block->exists) continue;

    // Check for collisions with the ground plane
    if (!cData.hasMoreContacts()) return;
    physics::CollisionDetector::boxAndHalfSpace(*block, plane, &cData);

    if (ball_active)
    {
      // And with the sphere
      if (!cData.hasMoreContacts()) return;
      if (physics::CollisionDetector::boxAndSphere(*block, ball, &cData))
      {
        hit = true;
        fracture_contact = cData.contactCount - 1;
      }
    }

    // Check for collisions with each other box
    for (Block *other = block + 1; other < blocks + MAX_BLOCKS; other++)
    {
      if (!other->exists) continue;

      if (!cData.hasMoreContacts()) return;
      physics::CollisionDetector::boxAndBox(*block, *other, &cData);
    }
  }

  // Check for sphere ground collisions
  if (ball_active)
  {
    if (!cData.hasMoreContacts()) return;
    physics::CollisionDetector::sphereAndHalfSpace(ball, plane, &cData);
  }
}

void PhyFractureTest::update()
{
  for (Block *block = blocks; block < blocks + MAX_BLOCKS; block++)
  {
    if (block->exists)
    {
      block->body->integrate(INFECT_FRAMERATE.GetFrameTime());
      block->calculateInternals();
    }
  }

  //if (ball_active)
  {
    ball.body->integrate(INFECT_FRAMERATE.GetFrameTime());
    ball.calculateInternals();
  }

  auto a = ball.body->getPosition();
  pTransComp->SetPosition(ball.body->getPosition());
  pTransComp->SetScale(2.0f, 2.0f, 2.0f);
  for (int i = 0; i < 9; i++)
  {
    if (hit == false && i != 0)
      break;
    cubeTransComp[i]->SetPosition(blocks[i].body->getPosition());
    cubeTransComp[i]->SetScale(Vector3D(1, 1, 1));
    cubeTransComp[i]->RotateX(blocks[i].body->getRotation().x);
    cubeTransComp[i]->RotateY(blocks[i].body->getRotation().y);
    cubeTransComp[i]->RotateZ(blocks[i].body->getRotation().z);
  }



  generateContacts();
  resolver.resolveContacts(
    cData.contactArray,
    cData.contactCount,
    INFECT_FRAMERATE.GetFrameTime()
  );




  if (hit)
  {
    blocks[0].divideBlock(
      cData.contactArray[fracture_contact],
      blocks,
      blocks + 1
    );
    ball_active = false;
  }

}

void PhyFractureTest::PhyTest_Setup()
{
  // render a ball
  GameObject* pGO = INFECT_GOM.SpawnGameObject();
  MeshComponent* pMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
  pMeshComp->SetScene(SPHERE_PRIMITIVE);


  pTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);
  pTransComp->SetPosition(ball.body->getPosition());
  pTransComp->SetScale(0.25f, 0.25f, 0.25f);
  /*pTransComp->SetPosition(Vector3D(0, 0, 0, 1));
  pTransComp->SetScale(2.0f, 2.0f, 2.0f);*/

  pGO->AddComponent(pMeshComp);
  pGO->AddComponent(pTransComp);
  pGO->LateInitialize();

  // render a plane
  GameObject* pplane = INFECT_GOM.SpawnGameObject();
  MeshComponent* planeMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
  planeMeshComp->SetScene(PLANE_PRIMITIVE);

  TransformComponent* planeTransComp = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);

  planeTransComp->SetPosition(Vector3D(0, -10, 0, 1));
  planeTransComp->SetScale(100.0f, 100.0f, 100.0f);
  planeTransComp->RotateX(180 + 90);

  pplane->AddComponent(planeMeshComp);
  pplane->AddComponent(planeTransComp);
  pplane->LateInitialize();

  //set = plane
  plane.direction = Vector3D(0, 1, 0);
  plane.offset = -10; // SetPosition(Vector3D(0, -10, 0, 1));


  GameObject* cube[9];

  for (int i = 0; i < 9; i++)
  {
    cube[i] = INFECT_GOM.SpawnGameObject();
    MeshComponent* cubeMeshComp = (MeshComponent*)INFECT_MEMORY.GetNewComponent(MeshComponent::Type);
    cubeMeshComp->SetScene(CUBE_PRIMITIVE);
    cubeMeshComp->SetIsLit(false);
    cubeTransComp[i] = (TransformComponent*)INFECT_MEMORY.GetNewComponent(TransformComponent::Type);

    if (i != 0)
    {
      cubeTransComp[i]->SetPosition(Vector3D(-99, -99, 0));
      //cubeTransComp[i]->SetScale(blocks[i].halfSize);
      cubeTransComp[i]->SetScale(Vector3D(1, 1, 1));
    }

    else
    {
      cubeTransComp[i]->SetPosition(blocks[i].body->getPosition());
      cubeTransComp[i]->SetScale(4.0f, 4.0f, 4.0f);
    }

    cubeTransComp[i]->RotateX(blocks[i].body->getRotation().x);
    cubeTransComp[i]->RotateY(blocks[i].body->getRotation().y);
    cubeTransComp[i]->RotateZ(blocks[i].body->getRotation().z);


    cube[i]->AddComponent(cubeMeshComp);
    cube[i]->AddComponent(cubeTransComp[i]);
    cube[i]->LateInitialize();


    // set collision
    cData.contactArray = contacts;
  }
}