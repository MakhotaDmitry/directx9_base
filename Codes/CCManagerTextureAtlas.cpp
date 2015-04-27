#include "CCManagerTextureAtlas.h"



////////////////////////////// Класс CCTextureAltas ( begin ) ///////////////////////////////
CCTextureAltas::CCTextureAltas(LPDIRECT3DDEVICE9 lpD3DDevice)
{
	m_lpD3DDevice = lpD3DDevice;


	m_uintQuantFreePixel = 0;
	m_dwColorClear = D3DCOLOR_XRGB(255,0,0);


	// Менять эти 2 значения только здесь
	m_uintDefaultTextureAtlasWidth = 512;
	m_uintDefaultTextureAtlasHeight = 512;

	m_uintTextureRecomendWidth = m_uintDefaultTextureAtlasWidth;
	m_uintTextureRecomendHeight = m_uintDefaultTextureAtlasHeight;
	m_bReleased = true;
}
CCTextureAltas::~CCTextureAltas()
{
}

//---------------------------------------------------------------------------------------
// Name: iAddTexture()
// Desc: Не контролирует корректность передаваемой сюда текстуры
//----------------------------------------------------------------------------------------
int CCTextureAltas::iAddTexture(LPDIRECT3DTEXTURE9 *plpTexture)
{
	D3DSURFACE_DESC	d3dsd;
	(*plpTexture)->GetLevelDesc(0,&d3dsd);


	// Если это первый вызов то проконтролировать размеры передаваемой текстуры
	// и атласной текстуры и в случае необходимости увеличить размер атласной текстуры
	if(m_bReleased)
	{
		m_bReleased = false;

		
		/*	
			если нужно увеличить атласную текстуру
			приводим нужные размеры к степени 2 
		*/
		UINT uintMaxPower2 = 20; // максимальная степень в которую можно возвести 2
		if(d3dsd.Width > m_uintTextureRecomendWidth) 
		{
			UINT uintPower = 0;
			for(; (uintPower <= uintMaxPower2) && 
					(d3dsd.Width > (UINT)pow(2.0f, (float)uintPower)); uintPower++) {}
			
			m_uintTextureRecomendWidth = (UINT)pow(2.0f, (float)uintPower);
		}
		if(d3dsd.Height > m_uintTextureRecomendHeight) 
		{
			UINT uintPower = 0;
			for(; (uintPower <= uintMaxPower2) && 
					(d3dsd.Height > (UINT)pow(2.0f, (float)uintPower)); uintPower++) {}

			m_uintTextureRecomendHeight = (UINT)pow(2.0f, (float)uintPower);
		}
		

		m_lpD3DDevice->CreateTexture(m_uintTextureRecomendWidth,m_uintTextureRecomendHeight,
									0,0,D3DFMT_A8R8G8B8,
									D3DPOOL_MANAGED,&m_lpTexture,0);
		
		// ---------------------- Заливаем атласную текстуру цветом ----------------------
		D3DLOCKED_RECT	d3dlr;
		m_lpTexture->LockRect(0,&d3dlr,0,0);
		DWORD *colorCur = (DWORD*)d3dlr.pBits;

		for(UINT str = 0; str < m_uintTextureRecomendHeight; str++)
			for(UINT stb = 0; stb < m_uintTextureRecomendWidth; stb++)
				colorCur[str*d3dlr.Pitch/4 + stb] = m_dwColorClear;
		m_lpTexture->UnlockRect(0);
		//---------------------------------------------------------------------------------


		// объявляем что все пиксели атласной текстуры свободны
		m_uintQuantFreePixel = m_uintTextureRecomendWidth * m_uintTextureRecomendHeight;
		m_pbFreeMap = new bool[m_uintQuantFreePixel];
		for(UINT str = 0; str < m_uintTextureRecomendHeight; str++)
			for(UINT stb = 0; stb < m_uintTextureRecomendWidth; stb++)
				m_pbFreeMap[str*m_uintTextureRecomendWidth + stb] = true;
	}

	// если размеры текстуры больше свободного места тогда поиск не производим
	if( ((d3dsd.Width * d3dsd.Height) > m_uintQuantFreePixel) ||
			(d3dsd.Width > m_uintTextureRecomendWidth) ||
			(d3dsd.Height > m_uintTextureRecomendHeight) )
		return -1;



	// ------------ Ищем свободное место для текстуры и при нахождении заполняем ---------
	D3DLOCKED_RECT	d3dlrDest, d3dlrSrc;
	m_lpTexture->LockRect(0,&d3dlrDest,0,0);
	(*plpTexture)->LockRect(0,&d3dlrSrc,0,0);
	DWORD *colorDest = (DWORD*)d3dlrDest.pBits;
	DWORD *colorSrc = (DWORD*)d3dlrSrc.pBits;


	bool bExitLoop = false;

	for(UINT str = 0; str < m_uintTextureRecomendHeight; str++)
		for(UINT stb = 0; stb < m_uintTextureRecomendWidth; stb++)
		{
			// Если тексель атласной текстуры свободен 
			// и от него хватает места для текстуры то произвожу проверку области
			if(m_pbFreeMap[str*m_uintTextureRecomendWidth + stb])
			if(stb + d3dsd.Width <= m_uintTextureRecomendWidth)
			if(str + d3dsd.Height <= m_uintTextureRecomendHeight)
			{
				bExitLoop = false;
				for(UINT str_Sub_1 = str; (str_Sub_1 < str + d3dsd.Height) && (!bExitLoop); str_Sub_1++)
					for(UINT stb_Sub_1 = stb; (stb_Sub_1 < stb + d3dsd.Width) && (!bExitLoop); stb_Sub_1++)
					{
						// Если в области найден занятый тексель то область не подходит прекращаем поиск
						if(!m_pbFreeMap[str_Sub_1*m_uintTextureRecomendWidth + stb_Sub_1]) 
						{
							bExitLoop = true;							
							stb = stb_Sub_1; // не обязательно, просто пропукает несколько итераций
							break;
						}
					}

					// Если не было преждевременного выхода из проверки значит вся область 
					// подходит для копирования текстуры, копируем и выходим из функции,
					// перед этим разблокировав текстуры
					// Обозначаем область как занятую
					if(!bExitLoop)
					{
						for(UINT strDest = str, strSrc = 0; strDest < str + d3dsd.Height; strDest++, strSrc++)
							for(UINT stbDest = stb, stbSrc = 0; stbDest < stb + d3dsd.Width; stbDest++, stbSrc++)
							{
								colorDest[strDest*d3dlrDest.Pitch/4 + stbDest] = colorSrc[strSrc*d3dlrSrc.Pitch/4 + stbSrc];
								m_pbFreeMap[strDest*m_uintTextureRecomendWidth + stbDest] = false;
								m_uintQuantFreePixel--;
							}

						(*plpTexture)->UnlockRect(0);
						m_lpTexture->UnlockRect(0);


						// ищем куда можно пристроить данные текстуры и добавляем
						int iIndexTexture;
						if(m_stackFreeIndexTexture.size() > 0)
						{
							iIndexTexture = m_stackFreeIndexTexture.top();
							m_stackFreeIndexTexture.pop();
						}
						else
						{
							CCManagerTextureAtlas_h::stTexture stTexture;
							m_vecTexture.push_back(stTexture);

							iIndexTexture = m_vecTexture.size() - 1;
						}
						m_vecTexture[iIndexTexture].fCoordInTextureAtlas_BegPosX = (float)stb/(float)m_uintTextureRecomendWidth;
						m_vecTexture[iIndexTexture].fCoordInTextureAtlas_BegPosY = (float)str/(float)m_uintTextureRecomendHeight;
						m_vecTexture[iIndexTexture].fCoordInTextureAtlas_EndPosX = (float)(stb + d3dsd.Width)/(float)m_uintTextureRecomendWidth;
						m_vecTexture[iIndexTexture].fCoordInTextureAtlas_EndPosY = (float)(str + d3dsd.Height)/(float)m_uintTextureRecomendHeight;

						m_vecTexture[iIndexTexture].uintCoordInTextureAtlas_BegPosX = stb;
						m_vecTexture[iIndexTexture].uintCoordInTextureAtlas_BegPosY = str;
						m_vecTexture[iIndexTexture].uintCoordInTextureAtlas_EndPosX = stb + d3dsd.Width - 1;
						m_vecTexture[iIndexTexture].uintCoordInTextureAtlas_EndPosY = str + d3dsd.Height - 1;

						return iIndexTexture;
					}
			}
		}
	(*plpTexture)->UnlockRect(0);
	m_lpTexture->UnlockRect(0);
	// ------------------------------------------------------------------------------------

	// Место для текстуры не найдено
	return -1;
}

