/*
 *  MuskMemoryManager.cpp
 *  MuskMemoryManager
 *
 *  Created by Muneeb Khan on 2016-06-25.
 *  Copyright © 2016 Muneeb Khan. All rights reserved.
 *
 */

#include <iostream>
#include "MuskMemoryManager.hpp"

/* start-up code for the memory manager goes in the Constructor */
MuskMemoryManager::MuskMemoryManager(){
    
    mm_headMemChunk = NULL;
    
    /*Request memory from the OS at Memory Manager startup and allocate it as a free chunk*/
    
    /* User data is preceded by MemoryChunk which acts as header and links to subsequent user data */
    mm_headMemChunk = (MemoryChunk*) askOSMemAllocate(MEGA);
    mm_headMemChunk->mc_next = NULL;
    mm_headMemChunk->mc_prev = NULL;
    mm_headMemChunk->mc_chunkSize = MEGA - sizeof(MemoryChunk);
    mm_headMemChunk->mc_free = true;
    
    mm_tailMemChunk = mm_headMemChunk;
    
    mm_totalPoolSize = MEGA;
    mm_freePoolSize = MEGA - sizeof(MemoryChunk);
    
    memoryPoolsList.push_back((void*) mm_headMemChunk);
    freeMemChunk_size_ptr_m.insert(pair<size_t,MemoryChunk*>(mm_headMemChunk->mc_chunkSize,mm_headMemChunk));
    
}

MuskMemoryManager& MuskMemoryManager::getInstance(){
    
    static MuskMemoryManager mm;
    return mm;
    
}

/* The MemoryManager requests large memory chunks from OS and only when the memoryPool doesn't have enough free space */
void* MuskMemoryManager::askOSMemAllocate(const size_t alloc_size=MEGA){
    return (void*) malloc(alloc_size);
}

