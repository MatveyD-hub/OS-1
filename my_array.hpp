#ifndef _my_array_HPP_
#define _my_array_HPP_ 1

#include <iostream>
#include <memory>

class my_array {
    typedef int32_t ElemType;
private:
    int MAXSIZE = 15;
    int size;
    ElemType* head;
    
public:
    my_array(int s) {
        if (s < MAXSIZE) {
            size = s;
            head = new ElemType(size);
        }
        else {
            std::cout << "Error: wrong size" << std::endl;
        }
    }
    int get_size() {
        return size;
    }
    ElemType* operator[](int i) {
        if (i > size || i < 0) {
            std::cout << "Error: wrong index" << std::endl;
            return NULL;
        }
        ElemType* curr = head;
        int k = 0;
        while (k != i) {
            k++;
            curr = curr + sizeof(ElemType);
        }
        return (curr);
    }
};
 
#endif;