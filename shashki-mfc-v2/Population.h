#pragma once
#include "Neiron.h"
#include "Game.h"
#include <time.h>
#define _AFXDLL
#include <afxwin.h>         // �������� � ����������� ���������� MFC>



using namespace std;

class Population {
private:
	HANDLE* threads;
	CRITICAL_SECTION idSect;


	vector<Neiron*> pop;
	double mutationPer = 1; //0.01  //0.07
	double maxMutper = 0.03;  //0.05   //0.17
	int Size;
	
	int layers = 2;
	int* newirons = new int[layers];
	
	Neiron* white[15];
	Neiron* black[15];
	Neiron* buf[30];

	int IdThread;	

	//�������, ������� �������� � ������ � �������
	DWORD WINAPI Funk();

	static DWORD WINAPI StaticModelFunk(PVOID param) {
		Population* This = (Population*)param;
		/*int id;
		EnterCriticalSection(&This->idSect);
		id = This->IdThread;
		This->IdThread++;
		LeaveCriticalSection(&This->idSect);*/

		return This->Funk();
	}

public:
	

	double fsr;
	double best;
	Neiron* Best;
	Population(int count);

	//������� ������ ������
	void Selection();

	//������� �����������
	void Crossing();

	//������� �������
	void Mutations();

	void CreateNew();
	//���������� ������ �����
	Neiron* GetBest();

};