#pragma once
#ifndef _CCMANAGER_TEXTURE_ATLAS_H_
#define _CCMANAGER_TEXTURE_ATLAS_H_

#include "Resource.h"


// ----------------------------- Message codes ---------------------------
#define MANAGER_TEXTURE_ATLAS_SUCCESS		-1	/* ����� */
#define TEXTURE_ATLAS_CLASS_ENABLE_RELEASE	-2  /* ����� ����������� ������ ����� ������� */


namespace CCManagerTextureAtlas_h
{
	struct stHandleTexture
	{
		int iIndexAtlas;    // ������ ������
		int iIndexTexture;  // ������ �������� �� ������

		stHandleTexture()
		{
			iIndexAtlas = -1;
			iIndexTexture = -1;
		}
	};


	struct stTexture
	{	
		// ���������� �������� � �������� ��������, �������� �������� [0..1]
		float fCoordInTextureAtlas_BegPosX;
		float fCoordInTextureAtlas_BegPosY;
		float fCoordInTextureAtlas_EndPosX;
		float fCoordInTextureAtlas_EndPosY;

		// ���������� �������� � �������� � �������� ��������
		UINT uintCoordInTextureAtlas_BegPosX;
		UINT uintCoordInTextureAtlas_BegPosY;
		UINT uintCoordInTextureAtlas_EndPosX;
		UINT uintCoordInTextureAtlas_EndPosY;

		stTexture()
		{
			fCoordInTextureAtlas_BegPosX = 0.0f;
			fCoordInTextureAtlas_BegPosY = 0.0f;
			fCoordInTextureAtlas_EndPosX = 0.0f;
			fCoordInTextureAtlas_EndPosY = 0.0f;

			uintCoordInTextureAtlas_BegPosX = 0;
			uintCoordInTextureAtlas_BegPosY = 0;
			uintCoordInTextureAtlas_EndPosX = 0;
			uintCoordInTextureAtlas_EndPosY = 0;
		}
	};
};

////////////////////////////// ����� CCTextureAltas ( begin ) ///////////////////////////////
class CCTextureAltas
{
	private:
		LPDIRECT3DDEVICE9	m_lpD3DDevice;
		LPDIRECT3DTEXTURE9	m_lpTexture;
		D3DCAPS9			m_d3dCaps;
		DWORD				m_dwColorClear;  // ���� ������ ������������� �������� ��������
		UINT				m_uintDefaultTextureAtlasWidth;  // ������� �������� �������� �� ��������� (������������ �� ���)
		UINT				m_uintDefaultTextureAtlasHeight; // ������ ������ � ������������
		UINT				m_uintTextureRecomendWidth; // ��� ������������� ������ ������� �������
		UINT				m_uintTextureRecomendHeight;
		bool				*m_pbFreeMap; // ������ ��������� ������� �������� ��������
		UINT				m_uintQuantFreePixel;  // ���������� ��������� �������� ��������
		bool				m_bReleased;
		
		std::vector<CCManagerTextureAtlas_h::stTexture> m_vecTexture; // ������ ����������� �������
		std::stack<int, std::vector<int>> m_stackFreeIndexTexture; // ��� �������� ���� ����������� ������
	public:
		CCTextureAltas(LPDIRECT3DDEVICE9 lpD3DDevice);
		~CCTextureAltas();

		int iAddTexture(LPDIRECT3DTEXTURE9 *plpTexture);
		int iDeleteTexture(int *piIndexTexture);
		void vSaveTextureAtlasToFile(char *name);
		bool bGetStatusReleased();
		void vRelease();
};
/////////////////////////////// ����� CCTextureAltas ( end ) ////////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------



//////////////////////// ����� CCManagerTextureAtlas ( begin ) //////////////////////////////
class CCManagerTextureAtlas
{
	private:
		LPDIRECT3DDEVICE9			m_lpD3DDevice;
		std::vector<CCTextureAltas> m_vecCTextureAltas;

	public:
		CCManagerTextureAtlas();
		~CCManagerTextureAtlas();

		void vInit(LPDIRECT3DDEVICE9 lpD3DDevice);
		void vAddTexture(char *name, CCManagerTextureAtlas_h::stHandleTexture *pHandleTexture);
		void vDeleteTexture(CCManagerTextureAtlas_h::stHandleTexture *pHandleTexture);
		void vCreateTextureDefault(LPDIRECT3DTEXTURE9 *plpTexture, UINT width, UINT height);
		void vSaveTextureAtlasesToFiles(char *nameCatalog);
		void vRelease();
};
///////////////////////// ����� CCManagerTextureAtlas ( end ) ///////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------

#endif  // _CCMANAGER_TEXTURE_ATLAS_H_