#include "CMain.h"


LRESULT CALLBACK WindowProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
			PostQuitMessage (0);
		break;

		case WM_SETCURSOR:
			SetCursor (NULL);
		break; 
	};
	return DefWindowProc(hWnd, message, wParam, lParam);
};


/////////////////////////////////////// Класс CMain ( Begin ) //////////////////////////////
CMain::CMain()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	ZeroMemory(&m_d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	m_pLua = NULL;
	m_pLua = lua_open();
	luaL_openlibs(m_pLua);  // load the libs

	SQUALL_Init(0);
}

CMain::~CMain()
{
}

//------------------------------------------------------------------------------------
// Name: vRelease()
// Desc:
//------------------------------------------------------------------------------------
void CMain::vRelease()
{
	if(m_pLua != NULL)
		lua_close(m_pLua);

	SQUALL_Sample_UnloadAll();
	SQUALL_Free();

	// Release other classes
	m_CFont.vRelease();
	m_CTexture.vRelease();
	m_CGraphics.vRelease();
	m_CGraphics2D.vRelease();
	m_CInput.vRelease();
	m_CManagerTextureAtlas.vRelease();

	_SAFE_RELEASE_(m_pD3DDevice);
	_SAFE_RELEASE_(m_pD3D);

	m_CBase.m_CLog.vAddString("Class CMain = Release");
}

