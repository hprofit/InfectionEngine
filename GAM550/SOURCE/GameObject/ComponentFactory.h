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
typedef void RegisterFunction(Component*);

class ComponentFactory {
private:
	//std::unordered_map <ComponentType, CreationFunction*> m_creationFunctions;
	CreationFunction* m_creationFunctions[ComponentType::NUM_COMPONENTS];
	RegisterFunction* m_registerFunctions[ComponentType::NUM_COMPONENTS];

	InfectGUID _GetGUID() const;
public:
	ComponentFactory();
	~ComponentFactory() {};

	template <typename C>
	C* CreateComponent(std::string component) 
	{
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
		CreationFunction* Create = m_creationFunctions[C::Type];
		if (Create)
			return  static_cast<C*>(Create(_GetGUID()));
		else
			return nullptr;
	}

	//Component* CreateComponent(ComponentType cType)
	//{
	//	CreationFunction* Create = m_creationFunctions[cType];
	//	if (Create)
	//		return  Create(_GetGUID());
	//	else
	//		return nullptr;
	//}

	void RegisterComponent(ComponentType cType);
};

#endif 