void* MuskMemoryManager::mm_allocate(const size_t alloc_size){
    
    /* return the MemoryChunk whose size is closest to (and not less than) alloc_size */
    auto it_ptrToAllocMemChunk = freeMemChunk_size_ptr_m.lower_bound(alloc_size);
    
    MemoryChunk* searchMemChunk = NULL;
    if(it_ptrToAllocMemChunk != freeMemChunk_size_ptr_m.end())
		searchMemChunk = (MemoryChunk*) it_ptrToAllocMemChunk->second;
    
    /* remove from the free list */
    if (searchMemChunk)
        freeMemChunk_size_ptr_m.erase(it_ptrToAllocMemChunk);
    
    /* if we have reached the end of current pool without */
    /* we need a new buffer allocation from the OS */
    if(!searchMemChunk){
        
        MemoryChunk* newMemoryChunk = NULL;
        
        if ((alloc_size + sizeof(MemoryChunk)) > MEGA){
            /* newMemoryChunk is at the start address of a new memory buffer */
            newMemoryChunk = (MemoryChunk*) askOSMemAllocate(alloc_size + sizeof(MemoryChunk));
            mm_totalPoolSize += alloc_size + sizeof(MemoryChunk);
            mm_freePoolSize += alloc_size;
        }
        else{
            /* newMemoryChunk is at the start address of a new memory buffer */
            newMemoryChunk = (MemoryChunk*) askOSMemAllocate(MEGA);
            mm_totalPoolSize += MEGA;
            mm_freePoolSize += (MEGA - sizeof(MemoryChunk));
        }
        
        /* Setting the following pointers to null is not really needed as they are set anyways a few lines later towards the end of this block */
        newMemoryChunk->mc_prev = newMemoryChunk->mc_next = NULL;
        
        _LIBCPP_ASSERT(mm_totalPoolSize < TEN_MEGABYTES, "Total memory pool size has exceeded 10 MB limit");
        
        memoryPoolsList.push_back((void*) newMemoryChunk);
        
        newMemoryChunk->mc_free = true;
        
        if ((alloc_size + sizeof(MemoryChunk)) > MEGA)
        	newMemoryChunk->mc_chunkSize = alloc_size;
        else
        	newMemoryChunk->mc_chunkSize = MEGA - sizeof(MemoryChunk);
        
        /* since this is a new buffer allocation it needs to be connected to the rest of the memory space being managed by the Memory Manager */
        /* newMemoryChunk will be appended at the tail in the list */
        mm_tailMemChunk->mc_next = newMemoryChunk;
        newMemoryChunk->mc_prev = mm_tailMemChunk;
        mm_tailMemChunk = newMemoryChunk;
        
        searchMemChunk = mm_tailMemChunk;
        
    }
    _LIBCPP_ASSERT(searchMemChunk != NULL , "searchMemChunk should have been made available by now");
    /* Now we should have a memory chunk to accommodate the user request */
    /* if the requested memory space wasn't available with the Memory Manager a new one (large enough to accomodate user data) has been requested from OS and added */
    
    /* create a new free memory chunk if there is space left after the memory space allocated to user */
    /* if this is not the case a few bytes towards the end of the new buffer could be wasted by user -- but there is no solution to that so we keep it as it is */
    if ( (alloc_size + sizeof(MemoryChunk)) < searchMemChunk->mc_chunkSize ){
        
        MemoryChunk* newMemoryChunk = (MemoryChunk*) ((BYTE*) searchMemChunk + sizeof(MemoryChunk) + alloc_size);
        newMemoryChunk->mc_free = true;
        newMemoryChunk->mc_chunkSize = searchMemChunk->mc_chunkSize - alloc_size - sizeof(MemoryChunk);
        searchMemChunk->mc_chunkSize = alloc_size;
        
        newMemoryChunk->mc_next = searchMemChunk->mc_next;
        searchMemChunk->mc_next = newMemoryChunk;
        newMemoryChunk->mc_prev = searchMemChunk;
        
        if(searchMemChunk == mm_tailMemChunk)
        	mm_tailMemChunk = newMemoryChunk;
        
        /* insert free memory chunk only when there is opportunity to do so*/
        freeMemChunk_size_ptr_m.insert(pair<size_t,MemoryChunk*>(newMemoryChunk->mc_chunkSize, newMemoryChunk));
        
    }
    
    searchMemChunk->mc_free = false;
    
    mm_userDataSize += alloc_size;
    mm_freePoolSize = mm_freePoolSize - alloc_size - sizeof(MemoryChunk);
    
    /* insert the allocated memory chunk as in use in the inUseMemChunk_size_ptr_m */
    inUseMemChunk_size_ptr_m.insert(pair<MemoryChunk*,MemoryChunk*>(searchMemChunk, searchMemChunk));
    
    /* return address after skipping the header */
    return (void*) ((BYTE*) searchMemChunk + sizeof(MemoryChunk));
}

