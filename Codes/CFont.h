#pragma  once

#include "Resource.h"
#include "CBase.h"
#include "CGraphics.h"


namespace font_h
{
	struct stFont
	{
		char cNameFont[20];
		LPDIRECT3DTEXTURE9 texture;
		resource_h::stRECT rectLetter[224];  // size letter in pixel
		resource_h::stSIZE sizeInTexLetter[224];  // size letter in texture coord
	};
}

class CFont
{
	private:
		LPDIRECT3DDEVICE9	m_pD3DDevice;
		CGraphics	*m_pCGraphics;
		CBase		*m_pCBase;

		std::vector<font_h::stFont> m_vecFont;
		int m_iQuantFonts; // количество шрифтов
	public:
		CFont();
		~CFont();

		void vInit(LPDIRECT3DDEVICE9 pD3DDevice, CBase *base, CGraphics *graphics);
		void vRelease();
		int  iAddFont(char *nameFont);

		//void vPrint(); // Дополняет в вершинный буффер текст
		void vDrawText(int iIndexFont, char *text, DWORD dwColor, resource_h::stRECT pos); // выводит ранее заполненый текст
};