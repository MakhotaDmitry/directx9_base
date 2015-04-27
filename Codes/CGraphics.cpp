#include "CGraphics.h"

/////////////////////////////////////// Класс CSprite ( begin ) //////////////////////////////
CSprite::CSprite()
{
	m_iQuantVertex = 3 * 1000;
	m_pstVertex = new resource_h::stVertexSprite[m_iQuantVertex];
}

CSprite::~CSprite()
{
	CSprite::vRelease();
}

void CSprite::vRelease()
{
	_SAFE_RELEASE_(m_pVertexBuffer);
	_SAFE_RELEASE_(m_pIndexBuffer);

	_SAFE_DELETE_(m_pstVertex);
}

void CSprite::vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight)
{
	m_pD3DDevice = pD3DDevice;
	m_uintBackBufferWidth = uintBackBufferWidth;
	m_uintBackBufferHeight = uintBackBufferHeight;

// 	grahics_h::stVertexSprite vertex[] = 
// 	{
// 		D3DXVECTOR3(0.0f,  0.0f, 0.0f),  D3DCOLOR_ARGB(255, 255,255,255), 0.0f, 0.0f,
// 		D3DXVECTOR3(0.0f,  1.0f, 0.0f),  D3DCOLOR_ARGB(255, 255,255,255), 0.0f, 1.0f,
// 		D3DXVECTOR3(1.0f,  1.0f, 0.0f),  D3DCOLOR_ARGB(255, 255,255,255), 1.0f, 1.0f,
// 		D3DXVECTOR3(1.0f,  0.0f, 0.0f),  D3DCOLOR_ARGB(255, 255,255,255), 1.0f, 0.0f,		
// 	};

	/*m_stVertex[0].vectorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_stVertex[0].color = D3DCOLOR_ARGB(255, 255,255,255); 
	m_stVertex[0].tu = 0.0f;
	m_stVertex[0].tv = 0.0f;
	m_stVertex[1].vectorPos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_stVertex[1].color = D3DCOLOR_ARGB(255, 255,255,255);
	m_stVertex[1].tu = 0.0f;
	m_stVertex[1].tv = 1.0f;
	m_stVertex[2].vectorPos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	m_stVertex[2].color = D3DCOLOR_ARGB(255, 255,255,255); 
	m_stVertex[2].tu = 1.0f;
	m_stVertex[2].tv = 1.0f;
	m_stVertex[3].vectorPos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_stVertex[3].color = D3DCOLOR_ARGB(255, 255,255,255); 
	m_stVertex[3].tu = 1.0f;
	m_stVertex[3].tv = 0.0f;*/

	if(FAILED(m_pD3DDevice->CreateVertexBuffer(
		m_iQuantVertex*sizeof(resource_h::stVertexSprite),0,
		D3DFVF_SPRITE, D3DPOOL_MANAGED,
		&m_pVertexBuffer, 0)))
	{
		MessageBox(0,"CSprite: Error create vertex buffer", "Fatal error", MB_OK);
		return;
	}

	/*void *pVB;
	m_pVertexBuffer->Lock(0,4*sizeof(grahics_h::stVertexSprite), &pVB, 0);
		memcpy(pVB, m_stVertex, 4*sizeof(grahics_h::stVertexSprite));
	m_pVertexBuffer->Unlock();*/


	//  ----------------------------- Шейдеры ------------------------------
	for(int i = 0; i < m_iQuantVertex; )
	{
		m_pstVertex[i + 0].vectorPos = D3DXVECTOR3(0,0,0);
		m_pstVertex[i + 1].vectorPos = D3DXVECTOR3(0,32,0);
		m_pstVertex[i + 2].vectorPos = D3DXVECTOR3(32,32,0);

		m_pstVertex[i + 3].vectorPos = D3DXVECTOR3(0,0,0);
		m_pstVertex[i + 4].vectorPos = D3DXVECTOR3(32,0,0);
		m_pstVertex[i + 5].vectorPos = D3DXVECTOR3(32,32,0);

		m_pstVertex[i + 0].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 1].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 2].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 3].color = D3DCOLOR_XRGB(255,0,0);
		m_pstVertex[i + 4].color = D3DCOLOR_XRGB(255,0,0);
		m_pstVertex[i + 5].color = D3DCOLOR_XRGB(255,0,0);

		i += 6;
	}

	void *pVB;
	m_pVertexBuffer->Lock(0,m_iQuantVertex * sizeof(resource_h::stVertexSprite), &pVB, D3DLOCK_DISCARD);
	memcpy(pVB, m_pstVertex, m_iQuantVertex * sizeof(resource_h::stVertexSprite));
	m_pVertexBuffer->Unlock();
}

