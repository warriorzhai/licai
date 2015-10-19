#include <Python.h>
#include <math.h>
#include "base.h"
#include "py_interface.h"
#include "fimath.h"





PyObject* roi_nominate(PyObject *self, PyObject *args){
    float r,r_annual;
    int time,time_type;
    if (!PyArg_ParseTuple( args, "fii", &r,&time,&time_type)) {
        return NULL;
    }
    //��һ���а��������ڣ��������ȡ��
    r_annual=roi_to_nominate(r,time,time_type);
    return PyFloat_FromDouble((double)r_annual);
}


PyObject* roi_effective(PyObject *self, PyObject *args){
    float r,r_annual;
    int time,time_type;
    if (!PyArg_ParseTuple( args, "fii", &r,&time,&time_type)) {
        return NULL;
    }
    //��һ���а��������ڣ��������ȡ��
    r_annual=roi_to_effective(r,time,time_type);
    return PyFloat_FromDouble((double)r_annual);
}


//����xirr
//����˵��
//args---����python list��һ�����ֽ�����һ����ÿ���ֽ�����Ӧ������
//��Ͷ��100Ԫ��2���º����80Ԫ���ٹ�3���º����50Ԫ
//�ֽ���=[-100,80,50],����=[5,3,0]

//����ֵ
//�����ʣ��ա��¡��꣩
PyObject* roi_xirr(PyObject *self, PyObject *args){
    PyObject *py_c,*py_t;
    int time_type,time_gap;
    float* cashes;
    float* times;
    float x0=0,rate;

    if (!PyArg_ParseTuple( args, "O!O!ii", &PyList_Type,&py_c,&PyList_Type,&py_t,&time_gap,&time_type)) {
        return NULL;
    }

    int l=PyList_Size(py_c);

    times=pylist_to_fltarray(py_t);
    cashes=pylist_to_fltarray(py_c);
    rate=root_secant(cashes,times,x0+l,l)-1;

    if(time_type<2)
        rate=roi_to_effective(rate,time_gap,time_type);

    return PyFloat_FromDouble((double)rate);
}



static PyMethodDef PyfimathMethods[] = {
    //mapping��ʽΪ "python������"��c��������"����˵��"
    {"xirr",roi_xirr,METH_VARARGS, "testing"},
    {"nomirate",roi_nominate,METH_VARARGS, "testing"},
    {"effrate",roi_effective,METH_VARARGS, "testing"},
    //������һ������mapping��ĩβ����ӵ�һ��
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpyfimath(void)
{
    //�������Ƹ�ʽ��init+modulename
    //m��һ��ָ��PyObject��ָ��
    PyObject *m;
    //�����Զ���python module���󶨷�����module������ָ��
    m = Py_InitModule("pyfimath", PyfimathMethods);
    if (m == NULL) {
        return;
    }
}


