/*
 *  MuskMemoryManager.hpp
 *  MuskMemoryManager
 *
 *  Created by Muneeb Khan on 2016-06-25.
 *  Copyright © 2016 Muneeb Khan. All rights reserved.
 *
 */

#ifndef MuskMemoryManager_
#define MuskMemoryManager_

#define KILO 1024
#define MEGA (KILO*KILO)
#define TEN_MEGABYTES (10*MEGA)

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <stdlib.h>
#include <list>
#include <iostream>
using namespace std;

typedef unsigned char BYTE;

class MemoryChunk{
    
    public:
    MemoryChunk(size_t userDataSize):mc_next(NULL),mc_prev(NULL),mc_chunkSize(userDataSize),mc_free(true){};
    
    size_t mc_chunkSize;
    MemoryChunk* mc_next;
    MemoryChunk* mc_prev;
    bool mc_free;
};

class BaseMemoryManager
{
    
    public:
    
    MemoryChunk* mm_headMemChunk;
    MemoryChunk* mm_tailMemChunk;
    
    virtual void* mm_allocate(const size_t alloc_size) = 0;
    virtual void mm_free(const void* ptrToFree) = 0;
    
    size_t mm_totalPoolSize;
    size_t mm_userDataSize;
    size_t mm_freePoolSize;

};

class MuskMemoryManager: private BaseMemoryManager{
    
    private:
    
    list<void*> memoryPoolsList;
    
    MuskMemoryManager();
    
    public:
    
    static MuskMemoryManager& getInstance();
    
    void* askOSMemAllocate(const size_t);
    
    void* mm_allocate(const size_t);
    
    void mm_free(const void*);
    
    void displayInfo();
    
    ~MuskMemoryManager();
    
};

#pragma GCC visibility pop
#endif
