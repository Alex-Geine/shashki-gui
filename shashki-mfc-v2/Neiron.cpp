#include "pch.h"
#include "Neiron.h"



Neiron::Neiron() :
    layers(layers),
    neironsCount(neironsCount) {
    layers = 3;

    neironsCount = new int[layers];
    neironsCount[0] = 40;
    neironsCount[1] = 10;
    neironsCount[2] = 1;
    
    xSize = new int[layers];
    xSize[0] = 32;
    xSize[1] = 40;
    xSize[2] = 42;  //10 с прошлого, 32 со входа

    //заполнение массива пороговых коэффициентов

    koef = new double* [layers];
    for (int i = 0; i < layers; i++) {       
        koef[i] = new double[neironsCount[i]];
        for (int j = 0; j < neironsCount[i]; j++)
            koef[i][j] = -0.2 + 0.4 * (double)rand() / RAND_MAX;
    }

    //заполнение массива весов

    w = new double** [layers];
    for (int i = 0; i < layers; i++) {
        w[i] = new double* [neironsCount[i]];

        for (int j = 0; j < neironsCount[i]; j++) {
            

            w[i][j] = new double[xSize[i]];

            for (int k = 0; k < xSize[i]; k++) {
                w[i][j][k] = -0.2 + 0.4 * (double)rand() / RAND_MAX;
                if ((k > 9) && i == 2)
                    w[i][j][k] = 1;
            }
                
        }

    }

    //заполнение массива иксов

    x = new double* [layers + 1];
    x[0] = new double[32];
    for (int i = 1; i <= layers; i++) {
        x[i] = new double[neironsCount[i - 1]];
    }
}


//функция активации
double Neiron::Function(double S) {
    return (1 / (1 + exp(-B * S)));
}

//перебор слоя
void Neiron::LayerFunc(int layer) {
    for (int j = 0; j < neironsCount[layer]; j++) {
        double sum = 0;    
        
        for (int i = 0; i < xSize[layer]; i++) {
            if ((i > 9) && layer == 2) {
                sum += w[layer][j][i] * x[0][i - 10] - koef[layer][j];
            }
            else
                sum += w[layer][j][i] * x[layer][i] - koef[layer][j];
        }
            
                     
        x[layer + 1][j] = Function(sum);
    }
}


//вывод результатов работы сети
double Neiron::Work(double * in) {
    x[0] = in;

    for (int i = 0; i < layers; i++)
        LayerFunc(i);

    return x[layers][0];
}

//деструктор
Neiron::~Neiron() {


    for (int i = 0; i < layers; i++)
        delete[] koef[i];

    delete[] koef;

    for (int i = 0; i < layers; i++) {
        for (int j = 0; j < neironsCount[i]; j++) {
            int xSize = 3;
            if (i != 0)
                xSize = neironsCount[i - 1];

            delete[] w[i][j];
        }
        delete[] w[i];
    }

    delete[] w;

    for (int i = 1; i <= layers; i++)
        delete[] x[i];

    delete[] x;
      
}

//печатает информацию о неироне
void Neiron::PrintInfo() {

    cout << "INFO ABOUT NEIRON" << endl << "koef: " << endl;
    for (int i = 0; i < layers; i++) {
        cout << endl << "laer: " << i << endl;
        for (int j = 0; j < neironsCount[i]; j++)
            cout << " k: " << j << " = " << koef[i][j] << endl;
    }

    cout << "weights: " << endl;

    for (int i = 0; i < layers; i++) {
        cout << endl << "laer: " << i << endl;

        for (int j = 0; j < neironsCount[i]; j++) {
            cout << "neiron: " << j << endl;
                   

            for (int k = 0; k < xSize[i]; k++)
                cout << " w k: " << k << " = " << w[i][j][k] << endl;                
        }

    }
}