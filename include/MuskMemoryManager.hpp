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
#include <map>

#include "BaseMemoryManager.hpp"

using namespace std;

typedef unsigned char BYTE;

/***************************** VIEW: MemoryChunk and BaseMemoryManager structure *********************************
 ***************************** DONOT UNCOMMENT -- THIS IS INCLUDED IN THE .hpp FILE ******************************

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
 
**************************** DONOT UNCOMMENT -- THIS IS INCLUDED IN THE .hpp FILE *******************************/

class MuskMemoryManager: private BaseMemoryManager{
    
    private:
    
    /* This structure is maintained to keep track of all memory pools allocated -- used at the end of application to destroy all allocated pools */
    list<void*> memoryPoolsList;
    
    /* The following two structures will be used to speed-up allocate and free operations */
    
    /* freeMemChunk_size_ptr_m is a map structure used to maintain the free memory heap. Used to make allocation fast */
    map<size_t,MemoryChunk*> freeMemChunk_size_ptr_m;
    
    /* inUseMemChunk_size_ptr_m is used to maintain track of all used memory chunks. Used to make deallocation (free) fast */
    map<MemoryChunk*,MemoryChunk*> inUseMemChunk_size_ptr_m;
    
    MuskMemoryManager();
    
    void* askOSMemAllocate(const size_t);
    
    public:
    
    static MuskMemoryManager& getInstance();
    
    void* mm_allocate(const size_t);
    
    void mm_free(const void*);
    
    void displayInfo();
    
    ~MuskMemoryManager();
    
};

#pragma GCC visibility pop
#endif
