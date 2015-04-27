#include "CGraphics2D.h"

CGraphics2D::CGraphics2D()
{
	//m_iSizeVertexBufferInVertex = 10000;
	m_iSizeVertexBufferInVertex = 8388607;
	m_iSizeIndexBufferInNumber = 40000;
}

CGraphics2D::~CGraphics2D()
{}

void CGraphics2D::vInit(LPDIRECT3DDEVICE9 pD3DDevice, UINT uintBackBufferWidth, UINT uintBackBufferHeight, CBase *base)
{
	m_pD3DDevice = pD3DDevice;
	m_uintBackBufferWidth = uintBackBufferWidth;
	m_uintBackBufferHeight = uintBackBufferHeight;
	m_pCBase	=  base;


	// --------------- Index Buffer -------------
	if(FAILED(m_pD3DDevice->CreateIndexBuffer(
								m_iSizeIndexBufferInNumber*sizeof(int),
								0, D3DFMT_INDEX32, D3DPOOL_MANAGED, 
								&m_pIndexBuffer,0)))
	{
		MessageBox(0,"CGraphics2D: Error create index buffer","Fatal error",MB_OK);
		return;
	}


	// --------------- Vertex Buffer -------------
	if(FAILED(m_pD3DDevice->CreateVertexBuffer(
								m_iSizeVertexBufferInVertex * sizeof(resource_h::stVertexSprite),
								D3DUSAGE_WRITEONLY,
								D3DFVF_SPRITE, D3DPOOL_MANAGED,
								&m_pVertexBuffer, 0)))
	{
		MessageBox(0,"CGraphics2D: Error create vertex buffer","Fatal error",MB_OK);
		return;
	}


	m_pCBase->m_CLog.vAddString("Class CGraphics2D = Init");




	const unsigned int iQuantWidth = 640;
	const unsigned int iQuantHeight = 480;
	const unsigned int iQuantVert = iQuantWidth * iQuantHeight;

	resource_h::stVertexSprite *vertexArr = new resource_h::stVertexSprite[iQuantVert];
	for(int j = 0; j < iQuantHeight; j++)
		for(int i = 0; i < iQuantWidth; i++)
		{
			vertexArr[j*iQuantWidth + i].color = D3DCOLOR_XRGB(255,0,0);
			vertexArr[j*iQuantWidth + i].vectorPos = D3DXVECTOR3(i,j,0);
		}

		void *pVB;
		m_pVertexBuffer->Lock(0,iQuantVert * sizeof(resource_h::stVertexSprite), &pVB, 0);
			memcpy(pVB, vertexArr, iQuantVert * sizeof(resource_h::stVertexSprite));
		m_pVertexBuffer->Unlock();

	delete[] vertexArr;
}

void CGraphics2D::vRelease()
{
	_SAFE_RELEASE_(m_pVertexBuffer);
	_SAFE_RELEASE_(m_pIndexBuffer);

	m_pCBase->m_CLog.vAddString("Class CGraphics2D = Release");
}

void CGraphics2D::vBegin()
{
	// MatrixWorld
	D3DXMatrixIdentity(&m_matWorld);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
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


	//----------------------------------------------------------------------------------------
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pD3DDevice->LightEnable(0,false);
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->SetFVF(D3DFVF_SPRITE);
	m_pD3DDevice->SetStreamSource(0,m_pVertexBuffer, 0, sizeof(resource_h::stVertexSprite));
	m_pD3DDevice->SetIndices(m_pIndexBuffer);


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

void CGraphics2D::vEnd()
{
	m_pD3DDevice->SetTexture(0, NULL);

	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pD3DDevice->LightEnable(0,false);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CGraphics2D::vDrawLine(int x1, int y1, int x2, int y2)
{
	int index[10] = {0,1};

	resource_h::stVertexSprite vertexArr[2];
	vertexArr[0].color = D3DCOLOR_XRGB(255,0,0);
	vertexArr[0].vectorPos = D3DXVECTOR3(x1, y1, 0.0f);
	vertexArr[1].color = D3DCOLOR_XRGB(255,0,0);
	vertexArr[1].vectorPos = D3DXVECTOR3(x2, y2, 0.0f);

	void *pBI;
	m_pIndexBuffer->Lock(0,2 * sizeof(int),&pBI,0);
		memcpy(pBI, index, 2*sizeof(int));
	m_pIndexBuffer->Unlock();

	void *pVB;
	m_pVertexBuffer->Lock(0,2 * sizeof(resource_h::stVertexSprite), &pVB, 0);
		memcpy(pVB, vertexArr, 2 * sizeof(resource_h::stVertexSprite));
	m_pVertexBuffer->Unlock();

	m_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	m_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, m_pCBase->dwFloat_To_DWORD(5.0f));
	//m_pCDirectx->m_pD3DDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, true);
	//m_pCDirectx->m_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(3.2f));
	//m_pCDirectx->m_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(5.0f));

	m_pD3DDevice->DrawIndexedPrimitive(D3DPT_POINTLIST,0,0,2,0,2);
}

void CGraphics2D::vDrawPoint(int x, int y, int z, D3DCOLOR color)
{
	const int iQuantVert = 1;

	resource_h::stVertexSprite vertexArr[iQuantVert];
	vertexArr[0].color = color;
	vertexArr[0].vectorPos = D3DXVECTOR3(x,y,z);

	void *pVB;
	m_pVertexBuffer->Lock(0,iQuantVert * sizeof(resource_h::stVertexSprite), &pVB, 0);
	memcpy(pVB, vertexArr, iQuantVert * sizeof(resource_h::stVertexSprite));
	m_pVertexBuffer->Unlock();

	m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,1);
}

void CGraphics2D::vDrawPointTest()
{
	//m_pCDirectx->m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,307200);
	m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,100000);
	m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,100000);
	m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,100000);
	m_pD3DDevice->DrawPrimitive(D3DPT_POINTLIST,0,100000);
}