void CSprite::vSetTexure(LPDIRECT3DTEXTURE9 *texture)
{
	m_pD3DDevice->SetTexture(0, *texture);
	
	// Заодно получим информацию о текстуре
	m_pTexture = texture;
	(*m_pTexture)->GetLevelDesc(0,&m_d3dsd_Texture);
}

void CSprite::vBegin()
{
		// MatrixWorld
	/*static D3DXMATRIX matTranMultScale;
	D3DXMatrixTranslation(&m_matTrans, rectPos.iPosX, rectPos.iPosY, 0.0f);
	D3DXMatrixScaling(&m_matScale, rectPos.iWidth, rectPos.iHeight, 1.0f);
	D3DXMatrixMultiply(&matTranMultScale, &m_matScale, &m_matTrans);

	m_directx->m_pD3DDevice->SetTransform(D3DTS_WORLD, &matTranMultScale);*/

	// MatrixView
	D3DXMatrixLookAtLH(&m_matView, &D3DXVECTOR3(0.0f, 0.0f, -1.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &m_matView);

	// MatrixProjection
	D3DXMatrixOrthoOffCenterLH ( &m_matProjection ,0.0f , 
									(float)m_uintBackBufferWidth, 
									(float)m_uintBackBufferHeight,
									0.0f, -10 , 1000.0f );
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
	//----------------------------------------------------------------------------------------

	/*void *pVB;
	m_pVertexBuffer->Lock(0,4*sizeof(stVertexSprite), &pVB, 0);
		memcpy(pVB, vertex, 4*sizeof(stVertexSprite));
	m_pVertexBuffer->Unlock();*/


	//----------------------------------------------------------------------------------------
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3DDevice->LightEnable(0,false);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(resource_h::stVertexSprite));
	m_pD3DDevice->SetFVF(D3DFVF_SPRITE);


	// Функции смешивания
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);     // Для источника.
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA); // Для цели.


	//m_directx->m_pD3DDevice->SetTexture(0, *texture);

	// Включаем альфаблендинг и выключаем запись в Z-буфер
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  true);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false); 

	// Устанавливаем стейты для отображения текстуры ...
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE ); 

	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );


	// Включаем сглаживание текстуры
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
}

