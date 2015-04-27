#pragma once

#include "Resource.h"
#include "CBase.h"

// Error Message
#define TEXTURE_OK				0
#define TEXTURE_ERROR_LOAD		-1



class CTexture
{
	private:
		LPDIRECT3DDEVICE9		m_pD3DDevice;
		CBase	*m_pCBase;

		int m_iDefaultSizeTextureWidth;  // если не удалось загрузить текстуру 
		int m_iDefaultSizeTextureHeight; // то создаст свою с этими размерами

		void vCreateDefaultTexture(LPDIRECT3DTEXTURE9 *texture);
	public:
		CTexture();
		~CTexture();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, CBase *base);
		void vRelease();
		int iCreateTextureFromFile(char *name, LPDIRECT3DTEXTURE9 *texture);
		int iCreateTextureFromFileNoScale(char *name, LPDIRECT3DTEXTURE9 *texture);
		void vBrightnessTexture(LPDIRECT3DTEXTURE9 *texture, float fCoefBrignes);
};