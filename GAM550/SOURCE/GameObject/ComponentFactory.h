/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

typedef Component* CreationFunction(InfectGUID);

class ComponentFactory {
private:
	//std::unordered_map <ComponentType, CreationFunction*> m_creationFunctions;
	CreationFunction* m_creationFunctions[ComponentType::NUM_COMPONENTS];

	InfectGUID _GetGUID() const;
public:
	ComponentFactory();
	~ComponentFactory() {};

	template <typename C>
	C* CreateComponent(std::string component) 
	{
		// check if there's empty component stored in cache
		//Component* cachedComp = INFECT_MEMORY.GetNewComponent(component);
		//if (cachedComp) {
		//	return cachedComp;
		//}

		// no empty component available, return create new comp
		ComponentType cType;
		for (int i = 0; i < ComponentType::NUM_COMPONENTS; ++i) {
			if (strcmp(ComponentTypeText[i], component.c_str())) {
				cType = (ComponentType)i;
				i = ComponentType::NUM_COMPONENTS;
			}
		}

		CreationFunction* Create = m_creationFunctions[cType];
		if (Create)
			return  static_cast<C*>(Create(_GetGUID()));
		else
			return nullptr;
	}

	template <typename C>
	C* CreateComponent()
	{
		return  static_cast<C*>(CreateComponent<C>(std::string(ComponentTypeText[C::Type])));
	}

	void RegisterComponent(ComponentType cType, CreationFunction*) {};
};

#endif 