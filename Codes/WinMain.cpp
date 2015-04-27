#include "CMain.h"

// --------------------------------- Structures (begin) -------------------------------
// --------------------------------- Structures (end) ---------------------------------



// --------------------------------- Variable (begin) ----------------------------------
CMain main;
bool g_bExitProgram = false;
bool g_bPauseProgram = false; // если не надо обрабатывать игру ( например при потери фокуса )
resource_h::stRECT mousePos(0,0,32,32);
#define QUANT_TEXT_LINE 7
char g_pcText[QUANT_TEXT_LINE][100];

LPDIRECT3DTEXTURE9 texture;
// --------------------------------- Variable (end) -------------------------------------



// ----------------------------- Prototype functions (begin) -----------------------------
bool bInitResource(HINSTANCE hThisInst);
void vKeyboard();
void vMouse();
// ----------------------------- Prototype functions (end) --------------------------------
///////////////////////////////////////////////////////////////////////////////////////////
//=========================================================================================



//---------------------------------
// Name: RenderScene ()
// Desc: 
//---------------------------------
void vRender()
{
	// Если устройство потеряно выходим из цикла рисования
	if(!main.bTestCooperativeLevel())
		return;

	// ------------------------------------ Render ------------------------------------
	//resource_h::stRECT rectPos(0,0,main.m_CDirectx.m_d3dpp.BackBufferWidth,32);
	static int red = 0;
	main.m_pD3DDevice->BeginScene();
	{
		main.m_pD3DDevice->Clear (0, NULL, 
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
			D3DCOLOR_XRGB (red++, 170, 128), 
			1.0f, 0.0f);


		// -------------------------------- 3D ---------------------------------------------
		// -------------------------------- 2D ---------------------------------------------
			/*main.m_CGraphics.m_CSprite.vBegin();
				main.m_CGraphics.m_CSprite.vSetTexure(&texture);
				main.m_CGraphics.m_CSprite.vDrawUP(&rectPos,NULL,0);

				// mouse
				main.m_CGraphics.m_CAnimation.vSetTexture(0);
				main.m_CGraphics.m_CAnimation.vSetAnimationPos(0,mousePos);
				main.m_CGraphics.m_CAnimation.vDraw(0);

				// Text				
				main.m_CFont.vDrawText(1,main.m_CBase.m_stFPS.cFPS, 
										D3DCOLOR_XRGB(255,255,0),
										resource_h::stRECT(0,0,20,16));

				// probe sprite
// 				main.m_CGraphics.m_CSprite.vSetTexure(&texture);
// 				resource_h::stRECT rectPos_(100,100,32,32);
// 				for(int i = 0; i < 1000; i++)
// 					main.m_CGraphics.m_CSprite.vDrawUP(&rectPos_,NULL,0);
				//main.m_CGraphics.m_CSprite.vDraw(0,0,0);

				// Primitive
				//main.m_CGraphics.m_primGraph.vDrawCircle(100,100,70,D3DCOLOR_XRGB(255,0,255));

			main.m_CGraphics.m_CSprite.vEnd();*/
			
			/*main.m_CMap3D.vBegin();
				main.m_CMap3D.vDraw();
			main.m_CMap3D.vEnd();*/

			main.m_CGraphics.m_CSprite.vBegin();
				// mouse
				main.m_CGraphics.m_CAnimation.vSetTexture(0);
				main.m_CGraphics.m_CAnimation.vSetAnimationPos(0,mousePos);
				main.m_CGraphics.m_CAnimation.vDraw(0);
			main.m_CGraphics.m_CSprite.vEnd();


			main.m_CGraphics2D.vBegin();
				//main.m_CGraphics2D.vDrawLine(100,100,500,500);
				/*for(int i = 0; i < 100; i++)
					for(int j = 0; j < 100; j++)
						main.m_CGraphics2D.vDrawPoint(i,j,0, D3DCOLOR_XRGB(255,0,0));*/
				//main.m_CGraphics2D.vDrawPointTest();
			main.m_CGraphics2D.vEnd();


			// Draw FPS
			main.m_CGraphics.m_CSprite.vBegin();
				main.m_CFont.vDrawText(1,main.m_CBase.m_stFPS.cFPS, 
										D3DCOLOR_XRGB(255,255,0),
										resource_h::stRECT(650,0,20,16));

				for(int i = 0; i < QUANT_TEXT_LINE; i++)
					main.m_CFont.vDrawText(0,g_pcText[i], 
										D3DCOLOR_XRGB(255,255,0),
										resource_h::stRECT(0,i*25,20,16));
			main.m_CGraphics.m_CSprite.vEnd();

		main.m_pD3DDevice->EndScene();
		main.m_pD3DDevice->Present(0,0,0,0);
	}
}

//---------------------------------
// Name: WinMain ()
// Desc: 
//---------------------------------
int APIENTRY WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;	
	if(!bInitResource(hThisInst))
		g_bExitProgram = true;

	
	// -------- Main -------
	while (!g_bExitProgram)
	{
		if(PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage (&msg, NULL, 0, 0)) break;
			TranslateMessage (&msg); 
			DispatchMessage (&msg);
		}

		else if(!g_bPauseProgram)
		{
			//Sleep(1500);
			main.m_CBase.vPreCalculate();
			if(main.m_CBase.m_stFPS.bModified)
				main.m_CBase.m_CLog.vAddString(main.m_CBase.m_stFPS.cFPS);
			main.m_CGraphics.m_CAnimation.vCalculate();

			vMouse();
			vKeyboard();

			//if(main.m_CBase.m_stFPS.iFPS % 30 == 0)
				vRender();
		}
	};

	main.vRelease();

	return 0;
};

