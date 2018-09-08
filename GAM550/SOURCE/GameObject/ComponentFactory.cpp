/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Moodie Ghaddar>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

ComponentFactory::ComponentFactory() {
	//m_creationFunctions["Transform"] = Transform::CreateInstance;
	m_creationFunctions["Mesh"] = MeshComponent::CreateInstance;
	//m_creationFunctions["Camera"] = Camera::CreateInstance;
}

Component* ComponentFactory::CreateComponent(std::string component) {
	//// check if there's empty component stored in cache
	//Component* cachedComp = TETRA_MEMORY.GetNewComponent(component);
	//if (cachedComp) {
	//	return cachedComp;
	//}
	//// no empty component available, return create new comp
	//CreationFunction* Create = m_creationFunctions[component];
	//if (Create)
	//	return Create();

	return nullptr;
}