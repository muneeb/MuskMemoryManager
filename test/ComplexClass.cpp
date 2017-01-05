#include <iostream>
#include <vector>
#include <assert.h>
#include "MuskMemoryManager.hpp"
using namespace std;

MuskMemoryManager* mm;

template <typename T>
class Point{
  
    private:
    
    T x, y;
    
    public:
    
    Point(T xi, T yi):x(xi),y(yi){}
    Point(const Point& p):x(p.x),y(p.y){}
    
    T getXCoordinate(){
    	return x;
    }
    
    T getYCoordinate(){
        return y;
    }
    
};

class Complex{
    
    private:
    
    vector<Point<float>> point_v;
    
    public:
    
    void insertPointInGraph(Point<float>& p){
        point_v.push_back(Point<float>(p.getXCoordinate(),p.getYCoordinate()));
        point_v.push_back(Point<float>(p.getXCoordinate()+1,p.getYCoordinate()));
        point_v.push_back(Point<float>(p.getXCoordinate()+2,p.getYCoordinate()));
        point_v.push_back(Point<float>(p.getXCoordinate()+3,p.getYCoordinate()));
        point_v.push_back(Point<float>(p.getXCoordinate()+4,p.getYCoordinate()));
    }
    
    static void* operator new (size_t size){
        assert(size == sizeof(Complex));
        
        return (void*) mm->mm_allocate(size);
        
    }
    
    static void operator delete (void* ptr) noexcept {
        mm->mm_free(ptr);
    }
    
    static void* operator new[](size_t size){
        
        return (void*) mm->mm_allocate(size);
        
    }
    
    static void operator delete[](void* ptr, size_t size) noexcept {
        mm->mm_free(ptr);
    }
    
    void printVector(){
        
        cout << "Point " << endl;
        for (auto it = point_v.begin(); it != point_v.end(); ++it) {
            cout << "X: " << it->getXCoordinate() << " Y: " << it->getYCoordinate() << endl;
        }
        cout << endl;
    }
    
};

int main(){
    
    mm = &MuskMemoryManager::getInstance();
    
    Complex* c_ptr1 = (Complex*) mm->mm_allocate(sizeof(Complex));
    
    Point<float> p1(1.3,2.2);
	
    Complex* c_ptr2 = new Complex();
    
    Point<float> p2(3.7,4.4);
    
    c_ptr1->insertPointInGraph(p1);
    c_ptr2->insertPointInGraph(p2);
    
    mm->displayInfo();
    
    c_ptr1->printVector();
    c_ptr2->printVector();
    
    mm->mm_free(c_ptr1);
    delete c_ptr2;
    
    Complex* c_ptrArr = new Complex[10];
    
    mm->displayInfo();
    
    delete[] c_ptrArr;
    
    return 0;
    
}