//---------------------------------------------------------------------------------------
// Name: iDeleteTexture()
// Desc: Удаляет текстуру из атласа - если атлас станет пустым он удалит свои данные
//----------------------------------------------------------------------------------------
int CCTextureAltas::iDeleteTexture(int *piIndexTexture)
{
	if((*piIndexTexture) >= 0) // на всякий случай
	{
		m_stackFreeIndexTexture.push(*piIndexTexture);

		D3DLOCKED_RECT	d3dlrDest;
		m_lpTexture->LockRect(0,&d3dlrDest,0,0);
		DWORD *colorDest = (DWORD*)d3dlrDest.pBits;

		for(UINT str = m_vecTexture[*piIndexTexture].uintCoordInTextureAtlas_BegPosY; str <= m_vecTexture[*piIndexTexture].uintCoordInTextureAtlas_EndPosY; str++)
			for(UINT stb = m_vecTexture[*piIndexTexture].uintCoordInTextureAtlas_BegPosX; stb <= m_vecTexture[*piIndexTexture].uintCoordInTextureAtlas_EndPosX; stb++)
			{
				m_pbFreeMap[str*m_uintTextureRecomendWidth + stb] = true;
				m_uintQuantFreePixel++;
			
				colorDest[str*d3dlrDest.Pitch/4 + stb] = m_dwColorClear;
			}
		m_lpTexture->UnlockRect(0);
		
		*piIndexTexture = -1;


		// Если после удаления атласная текстура станет свободной, освободить внутренние данные
		if(m_uintQuantFreePixel >= m_uintTextureRecomendWidth * m_uintTextureRecomendHeight)
		{
			vRelease();
			return TEXTURE_ATLAS_CLASS_ENABLE_RELEASE;
		}
	}

	return MANAGER_TEXTURE_ATLAS_SUCCESS;
}

