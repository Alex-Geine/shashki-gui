#pragma once

#include <time.h>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;


class Neiron {
   
private:
    int layers;
    int* neironsCount;
    int* xSize;
    double** koef;
    double*** w;
    double** x;

    double B = 1;

    //������� ���������
    double Function(double S);

    //������� ����
    void LayerFunc(int layer);  
public:
    double score = 0;
    bool isWin = false;
    //�����������
    Neiron();
    
    //����������
    ~Neiron();  

    //����� ����������� ������ ����
    double Work(double *in);

    //�������� ���������� � �������
    void PrintInfo();
};