void CSprite::vEnd()
{
	m_pD3DDevice->SetTexture(0, NULL);

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->LightEnable(0,false);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
void CSprite::vSetColor(D3DCOLOR color_lt, D3DCOLOR color_lb, D3DCOLOR color_rb, D3DCOLOR color_rt)
{
	/*m_stVertex[0].color = color_lt; 
	m_stVertex[1].color = color_lb;
	m_stVertex[2].color = color_rb; 
	m_stVertex[3].color = color_rt;*/ 
}
void CSprite::vSetColor(D3DCOLOR color)
{
	/*m_stVertex[0].color = color; 
	m_stVertex[1].color = color;
	m_stVertex[2].color = color; 
	m_stVertex[3].color = color;*/ 
}

void CSprite::vDrawUP(resource_h::stRECT *rectPos, resource_h::stRECT *rectCadr, float fAngle)
{
	resource_h::stVertexSprite stVertex[4];
	stVertex[0].vectorPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	stVertex[0].color = D3DCOLOR_ARGB(255, 255,255,255); 
	stVertex[0].tu = 0.0f;
	stVertex[0].tv = 0.0f;
	stVertex[1].vectorPos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	stVertex[1].color = D3DCOLOR_ARGB(255, 255,255,255);
	stVertex[1].tu = 0.0f;
	stVertex[1].tv = 1.0f;
	stVertex[2].vectorPos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	stVertex[2].color = D3DCOLOR_ARGB(255, 255,255,255); 
	stVertex[2].tu = 1.0f;
	stVertex[2].tv = 1.0f;
	stVertex[3].vectorPos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	stVertex[3].color = D3DCOLOR_ARGB(255, 255,255,255); 
	stVertex[3].tu = 1.0f;
	stVertex[3].tv = 0.0f;

	if(rectCadr != NULL)
	{
		//  ---------- Коректируем текстурные координаты по размерам rectCadr ----------
		m_fCadr_sx = (float)rectCadr->iWidth/(float)m_d3dsd_Texture.Width;
		m_fCadr_sy = (float)rectCadr->iHeight/(float)m_d3dsd_Texture.Height - 0.0001;
		m_fCadrPosX = (float)rectCadr->iPosX/(float)m_d3dsd_Texture.Width;
		m_fCadrPosY = (float)rectCadr->iPosY/(float)m_d3dsd_Texture.Height;

		stVertex[0].tu = m_fCadrPosX;
		stVertex[0].tv = m_fCadrPosY;
		stVertex[1].tu = m_fCadrPosX;
		stVertex[1].tv = m_fCadrPosY + m_fCadr_sy;
		stVertex[2].tu = m_fCadrPosX + m_fCadr_sx;
		stVertex[2].tv = m_fCadrPosY + m_fCadr_sy;
		stVertex[3].tu = m_fCadrPosX + m_fCadr_sx;
		stVertex[3].tv = m_fCadrPosY;
	}
	// -------------------------------------------------------------------------------


	// MatrixWorld
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matRot);


	D3DXMatrixScaling(&m_matScale, (float)rectPos->iWidth, (float)rectPos->iHeight, 1.0f);
	D3DXMatrixRotationZ(&m_matRot, -fAngle);

	D3DXMatrixTranslation(&m_matTrans, -0.5, -0.5, 0.0f);
	m_matWorld *= m_matTrans;
	m_matWorld *= m_matRot;
	D3DXMatrixTranslation(&m_matTrans, 0.5, 0.5, 0.0f);
	m_matWorld *= m_matTrans;
	m_matWorld *= m_matScale;
	D3DXMatrixTranslation(&m_matTrans, (float)rectPos->iPosX +0.5, (float)rectPos->iPosY, 0.0f);
	m_matWorld *= m_matTrans;
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);


	m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,stVertex,sizeof(resource_h::stVertexSprite));


	// Востанавливаем текстурные координаты по умолчанию
	/*m_stVertex[0].tu = 0.0f;
	m_stVertex[0].tv = 0.0f;
	m_stVertex[1].tu = 0.0f;
	m_stVertex[1].tv = 1.0f;
	m_stVertex[2].tu = 1.0f;
	m_stVertex[2].tv = 1.0f;
	m_stVertex[3].tu = 1.0f;
	m_stVertex[3].tv = 0.0f;*/
}

void CSprite::vDraw(resource_h::stRECT *rectPos, resource_h::stRECT *rectCadr, float fAngle)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matRot);

	D3DXMatrixRotationZ(&m_matRot, 30*D3DX_PI/180);
	D3DXMatrixTranslation(&m_matTrans, -0.5, -0.5, 0.0f);
	m_matWorld *= m_matTrans;
	m_matWorld *= m_matRot;
	D3DXMatrixTranslation(&m_matTrans, 0.5, 0.5, 0.0f);
	m_matWorld *= m_matTrans;
	//m_matWorld *= m_matScale;
	D3DXMatrixTranslation(&m_matTrans, 0 +0.5, 0, 0.0f);
	m_matWorld *= m_matTrans;
	
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	/*D3DXMatrixIdentity(&m_matTrans);
	D3DXMatrixTranslation(&m_matTrans, 0.0f, 0.0f, 0.0f);
	m_pCDirectx->m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matTrans);

	for(int i = 0; i < m_iQuantVertex; )
	{
		m_pstVertex[i + 0].vectorPos = D3DXVECTOR3(0,0,0);
		m_pstVertex[i + 1].vectorPos = D3DXVECTOR3(0,32,0);
		m_pstVertex[i + 2].vectorPos = D3DXVECTOR3(32,32,0);

		m_pstVertex[i + 3].vectorPos = D3DXVECTOR3(0,0,0);
		m_pstVertex[i + 4].vectorPos = D3DXVECTOR3(32,0,0);
		m_pstVertex[i + 5].vectorPos = D3DXVECTOR3(32,32,0);

		m_pstVertex[i + 0].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 1].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 2].color = D3DCOLOR_XRGB(255,0,255);
		m_pstVertex[i + 3].color = D3DCOLOR_XRGB(255,0,0);
		m_pstVertex[i + 4].color = D3DCOLOR_XRGB(255,0,0);
		m_pstVertex[i + 5].color = D3DCOLOR_XRGB(255,0,0);

		i += 6;
	}

	void *pVB;
	m_pVertexBuffer->Lock(0,m_iQuantVertex * sizeof(grahics_h::stVertexSprite), &pVB, 0);
		memcpy(pVB, m_pstVertex, m_iQuantVertex * sizeof(grahics_h::stVertexSprite));
	m_pVertexBuffer->Unlock();*/

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(resource_h::stVertexSprite));
	m_pD3DDevice->SetFVF(D3DFVF_SPRITE);
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST,0,m_iQuantVertex/3);
}
/////////////////////////////////////// Класс CSprite ( end ) ///////////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------





