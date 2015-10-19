#ifndef FIMATH_H
#define FIMATH_H


#include <math.h>
#include <stdio.h>
#include "base.h"

//构造一个一元n次函数，计算函数结果
float func_polyno(float* k,float* n,float x,int l);
//给出两点(x0,y0),(x1，y1)返回两点间直线函数（kx+b）的k、b
float* func_line(float x0,float x1,float y0,float y1);
//根据期利率，计算nominated rate
float roi_to_nominate(float r,int time,int time_type);
//根据期利率，计算effective rate
float roi_to_effective(float r,int time,int time_type);

float root_secant(float* k,float* n,float x0,int l);
float root_bisec(float* k,float* n,float x0,float x1,int l);
#endif
