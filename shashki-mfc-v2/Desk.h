#pragma once
#include "afxdialogex.h"
#include <afxwin.h>
#include <gdiplus.h>
#include "Game.h"


using namespace Gdiplus;
using namespace std;


// Диалоговое окно Desk

class Desk : public CDialogEx
{
	DECLARE_DYNAMIC(Desk)

public:
	Desk(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~Desk();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	ULONG_PTR token;

	Role human;
	Cell curCell;

	//Neiron
	Neiron* neiro;
	


	double** Board = NULL;

	double
		xClick,
		yClick;
	
	//колонка и линия выбранной шашки
	int col, line;	

	//колонка и линия места, куда хотим сходить
	int colStep, lineStep;

	//переменная отвечающая за отрисовку выбранной клетки
	bool Click = false;
	//является ли шашка дамкой
	bool king = false;

	double
		f,
		df,
		top,
		bottom,
		left,
		right,
		xPadding,
		yPadding,
		steplenY,
		steplenX,
		ItemWidth,
		ItemHeight,
		xScale,
		yScale;

	//координаты выбранного прямоугольника
	double leftx, rightx;
	double boty, topy;

	//находит выбранную клетку
	void FindCell(int &col, int &line);

	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);


	//PLAY
	
	//устанавливает игроков
	void setPlayers(int role);

	

	//ход игрока, когда не надо есть
	void HumanQuiet();

	//ход игрока, когда надо есть
	void HumanHungry();

	//ход игрока, когда не надо есть дамкой
	void HumanQuietKing();

	//ход игрока, когда надо есть дамкой
	void HumanHungryKing();

	PlayerFunction players[2];
	BoardState board;

};