/////////////////////////////////////// Класс CAnimation ( Begin ) //////////////////////////////

CAnimation::CAnimation()
{
	m_iQuantTexture = 0;
	m_iQuantAnimation = 0;
}

CAnimation::~CAnimation()
{
	CAnimation::vRelease();
}

void CAnimation::vRelease()
{
	for(unsigned int i = 0; i < m_iQuantTexture; i++)
	{
		_SAFE_RELEASE_(m_vecTexture[i]);
	}
}

void CAnimation::vInit(LPDIRECT3DDEVICE9 pD3DDevice, CSprite *sprite, CBase *base, CTexture *pCTexture)
{
	m_pD3DDevice = pD3DDevice;
	m_pCSprite = sprite;
	m_pCBase = base;
	m_pCTexture = pCTexture;
}

//------------------------------------------------------
// Name: iLoadTexture
// Desc: возвращает индекс загруженной текстуры или код ошибки
//------------------------------------------------------
int CAnimation::iLoadTexture(char *nameTexture)
{
	LPDIRECT3DTEXTURE9 texture;
	m_pCTexture->iCreateTextureFromFileNoScale(nameTexture,&texture);

	m_vecTexture.push_back(texture);
	m_iQuantTexture++;

	return m_iQuantTexture - 1;
}

//------------------------------------------------------
// Name: iAddAnimation
// Desc: Добавить анимацию, возвращает индекс анимации
//------------------------------------------------------
int CAnimation::iAddAnimation(int iIndexTexture, int iRateInSec, resource_h::stRECT rectPos, resource_h::stSIZE sizeCadr)
{
	grahics_h::stAnimation animation;

	animation.iRateInSec = iRateInSec;
	animation.iDelayFrame = 1000/animation.iRateInSec;
	animation.iIndexTexture = iIndexTexture;
	animation.rectPos = rectPos;

	D3DSURFACE_DESC	d3dsd;
	LPDIRECT3DTEXTURE9(m_vecTexture[iIndexTexture])->GetLevelDesc(0,&d3dsd);
	
	int iQuantCadrWidth = d3dsd.Width/sizeCadr.cx;
	int iQuantCadrHeight = d3dsd.Height/sizeCadr.cy;
	animation.iQuantCadrs = iQuantCadrWidth * iQuantCadrHeight;
	animation.rectCadr = new resource_h::stRECT[animation.iQuantCadrs];

	for(int j = 0; j < iQuantCadrHeight; j++)
		for(int i = 0; i < iQuantCadrWidth; i++)
			animation.rectCadr[j*iQuantCadrWidth + i] = resource_h::stRECT(i*sizeCadr.cx,j*sizeCadr.cy,sizeCadr.cx,sizeCadr.cy);

	m_vecStAnimation.push_back(animation);
	m_iQuantAnimation ++;

	return m_iQuantAnimation;
}

//------------------------------------------------------
// Name: vSetAnimationPos
// Desc: Устанавливает позицию анимации
//------------------------------------------------------
void CAnimation::vSetAnimationPos(int iIndexAnim, resource_h::stRECT rectPos)
{
	m_vecStAnimation[iIndexAnim].rectPos = rectPos;
}

//------------------------------------------------------
// Name: vSetTexture
// Desc: устанавливает текстуру для вывода
//		 без проверки выхода за пределы массива
//------------------------------------------------------
void CAnimation::vSetTexture(int iIndexTexture)
{
	m_pCSprite->vSetTexure(&m_vecTexture[iIndexTexture]);
}

//------------------------------------------------------
// Name: vCalculate
// Desc: Перерасчёт анимации
//------------------------------------------------------
void CAnimation::vCalculate()
{
	for(unsigned int i = 0; i < m_iQuantAnimation; i++)
		if(m_vecStAnimation[i].bAnimEnable)		
			if(m_pCBase->m_dwTickCount - m_vecStAnimation[i].dwLastUpdate >= (DWORD)m_vecStAnimation[i].iDelayFrame)
			{
				// теперь будет пропускать кадры которые должны были отрисоваться но не хватило времени
				m_vecStAnimation[i].iNumCurCadr += (m_pCBase->m_dwTickCount - m_vecStAnimation[i].dwLastUpdate)/(DWORD)m_vecStAnimation[i].iDelayFrame;
				m_vecStAnimation[i].dwLastUpdate = m_pCBase->m_dwTickCount;

				if(m_vecStAnimation[i].iNumCurCadr >= m_vecStAnimation[i].iQuantCadrs)
					m_vecStAnimation[i].iNumCurCadr = 0;
			}
}

