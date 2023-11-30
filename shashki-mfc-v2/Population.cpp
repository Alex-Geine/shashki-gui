#include "pch.h"
#include "Population.h"
#include "Game.h"
#include <algorithm>

Population::Population(int count): Size(count) {
	for (int i = 0; i < count; i++) {
		Neiron *n = new Neiron();
		pop.push_back(n);
	}
	
	threads = new HANDLE[6];
	
	srand(time(0));
}

//функция, которая работает в потоке с моделью
DWORD WINAPI Population::Funk() {
	int id;
	int idPlay[5] = { -1, -1, -1, -1,-1 };
	int leftid = 0, rightid = 5;

	//EnterCriticalSection(&idSect);
	id = IdThread;
	IdThread++;
	//LeaveCriticalSection(&idSect);

	for (int i = 0; i < id; i++) {
		leftid += 5;
		rightid += 5;
	}


	for (int i = leftid; i < rightid; i++) {
		//cout << "play id: " << i << endl;
		for (int j = 0; j < 5; j++) {
			//games++;
			//cout << "play n: " << games << endl;
			//int idOpponent = (15 * (double)rand() / RAND_MAX);
			int idOpponent = rand() % 15;
			//проверка на уже сыгранные партии
			for (int k = 0; k < j; k++) {
				if (idOpponent == idPlay[k]) {
					idOpponent = rand() % 15;
					k = 0;
				}
			}

			idPlay[j] = idOpponent;

			//играют белые
			if (i < 15) {
				NeiroVsNeiro(white[i], black[idOpponent]);

				//смотрим, как изменился счет(убераем изменения для тех, кто не тренеруется
				if (white[i]->isWin)
					black[idOpponent]->score += 2;
				else
					black[idOpponent]->score -= 1;
			}
			//играют черные
			else {
				NeiroVsNeiro(white[idOpponent], black[i - 15]);

				//смотрим, как изменился счет(убераем изменения для тех, кто не тренеруется
				if (black[i - 15]->isWin)
					white[idOpponent]->score += 2;
				else
					white[idOpponent]->score -= 1;
			}

		}
		//возвращение массива сыгранных игр в начальное состояние
		for (int j = 0; j < 5; j++)
			idPlay[j] = -1;
	}
	return 1;
}

//Функция отбора особей
void Population::Selection() {
	random_shuffle(pop.begin(), pop.end());

	//запихиваем данные в два массива
	for (int i = 0; i < 30; i++) {
		if (i < 15)
			white[i] = pop[i];
		else
			black[i - 15] = pop[i];
	}

	//int games = 0;
	//проводим по 5 игр для каждой особи
	//так как при игре баллы даются обеим неиросеткам, для той, которая не обучается, баллы не присваиваются
	
	//создаем потоки
	IdThread = 0;
	for (int i = 0; i < 6; i++) 
		threads[i] = CreateThread(NULL, NULL, StaticModelFunk, (void*)this, 0, NULL);
		

	//ждем, пока отработают
	WaitForMultipleObjects(6, threads, TRUE, INFINITE);

	cout << "After tour: " << endl;
	//по результатам турнира отбираем лучших особей
	for (int i = 0; i < 30; i++) {
		if (i < 15)
			buf[i] = white[i];
		else
			buf[i] = black[i - 15];
		cout << "i: " << i << " score: " << buf[i]->score << endl;
	}

	Neiron* temp; // временная переменная для обмена элементов местами

	// Сортировка массива пузырьком
	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 29 - i; j++) {
			if (buf[j]->score < buf[j + 1]->score) {
				// меняем элементы местами
				temp = buf[j];
				buf[j] = buf[j + 1];
				buf[j + 1] = temp;
			}
		}
	}

	//вывод после сортировки
	cout << "After sort: " << endl;
	for (int i = 0; i < 30; i++) {
		cout << "i: " << i << " score: " << buf[i]->score << endl;
	}

	//заполняем первые 15 элементов вектора лучшими особями
	for (int i = 0; i < 15; i++)
		pop[i] = buf[i];
}

//Функция скрецивания
void Population::Crossing() {
	//vector<Neiron*> n;	

	////поиск особей для скрещивания
	//for (int i = 0; i < pop.size(); i++)
	//		n.push_back(pop[i]);
	//	
	//while (pop.size() <= Size) {
	//	random_shuffle(n.begin(), n.end());
	//	for (int i = 0; i < n.size(); i++) {
	//		if (n.size() - i != 1) {					
	//			pop.push_back(n[i]->Cross(n[i + 1]));				
	//		}
	//		if (pop.size() >= Size)
	//			i = n.size();
	//	}		
	//}
	//n.clear();
}

//Функция мутации
void Population::Mutations() {
	/*int Size = pop.size();
	for (int i = 0; i < Size; i++) {
		double ran = (double)rand() / RAND_MAX;
		if (ran <= mutationPer) 					
			pop.push_back(pop[i]->Mutation(maxMutper));		
	}*/

}

void Population::CreateNew() {
	for (int i = 0; i < 15; i++) {
		Neiron* neiro = new Neiron();
		pop[i + 15] = neiro;

		//сброс старого счета
		pop[i]->score = 0;
	}
}

//возвращает лучшую особь
Neiron* Population::GetBest() {
	return pop[0];
}