/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Holden Profit>
- End Header --------------------------------------------------------*/

#pragma once
#ifndef GUID_GENERATOR_H
#define GUID_GENERATOR_H

class GUID_Generator {
private:
	InfectGUID m_GUID;

public:
	GUID_Generator() : m_GUID(0) {};
	~GUID_Generator() {};

	InfectGUID GetGUID() { return m_GUID++; }
};

#endif 