//------------------------------------------------------
// Name: vDraw
// Desc: вывод на экран анимации по индексу без проверки
//		 выхода за границы массива
//------------------------------------------------------
void CAnimation::vDraw(int iIndexAnim)
{
	m_pCSprite->vDrawUP(&m_vecStAnimation[iIndexAnim].rectPos, 
					  &m_vecStAnimation[iIndexAnim].rectCadr[m_vecStAnimation[iIndexAnim].iNumCurCadr], 
					   m_vecStAnimation[iIndexAnim].fAngle);
}
/////////////////////////////////////// Класс CAnimation ( End ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------





/////////////////////////////////////// Класс CPrimitiveGraphic ( Begin ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------
CPrimitiveGraphic::CPrimitiveGraphic(){}
CPrimitiveGraphic::~CPrimitiveGraphic(){ CPrimitiveGraphic::vRelease(); }

void CPrimitiveGraphic::vInit(LPDIRECT3DDEVICE9 pD3DDevice)
{
	m_pD3DDevice = pD3DDevice;
}

void CPrimitiveGraphic::vRelease()
{
}

void CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE pos, D3DCOLOR color)
{
	LPDIRECT3DSURFACE9 backsurf;
	m_pD3DDevice->GetRenderTarget(0,&backsurf);

	RECT rect;
	rect.left = pos.cx;
	rect.top = pos.cy;
	rect.right = pos.cx + 1;
	rect.bottom = pos.cy + 1;
	m_pD3DDevice->ColorFill(backsurf,&rect,color);
}

void CPrimitiveGraphic::vDrawCircle(int x0, int y0, int radius, D3DCOLOR color)
{
	int x = 0;
	int y = radius;
	int delta = 2 - 2 * radius;
	int error = 0;
	while(y >= 0) 
	{
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x0 + x, y0 + y),color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x0 + x, y0 - y),color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x0 - x, y0 + y),color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x0 - x, y0 - y),color);
		error = 2 * (delta + y) - 1;
		if(delta < 0 && error <= 0) 
		{
			++x;
			delta += 2 * x + 1;
			continue;
		}
		error = 2 * (delta - x) - 1;
		if(delta > 0 && error > 0) 
		{
			--y;
			delta += 1 - 2 * y;
			continue;
		}
		++x;
		delta += 2 * (x - y);
		--y;
	}
}

void CPrimitiveGraphic::vDrawLine(int x1, int y1, int x2, int y2, D3DCOLOR color)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x2, y2), color);
	while(x1 != x2 || y1 != y2) 
	{
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x1, y1), color);
		const int error2 = error * 2;
		//
		if(error2 > -deltaY) 
		{
			error -= deltaY;
			x1 += signX;
		}
		if(error2 < deltaX) 
		{
			error += deltaX;
			y1 += signY;
		}
	}
}

void CPrimitiveGraphic::vDrawLine(int x1, int y1, int x2, int y2, int width, D3DCOLOR color)
{
	const int deltaX = abs(x2 - x1);
	const int deltaY = abs(y2 - y1);
	const int signX = x1 < x2 ? 1 : -1;
	const int signY = y1 < y2 ? 1 : -1;
	//
	int error = deltaX - deltaY;
	//
	for(int i = 0; i < (float)width/2.0f; i++)
		for (int j = 0; j < (float)width/2.0f; j++)
		{
			CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x2 + i - (float)width/2.0f, y2 + j - (float)width/2.0f), color);
		}

		while(x1 != x2 || y1 != y2) 
		{
			for(int i = 0; i < (float)width/2.0f; i++)
				for (int j = 0; j < (float)width/2.0f; j++)
				{
					CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x1 + i - (float)width/2.0f, y1 + j - (float)width/2.0f), color);
				}

				const int error2 = error * 2;
				//
				if(error2 > -deltaY) 
				{
					error -= deltaY;
					x1 += signX;
				}
				if(error2 < deltaX) 
				{
					error += deltaX;
					y1 += signY;
				}
		}
}

