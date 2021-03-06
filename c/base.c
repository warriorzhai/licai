#include "base.h"

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
float roundto(float x,int n){
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
float absto(float x){
    return x>0?x:(-1)*x;
}


//=====================================================
