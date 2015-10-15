#include <Python.h>
#include <math.h>

#define ROUND_DIGIT 4
#define true 1
#define false 0




//定义单位年包含时间：
//天，一年365天
//月，一年12月
static int time_table[]={365,12};
typedef int bool;
//==================基本函数=====================

//四舍五入
//四舍五入成整数，round；
//四舍五入任意小数位，roundto。

//例子：
//roundto(2.67,1)--》2.7
//roundto(2.67,0)--》3
//roundto(26.7,-1)--》30

//参数说明
//x：浮点数
//n：保留小数后几位，整数，-inf~+inf

//返回：
//四舍五入后的结果，浮点
static float roundto(float x,int n){
    float mul=pow(10,n);
    return round(x*mul)/mul;
}


//取绝对值
//整数取绝对值--abs
//浮点数取绝对值--absto

//例子：
//absto(-2.34)--》2.34

//参数说明
//x：浮点数

//返回：
//绝对值的结果，浮点
static float absto(float x){
    return x>0?x:(-1)*x;
}


//=====================================================

//==================python接口函数=====================

//将只包含INT类的list type pyobject转为c的数组

//例子：
//listobj=/....pylist，[1,2,3].../
//pylist_to_intarray(listobj)--》array[]={1,2,3}

//参数说明
//listobj:指向于pylist类型pyobject的指针

