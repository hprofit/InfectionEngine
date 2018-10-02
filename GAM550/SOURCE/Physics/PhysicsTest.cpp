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