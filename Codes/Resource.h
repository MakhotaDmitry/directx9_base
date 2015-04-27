#pragma once

#include <fstream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "squall.h"

#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>

extern "C"
{
	#include "lua.h"  
	#include "lualib.h"  
	#include "lauxlib.h"  
}


#pragma comment(lib,"lua51.lib")
#pragma comment(lib,"Squall.lib")
#pragma comment(lib,"Squalld.lib")

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")




// Освобождение ресурсов
#define _SAFE_DELETE_ARRAY_(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define _SAFE_RELEASE_(p)		{ if(p) { (p)->Release(); (p)=NULL; } }
#define _SAFE_DELETE_(p)		{ if(p) { delete (p);     (p)=NULL; } }


// Перевод	радианы <-> градусы
#define GRAD_TO_RAD(grad)	 grad * 3.141592654f/180.0f
#define RAD_TO_GRAD(radian)  radian * 180.0f/3.141592654f


/*
	DWORD = 32 bit = 4 byte
	WORD  = 16 bit = 2 byte
	BYTE  = 8 bit  = 1 byte


	8 bit = 0..255 = FF
	6 bit = 0..63  = 3F
	5 bit = 0..31  = 1F
	3 bit = 0..7   = 7
	2 bit = 0..3   = 3
*/
#define COLOR_TO_A8R8G8B8(a,r,g,b) (DWORD)(((a&0xff)<<24)|((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff))
#define COLOR_TO_X8R8G8B8(r,g,b) COLOR_TO_A8R8G8B8(255,r,g,b)
#define COLOR_TO_R5G6B5(r,g,b) (WORD)(((r&0x1f)<<11)|((g&0x3f)<<5)|(b&0x1f))
#define COLOR_TO_R3G3B2(r,g,b) (BYTE)(((r&0x7)<<5)|((g&0x7)<<2)|(b&0x3))


// Get 32,24 bit color in DWORD
#define getColorA_32(x) ((x&0xff000000)>>24)
#define getColorR_32(x) ((x&0x00ff0000)>>16)
#define getColorG_32(x) ((x&0x0000ff00)>>8)
#define getColorB_32(x)  (x&0x000000ff)

// ////////////////////////////// Различные полезные структуры (Begin) //////////////////////////////////
namespace resource_h
{
	struct stFPS
	{
		stFPS()
		{
			iFPS = 0;
			dwBegTime = 0;
			bModified = false;

			sprintf_s(cFPS,"FPS: 0");
		};

		DWORD dwBegTime;
		int iFPS;
		bool bModified; // fps был измерен, при следующей итерации сбросится
		char cFPS[10];
	};
//------------------------------------------------------------------------------------------

	struct stRECT
	{
		stRECT()
		{
			iPosX = 0;
			iPosY = 0;
			iWidth = 0;
			iHeight = 0;
			iPosX_End = 0;
			iPosY_End = 0;
		};
		stRECT(int posX, int posY, int width, int height)
		{
			iPosX = posX;
			iPosY = posY;
			iWidth = width;
			iHeight = height;
			iPosX_End = posX + width;
			iPosY_End = posY + height;
		};

		int iPosX; // позиция по X
		int iPosY; // позиция по Y
		int iWidth; // Ширина
		int iHeight; // Высота
		int iPosX_End;
		int iPosY_End;
	};
//------------------------------------------------------------------------------------------

	struct stSIZE
	{
		stSIZE()
		{
			cx = 0;
			cy = 0;
		};

		stSIZE(int x, int y)
		{
			cx = x;
			cy = y;
		};

		int cx;
		int cy;
	};
//------------------------------------------------------------------------------------------


	// Структура для Vertex'ов спрайтов
	#define  D3DFVF_SPRITE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	struct stVertexSprite
	{
		D3DXVECTOR3 vectorPos;
		D3DCOLOR color;
		float tu, tv;

		stVertexSprite()
		{
			color = D3DCOLOR_ARGB(255,255,255,255);
			vectorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			tu = 0.0f;
			tv = 0.0f;
		}
	};
//------------------------------------------------------------------------------------------
};
////////////////////////////////// Различные полезные структуры (End) /////////////////////////