//返回：
//int数组的头指针
static int* pylist_to_intarray(PyObject *listobj){
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
static float* pylist_to_fltarray(PyObject *listobj){
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

//==================数学计算===========================

//构造一个一元n次函数，计算函数结果

//例子：
//函数=2*x+3*x^4+90
//系数，k=[2,3,90]
//幂，n=[1,4,0]
//l=length(n)=3
//求x=1的函数值
//func_polyno(k,n,1,3)-->95

//参数说明
//k:浮点数组，函数的系数
//n:浮点数组，函数的幂
//x：变量的值
//l：整数，数组的长度，之所以需要这个参数是因为c数组不方便获取长度，只好记录下来

//返回：
//函数计算结果，浮点
static float func_polyno(float* k,float* n,float x,int l){

    float res=0;
    //循环临时计数器,初始值为-1而不是0是和下文中while的写法有关
    int i=-1;

    //计算一元n次函数
    while(++i<l){
        res+=k[i]*pow(x,n[i]);
    }

    //将计算结果，按照位数四舍五入
    return roundto(res,ROUND_DIGIT);
}


//给出两点(x0,y0),(x1，y1)返回两点间直线函数（kx+b）的k、b
static float* func_line(float x0,float x1,float y0,float y1){
    static float line_para[2]={0};
    if(x1!=x0)
        line_para[0]=(y1-y0)/(x1-x0);
    line_para[1]=y0-line_para[0]*x0;
    return line_para;
}

//=====================================================

//==================投资计算===========================


//根据期利率，计算nominated rate
//公式：r*n

//例子：
//每月利率为r，求nominated利率
//roi_to_nominate(r,1,1)
//每天利率为r，求nominated利率
//roi_to_nominate(r,1,0)
//每期90天的票据期利率为r，求nominated利率（这种情况可以使用但是不常用）
//roi_to_nominate(r,90,0)

//参数说明
//r:每期利率，期为几天几个月
//time：正整数，每期中包含几天/月
//time_type：正整数，0-天，1-月，这里不包括年因为nominated rate就是年利率了

//返回：
//利率，浮点
static float roi_to_nominate(float r,int time,int time_type){
    //求一年中包含多少期，结果向下取整
    int n=floor(time_table[time_type]/time);
    return n*r;
}

//根据期利率，计算effective rate
//公式：(1+r)^n

//例子：
//每月利率为r，求nominated利率
//roi_to_effective(r,1,1)
//每天利率为r，求nominated利率
//roi_to_effective(r,1,0)
//每期90天的票据期利率为r，求nominated利率（这种情况可以使用但是不常用）
//roi_to_effective(r,90,0)

//参数说明
//r:每期利率，期为几天几个月
//time：正整数，每期中包含几天/月
//time_type：正整数，0-天，1-月，这里不包括年因为nominated rate就是年利率了

//返回：
//利率，浮点
static float roi_to_effective(float r,int time,int time_type){
    int n=floor(time_table[time_type]/time);
    return pow((1+r),n)-1;
}

static bool root_errcheck(x0,x1,y){
    //求根算法逼近时候的阈值，当代入根后，函数值小于此值则算法结束
    //此收敛阈值的确定方法待探讨
    float err=0.5;
    printf("check:%d\n",(absto(y>=err)&&roundto(absto(x0-x1),4)>0));
    return (absto(y>=err)&&roundto(absto(x0-x1),4)>0);
}


static float root_bisec_nxtroot(float x0,float x1){
    return (x0+x1)/2;
}

static float root_liinter_nxtroot(float x0,float x1,float y0,float y1){

    float* line_para=func_line(x0,x1,y0,y1);
    if(line_para[1]==0){
        if(line_para[0]==0)
            return 0;
        else{
            printf("k为0、直线不过x轴，无解");
            return '0';}
        }

    else{
        printf("k=%f\n",line_para[0]);
        printf("b=%f\n",line_para[1]);
        return -1*(line_para[1]/line_para[0]);
    }
}

//使用Bisection method求解一元n次方程的一个根
//算法概述：
//给定一元n次方程，方程连续；
//给定初始两点x0,x1，其中f(x0),f(x1)异号；
//取x0,x1的中点x2，求解f(x2)，f(x2)是否足够接近于0；
//如果是，算法结束，x2为所求根，若不是，则重复求中点。

//例子：
//求x^5+4*x+9=0的根
//k={1,4,9},n={5,1,0},l=3
//已知x范围在0~1之间，x0=0,x1=1
//root_bisec(k,n,x0,x1,l)

//参数说明
//k:浮点数组，函数的系数
//n:浮点数组，函数的幂
//x0,x1：根的初始值，浮点
//l：整数，数组的长度，之所以需要这个参数是因为c数组不方便获取长度，只好记录下来

//返回：
//根，浮点
static float root_bisec(float* k,float* n,float x0,float x1,int l){
    //计算初始两个点的函数值
    float y0=func_polyno(k,n,x0,l);
    float y1=func_polyno(k,n,x1,l);
    float y,x;


    //是否满足条件：初始两个点函数值异号
    if(y0*y1>0){
        return '0';
    }

    if(root_errcheck(x0,x1,y0))
        return x0;
    if(root_errcheck(x0,x1,y1))
        return x1;

    //当中点处(x0+x1)/2满足条件cl，则：
    //中点与和他异号的断点构成新的x0，x1端点；
    //求下一个中点。
    //条件cl：
    //1.中点处函数值大于等于阈值；
    //2.端点之间距离较远，有变化；
    do{
        //x=root_bisec_nxtroot(x0,x1);
        x=root_liinter_nxtroot(k,n,x0,x1,l);
        printf("new root:%f\n",x);
        y=func_polyno(k,n,x,l);
        printf("new err:%f\n",y);
        if(y*y0>0){
            x0=x;
            y0=y;
        }
        else{
            x1=x;
            y1=y;
        }
        }while(root_errcheck(x0,x1,y));
    printf("fin err:%f\n",y);
    return x;
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
    float x0=-1,x1=1,rate;

    if (!PyArg_ParseTuple( args, "O!O!ii", &PyList_Type,&py_c,&PyList_Type,&py_t,&time_type,&time_gap)) {
        return NULL;
    }

    int l=PyList_Size(py_c);

    times=pylist_to_fltarray(py_t);
    cashes=pylist_to_fltarray(py_c);
    rate=root_bisec(cashes,times,x0+1,x1+1,l)-1;
    if(time_type<2)
        rate=roi_to_effective(rate,time_gap,time_type);

    return PyFloat_FromDouble((double)rate);
}

int main(){
    float k[]={1,-4,4};
    float n[]={2,1,0};
    float root=root_bisec(k,n,1.1,2.1,3);
    printf("%f\n",root);

}





static PyMethodDef FimathMethods[] = {
    //mapping格式为 "python函数名"，c函数名，"函数说明"
    {"xirr",roi_xirr,METH_VARARGS, "testing"},
    //下面这一行是在mapping的末尾必须加的一行
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initfimath(void)
{
    //函数名称格式：init+modulename
    //m是一个指向PyObject的指针
    PyObject *m;
    //创建自定义python module，绑定方法给module，赋给指针
    m = Py_InitModule("fimath", FimathMethods);
    if (m == NULL) {
        return;
    }
}


