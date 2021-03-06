/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/

#include <Stdafx.h>

static void PrintLinkedList(MemoryBlock* node) {
	if (!node) {
		printf("\n\nFINISHED\n\n");
		return;
	}
	//printf("MEMORY BLOCK :: node: %p :: pData: %p :: Size: %d :: FreeSize: %d :: Next: %p :: Prev: %p\n",
	//	node, node->pData, node->size, node->freesize, node->next, node->prev);
	PrintLinkedList(node->next);
}

static void PrintPointer(const char* msg, void* ptr) {
	printf("%s: %p\n", msg, ptr);
}

static void PrintMemoryBlock(MemoryBlock* block) {
	PrintPointer("Next MB", block->next);
	if (block->next)
		PrintPointer("Next->Prev MB", block->next->prev);
	PrintPointer("Prev MB", block->prev);
	printf("Block Size: %u : Block Freesize: %u\n", (unsigned) block->size, (unsigned) block->freesize);
}

MemoryManager::MemoryManager():
	m_TotalBufferSize(DEFAULT_BUFFER_SIZE_BYTE), m_pHead(nullptr) {

	m_Buffer = malloc(m_TotalBufferSize);
	m_pHead = (MemoryBlock*)malloc(sizeof(MemoryBlock));
	m_pHead->Initialize(m_Buffer, 0, m_TotalBufferSize);
	m_NumCachedBlock = 0;

	m_ComponentFactory = new ComponentFactory();
	m_AIFactory = new AI_Factory();
	// GameObject preallocation
	for (unsigned i = 0; i < MAX_GAMEOBJECT_CACHESIZE; i++) {
		m_GameObjectPool.push_back( new GameObject(INFECT_GUID.GetGUID()));
	}
	m_GameObjectFirstDead = m_GameObjectPool.begin();
	// ComponentPool preallocation, components will be preallocated by the comp mngrs
	for (unsigned _enum = 0; _enum < NUM_COMPONENTS; _enum++) {
		ComponentType type = static_cast<ComponentType>(_enum);
		// TODO: ask each component for cache size
		m_ComponentPool[type] = new std::list<Component*>();
	}

}
void MemoryManager::LateInit() {
	for (unsigned _enum = 0; _enum < NUM_COMPONENTS; _enum++) {
		ComponentType type = static_cast<ComponentType>(_enum);
		m_ComponentFirstDeadIter[type] = m_ComponentPool[type]->begin();
	}
}

MemoryManager::~MemoryManager() {
	if (m_pHead){
		m_pHead->CleanUp();
		free(m_pHead);
		m_pHead = nullptr;
	}
	free(m_Buffer);

	// maybe clear pools? 
	for (unsigned i = 0; i < NUM_COMPONENTS; i++) {
		std::list<Component*>* pCompPool = m_ComponentPool[static_cast<ComponentType>(i)];
		std::list<Component*>::iterator iter = pCompPool->begin();
		while (iter != pCompPool->end()) {
			Component* pComp = *iter;
			delete pComp;
			iter++;
		}
	}
	std::list<GameObject*>::iterator iter = m_GameObjectPool.begin();
	while (iter != m_GameObjectPool.end()) {
		delete *iter;
		iter++;
	}
	
	delete m_ComponentFactory;
	delete m_AIFactory;
}

void* MemoryManager::Alloc(std::size_t size) {
	MemoryBlock *current = m_pHead;
	// find next available slot 
	while (current && (current->freesize < size)) {
		current = current->next;
	}

	// request failed
	if (!current) {
		printf("MemoryManager::Alloc() FAILED - No Available Slot\n");
		return nullptr;
	}
	// create a new memoryblock and insert into the list
	MemoryBlock* temp = current->next;
	current->next = NewMemoryBlock();
	current->next->Initialize(	static_cast<unsigned char*>(current->pData) + current->size, 
								size, 
								current->freesize - size );

	if (temp) 
		temp->prev = current->next;
	current->next->next = temp;
	current->freesize = 0;
	current->next->prev = current;

	return current->next->pData;
}

void MemoryManager::Free(void* ptr){
	MemoryBlock *current = m_pHead->next;
	if (!current || (current->pData == nullptr)) {
		printf("MEMMNGR::FREE() NEVER REACH HERE!\n");
		return;
	}
	while(current && (current->pData != ptr)){
		current = current->next;
	}
	if (!current) {
		//printf("MEMMNGR::FREE() NEVER REACH HERE --2 !!!\n");
		return;
	}
	// remove links
	if (current->next)
		current->next->prev = current->prev;
	if (current->prev) {
		current->prev->next = current->next;
		current->prev->freesize += current->size + current->freesize;
	}
	current->freesize = 0;

	current->next = nullptr;
	current->prev = nullptr;
	// try caching current block. If not, free.
	Recycle(current);
}

 MemoryBlock* MemoryManager::NewMemoryBlock(){
	if (m_NumCachedBlock == 0) { // no cached memoryblock to be reused
		return (MemoryBlock*)malloc(sizeof(MemoryBlock));
	}
	else {									// use previously recycled memory block
		return m_Cache[--m_NumCachedBlock]; // NOTE: cacheIndex = m_NumCachedBlock-1
	}
}

 void MemoryManager::Recycle(MemoryBlock* pNode){
	if (m_NumCachedBlock == MAX_CACHE_SIZE_NUM-1){
		//printf("CACHE IS FULL -- MAX_CACHE_SIZE_NUM: %d\n", MAX_CACHE_SIZE_NUM);
		free(pNode);
		return;
	}
	m_Cache[m_NumCachedBlock++] = pNode;
}

void* MemoryManager::m_Buffer = nullptr;


// GAMEOBJECT RECYCLE 
GameObject* MemoryManager::GetNewGameObject() {
	//return new GameObject(id);
	GameObject* pGO = *m_GameObjectFirstDead;
	if (!pGO->IsActive()) {
		pGO->SetActive(true);
		m_GameObjectFirstDead++;
		return pGO; // this pGO needs to be reset with appropriate data
					// only GUID is valid
	}
	printf("NO MORE EMPTY GAMEOBJECT - Current cache size: %d\n", MAX_GAMEOBJECT_CACHESIZE);
	return nullptr;
}

// GAMEOBJECT CACHING 
void MemoryManager::DeleteGameObject(GameObject* ptr) {
	ptr->Deactivate();
	ptr->SetActive(false);
	std::list<GameObject*>::iterator it = std::find(m_GameObjectPool.begin(), m_GameObjectPool.end(), ptr);
	m_GameObjectPool.splice(m_GameObjectPool.end(), m_GameObjectPool, it);
}

Component* MemoryManager::GetNewComponent(ComponentType cType) {
	Component* compPtr = *m_ComponentFirstDeadIter[cType];
	if (!compPtr->IsActive()) {
		compPtr->SetDirty(true);
		compPtr->SetActive(true);
		m_ComponentFirstDeadIter[cType]++;
		return compPtr;
	}
	return nullptr;
}

void MemoryManager::DeleteComponent(Component* ptr) {
	ptr->SetActive(false);
	std::list<Component*>* cList = m_ComponentPool[ptr->GetType()];
	std::list<Component*>::iterator it = std::find(cList->begin(), cList->end(), ptr);
	cList->splice(cList->end(), *cList, it);
}

AI_State* MemoryManager::GetNewState(std::string stateName) {
	return m_AIFactory->CreateState(stateName);
}
void MemoryManager::DeleteState(AI_State* ptr) {
	delete ptr;
}