#include "MuskMemoryManager.hpp"

int main(){
    
    MuskMemoryManager& mm = MuskMemoryManager::getInstance();
    
    mm.displayInfo();
    
    cout << "----- Test #1 -----" << endl;
    
    void* ptr1 = (void*) mm.mm_allocate(5 * sizeof(int));
    void* ptr2 = (void*) mm.mm_allocate(5 * sizeof(int));
    
    cout << "ptr1 " << ptr1 << endl;
    cout << "ptr2 " << ptr2 << endl;
    
    mm.displayInfo();
    
    mm.mm_free(ptr1);
    
    mm.displayInfo();
    
    mm.mm_free(ptr2);
    
    mm.displayInfo();
    
    cout << "----- Test #2 -----" << endl;
    
    ptr1 = (void*) mm.mm_allocate(10 * sizeof(int));
    ptr2 = (void*) mm.mm_allocate(10 * sizeof(int));
	
    cout << "ptr1 " << ptr1 << endl;
    cout << "ptr2 " << ptr2 << endl;
    
    mm.displayInfo();
    
    cout << "----- Test #3 -----" << endl;
    
    mm.mm_free(ptr1);
    
    mm.displayInfo();
    
    cout << "----- Test #4 -----" << endl;
    
    ptr1 = (void*) mm.mm_allocate(MEGA + 5);
    
    mm.displayInfo();
    
    cout << "----- Test #5 -----" << endl;
    
    mm.mm_free(ptr2);
    
    mm.displayInfo();
    
    cout << "----- Test #6 -----" << endl;
    
    mm.mm_free(ptr1);
    
    mm.displayInfo();
    
    return 0;
}
