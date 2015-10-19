#include "py_interface.h"


//==================python接口函数=====================

//将只包含INT类的list type pyobject转为c的数组

//例子：
//listobj=/....pylist，[1,2,3].../
//pylist_to_intarray(listobj)--》array[]={1,2,3}

//参数说明
//listobj:指向于pylist类型pyobject的指针

//返回：
//int数组的头指针
int* pylist_to_intarray(PyObject *listobj){
    //数组头指针
    int* arr;
    //循环临时计数器,初始值为-1而不是0是和下文中while的写法有关
    int i=-1;

    //检查输入必须是pylist
    if(!PyList_Check(listobj)){
        return NULL;
    }

    //数组长度，循环计数用
    int length=PyList_Size(listobj);

    //动态分配数组空间
    arr = (int *) malloc(sizeof(int)*length);

    //循环pylist，将每个元素拷贝到数组
    while(++i<length){
        arr[i]=(int)PyInt_AsLong(PyList_GetItem(listobj,i));
    }
    return arr;
}

//将只包含FLOAT类的list type pyobject转为c的数组

//例子：
//listobj=/....pylist，[1.1,2.0,3.89].../
//pylist_to_fltarray(listobj)--》array[]={1.1,2.0,3.89}

//参数说明
//listobj:指向于pylist类型pyobject的指针

//返回：
//float数组的头指针
float* pylist_to_fltarray(PyObject *listobj){
    float* arr;
    int i=-1;

    //检查输入必须是pylist
    if(!PyList_Check(listobj)){
        return NULL;
    }

    int length=PyList_Size(listobj);
    arr = (float *) malloc(sizeof(float)*length);
    while(++i<length){
        arr[i]=(float)PyFloat_AsDouble(PyList_GetItem(listobj,i));
    }
    return arr;
}

//=====================================================
