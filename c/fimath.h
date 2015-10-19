#ifndef FIMATH_H
#define FIMATH_H


#include <math.h>
#include <stdio.h>
#include "base.h"

//����һ��һԪn�κ��������㺯�����
float func_polyno(float* k,float* n,float x,int l);
//��������(x0,y0),(x1��y1)���������ֱ�ߺ�����kx+b����k��b
float* func_line(float x0,float x1,float y0,float y1);
//���������ʣ�����nominated rate
float roi_to_nominate(float r,int time,int time_type);
//���������ʣ�����effective rate
float roi_to_effective(float r,int time,int time_type);

float root_secant(float* k,float* n,float x0,int l);
float root_bisec(float* k,float* n,float x0,float x1,int l);
#endif
