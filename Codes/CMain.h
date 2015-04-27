#pragma once


#include "Resource.h"
#include "CBase.h"
#include "CGraphics.h"
#include "CGraphics2D.h"
#include "CTexture.h"
#include "CFont.h"
#include "CDirectInput.h"
#include "CCManagerTextureAtlas.h"


// ----------------------- Global define's and variables (begin) -------------------

// ----------------- Error codes ---------------------
#define GAME_ENGINE_SUCCESS							-1  /* ������ ��������������� ��� ������ */
#define GAME_ENGINE_ERROR_CREATE_WINDOW				-2  /* ������ ��� �������� ���� */
#define GAME_ENGINE_ERROR_CREATE_DIRECT3D			-3  /* ������ ��� �������� Direct3D */
#define GAME_ENGINE_ERROR_CREATE_DIRECT3D_DEVICE	-4	/* ������ ��� �������� Direct3D Device*/

// ----------------------- Global define's and variables (end)   -------------------




/////////////////////////////////////// ����� CMain ( Begin ) //////////////////////////////



class CMain
{
	public:
		LPDIRECT3D9				m_pD3D;
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		D3DPRESENT_PARAMETERS	m_d3dpp;
		D3DDISPLAYMODE			m_d3ddm;
		D3DCAPS9				m_d3dCaps;


		HWND					m_hWnd;
		CGraphics				m_CGraphics;
		CGraphics2D				m_CGraphics2D;
		CBase					m_CBase;
		CTexture				m_CTexture;
		CFont					m_CFont;
		CDirectInput			m_CInput;
		CCManagerTextureAtlas	m_CManagerTextureAtlas;
		lua_State				*m_pLua;		// Lua �������

	public:
		CMain();
		~CMain();

		int iChangePresentationD3D();
		bool bTestCooperativeLevel();
		void vSaveInfoVideoDevice(char *nameFile);

		// ������������� �� *.ini �����
		int iInitFromFile_INI(LPCSTR lpClassName, LPCSTR lpWindowName,
								HINSTANCE hThisInst, char *nameFileINI);
		void vRelease();
};
/////////////////////////////////////// ����� CMain ( End ) //////////////////////////////