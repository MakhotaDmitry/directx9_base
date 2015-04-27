#include "CFont.h"

CFont::CFont()
{
	m_iQuantFonts = 0;
}

CFont::~CFont()
{
	//CFont::vRelease();
}

void CFont::vInit(LPDIRECT3DDEVICE9 pD3DDevice, CBase *base, CGraphics *graphics)
{
	m_pD3DDevice = pD3DDevice;
	m_pCGraphics = graphics;
	m_pCBase = base;
}

void CFont::vRelease()
{
	m_pCBase->m_CLog.vAddString("Class CFont = Release");
}

//------------------------------------------------------------------------------------
// Name: iAddFont()
// Desc: Добавляет шрифт. Возвращает индекс созданого шрифта или "<0" в случае ошибки
//------------------------------------------------------------------------------------
int CFont::iAddFont(char* nameFont)
{
	font_h::stFont font;

	int iQuantSymName = strlen(nameFont); // количество символов в имени файла
	char *nameFile = new char[iQuantSymName + 10];
		sprintf(nameFile,"%s",nameFont);
	iQuantSymName = strlen(nameFile);

		nameFile[iQuantSymName - 3] = 'p';
		nameFile[iQuantSymName - 2] = 'n';
		nameFile[iQuantSymName - 1] = 'g';

	if(FAILED(D3DXCreateTextureFromFile(m_pD3DDevice, nameFile, &font.texture)))
		return -1;


	// заполняем rect - данными о каждой букве
	nameFile[iQuantSymName - 3] = 't';
	nameFile[iQuantSymName - 2] = 'x';
	nameFile[iQuantSymName - 1] = 't';
	
	FILE *file;
	file = fopen(nameFile,"r");
	delete[] nameFile;
	if(file == NULL)
		return -1;
	
	// чтение данных
	char *str = new char[100];
	int iHeightLetter;

	// ; Font params:
	fgets(str,50,file);
	fgets(str,50,file);
		sscanf(str,"Font=%s", &font.cNameFont);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);
		sscanf(str,"Height=%d",&iHeightLetter);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);

	// ; Texture params:
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);
	fgets(str,50,file);

	// ; Simbol params (x,y,width):
	fgets(str,50,file);
	for(int i =0; i < 224; i++)
	{
		fgets(str,50,file); 
		char sym;
		sscanf(str,"%c=%d,%d,%d", &sym, &font.rectLetter[i].iPosX, 
										&font.rectLetter[i].iPosY, 
										&font.rectLetter[i].iWidth);
		font.rectLetter[i].iHeight = iHeightLetter;
	}
	fclose(file);
	delete[] str;


	m_vecFont.push_back(font);
	m_iQuantFonts++;
	return m_iQuantFonts - 1;
}

void CFont::vDrawText(int iIndexFont, char *text, DWORD dwColor, resource_h::stRECT pos)
{
	m_pCGraphics->m_CSprite.vSetTexure(&m_vecFont[iIndexFont].texture);
	m_pCGraphics->m_CSprite.vSetColor(dwColor);

	for(int iter = 0, index = 0; iter < strlen(text); iter++)
	{
		index = (int)text[iter];
		if(index < 0)
			index = 256 + index - 32;
		else
			index -= 32;

		pos.iWidth = m_vecFont[iIndexFont].rectLetter[index].iWidth;
		pos.iHeight = m_vecFont[iIndexFont].rectLetter[index].iHeight;

		m_pCGraphics->m_CSprite.vDrawUP(&pos,
										&m_vecFont[iIndexFont].rectLetter[index],0);

		pos.iPosX += m_vecFont[iIndexFont].rectLetter[index].iWidth;
	}

	m_pCGraphics->m_CSprite.vSetColor(D3DCOLOR_XRGB(255,255,255));
}