void CPrimitiveGraphic::vDrawRectangle(int x1, int y1, int x2, int y2, D3DCOLOR color)
{
	// рисует прямоугольник

	CPrimitiveGraphic::vDrawLine(x1,y1,x2,y1, color); // top
	CPrimitiveGraphic::vDrawLine(x1,y2,x2,y2, color); // bottom
	CPrimitiveGraphic::vDrawLine(x1,y1,x1,y2, color); // left
	CPrimitiveGraphic::vDrawLine(x2,y1,x2,y2, color); // right
}

void CPrimitiveGraphic::vDrawRectangle(int x1, int y1, int x2, int y2, int width, D3DCOLOR color)
{
	// рисует прямоугольник с шириной рамки относительно середины

	for(int i = 0; i <= (float)width/2.0f; i++)
	{
		CPrimitiveGraphic::vDrawRectangle(x1 - i, y1 - i, x2 + i, y2 + i, color);
		CPrimitiveGraphic::vDrawRectangle(x1 + i, y1 + i, x2 - i, y2 - i, color);
	}
}

void CPrimitiveGraphic::vDrawRectangleB(int x1, int y1, int x2, int y2, int width, D3DCOLOR color)
{
	// рисует прямоугольник с рамкой внутрь

	int wx = abs(x1 - x2);
	int wy = abs(y1 - y2);

	if(width > min(wx,wy))   // проверяем чтобы ширина рамки была меньше размеров прямоугольника
		width = min(wx,wy);

	for(int i = 0; i < width; i++)
		CPrimitiveGraphic::vDrawRectangle(x1 + i, y1 + i, x2 - i, y2 - i, color);
}

void CPrimitiveGraphic::vDrawRectangleT(int x1, int y1, int x2, int y2, int width, D3DCOLOR color)
{
	// рисует прямоугольник с рамкой внутрь

	for(int i = 0; i < width; i++)
		CPrimitiveGraphic::vDrawRectangle(x1 - i, y1 - i, x2 + i, y2 + i, color);
}

void CPrimitiveGraphic::vDrawEllipse(int x, int y, int a, int b, D3DCOLOR color)
{
	//int col,i,row,bnew;
	int col,row;
	long a_square,b_square,two_a_square,two_b_square,four_a_square,four_b_square,d;

	b_square=b*b;
	a_square=a*a;
	row=b;
	col=0;
	two_a_square=a_square<<1;
	four_a_square=a_square<<2;
	four_b_square=b_square<<2;
	two_b_square=b_square<<1;
	d=two_a_square*((row-1)*(row))+a_square+two_b_square*(1-a_square);
	while(a_square*(row)>b_square*(col))
	{
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(col+x,row+y), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(col+x,y-row), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x-col,row+y), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x-col,y-row), color);
		if (d>=0)
		{
			row--;
			d-=four_a_square*(row);
		}
		d+=two_b_square*(3+(col<<1));
		col++;
	}
	d=two_b_square*(col+1)*col+two_a_square*(row*(row-2)+1)+(1-two_a_square)*b_square;
	while ((row) + 1)
	{
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(col+x,row+y), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(col+x,y-row), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x-col,row+y), color);
		CPrimitiveGraphic::vDrawPoint(resource_h::stSIZE(x-col,y-row), color);
		if (d<=0)
		{
			col++;
			d+=four_b_square*col;
		}
		row--;
		d+=two_a_square*(3-(row <<1));
	}
}
/////////////////////////////////////// Класс CPrimitiveGraphic ( end ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------




/////////////////////////////////////// Класс CGraphics ( begin ) //////////////////////////////
CGraphics::CGraphics()
{}

CGraphics::~CGraphics()
{}

void CGraphics::vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight, 
					  CBase *base, CTexture *pCTexture)
{
	m_pD3DDevice = pD3DDevice;
	m_pBase = base;
	m_CSprite.vInit(pD3DDevice, uintBackBufferWidth, uintBackBufferHeight);
	m_CAnimation.vInit(pD3DDevice, &m_CSprite, base, pCTexture);
	m_primGraph.vInit(pD3DDevice);

	m_pBase->m_CLog.vAddString("Class CGraphics = Init");
}

void CGraphics::vRelease()
{
	m_CSprite.vRelease();
	m_CAnimation.vRelease();
	m_primGraph.vRelease();
	m_pBase->m_CLog.vAddString("Class CGraphics = Release");
}

void CGraphics::vDrawRectChangeSize(grahics_h::stRectChangeSize rectProp)
{
}
/////////////////////////////////////// Класс CGraphics ( end ) //////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------