void MuskMemoryManager::mm_free(const void* ptrToFree){
    
    /* on allocation users are given a pointer pointing to the address right after the header we need to move back by as many bytes to get to the header start */
    MemoryChunk* ptrMemoryChunk = (MemoryChunk*) ((BYTE*) ptrToFree - sizeof(MemoryChunk));
    
    auto it_ptrToFree = inUseMemChunk_size_ptr_m.find(ptrMemoryChunk);
    MemoryChunk* memChunkFreed = NULL;
    if (it_ptrToFree != inUseMemChunk_size_ptr_m.end())
    	memChunkFreed = (MemoryChunk*) it_ptrToFree->first;
    
    _LIBCPP_ASSERT(memChunkFreed != NULL, "Invalid pointer --  no memory was allocated at this address");
    
    memChunkFreed->mc_free = true;
    
    /* merge newly freed memory chunk with any adjacent free memory chunks */
    
    /* if next chunk is free and contiguous in memory then merge it with this one */
    if (memChunkFreed->mc_next && memChunkFreed->mc_next->mc_free && ( (MemoryChunk*) ((BYTE*) memChunkFreed + sizeof(MemoryChunk) + memChunkFreed->mc_chunkSize) == memChunkFreed->mc_next) ) {
        
        auto rm_pair = freeMemChunk_size_ptr_m.equal_range(memChunkFreed->mc_next->mc_chunkSize);
        for (auto it=rm_pair.first; it!=rm_pair.second; ++it)
            if ((MemoryChunk*) it->second == (MemoryChunk*) memChunkFreed->mc_next){
        		freeMemChunk_size_ptr_m.erase(it);
                break;
            }
        
        /* merge the space of next "free" chunk and also reclaim the header space for that chunk */
        memChunkFreed->mc_chunkSize += memChunkFreed->mc_next->mc_chunkSize + sizeof(MemoryChunk);
        
        /* check and update (if needed) the tail of the memory chunk list on merging free blocks */
        if(memChunkFreed->mc_next == mm_tailMemChunk)
            mm_tailMemChunk = memChunkFreed;
        
        /* update the memory chunk list */
        memChunkFreed->mc_next = memChunkFreed->mc_next->mc_next;
        if(memChunkFreed->mc_next)
        	memChunkFreed->mc_next->mc_prev = memChunkFreed;
        
    }
    
    /* if prev chunk is free and contiguous in memory then merge it with this one */
    if (memChunkFreed->mc_prev && memChunkFreed->mc_prev->mc_free && ( (MemoryChunk*) ((BYTE*) memChunkFreed->mc_prev + sizeof(MemoryChunk) + memChunkFreed->mc_prev->mc_chunkSize) == memChunkFreed)) {
        
        auto rm_pair = freeMemChunk_size_ptr_m.equal_range(memChunkFreed->mc_prev->mc_chunkSize);
        for (auto it=rm_pair.first; it!=rm_pair.second; ++it)
            if ((MemoryChunk*) it->second == (MemoryChunk*) memChunkFreed->mc_prev){
                freeMemChunk_size_ptr_m.erase(it);
                break;
            }
        
        /* merge the space of "freed" chunk with previous and also reclaim the header space for the freed chunk */
        memChunkFreed->mc_prev->mc_chunkSize += memChunkFreed->mc_chunkSize + sizeof(MemoryChunk);
        
        /* update the memory chunk list */
        memChunkFreed->mc_prev->mc_next = memChunkFreed->mc_next;
        if(memChunkFreed->mc_next)
        	memChunkFreed->mc_next->mc_prev = memChunkFreed->mc_prev;
        
        /* check and update (if needed) the tail of the memory chunk list on merging free blocks */
        if(memChunkFreed == mm_tailMemChunk)
        	mm_tailMemChunk = memChunkFreed->mc_prev;
        
        memChunkFreed = memChunkFreed->mc_prev;
    }
    
    freeMemChunk_size_ptr_m.insert(pair<size_t,MemoryChunk*>(memChunkFreed->mc_chunkSize,memChunkFreed));
    
    /* remove the iterator from inUseMemChunk_size_ptr_m map in the end since the iterator is lost */
    inUseMemChunk_size_ptr_m.erase(it_ptrToFree);
}

void MuskMemoryManager::displayInfo(){
    cout << "Memory Manager has been initialized at " << mm_headMemChunk << endl;
    cout << "Memory pool size " << (mm_totalPoolSize/MEGA) << "MB" << endl;
    
    int i = 0;
    
    MemoryChunk* searchMemChunk = mm_headMemChunk;
    while (searchMemChunk && i++ < 10) {
        cout << "MemChunk #" << i << " @ address " << searchMemChunk << endl;
        cout << "MemChunk->mc_chunkSize " << searchMemChunk->mc_chunkSize << endl;
        cout << "MemChunk->mc_free " << searchMemChunk->mc_free << endl;
        cout << "MemChunk->mc_prev " << searchMemChunk->mc_prev << endl;
        cout << "MemChunk->mc_next " << searchMemChunk->mc_next << endl << endl;
        searchMemChunk = searchMemChunk->mc_next;
    }
    
}

/* clean-up code for the Memory Manager goes in the destructor */
MuskMemoryManager::~MuskMemoryManager(){
    
    cout << "Freeing all memory pools..." << endl;
    
    for (auto it = memoryPoolsList.begin(); it != memoryPoolsList.end(); ++it){
            free(*it);
    }
    memoryPoolsList.clear();
    
}

