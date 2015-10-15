#include <Python.h>
#include <math.h>

#define ROUND_DIGIT 4
#define true 1
#define false 0




//���嵥λ�����ʱ�䣺
//�죬һ��365��
//�£�һ��12��
static int time_table[]={365,12};
typedef int bool;
//==================��������=====================

//��������
//���������������round��
//������������С��λ��roundto��

//���ӣ�
//roundto(2.67,1)--��2.7
//roundto(2.67,0)--��3
//roundto(26.7,-1)--��30

//����˵��
//x��������
//n������С����λ��������-inf~+inf

//���أ�
//���������Ľ��������
static float roundto(float x,int n){
    float mul=pow(10,n);
    return round(x*mul)/mul;
}


//ȡ����ֵ
//����ȡ����ֵ--abs
//������ȡ����ֵ--absto

//���ӣ�
//absto(-2.34)--��2.34

//����˵��
//x��������

//���أ�
//����ֵ�Ľ��������
static float absto(float x){
    return x>0?x:(-1)*x;
}


//=====================================================

//==================python�ӿں���=====================

//��ֻ����INT���list type pyobjectתΪc������

//���ӣ�
//listobj=/....pylist��[1,2,3].../
//pylist_to_intarray(listobj)--��array[]={1,2,3}

//����˵��
//listobj:ָ����pylist����pyobject��ָ��

//���أ�
//int�����ͷָ��
static int* pylist_to_intarray(PyObject *listobj){
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
static float* pylist_to_fltarray(PyObject *listobj){
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

//==================��ѧ����===========================

//����һ��һԪn�κ��������㺯�����

//���ӣ�
//����=2*x+3*x^4+90
//ϵ����k=[2,3,90]
//�ݣ�n=[1,4,0]
//l=length(n)=3
//��x=1�ĺ���ֵ
//func_polyno(k,n,1,3)-->95

//����˵��
//k:�������飬������ϵ��
//n:�������飬��������
//x��������ֵ
//l������������ĳ��ȣ�֮������Ҫ�����������Ϊc���鲻�����ȡ���ȣ�ֻ�ü�¼����

//���أ�
//����������������
static float func_polyno(float* k,float* n,float x,int l){

    float res=0;
    //ѭ����ʱ������,��ʼֵΪ-1������0�Ǻ�������while��д���й�
    int i=-1;

    //����һԪn�κ���
    while(++i<l){
        res+=k[i]*pow(x,n[i]);
    }

    //��������������λ����������
    return roundto(res,ROUND_DIGIT);
}


//��������(x0,y0),(x1��y1)���������ֱ�ߺ�����kx+b����k��b
static float* func_line(float x0,float x1,float y0,float y1){
    static float line_para[2]={0};
    if(x1!=x0)
        line_para[0]=(y1-y0)/(x1-x0);
    line_para[1]=y0-line_para[0]*x0;
    return line_para;
}

//=====================================================

//==================Ͷ�ʼ���===========================


//���������ʣ�����nominated rate
//��ʽ��r*n

//���ӣ�
//ÿ������Ϊr����nominated����
//roi_to_nominate(r,1,1)
//ÿ������Ϊr����nominated����
//roi_to_nominate(r,1,0)
//ÿ��90���Ʊ��������Ϊr����nominated���ʣ������������ʹ�õ��ǲ����ã�
//roi_to_nominate(r,90,0)

//����˵��
//r:ÿ�����ʣ���Ϊ���켸����
//time����������ÿ���а�������/��
//time_type����������0-�죬1-�£����ﲻ��������Ϊnominated rate������������

//���أ�
//���ʣ�����
static float roi_to_nominate(float r,int time,int time_type){
    //��һ���а��������ڣ��������ȡ��
    int n=floor(time_table[time_type]/time);
    return n*r;
}

//���������ʣ�����effective rate
//��ʽ��(1+r)^n

//���ӣ�
//ÿ������Ϊr����nominated����
//roi_to_effective(r,1,1)
//ÿ������Ϊr����nominated����
//roi_to_effective(r,1,0)
//ÿ��90���Ʊ��������Ϊr����nominated���ʣ������������ʹ�õ��ǲ����ã�
//roi_to_effective(r,90,0)

//����˵��
//r:ÿ�����ʣ���Ϊ���켸����
//time����������ÿ���а�������/��
//time_type����������0-�죬1-�£����ﲻ��������Ϊnominated rate������������

//���أ�
//���ʣ�����
static float roi_to_effective(float r,int time,int time_type){
    int n=floor(time_table[time_type]/time);
    return pow((1+r),n)-1;
}

static bool root_errcheck(x0,x1,y){
    //����㷨�ƽ�ʱ�����ֵ����������󣬺���ֵС�ڴ�ֵ���㷨����
    //��������ֵ��ȷ��������̽��
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
            printf("kΪ0��ֱ�߲���x�ᣬ�޽�");
            return '0';}
        }

    else{
        printf("k=%f\n",line_para[0]);
        printf("b=%f\n",line_para[1]);
        return -1*(line_para[1]/line_para[0]);
    }
}

//ʹ��Bisection method���һԪn�η��̵�һ����
//�㷨������
//����һԪn�η��̣�����������
//������ʼ����x0,x1������f(x0),f(x1)��ţ�
//ȡx0,x1���е�x2�����f(x2)��f(x2)�Ƿ��㹻�ӽ���0��
//����ǣ��㷨������x2Ϊ������������ǣ����ظ����е㡣

//���ӣ�
//��x^5+4*x+9=0�ĸ�
//k={1,4,9},n={5,1,0},l=3
//��֪x��Χ��0~1֮�䣬x0=0,x1=1
//root_bisec(k,n,x0,x1,l)

//����˵��
//k:�������飬������ϵ��
//n:�������飬��������
//x0,x1�����ĳ�ʼֵ������
//l������������ĳ��ȣ�֮������Ҫ�����������Ϊc���鲻�����ȡ���ȣ�ֻ�ü�¼����

//���أ�
//��������
static float root_bisec(float* k,float* n,float x0,float x1,int l){
    //�����ʼ������ĺ���ֵ
    float y0=func_polyno(k,n,x0,l);
    float y1=func_polyno(k,n,x1,l);
    float y,x;


    //�Ƿ�������������ʼ�����㺯��ֵ���
    if(y0*y1>0){
        return '0';
    }

    if(root_errcheck(x0,x1,y0))
        return x0;
    if(root_errcheck(x0,x1,y1))
        return x1;

    //���е㴦(x0+x1)/2��������cl����
    //�е��������ŵĶϵ㹹���µ�x0��x1�˵㣻
    //����һ���е㡣
    //����cl��
    //1.�е㴦����ֵ���ڵ�����ֵ��
    //2.�˵�֮������Զ���б仯��
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
    //mapping��ʽΪ "python������"��c��������"����˵��"
    {"xirr",roi_xirr,METH_VARARGS, "testing"},
    //������һ������mapping��ĩβ����ӵ�һ��
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initfimath(void)
{
    //�������Ƹ�ʽ��init+modulename
    //m��һ��ָ��PyObject��ָ��
    PyObject *m;
    //�����Զ���python module���󶨷�����module������ָ��
    m = Py_InitModule("fimath", FimathMethods);
    if (m == NULL) {
        return;
    }
}


