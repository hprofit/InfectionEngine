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
}


void PhyTest::PhyTest_Update()
{
	RunPHY();
	pTransComp->SetPosition(bullet.body->getPosition());
	pTransComp->SetScale(2.0f, 2.0f, 2.0f);
}

void PhyTest::RunPHY()
{
	bullet.body->integrate(INFECT_FRAMERATE.GetFrameTime());
	bullet.calculateInternals();
}

void PhyTest::AmmoRound::setState()
{
	body->setMass(200.0f); // 200.0kg
	body->setVelocity(0.0f, 30.0f, 40.0f); // 50m/s
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