//---------------------------------------------------------------------------------------
// Name: vRelease()
// Desc:
//----------------------------------------------------------------------------------------
void CCTextureAltas::vRelease()
{
	// Сбрасываем размеры атласной текстуры
	m_uintTextureRecomendWidth = m_uintDefaultTextureAtlasWidth;
	m_uintTextureRecomendHeight = m_uintDefaultTextureAtlasHeight;

	_SAFE_RELEASE_(m_lpTexture);
	m_uintQuantFreePixel = 0;
	m_bReleased = true;
	_SAFE_DELETE_ARRAY_(m_pbFreeMap);

	// Очищаем stack
	for(int i = m_stackFreeIndexTexture.size(); i > 0; i--)
	{
		m_stackFreeIndexTexture.top();
		m_stackFreeIndexTexture.pop();
	}

	// очищаем вектор индексов
	m_vecTexture.clear();
}

//---------------------------------------------------------------------------------------
// Name: bGetStatusReleased()
// Desc:
//----------------------------------------------------------------------------------------
bool CCTextureAltas::bGetStatusReleased()
{
	if(m_bReleased)
		return true;
	return false;
}

//---------------------------------------------------------------------------------------
// Name: vSaveTextureAtlasToFile()
// Desc: Сохранит атласную текстуру в файл
//----------------------------------------------------------------------------------------
void CCTextureAltas::vSaveTextureAtlasToFile(char *name)
{
	if(m_lpTexture != NULL)
		D3DXSaveTextureToFile(name,D3DXIFF_BMP,m_lpTexture,0);
}
/////////////////////////////// Класс CCTextureAltas ( end ) ////////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------



//////////////////////// Класс CCManagerTextureAtlas ( begin ) //////////////////////////////
CCManagerTextureAtlas::CCManagerTextureAtlas()
{
}
CCManagerTextureAtlas::~CCManagerTextureAtlas()
{
}

//---------------------------------------------------------------------------------------
// Name: vInit()
// Desc:
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vInit(LPDIRECT3DDEVICE9 lpD3DDevice)
{
	m_lpD3DDevice = lpD3DDevice;
}

