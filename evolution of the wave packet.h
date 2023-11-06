
// evolution of the wave packet.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CevolutionofthewavepacketApp:
// Сведения о реализации этого класса: evolution of the wave packet.cpp
//

class CevolutionofthewavepacketApp : public CWinApp
{
public:
	CevolutionofthewavepacketApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CevolutionofthewavepacketApp theApp;
