#include "Stdafx.h"

void LightBaseComponent::LateInitialize()
{
	INFECT_GOM.RegisterLight(mp_Parent);
}
