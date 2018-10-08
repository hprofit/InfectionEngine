
/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Jiyun Ruan>
- End Header --------------------------------------------------------*/
 
#include <Stdafx.h>

RigidBodyComponentManager::RigidBodyComponentManager() {
  m_Components = (std::list<RigidBodyComponent*>*)INFECT_MEMORY.GetComponentPool(RigidBodyComponent::Type);
}

/*void printMatrix4x4(const Matrix4x4 & mat)
{
  cout << setprecision(2);
  cout.setf(ios::fixed, ios::floatfield);
  std::cout << " " << mat.m_matrix[0][0] << " " << mat.m_matrix[0][1] << " " << mat.m_matrix[0][2] << " " << mat.m_matrix[0][3] << std::endl
    << " " << mat.m_matrix[1][0] << " " << mat.m_matrix[1][1] << " " << mat.m_matrix[1][2] << " " << mat.m_matrix[1][3] << std::endl
    << " " << mat.m_matrix[2][0] << " " << mat.m_matrix[2][1] << " " << mat.m_matrix[2][2] << " " << mat.m_matrix[2][3] << std::endl
    << " " << mat.m_matrix[3][0] << " " << mat.m_matrix[3][1] << " " << mat.m_matrix[3][2] << " " << mat.m_matrix[3][3] << std::endl;
}*/

void RigidBodyComponentManager::Update(float dt)
{
  for each (RBC rgbComp in *m_Components) { 
	  if (!rgbComp->IsActive()) break;
	  Vector3D pos = rgbComp->Parent()->GetComponent<TransformComponent>()->WorldPosition();
	  Vector3D rot = rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector();
	  Vector3D scale = rgbComp->Parent()->GetComponent<TransformComponent>()->GetScaleVector();

	  Vector3D temp(rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector());
    Vector3D rotation;
	  switch (rgbComp->cur_type)
	  {
	  case RigidBodyComponent::RigidBodyType::BoxRigidBody:
		  rgbComp->Parent()->GetComponent<TransformComponent>()->SetPosition(rgbComp->GetBoxPointer()->body->getPosition());

      rotation = rgbComp->GetBoxPointer()->body->getOrientation().QuaternionToDegree(rgbComp->GetBoxPointer()->body->getOrientation());

      rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles(-rotation.x, rotation.z, rotation.y);

     /* printMatrix4x4(rgbComp->Parent()->GetComponent<TransformComponent>()->GetTransform());
      std::cout <<"-------------------" << std::endl;
      printMatrix4x4(rgbComp->GetBoxPointer()->body->getTransform());
      std::cout << rotation << std::endl;
      std::cout << "-------------------" << std::endl;*/
      //cout << rgbComp->Parent()->GetComponent<TransformComponent>()->GetTransform() << endl;

		  /*rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles((rgbComp->GetBoxPointer()->body->getRotation().x*RAD_TO_DEG ) ,
			  (rgbComp->GetBoxPointer()->body->getRotation().y*RAD_TO_DEG ) ,
			  (rgbComp->GetBoxPointer()->body->getRotation().z*RAD_TO_DEG ) );*/
		  //rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles(0,
			 // 45,
			 // 0);


		  /*if (abs(rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector().x) < .2f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleX(temp.x);
		  if (abs(rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector().y) < .2f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleY(temp.y);
		  if (abs(rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector().z) < .2f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleZ(temp.z);*/

		  /*if (abs(rgbComp->GetBoxPointer()->body->getRotation().x) < .00010f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleX(temp.x);
		  else
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleX((temp.x + rgbComp->GetBoxPointer()->body->getRotation().x) * -1.0f);

		  if (abs(rgbComp->GetBoxPointer()->body->getRotation().y) < .00010f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleY(temp.y);
		  else
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleY((temp.y + rgbComp->GetBoxPointer()->body->getRotation().y) * -1.0f);

		  if (abs(rgbComp->GetBoxPointer()->body->getRotation().z) < .00010f)
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleZ(temp.z);
		  else
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngleZ(temp.z + rgbComp->GetBoxPointer()->body->getRotation().z);
		  */
		  //rgbComp->Parent()->GetComponent<TransformComponent>()->SetAngles(temp.x ,
			 // temp.y - 5,
			 // temp.z ); 
		  //cout << rgbComp->Parent()->GetComponent<TransformComponent>()->GetRotVector() << endl;

		  //cout << rgbComp->GetBoxPointer()->body->getRotation() << endl;
		  break;
	  case RigidBodyComponent::RigidBodyType::SphereRigidBody:
			  rgbComp->Parent()->GetComponent<TransformComponent>()->SetPosition(rgbComp->GetSpherePointer()->body->getPosition());
			  
		 break;
	  }
	
  }
}

// TODO: Super naive attempt at this
void RigidBodyComponentManager::HandleEvent(Event * pEvent)
{
  for each (RBC plComp in *m_Components) {
    if (!plComp->IsActive()) break;
    plComp->HandleEvent(pEvent);
  }
}

void RigidBodyComponentManager::Init() {
  INFECT_MEMORY.ComponentPoolInit<RigidBodyComponent>(RigidBodyComponent::Type, RigidBodyComponent::CACHESIZE);
}