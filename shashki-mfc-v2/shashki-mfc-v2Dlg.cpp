
// shashki-mfc-v2Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "shashki-mfc-v2.h"
#include "shashki-mfc-v2Dlg.h"
#include "afxdialogex.h"
#include "board_state.h"
#include "minimax.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно Cshashkimfcv2Dlg



Cshashkimfcv2Dlg::Cshashkimfcv2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHASHKIMFCV2_DIALOG, pParent)
	, count(0)
	, IdTrain(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cshashkimfcv2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, Color);
	DDX_Text(pDX, IDC_EDIT1, count);
	DDX_Text(pDX, IDC_EDIT2, IdTrain);
}

BEGIN_MESSAGE_MAP(Cshashkimfcv2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cshashkimfcv2Dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &Cshashkimfcv2Dlg::OnBnClickedOk2)
	ON_EN_CHANGE(IDC_EDIT1, &Cshashkimfcv2Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDCANCEL, &Cshashkimfcv2Dlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// Обработчики сообщений Cshashkimfcv2Dlg

BOOL Cshashkimfcv2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	desk = new Desk();
	desk->Create(IDD_DIALOG1);

	//инициализация меню
	Color.AddString(L"Белые");
	Color.AddString(L"Черные");

	p = new Population(30);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void Cshashkimfcv2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void Cshashkimfcv2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR Cshashkimfcv2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//ок
void Cshashkimfcv2Dlg::OnBnClickedOk()
{
	UpdateData();

	int id = Color.GetCurSel();
	CString col; 
	Color.GetLBText(id, col);
	
	desk->neiro = p->GetBest();
	if (col == L"Белые") {
		desk->setPlayers(1);
	}
	else if (col == L"Черные") {
		desk->setPlayers(2);
	}
	else {
		MessageBox(L"Выберете цвет!", L"Выберете цвет!", NULL);
		return;
	}

	
	desk->ShowWindow(1);	
}

//Кнопка, запускающая тренеровку
void Cshashkimfcv2Dlg::OnBnClickedOk2()
{
	MessageBox(L"Тренеровка начата!");
	//timer = SetTimer(1, 10, 0);

	UpdateData();
	for (int i = 0; i < count; i++) {
		IdTrain = i;
		UpdateData(false);


		while (PeekMessage(&mes, 0, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&mes);
			DispatchMessage(&mes);
		}
		p->Selection();
		p->CreateNew();
	}
	MessageBox(L"Тренеровка завершена!");
	//KillTimer(timer);
}


void Cshashkimfcv2Dlg::OnEnChangeEdit1()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}


void Cshashkimfcv2Dlg::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnCancel();
}

//timer
void Cshashkimfcv2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData(false);
	

	while (PeekMessage(&mes, 0, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&mes);
		DispatchMessage(&mes);
	}

	CDialogEx::OnTimer(nIDEvent);
}