//---------------------------------
// EOF
//---------------------------------

bool bInitResource(HINSTANCE hThisInst)
{
	//main.m_CBase.m_CLog.vInit();
	int iAnswer;
	if( GAME_ENGINE_SUCCESS != (iAnswer = main.iInitFromFile_INI("BaseProgram v0.3 (Direct3D9)",
																"BaseProgram v0.3 (Direct3D9)",
																hThisInst, "Data\\Settings.ini")))
	{
		switch(iAnswer)
		{
			case GAME_ENGINE_ERROR_CREATE_DIRECT3D:
					MessageBox(0,"GAME_ENGINE_ERROR_CREATE_DIRECT3D","Fatal error",MB_OK);
				break;
			case GAME_ENGINE_ERROR_CREATE_DIRECT3D_DEVICE:
					MessageBox(0,"GAME_ENGINE_ERROR_CREATE_DIRECT3D_DEVICE","Fatal error",MB_OK);
				break;
			case GAME_ENGINE_ERROR_CREATE_WINDOW:
					MessageBox(0,"GAME_ENGINE_ERROR_CREATE_WINDOW","Fatal error",MB_OK);
				break;

			default:
					MessageBox(0,"GAME_ENGINE_ERROR","Fatal error", MB_OK);
				break;
		}

		return false;
	}
	//main.m_CDirectx.vGetInfoVideoCard("Reports\\Report_VGA.txt");

	// -------- Loads fonts (begin) --------
	// -------- Loads fonts (end) --------


	

	// -------- Load and Init cursor ( begin ) -------
	// -------- Load and Init cursor ( end ) -------



	// -------  LOAD game data (begin) --------
	main.m_CGraphics.m_CAnimation.iLoadTexture("Data\\gfx\\cursor_main.png");
	main.m_CGraphics.m_CAnimation.iAddAnimation(0,25,resource_h::stRECT(100,100,32,32),resource_h::stSIZE(32,32));

	main.m_CTexture.iCreateTextureFromFile("Data\\gfx\\panel.png",&texture);
	main.m_CTexture.vBrightnessTexture(&texture,0.0f);

	main.m_CFont.iAddFont("Data\\Fonts\\Arial.***");
	main.m_CFont.iAddFont("Data\\Fonts\\Electonic.***");
	main.m_CFont.iAddFont("Data\\Fonts\\MatterhornCTT.***");
	main.m_CFont.iAddFont("Data\\Fonts\\Symbol.***");
	// -------  LOAD game data (end) ----------

	return true;
}

void vKeyboard()
{
	int items = main.m_CInput.iReadKeyboardBufferedData();
	sprintf_s(g_pcText[1],"Keyboard items = %d", items);

	if(items)
		for(int i = 0; i < items; i++)
		{
			if(main.m_CInput.m_bDIK_key[DIK_ESCAPE][i])
			{
				g_bExitProgram = true;
				break;
			}

			if(main.m_CInput.m_bDIK_key[DIK_F1][i])
			{
				main.m_d3dpp.Windowed = !main.m_d3dpp.Windowed;
				main.iChangePresentationD3D();
			}
		}
}

void vMouse()
{
	//int items = main.m_CInput.iReadMouseBufferedData();
	//sprintf(g_pcText[5],"Mouse items = %d", items);
	main.m_CInput.vReadMouseImmediateData();

	
	/*main.m_CInput.vReadMouseImmediateData();

	mousePos.iPosX = main.m_CInput.m_iMousePos_X;
	mousePos.iPosY = main.m_CInput.m_iMousePos_Y;*/


	mousePos.iPosX = main.m_CInput.m_iMousePos_X;
	mousePos.iPosY = main.m_CInput.m_iMousePos_Y;

	sprintf_s(g_pcText[0],"MousePos:  X = %d, Y = %d, Z = %d", 
						main.m_CInput.m_iMousePos_X,
						main.m_CInput.m_iMousePos_Y,
						main.m_CInput.m_iMousePos_Z);
	sprintf_s(g_pcText[6],"MouseSens:  X = %f, Y = %f, Z = %f", 
						main.m_CInput.m_fMouseSensitivity_X,
						main.m_CInput.m_fMouseSensitivity_Y,
						main.m_CInput.m_fMouseSensitivity_Z);

	if(main.m_CInput.m_bMouseButton_L)
		sprintf_s(g_pcText[2],"LeftButton DOWN");
	else
		sprintf_s(g_pcText[2],"LeftButton UP");
	if(main.m_CInput.m_bMouseButton_R)
		sprintf_s(g_pcText[3],"RightButton DOWN");
	else
		sprintf_s(g_pcText[3],"RightButton UP");
	if(main.m_CInput.m_bMouseButton_M)
		sprintf_s(g_pcText[4],"MiddleButton DOWN");
	else
		sprintf_s(g_pcText[4],"MiddleButton UP");
}