#pragma once
#include "commons.h"
#include "memory.h"
#include<iostream>
class ValueArray{
public:
    int capacity;
    int count;
    Value *values;

    void initArr(){
        capacity = 0;
        count = 0;
        values = NULL;
    }
    void writeArr(Value v){
        if(capacity < count + 1){
            int oldcapacity = capacity;
            growCapacity();
            values = GROW_ARRAY(Value, values, oldcapacity, capacity);
        }
        values[count++] = v;
    }
    void freeArr(){
        FREE_ARRAY(Value, values, capacity);
        initArr();
    }
    void growCapacity(){  
        if(capacity > 0)capacity = capacity*2;
        else capacity = 1;
    }
    void printValueArr(int index){
        printValue(values[index]);
    }
    
};