//------------------------------------------------------------------------------------
// Name: iInitFromFile_INI()
// Desc:
//------------------------------------------------------------------------------------
int CMain::iInitFromFile_INI(LPCSTR lpClassName, LPCSTR lpWindowName, 
							 HINSTANCE hThisInst, char *nameFileINI)
{
	// ---------------------------- Создание и регистрация окна ----------------------
	WNDCLASS		    wcl;
	wcl.hInstance		= hThisInst;
	wcl.lpszClassName	= lpClassName;
	wcl.lpfnWndProc		= WindowProc;
	wcl.style			= 0;
	wcl.hIcon			= LoadIcon (hThisInst, IDC_ICON);
	wcl.hCursor			= LoadCursor (hThisInst, IDC_ARROW);
	wcl.lpszMenuName	= NULL;
	wcl.cbClsExtra		= 0;
	wcl.cbWndExtra		= 0;
	wcl.hbrBackground	= (HBRUSH) GetStockObject (BLACK_BRUSH);
	RegisterClass (&wcl);

	m_hWnd = CreateWindowEx (
		WS_EX_TOPMOST,
		lpClassName,
		lpWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, 
		640,
		480,
		NULL,
		NULL,
		hThisInst,
		NULL);

	if(!m_hWnd) return GAME_ENGINE_ERROR_CREATE_WINDOW;

	ShowWindow (m_hWnd, SW_SHOWNORMAL);
	UpdateWindow (m_hWnd);
	SetForegroundWindow(m_hWnd);
	// ------------------------------------------------------------------------------------



	// ---------------------------------- Загрузка данных ---------------------------------
	int iAnswer; // ответ функций
	m_d3dpp.hDeviceWindow = m_hWnd;
	int typePresentInterval;

	m_CBase.vRead_UINT_InIni("VideoSettings", "ScreenWidth", nameFileINI, m_d3dpp.BackBufferWidth);
	m_CBase.vRead_UINT_InIni("VideoSettings", "ScreenHeight", nameFileINI, m_d3dpp.BackBufferHeight);
	m_CBase.vRead_BOOL_InIni("VideoSettings", "Fullscreen", nameFileINI, m_d3dpp.Windowed);
	m_CBase.vRead_UINT_InIni("VideoSettings", "BackBufferCount", nameFileINI, m_d3dpp.BackBufferCount);

	m_CBase.vRead_Integer_InIni("VideoSettings", "PresentationInterval", nameFileINI, typePresentInterval);
	switch(typePresentInterval)
	{
		case 1: m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; break;
		case 2: m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; break;
	}


	m_CBase.vRead_Float_InIni("MOUSE", "sensitivity_x", nameFileINI, m_CInput.m_fMouseSensitivity_X);
	m_CBase.vRead_Float_InIni("MOUSE", "sensitivity_y", nameFileINI, m_CInput.m_fMouseSensitivity_Y);
	m_CBase.vRead_Float_InIni("MOUSE", "sensitivity_z", nameFileINI, m_CInput.m_fMouseSensitivity_Z);
	
	
	m_d3dpp.Windowed = !m_d3dpp.Windowed;
	// ------------------------------------------------------------------------------------



	// -------------------------------- Инициализация классов ------------------------------
	if( GAME_ENGINE_SUCCESS != (iAnswer = iChangePresentationD3D()))
		return iAnswer;

	m_CManagerTextureAtlas.vInit(m_pD3DDevice);
	m_CTexture.vInit(m_pD3DDevice, &m_CBase);
	m_CGraphics.vInit(m_pD3DDevice, m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight,&m_CBase, &m_CTexture);
	m_CGraphics2D.vInit(m_pD3DDevice, m_d3dpp.BackBufferWidth, m_d3dpp.BackBufferHeight, &m_CBase);
	m_CFont.vInit(m_pD3DDevice, &m_CBase, &m_CGraphics);
	m_CInput.iInit(m_hWnd, hThisInst, DINPUT_INIT_KEYBOARD | DINPUT_INIT_MOUSE);

	m_CBase.m_CLog.vAddString("Class CMain = Init");
	// ------------------------------------------------------------------------------------

	vSaveInfoVideoDevice("Reports\\Report_VideoDevice.txt");

	// -------------------- ПРОВЕРКА МЕНЕДЖЕРА ТЕКСТУР ---------
	const int N = 20;
	CCManagerTextureAtlas_h::stHandleTexture stHandleTexture[N];

	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\diamond.png", &stHandleTexture[0]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\diamond.png", &stHandleTexture[1]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\diamond.png", &stHandleTexture[2]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\diamond.png", &stHandleTexture[3]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\pMain.png", &stHandleTexture[4]);

	m_CManagerTextureAtlas.vDeleteTexture(&stHandleTexture[0]);
	m_CManagerTextureAtlas.vDeleteTexture(&stHandleTexture[1]);
	m_CManagerTextureAtlas.vDeleteTexture(&stHandleTexture[2]);
	m_CManagerTextureAtlas.vDeleteTexture(&stHandleTexture[3]);

	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\1.png", &stHandleTexture[0]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\4.png", &stHandleTexture[1]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\2.png", &stHandleTexture[2]);
	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\0.png", &stHandleTexture[3]);

	m_CManagerTextureAtlas.vAddTexture("Data\\gfx\\diamond.png", &stHandleTexture[4]);

	


	m_CManagerTextureAtlas.vSaveTextureAtlasesToFiles("Data");
	return GAME_ENGINE_SUCCESS;
}


//------------------------------------------------------------------------------------
// Name: iChangePresentationD3D()
// Desc:
//------------------------------------------------------------------------------------
int CMain::iChangePresentationD3D()
{
	DWORD dwWindowStyle, dwWindowExStyle;
	RECT rectWindow;
	rectWindow.left = (long)0;
	rectWindow.top = (long)0;
	rectWindow.right = (long)m_d3dpp.BackBufferWidth;
	rectWindow.bottom = (long)m_d3dpp.BackBufferHeight;


	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;


	if(m_d3dpp.Windowed == TRUE)
	{
		// Windowed

		dwWindowExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwWindowStyle = WS_OVERLAPPEDWINDOW;

		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}
	else
	{
		// Fullscreen

		dwWindowExStyle = WS_EX_APPWINDOW;
		dwWindowStyle = WS_POPUP;

		m_d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
	}
	AdjustWindowRectEx(&rectWindow, dwWindowStyle, FALSE, dwWindowExStyle);
	SetWindowLong( m_hWnd, GWL_STYLE, dwWindowStyle );
		if(TRUE == m_d3dpp.Windowed)
			SetWindowPos( m_hWnd, HWND_NOTOPMOST, 0, 0, 
							rectWindow.right - rectWindow.left,
							rectWindow.bottom - rectWindow.top,
							SWP_SHOWWINDOW );

	m_CInput.vResizeWindow();

	// Если Direct3D не инициализирован -> инициализируем
	if(NULL == m_pD3D)
	{
		if( NULL == (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
			return GAME_ENGINE_ERROR_CREATE_DIRECT3D;

		DWORD dwFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			if(FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,
											D3DDEVTYPE_HAL,
											m_hWnd,dwFlags,
											&m_d3dpp,
											&m_pD3DDevice) ))
				return GAME_ENGINE_ERROR_CREATE_DIRECT3D_DEVICE;

			m_pD3DDevice->GetDeviceCaps(&m_d3dCaps);
			return GAME_ENGINE_SUCCESS;
	}

	m_pD3DDevice->Reset(&m_d3dpp);

	return GAME_ENGINE_SUCCESS;
}

