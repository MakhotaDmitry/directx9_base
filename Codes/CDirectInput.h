#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


// ----------------------- Global define's and variables (begin) -------------------

// ----------------- Keyboard buffer size -----------------
const int DINPUT_KEYBOARD_BUFFERSIZE	= 10;
const int DINPUT_MOUSE_BUFFERSIZE		= 16;

// ----------------- What's device's need initialization---------------
#define DINPUT_INIT_KEYBOARD	0x00000001
#define DINPUT_INIT_MOUSE		0x00000010

// ----------------- Error codes ---------------------
#define DINPUT_SUCCESS					-1	/* No problem */
#define DINPUT_ERROR_NO_INIT			-2	/* DirectInput not initializations */
#define DINPUT_ERROR_NO_KEYBOARD_INIT	-3	/* DirectKeyboard not initializations */
#define DINPUT_ERROR_NO_MOUSE_INIT		-4	/* DirectMouse not initializations */
#define DINPUT_ERROR_DINPUT_EXIST		-5	/* DirectInput before be created */
#define DINPUT_ERROR_KEYBOARD_EXIST		-6	/* DirectKeyboard before be created */
#define DINPUT_ERROR_MOUSE_EXIST		-7	/* DirectMouse before be created */

// ----------------------- Global define's and variables (end)   -------------------


class CDirectInput
{
	private:
		HINSTANCE				m_hInstance;
		HWND					m_hWnd;
		HRESULT					m_hResult;
		HKL						m_HKL_Layout;
		LPDIRECTINPUT8			m_pDI;
		LPDIRECTINPUTDEVICE8	m_pDI_Mouse;
		LPDIRECTINPUTDEVICE8	m_pDI_Keyboard;
		RECT					m_rectClientWindow;
		RECT					m_rectWindow;
		int						m_iCursorCenterPos_X;	// Позиция в которую будет сбрасываться курсор
		int						m_iCursorCenterPos_Y;

		DIDEVICEOBJECTDATA		m_didKeyboardBuffer[DINPUT_KEYBOARD_BUFFERSIZE];
		DWORD					m_dwItemsKeyboard;
		BYTE					m_byteASCII;

		DIDEVICEOBJECTDATA		m_didMouseBuffer[DINPUT_MOUSE_BUFFERSIZE];
		DWORD					m_dwItemsMouse;
		DIMOUSESTATE			m_diMouseState;

	public:
		// keyboard buffers
		bool m_bDIK_key		[256][DINPUT_KEYBOARD_BUFFERSIZE];
		bool m_bASCII_key	[256][DINPUT_KEYBOARD_BUFFERSIZE];

		// mouse buffer
		//bool m_b	[4][DINPUT_MOUSE_BUFFERSIZE];

		// Data mouse
		float	m_fMouseSensitivity_X;
		float	m_fMouseSensitivity_Y;
		float	m_fMouseSensitivity_Z;
		int		m_iMousePos_X;
		int		m_iMousePos_Y;
		int		m_iMousePos_Z;
		int		m_iMouseDelta_X;
		int		m_iMouseDelta_Y;
		int		m_iMouseDelta_Z;
		bool	m_bMouseButton_L;
		bool	m_bMouseButton_R;
		bool	m_bMouseButton_M;
		bool	m_bMouseButton_4;

	public:
		CDirectInput();
		~CDirectInput();

		int iInit(HWND hWnd, HINSTANCE hInst, DWORD dwFlagsEnumDevice);
		int iReadKeyboardBufferedData(void);
		int iReadMouseBufferedData(void);
		void vReadMouseImmediateData(void);
		void vSetMouseSensitivity(float x, float y, float z = 1.0f);
		void vResizeWindow();
		BYTE byteScanToASCII(DWORD scancode);
		void vRelease(void);
};