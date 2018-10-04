/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>


void SimpleParallel::OnInitialize(){
	for (m_currentNode = m_childBehaviors.begin();
		m_currentNode != m_childBehaviors.end();
		m_currentNode++) {
		BH_Observer obs = std::bind(&SimpleParallel::OnChildComplete, this, std::placeholders::_1);
		m_pBT->Init(**m_currentNode, &obs);
	}
}

BH_Status SimpleParallel::OnUpdate(){
	return BH_RUNNING;
}

void SimpleParallel::OnTerminate(BH_Status){

}


void SimpleParallel::OnChildComplete(BH_Status) {

}