﻿
// shashki-mfc-v2Dlg.h: файл заголовка
//

#pragma once
#include "Desk.h"
#include "Population.h"


// Диалоговое окно Cshashkimfcv2Dlg
class Cshashkimfcv2Dlg : public CDialogEx
{
// Создание
public:
	Cshashkimfcv2Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHASHKIMFCV2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	MSG mes;
	UINT_PTR timer;
	Desk* desk;
	afx_msg void OnBnClickedOk();
	CComboBox Color;
	Population* p;
	double count;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedCancel();
	int IdTrain;
};
