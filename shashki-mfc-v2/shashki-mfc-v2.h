
// shashki-mfc-v2.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// Cshashkimfcv2App:
// Сведения о реализации этого класса: shashki-mfc-v2.cpp
//

class Cshashkimfcv2App : public CWinApp
{
public:
	Cshashkimfcv2App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern Cshashkimfcv2App theApp;
