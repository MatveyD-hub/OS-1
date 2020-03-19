#ifndef _arr_H_
#define _arr_H_

#include <iostream>
#include <memory>

typedef int32_t ElemType;

class my_array {
private:
    int MAXSIZE = 15;
    int size;
    ElemType* head;
    
public:
    my_array(int s);
    int get_size();
    ElemType* operator[](int i);
};
 
#endif
