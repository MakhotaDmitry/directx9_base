#include "CTexture.h"

CTexture::CTexture()
{
	m_iDefaultSizeTextureWidth = 32;
	m_iDefaultSizeTextureHeight = 32;
}

CTexture::~CTexture()
{
	//CTexture::vRelease();
}

void CTexture::vInit(LPDIRECT3DDEVICE9 pD3DDevice, CBase *base)
{
	m_pD3DDevice = pD3DDevice;
	m_pCBase = base;

	m_pCBase->m_CLog.vAddString("Class CTexture = Init");
}

void CTexture::vRelease()
{
	m_pCBase->m_CLog.vAddString("Class CTexture = Release");
}

int CTexture::iCreateTextureFromFile(char *name, LPDIRECT3DTEXTURE9 *texture)
{
	if(FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, name, texture)))
	{
		// Не удалось загрузить текстуру значит создадим свою
		vCreateDefaultTexture(texture);

		return TEXTURE_ERROR_LOAD;
	}

	return TEXTURE_OK;
}

int CTexture::iCreateTextureFromFileNoScale(char *name, LPDIRECT3DTEXTURE9 *texture)
{
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile (name, &info);

	if(FAILED(D3DXCreateTextureFromFileEx(m_pD3DDevice, name, info.Width,info.Height,
				0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
				D3DX_DEFAULT, D3DX_DEFAULT, 0,0, // or NULL
				NULL,
				texture)))
	{
		// Не удалось загрузить текстуру значит создадим свою
		vCreateDefaultTexture(texture);

		return TEXTURE_ERROR_LOAD;
	}

	return TEXTURE_OK;
}

//------------------------------------------------------------------------------------
// Name: vCreateDefaultTexture()
// Desc: Создаёт текстуру по-умолчанию (например в случае ошибки загрузки нужной текстуры)
//------------------------------------------------------------------------------------
void CTexture::vCreateDefaultTexture(LPDIRECT3DTEXTURE9 *texture)
{
	D3DXCreateTexture(m_pD3DDevice,
						m_iDefaultSizeTextureWidth,
						m_iDefaultSizeTextureHeight,
						0,0,
						D3DFMT_X8R8G8B8,D3DPOOL_MANAGED,
						texture);
	// D3DFMT_R5G6B5
	// D3DFMT_X8R8G8B8

	D3DLOCKED_RECT	d3dlr;
	(*texture)->LockRect(0,&d3dlr,0,0);
	DWORD *colorCur = (DWORD*)d3dlr.pBits;

	char t[20];
	sprintf(t,"Pitch = %d",d3dlr.Pitch);
	m_pCBase->m_CLog.vAddString(t);

	DWORD color;
	for(DWORD str = 0; str < m_iDefaultSizeTextureHeight; str++)
		for(DWORD stl = 0; stl < m_iDefaultSizeTextureWidth; stl++)
			colorCur[str*d3dlr.Pitch/4 + stl] = D3DCOLOR_XRGB(0,0,255);
	(*texture)->UnlockRect(0);
}

//------------------------------------------------------------------------------------
// Name: vBrightnessTexture()
// Desc: Перезаполняет текстуру с заданым коеффициентом насыщения
//------------------------------------------------------------------------------------
void CTexture::vBrightnessTexture(LPDIRECT3DTEXTURE9 *texture, float fCoefBrignes)
{
	// Работает только для  24,32-х  битных текстур

	D3DSURFACE_DESC desc;
	(*texture)->GetLevelDesc(0,&desc);

	D3DLOCKED_RECT	d3dlr;
	(*texture)->LockRect(0,&d3dlr,0,0);
	DWORD *colorCur = (DWORD*)d3dlr.pBits;
	DWORD color;
	int A,R,G,B;
	int r,g,b;
	int a;


	for(DWORD str = 0; str < desc.Height; str++)
		for(DWORD stl = 0; stl < desc.Width; stl++)
		{
			// 0.30*getR(imageData[index]) + 0.59*getG(imageData[index]) + 0.11*getB(imageData[index])
			color = colorCur[str*d3dlr.Pitch/4 + stl];

			a = fCoefBrignes * 255;

			A = getColorA_32(color);
			R = getColorR_32(color);
			G = getColorG_32(color);
			B = getColorB_32(color);

			int coef = (R*54 + G*182 + B*18)/255;

			r = (coef*(255 - a) + R*a)/255;
			g = (coef*(255 - a) + G*a)/255;
			b = (coef*(255 - a) + B*a)/255;

			colorCur[str*d3dlr.Pitch/4 + stl] = D3DCOLOR_ARGB(A,r,g,b);
		}
	(*texture)->UnlockRect(0);
}
