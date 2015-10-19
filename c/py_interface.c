#include "py_interface.h"


//==================python�ӿں���=====================

//��ֻ����INT���list type pyobjectתΪc������

//���ӣ�
//listobj=/....pylist��[1,2,3].../
//pylist_to_intarray(listobj)--��array[]={1,2,3}

//����˵��
//listobj:ָ����pylist����pyobject��ָ��

//���أ�
//int�����ͷָ��
int* pylist_to_intarray(PyObject *listobj){
    //����ͷָ��
    int* arr;
    //ѭ����ʱ������,��ʼֵΪ-1������0�Ǻ�������while��д���й�
    int i=-1;

    //������������pylist
    if(!PyList_Check(listobj)){
        return NULL;
    }

    //���鳤�ȣ�ѭ��������
    int length=PyList_Size(listobj);

    //��̬��������ռ�
    arr = (int *) malloc(sizeof(int)*length);

    //ѭ��pylist����ÿ��Ԫ�ؿ���������
    while(++i<length){
        arr[i]=(int)PyInt_AsLong(PyList_GetItem(listobj,i));
    }
    return arr;
}

//��ֻ����FLOAT���list type pyobjectתΪc������

//���ӣ�
//listobj=/....pylist��[1.1,2.0,3.89].../
//pylist_to_fltarray(listobj)--��array[]={1.1,2.0,3.89}

//����˵��
//listobj:ָ����pylist����pyobject��ָ��

//���أ�
//float�����ͷָ��
float* pylist_to_fltarray(PyObject *listobj){
    float* arr;
    int i=-1;

    //������������pylist
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
