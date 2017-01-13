# MuskMemoryManager
a user-space memory manager with some cool features to be added in (near) future

General Structure of the heap maintained by the Memory-Manager

![mm_structure](https://cloud.githubusercontent.com/assets/213910/21885706/24be237a-d8b9-11e6-8ab4-b95a372e6f70.png)

Memory Space Reuqest -- applications using the Memory Manager requests a memory space using mm_allocate. 

A) If the Memory Manager can has enough free space to allocate in the existing Memory pools, the following happens 

![memory_alloca](https://cloud.githubusercontent.com/assets/213910/21926534/7095732e-d982-11e6-8a5a-666b78d74546.png)

B) If the Memory Manager does not have enough free space in the existing Memory pools a new Memory pool is requested from OS by the Memory Manager and the user request is completed

![memory_allocb](https://cloud.githubusercontent.com/assets/213910/21926628/e5c921fe-d982-11e6-846d-a7fb71f100da.png)

Memory Space Free -- applications using the Memory Manager requests a memory space using mm_free. If there are adjacen free Memory Chunks they are merged as follows

![memory_free](https://cloud.githubusercontent.com/assets/213910/21926732/54a58bf8-d983-11e6-85d7-a1c0c838bfaa.png)

# MuskMemoryManagerEX
MuskMemoryManagerEX is an optimized version of the simpler MuskMemoryManager. MuskMemoryManagerEX uses binary search trees (C++ STL Map and MultiMap) for faster search of free Memory-Chunks by size and in-use Memory-Chunks by address.
