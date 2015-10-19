
#include "fimath.h"
#define ROUND_DIGIT 4


//���嵥λ�����ʱ�䣺
//�죬һ��365��
//�£�һ��12��
int time_table[]={365,12};
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
float func_polyno(float* k,float* n,float x,int l){

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
float* func_line(float x0,float x1,float y0,float y1){
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
float roi_to_nominate(float r,int time,int time_type){
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
float roi_to_effective(float r,int time,int time_type){
    if(time<=0){
        printf("��λͶ��ʱ�䲻��С�ڵ���0\n");
        return '0';
    }
    int n=floor(time_table[time_type]/time);
    printf("type��%d\n",time_table[time_type]);
    printf("time��%d",time);
    printf("��Ͷ��������%d",n);
    return pow((1+r),n)-1;
}

static bool root_errcheck(float y){
    //����㷨�ƽ�ʱ�����ֵ����������󣬺���ֵС�ڴ�ֵ���㷨����
    //��������ֵ��ȷ��������̽��
    float err=0.001;
    return absto(y)>=err;
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

//������x�����еĺ�����������

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
float root_secant(float* k,float* n,float x0,int l){
    float sigma=0.0001;
    float x1=x0+sigma;
    //�����ʼ������ĺ���ֵ
    float y0=func_polyno(k,n,x0,l);
    printf("init y0:%f\n",y0);
    float y1=func_polyno(k,n,x1,l);
    printf("init y1:%f\n",y1);
    float y,x;

    if(!root_errcheck(y0))
        return x0;
    if(!root_errcheck(y1))
        return x1;

    //���е㴦(x0+x1)/2��������cl����
    //�е��������ŵĶϵ㹹���µ�x0��x1�˵㣻
    //����һ���е㡣
    //����cl��
    //1.�е㴦����ֵ���ڵ�����ֵ��
    //2.�˵�֮������Զ���б仯��
    do{
        //x=root_bisec_nxtroot(x0,x1);
        x=x1-y1*(x1-x0)/(y1-y0+sigma);
        printf("new root:%f\n",x);
        y=func_polyno(k,n,x,l);
        printf("new err:%f\n",y);

        x0=x1;
        x1=x;
        y0=y1;
        y1=y;


        }while(root_errcheck(y)&&(x0!=x1));
    printf("fin err:%f\n",y);
    return x;
}


float root_bisec(float* k,float* n,float x0,float x1,int l){
    //�����ʼ������ĺ���ֵ
    float y0=func_polyno(k,n,x0,l);
    float y1=func_polyno(k,n,x1,l);
    float y,x;


    //�Ƿ�������������ʼ�����㺯��ֵ���
    if(y0*y1>0){
        return '0';
    }

    if(!root_errcheck(y0))
        return x0;
    if(!root_errcheck(y1))
        return x1;

    //���е㴦(x0+x1)/2��������cl����
    //�е��������ŵĶϵ㹹���µ�x0��x1�˵㣻
    //����һ���е㡣
    //����cl��
    //1.�е㴦����ֵ���ڵ�����ֵ��
    //2.�˵�֮������Զ���б仯��
    do{
        //x=root_bisec_nxtroot(x0,x1);
        x=root_liinter_nxtroot(x0,x1,y0,y1);
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
        }while(root_errcheck(y));
    printf("fin err:%f\n",y);
    return x;
}
