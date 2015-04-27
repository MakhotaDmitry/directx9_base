#pragma once

#include "Resource.h"
#include "CBase.h"
#include "CTexture.h"

namespace grahics_h
{
	/////////////////////////////////////// Данные класса CSprite ( begin ) //////////////////////////////
// 	#define  D3DFVF_SPRITE (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
// 	struct stVertexSprite
// 	{
// 		D3DXVECTOR3 vectorPos;
// 		D3DCOLOR color;
// 		float tu, tv;
// 
// 		stVertexSprite()
// 		{
// 			color = D3DCOLOR_ARGB(255,255,255,255);
// 			vectorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
// 			tu = 0.0f;
// 			tv = 0.0f;
// 		}
// 	};
	/////////////////////////////////////// Данные класса CSprite ( end ) //////////////////////////////



	/////////////////////////////////////// Данные класса CGraphics ( begin ) //////////////////////////////
	struct stAnimation
	{
		stAnimation()
		{
			bAccess = true;
			bAnimEnable = true;
			fAngle = 0.0f;

			//dwBegTimeAnim = 0;
			iRateInSec = 0;
			iDelayFrame = 0;

			iQuantCadrs = 0;
			iNumCurCadr = 0;

			iIndexTexture = 0;
		}

		bool bAccess;  // Доступный ли элемент
		bool bAnimEnable;  // Разрешено ли анимирование
		resource_h::stRECT rectPos;  // Положение + размеры  кадра на экране
		resource_h::stRECT *rectCadr;  // Положение + размеры кадра в текстуре
		float fAngle;
		
		int iIndexTexture;  // индекс текстуры

		DWORD dwLastUpdate; // последнее обновление кадра рисунка
		int iRateInSec; // частота кадров в секунду
		int iDelayFrame; // задержка между кадрами

		int iQuantCadrs;  // Количество кадров
		int iNumCurCadr;
	};
	/////////////////////////////////////// Данные класса CGraphics ( end ) //////////////////////////////



	/////////////////////////////////////// Данные класса CGraphics ( begin ) //////////////////////////////
	struct stRectChangeSize
	{
		stRectChangeSize()
		{
			d3d_col_LineTop = D3DCOLOR_XRGB(121,242,242);
			d3d_col_LineBottom = D3DCOLOR_XRGB(121,242,242);
			d3d_col_LineLeft = D3DCOLOR_XRGB(121,242,242);
			d3d_col_LineRight = D3DCOLOR_XRGB(121,242,242);
		}

		// координаты прямоугольника
		int iBegPosX;
		int iBegPosY;
		int iEndPosX;
		int iEndPosY;
		int iWidth;
		int iHeight;

		// цвета рамок
		D3DCOLOR d3d_col_LineTop;
		D3DCOLOR d3d_col_LineBottom;
		D3DCOLOR d3d_col_LineLeft;
		D3DCOLOR d3d_col_LineRight;

		// Текстура линий под прямоугольник
		LPDIRECT3DTEXTURE9 textureLine;
	};
	/////////////////////////////////////// Данные класса CGraphics ( end ) //////////////////////////////
};




/////////////////////////////////////// Класс CSprite ( begin ) //////////////////////////////
class CSprite
{
	private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		UINT					m_uintBackBufferWidth;
		UINT					m_uintBackBufferHeight;
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;
		resource_h::stVertexSprite *m_pstVertex;
		int m_iQuantVertex;
		
		LPDIRECT3DTEXTURE9 *m_pTexture; // просто будем брать размеры текстуры
		D3DSURFACE_DESC	m_d3dsd_Texture;
		float m_fCadr_sx;  // По этим переменным определяем текстурные координаты
		float m_fCadr_sy;  // Вывел в лены класса тобы не создавать в функциях
		float m_fCadrPosX;
		float m_fCadrPosY;



		D3DXMATRIX m_matWorld;
		D3DXMATRIX m_matView;
		D3DXMATRIX m_matProjection;
		D3DXMATRIX m_matRot;
		D3DXMATRIX m_matTrans;
		D3DXMATRIX m_matScale;