//---------------------------------------------------------------------------------------
// Name: vAddTexture()
// Desc: по адресам (*piIndexAtlas,*piIndexTexture) записывает результат
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vAddTexture(char *name, CCManagerTextureAtlas_h::stHandleTexture *pHandleTexture)
{
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile (name, &info);

	LPDIRECT3DTEXTURE9 lpTexture = NULL;
	if(FAILED(D3DXCreateTextureFromFileEx(m_lpD3DDevice, name, info.Width,info.Height,
											0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
											D3DX_DEFAULT, D3DX_DEFAULT, 0,0, // or NULL
											NULL, &lpTexture)))
	{
		vCreateTextureDefault(&lpTexture,32,32);
	}


	/* 
		добавляем текстуру в атласную текстуру
		поиск свободного места для текстуры производим с первой атласной текстуры
		при необходимости создаём новую атласную текстуру

	*/
	/*int iIndexTextureInAtlas = -1;
	for(UINT indexAtlas = 0; iIndexTextureInAtlas < 0; indexAtlas++)
	{
		if(indexAtlas >= m_vecCTextureAltas.size())
		{
			CCTextureAltas CTextureAltas(m_lpD3DDevice);
			m_vecCTextureAltas.push_back(CTextureAltas);
		}
		iIndexTextureInAtlas = m_vecCTextureAltas[indexAtlas].iAddTexture(&lpTexture);
		pHandleTexture->iIndexAtlas = indexAtlas;
	}
	pHandleTexture->iIndexTexture = iIndexTextureInAtlas;*/

	int indexAtlas = 0;
	int iIndexTextureInAtlas = -1;
	
	if(m_vecCTextureAltas.size() == 0)
	{
		CCTextureAltas CTextureAltas(m_lpD3DDevice);
		m_vecCTextureAltas.push_back(CTextureAltas);

		indexAtlas = m_vecCTextureAltas.size() - 1;
		iIndexTextureInAtlas = m_vecCTextureAltas[indexAtlas].iAddTexture(&lpTexture);
	}
	else
	{
		// Просмотр для занятых CCTextureAltas
		for(indexAtlas = 0; (indexAtlas < m_vecCTextureAltas.size()) && (iIndexTextureInAtlas < 0); indexAtlas++)
			if(!m_vecCTextureAltas[indexAtlas].bGetStatusReleased())
				iIndexTextureInAtlas = m_vecCTextureAltas[indexAtlas].iAddTexture(&lpTexture);
		indexAtlas -= 1;

		if(iIndexTextureInAtlas < 0)
		{
			// Просмотр для свободных CCTextureAltas
			for(int indexAtlas = 0; (indexAtlas < m_vecCTextureAltas.size()) && (iIndexTextureInAtlas < 0); indexAtlas++)
				if(m_vecCTextureAltas[indexAtlas].bGetStatusReleased())
					iIndexTextureInAtlas = m_vecCTextureAltas[indexAtlas].iAddTexture(&lpTexture);
			indexAtlas -= 1;
		}
		if(iIndexTextureInAtlas < 0)
		{
			// создать новый экзкмпляр класса
			CCTextureAltas CTextureAltas(m_lpD3DDevice);
			m_vecCTextureAltas.push_back(CTextureAltas);

			indexAtlas = m_vecCTextureAltas.size() - 1;
			iIndexTextureInAtlas = m_vecCTextureAltas[indexAtlas].iAddTexture(&lpTexture);
		}
	}
	pHandleTexture->iIndexAtlas = indexAtlas;
	pHandleTexture->iIndexTexture = iIndexTextureInAtlas;

	_SAFE_RELEASE_(lpTexture);
}

//---------------------------------------------------------------------------------------
// Name: vCreateTextureDefault()
// Desc:
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vDeleteTexture(CCManagerTextureAtlas_h::stHandleTexture *pHandleTexture)
{
	if(pHandleTexture->iIndexAtlas >= 0)
		if(m_vecCTextureAltas[pHandleTexture->iIndexAtlas].iDeleteTexture(&pHandleTexture->iIndexTexture) == TEXTURE_ATLAS_CLASS_ENABLE_RELEASE)
			pHandleTexture->iIndexAtlas = -1;
}

//---------------------------------------------------------------------------------------
// Name: vCreateTextureDefault()
// Desc:
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vCreateTextureDefault(LPDIRECT3DTEXTURE9 *plpTexture, UINT width, UINT height)
{
	m_lpD3DDevice->CreateTexture(width,height,0,0,D3DFMT_A8R8G8B8,
								D3DPOOL_MANAGED,plpTexture,0);

	D3DLOCKED_RECT	d3dlr;
	(*plpTexture)->LockRect(0,&d3dlr,0,0);
	DWORD *colorCur = (DWORD*)d3dlr.pBits;

	for(DWORD str = 0; str < height; str++)
		for(DWORD stl = 0; stl < width; stl++)
			colorCur[str*d3dlr.Pitch/4 + stl] = D3DCOLOR_XRGB(255,0,255);
	(*plpTexture)->UnlockRect(0);
}

//---------------------------------------------------------------------------------------
// Name: vRelease()
// Desc:
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vRelease()
{
	// освобождаем все CCTextureAltas
	for(UINT i = 0; i < m_vecCTextureAltas.size(); i++)
		m_vecCTextureAltas[i].vRelease();

	// очищаем вектор индексов
	m_vecCTextureAltas.clear();
}

//---------------------------------------------------------------------------------------
// Name: vSaveTextureAtlasesToFiles()
// Desc: Сохраняет все атласные текстуры в каталог
//----------------------------------------------------------------------------------------
void CCManagerTextureAtlas::vSaveTextureAtlasesToFiles(char *nameCatalog)
{
	char *name = new char[strlen(nameCatalog) + 30];
	for(UINT i = 0; i < m_vecCTextureAltas.size(); i++)
	{
		sprintf(name,"%s\\Atlas_%d.bmp", nameCatalog,i);
		m_vecCTextureAltas[i].vSaveTextureAtlasToFile(name);
	}
	delete[] name;
}
///////////////////////// Класс CCManagerTextureAtlas ( end ) ///////////////////////////////
//===========================================================================================
//-------------------------------------------------------------------------------------------