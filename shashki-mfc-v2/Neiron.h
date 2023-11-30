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

    //функция активации
    double Function(double S);

    //перебор слоя
    void LayerFunc(int layer);  
public:
    double score = 0;
    bool isWin = false;
    //конструктор
    Neiron();
    
    //деструктор
    ~Neiron();  

    //вывод результатов работы сети
    double Work(double *in);

    //печатает информацию о неироне
    void PrintInfo();
};