	public:
		CSprite();
		~CSprite();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight);
		void vRelease();
		void vBegin();
		void vEnd();
		void vSetColor(D3DCOLOR color_lt, D3DCOLOR color_lb, D3DCOLOR color_rb, D3DCOLOR color_rt);
		void vSetColor(D3DCOLOR color);
		void vSetTexure(LPDIRECT3DTEXTURE9 *texture);
		void vDrawUP(resource_h::stRECT *rectPos, resource_h::stRECT *rectCadr, float fAngle);
		void vDraw(resource_h::stRECT *rectPos, resource_h::stRECT *rectCadr, float fAngle);
};
/////////////////////////////////////// Класс CSprite ( end ) ///////////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------




/////////////////////////////////////// Класс CAnimation ( Begin ) //////////////////////////////
#define ANIMATION_ERROR_LOAD_TEXTURE -1  /* ошибка загрузки текстуры */
class CAnimation
{
	private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		CSprite		*m_pCSprite;
		CBase		*m_pCBase;
		CTexture	*m_pCTexture;

		std::vector<LPDIRECT3DTEXTURE9> m_vecTexture;
		std::vector<grahics_h::stAnimation> m_vecStAnimation;  // вектор структур анимации
		std::stack <int, std::vector<int> > m_stackFreeIndex;  //  стек свобоных елементов массива

		unsigned int m_iQuantTexture;  // Количество текстур
		unsigned int m_iQuantAnimation;  // Количество структур анимации

	public:
		CAnimation();
		~CAnimation();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, CSprite *sprite, CBase *base, CTexture *pCTexture);
		void vRelease();
		int iLoadTexture(char *nameTexture);
		void vSetTexture(int iIndexTexture);

		int iAddAnimation(int iIndexTexture, int iRateInSec, resource_h::stRECT rectPos, resource_h::stSIZE sizeCadr);
		void vSetAnimationPos(int iIndexAnim, resource_h::stRECT rectPos);
		
		void vCalculate();
		void vDraw(int iIndexAnim);
};
/////////////////////////////////////// Класс CAnimation ( End ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------




/////////////////////////////////////// Класс CPrimitiveGraphic ( Begin ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------
class CPrimitiveGraphic
{
	private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;

	public:
		CPrimitiveGraphic();
		~CPrimitiveGraphic();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice);
		void vRelease();

		void vDrawPoint(resource_h::stSIZE pos, D3DCOLOR color);
		void vDrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color);
		void vDrawLine(int x1, int y1, int x2, int y2, int width, D3DCOLOR color);
		void vDrawCircle(int x0, int y0, int radius, D3DCOLOR color);
		void vDrawRectangle(int x1, int y1, int x2, int y2, D3DCOLOR color);
		void vDrawRectangle(int x1, int y1, int x2, int y2, int width, D3DCOLOR color);
		void vDrawRectangleB(int x1, int y1, int x2, int y2, int width, D3DCOLOR color);
		void vDrawRectangleT(int x1, int y1, int x2, int y2, int width, D3DCOLOR color);
		void vDrawEllipse(int x, int y, int a, int b, D3DCOLOR color);
};
/////////////////////////////////////// Класс CPrimitiveGraphic ( End ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------





/////////////////////////////////////// Класс CGraphics ( begin ) //////////////////////////////
class CGraphics
{		
	public:
		LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
		LPDIRECT3DINDEXBUFFER9  m_pIndexBuffer;
		int						m_iSizeVertexBuffer;  // размер вершинного буфера в вершинах

		LPDIRECT3DDEVICE9	m_pD3DDevice;
		CBase				*m_pBase;
		CSprite				m_CSprite;
		CAnimation			m_CAnimation;
		CPrimitiveGraphic	m_primGraph;

	public:
		CGraphics();
		~CGraphics();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight, 
					CBase *base, CTexture *pCTexture);
		void vRelease();
		void vDrawRectChangeSize(grahics_h::stRectChangeSize rectProp); // рисует прямоугольник с не постояными размерами ( например выделение мышкой )
		//void vDrawImage();
		//void vLoadSprite(char *pcTextureName); // загружает спрайт в класс
};
/////////////////////////////////////// Класс CGraphics ( end ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------