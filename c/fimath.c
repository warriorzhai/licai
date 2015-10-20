
#include "fimath.h"
#define ROUND_DIGIT 4


//定义单位年包含时间：
//天，一年365天
//月，一年12月
int time_table[]={365,12};
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
float func_polyno(float* k,float* n,float x,int l){

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
float* func_line(float x0,float x1,float y0,float y1){
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
float roi_to_nominate(float r,int time,int time_type){
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
float roi_to_effective(float r,int time,int time_type){
    if(time<=0){
        printf("单位投资时间不能小于等于0\n");
        return '0';
    }
    int n=floor(time_table[time_type]/time);
    printf("type：%d\n",time_table[time_type]);
    printf("time：%d",time);
    printf("年投资期数：%d",n);
    return pow((1+r),n)-1;
}

static bool root_errcheck(float y){
    //求根算法逼近时候的阈值，当代入根后，函数值小于此值则算法结束
    //此收敛阈值的确定方法待探讨
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
            printf("k为0、直线不过x轴，无解");
            return '0';}
        }

    else{
        printf("k=%f\n",line_para[0]);
        printf("b=%f\n",line_para[1]);
        return -1*(line_para[1]/line_para[0]);
    }
}

//使用Secant method求解一元n次方程的一个根

float root_secant(float* k,float* n,float x0,int l){
    float sigma=0.0001;
    float x1=x0+sigma;
    //计算初始两个点的函数值
    float y0=func_polyno(k,n,x0,l);
    printf("init y0:%f\n",y0);
    float y1=func_polyno(k,n,x1,l);
    printf("init y1:%f\n",y1);
    float y,x;

    if(!root_errcheck(y0))
        return x0;
    if(!root_errcheck(y1))
        return x1;


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

//使用Bisection method求解一元n次方程的一个根
//算法概述：
//给定一元n次方程，方程连续；
//给定初始两点x0,x1，其中f(x0),f(x1)异号；
//取x0,x1的中点x2，求解f(x2)，f(x2)是否足够接近于0；
//如果是，算法结束，x2为所求根，若不是，则重复求中点。

//计算与x轴相切的函数会有问题

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
float root_bisec(float* k,float* n,float x0,float x1,int l){
    //计算初始两个点的函数值
    float y0=func_polyno(k,n,x0,l);
    float y1=func_polyno(k,n,x1,l);
    float y,x;


    //是否满足条件：初始两个点函数值异号
    if(y0*y1>0){
        return '0';
    }

    if(!root_errcheck(y0))
        return x0;
    if(!root_errcheck(y1))
        return x1;

    //当中点处(x0+x1)/2满足条件cl，则：
    //中点与和他异号的断点构成新的x0，x1端点；
    //求下一个中点。
    //条件cl：
    //1.中点处函数值大于等于阈值；
    //2.（已删除）端点之间距离较远，有变化；
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
