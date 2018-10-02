/* Start Header -------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: <Hyoyup Chung>
- End Header --------------------------------------------------------*/
#pragma once
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

using namespace std;

class Component;

const unsigned	 MAX_CACHE_SIZE_NUM = 100;			// # of Cacheable MemoryBlocks
const size_t DEFAULT_BUFFER_SIZE_BYTE = 52428800;	// 50 MB = 50*1024*1024 byte
const unsigned	 MAX_GAMEOBJECT_CACHESIZE = 300;

class MemoryManagerInterface {
public:
	virtual void* Alloc(std::size_t) = 0;
	virtual void Free(void*) = 0;
};

typedef struct ListNode {
	void *pData; 				// pointer to data block's position in m_Buffer
	size_t size;				// size of this data block
	size_t freesize;			// available memory space b/w current and next block
	ListNode *next;				// next memoryblock
	ListNode *prev;				// previous memoryblock

	void Initialize(void *_ptr, size_t _size, size_t _freesize) {
		pData = _ptr;
		size = _size;
		freesize = _freesize;
		next = prev = nullptr;
	}

	void CleanUp() {
		// recursively delete blocks
		MemoryBlock *current = this;
		if (current->next)
		{
			current->next->CleanUp();
			free(current->next);
		}
	}
} MemoryBlock;

template <typename T>
class ObjNode {
public:
	T Data;
	ObjNode* next;
	ObjNode* prev;

	ObjNode() : Data(0), next(nullptr), prev(nullptr) {}
	ObjNode(T data) : Data(data), next(nullptr), prev(nullptr) {}
	~ObjNode<T>() { delete Data; }
	void setNext(ObjNode* pNode) {
		ObjNode* temp = next;
		next = pNode;
		pNode->prev = this;
		pNode->next = temp;
		if (temp != nullptr)
			temp->prev = pNode;
	}

	void setData(T d) { Data = d; }
};

template <typename T>
class ObjList {
public:
	ObjNode<T>* pHead;
	ObjNode<T>* pTail;
	ObjNode<T>* pFirstDead;
	unsigned size;

	ObjList(): pHead(nullptr), pTail(nullptr), pFirstDead(nullptr), size(0) {}
	ObjList(unsigned _size) : pHead(nullptr), pTail(nullptr), pFirstDead(nullptr), size(_size) {
		pHead = new ObjNode<T>();
		ObjNode<T>* cur = pHead;
		for (unsigned i = 1; i < size; ++i) {
			ObjNode<T>* newnode = new ObjNode<T>();
			cur->setNext(newnode);
			cur = cur->next;
		}
		pTail = cur;
		pFirstDead = pHead;
	}
	~ObjList() {
		CleanList();
	}
	void push_back(T pdata) {
		pFirstDead->setData(pdata);
		pFirstDead = pFirstDead->next;
	}

	void move_back(ObjNode<T>* pNode) {
		// prev Node link
		pNode->prev->next = pNode->next;
		// next Node link
		pNode->next->prev = pNode->prev;
		// cur Node link
		pTail->next = pNode;
		pNode->prev = pTail;
		pTail = pNode;
	}

	void CleanList() {
		ObjNode<T>* cur = pTail;
		while (cur->prev != nullptr) {
			cur = cur->prev;
			delete cur->next;
		}
		delete cur;
		pHead = pTail = pFirstDead = nullptr;
	}

};

class MemoryManager :public MemoryManagerInterface {
private:
	// RAW MEMORY STACK 
	static void* m_Buffer;						// Block of memory where all data exists
	const size_t m_TotalBufferSize;				// How big the buffer is in bytes
	MemoryBlock* m_pHead;						// First MemoryBlock in the linked list
	MemoryBlock* m_Cache[MAX_CACHE_SIZE_NUM];	// Stores MemoryBlocks to be deleted, when full all will be deleted at once
	int m_NumCachedBlock;						// Number of MemoryBlocks stored in the m_Cache

	MemoryBlock* NewMemoryBlock();
	void Recycle(MemoryBlock*);

	// OBJECT POOLING
	std::list<GameObject*> m_GameObjectPool;
	std::list<GameObject*>::iterator m_GameObjectFirstDead;
	std::unordered_map< ComponentType, std::list<Component*>* > m_ComponentPool;
	std::unordered_map< ComponentType, std::list<Component*>::iterator> m_ComponentFirstDeadIter;
	ComponentFactory* m_ComponentFactory;
public:
	MemoryManager();
	~MemoryManager();
	void LateInit();

	MemoryManager(const MemoryManager &) = delete;
	void operator=(const MemoryManager &) = delete;
	// operator new & delete 
	void* Alloc(std::size_t size);
	void Free(void* ptr);
	// ***********************************
	// GameObj Factory
	GameObject* GetNewGameObject();
	void DeleteGameObject(GameObject* ptr);
	// Component Factory
	template <typename C>
	void ComponentPoolInit(ComponentType cType, unsigned compPoolSize) {
		static_assert(std::is_base_of<Component, C>::value, "MemMngr: C must inherit from Component class.");
		unsigned poolSize = compPoolSize;
		for (unsigned i = 0; i < poolSize; i++) {
			m_ComponentPool[cType]->push_back(m_ComponentFactory->CreateComponent<C>());
		}
	}
	std::list<Component*>* GetComponentPool(ComponentType cType) { return m_ComponentPool[cType]; }
	Component* GetNewComponent(ComponentType type);
	void DeleteComponent(Component* ptr);
};
#endif