//------------------------------------------------------------------------------------
// Имя: bTestCooperativeLevel()
// Описание: Проверяем Direct3dDevice на потерянность
//------------------------------------------------------------------------------------
bool CMain::bTestCooperativeLevel()
{	
	HRESULT hr;

	// Проверяем Direct3dDevice на потерянность
	hr = m_pD3DDevice->TestCooperativeLevel();

	// если окно не в фокусе выходим из цикла рендеринга
	if(hr==D3DERR_DEVICELOST)
		return false; 

	// если окно опять в фокусе пытаемся восстановить устройство рендеринга
	if(hr==D3DERR_DEVICENOTRESET)
		m_pD3DDevice->Reset(&m_d3dpp); // если окно опять в фокусе пытаемся восстановить наш g_pd3dDevice

	if( NULL == m_pD3DDevice )
		return false;
	return true;
}


//------------------------------------------------------------------------------------
// Имя: vSaveInfoVideoDevice()
// Описание: Проверяем Direct3dDevice на потерянность
//------------------------------------------------------------------------------------
void CMain::vSaveInfoVideoDevice(char *nameFile)
{
	int iModeCount =0;
	int iBpp = 0;


	FILE *file = fopen(nameFile,"w");
	fprintf(file,"MaxActiveLights = %d\n", m_d3dCaps.MaxActiveLights);
	fprintf(file,"MaxAnisotropy = %d\n", m_d3dCaps.MaxAnisotropy);
	fprintf(file,"MaxPointSize = %f\n", m_d3dCaps.MaxPointSize);
	fprintf(file,"MaxPrimitiveCount = %d\n", m_d3dCaps.MaxPrimitiveCount);
	fprintf(file,"MaxVertexIndex = %d\n", m_d3dCaps.MaxVertexIndex);
	fprintf(file,"MaxVertexBlendMatrices = %d\n", m_d3dCaps.MaxVertexBlendMatrices);
	fprintf(file,"MaxStreams = %d\n", m_d3dCaps.MaxStreams);
	fprintf(file,"MaxStreamStride = %d\n", m_d3dCaps.MaxStreamStride);

	fprintf(file,"MaxTextureWidth = %d\n", m_d3dCaps.MaxTextureWidth);
	fprintf(file,"MaxTextureHeight = %d\n", m_d3dCaps.MaxTextureHeight);
	fprintf(file,"MaxTextureBlendStages = %d\n", m_d3dCaps.MaxTextureBlendStages);

	fprintf(file,"MaxVShaderInstructionsExecuted = %d\n", m_d3dCaps.MaxVShaderInstructionsExecuted);
	fprintf(file,"MaxPShaderInstructionsExecuted = %d\n", m_d3dCaps.MaxPShaderInstructionsExecuted);


	// Проверяем поддерживаемые версии шейдеров
	char cStrVersionShader[50];

	// Vertex shader
	for(int highVers = 0; highVers < 5; highVers++)
		for(int lowVers = 0; lowVers < 10; lowVers++)
			if(m_d3dCaps.VertexShaderVersion >= D3DVS_VERSION(highVers, lowVers))
				sprintf(cStrVersionShader,"VertexShaderVersion  = %d_%d", highVers, lowVers);
	fprintf(file, "%s\n", cStrVersionShader);

	// Pixel shader
	for(int highVers = 0; highVers < 5; highVers++)
		for(int lowVers = 0; lowVers < 10; lowVers++)
			if(m_d3dCaps.PixelShaderVersion >= D3DPS_VERSION(highVers, lowVers))
				sprintf(cStrVersionShader,"PixelShaderVersion  = %d_%d", highVers, lowVers);
	fprintf(file, "%s\n", cStrVersionShader);



	// Проверяем возможность аппаратной поддержки обработки вершин
	if(m_d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		fprintf(file,"HARDWARE_VERTEX_PROCESSING = true\n");
	else
		fprintf(file,"HARDWARE_VERTEX_PROCESSING = false\n");




	// Запишем поддерживаемые разрешения для разных форматов поверхности
	fprintf(file,"\n\n\n\n");
	for(iBpp = D3DFMT_R8G8B8; iBpp <= D3DFMT_A16B16G16R16; iBpp++)
	{
		fprintf(file,"\n");
		iModeCount = m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,(_D3DFORMAT)iBpp);

		switch(iBpp)
		{
		case D3DFMT_R8G8B8: fprintf(file,"Surface Format = D3DFMT_R8G8B8\n"); break;
		case D3DFMT_A8R8G8B8: fprintf(file,"Surface Format = D3DFMT_A8R8G8B8\n"); break;
		case D3DFMT_X8R8G8B8: fprintf(file,"Surface Format = D3DFMT_X8R8G8B8\n"); break;
		case D3DFMT_R5G6B5: fprintf(file,"Surface Format = D3DFMT_R5G6B5\n"); break;
		case D3DFMT_X1R5G5B5: fprintf(file,"Surface Format = D3DFMT_X1R5G5B5\n"); break;
		case D3DFMT_A1R5G5B5: fprintf(file,"Surface Format = D3DFMT_A1R5G5B5\n"); break;
		case D3DFMT_A4R4G4B4: fprintf(file,"Surface Format = D3DFMT_A4R4G4B4\n"); break;
		case D3DFMT_R3G3B2: fprintf(file,"Surface Format = D3DFMT_R3G3B2\n"); break;
		case D3DFMT_A8: fprintf(file,"Surface Format = D3DFMT_A8\n"); break;
		case D3DFMT_A8R3G3B2: fprintf(file,"Surface Format = D3DFMT_A8R3G3B2\n"); break;
		case D3DFMT_X4R4G4B4: fprintf(file,"Surface Format = D3DFMT_X4R4G4B4\n"); break;
		case D3DFMT_A2B10G10R10: fprintf(file,"Surface Format = D3DFMT_A2B10G10R10\n"); break;
		case D3DFMT_A8B8G8R8: fprintf(file,"Surface Format = D3DFMT_A8B8G8R8\n"); break;
		case D3DFMT_X8B8G8R8: fprintf(file,"Surface Format = D3DFMT_X8B8G8R8\n"); break;
		case D3DFMT_G16R16: fprintf(file,"Surface Format = D3DFMT_G16R16\n"); break;
		case D3DFMT_A2R10G10B10: fprintf(file,"Surface Format = D3DFMT_A2R10G10B10\n"); break;
		case D3DFMT_A16B16G16R16: fprintf(file,"Surface Format = D3DFMT_A16B16G16R16\n"); break;
		};

		fprintf(file,"CountModes = %d\n",iModeCount);
		for(int i= 0; i< iModeCount; i++)
		{
			m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,(_D3DFORMAT)iBpp,i,&m_d3ddm);
			fprintf(file,"%dx%d - %d HZ\n",m_d3ddm.Width,m_d3ddm.Height,m_d3ddm.RefreshRate);
		}
	}
	fclose(file);
}
/////////////////////////////////////// Класс CMain ( End ) //////////////////////////////