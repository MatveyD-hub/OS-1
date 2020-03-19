

#include <iostream>
#include <memory>
#include "/Users/amelia/Documents/arr.h"


    my_array::my_array(int s) {
        if (s < MAXSIZE) {
            size = s;
            head = new ElemType(size);
        }
        else {
            std::cout << "Error: wrong size" << std::endl;
        }
    }
    int my_array::get_size() {
        return size;
    }
    ElemType* my_array::operator[](int i) {
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

