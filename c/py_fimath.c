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
    //求一年中包含多少期，结果向下取整
    r_annual=roi_to_nominate(r,time,time_type);
    return PyFloat_FromDouble((double)r_annual);
}


PyObject* roi_effective(PyObject *self, PyObject *args){
    float r,r_annual;
    int time,time_type;
    if (!PyArg_ParseTuple( args, "fii", &r,&time,&time_type)) {
        return NULL;
    }
    //求一年中包含多少期，结果向下取整
    r_annual=roi_to_effective(r,time,time_type);
    return PyFloat_FromDouble((double)r_annual);
}


//计算xirr
//参数说明
//args---两个python list，一个是现金流，一个是每个现金流对应的期数
//如投资100元，2个月后回收80元，再过3个月后回收50元
//现金流=[-100,80,50],期数=[5,3,0]

//返回值
//期利率（日、月、年）
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
    //mapping格式为 "python函数名"，c函数名，"函数说明"
    {"xirr",roi_xirr,METH_VARARGS, "testing"},
    {"nomirate",roi_nominate,METH_VARARGS, "testing"},
    {"effrate",roi_effective,METH_VARARGS, "testing"},
    //下面这一行是在mapping的末尾必须加的一行
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initpyfimath(void)
{
    //函数名称格式：init+modulename
    //m是一个指向PyObject的指针
    PyObject *m;
    //创建自定义python module，绑定方法给module，赋给指针
    m = Py_InitModule("pyfimath", PyfimathMethods);
    if (m == NULL) {
        return;
    }
}


