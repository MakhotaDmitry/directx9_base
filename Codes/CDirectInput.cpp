#include "CDirectInput.h"

CDirectInput::CDirectInput()
{
	m_pDI = NULL;
	m_pDI_Mouse = NULL;
	m_pDI_Keyboard = NULL;

	m_fMouseSensitivity_X = 1.0f;
	m_fMouseSensitivity_Y = 1.0f;
	m_fMouseSensitivity_Z = 1.0f;

	m_iMousePos_X = 0;
	m_iMousePos_Y = 0;
	m_iMousePos_Z = 0;
}

CDirectInput::~CDirectInput()
{}


//--------------------------------------------------------------------------------------
// Name: iInit
// Desc:
//--------------------------------------------------------------------------------------
int CDirectInput::iInit(HWND hWnd, HINSTANCE hInst, DWORD dwFlagsEnumDevice)
{
	m_hWnd = hWnd;
	m_hInstance = hInst;
	vResizeWindow();
	

	// -------------------------------- Init DirectInput8 --------------------------------
	if(!m_pDI)
	{
		if(FAILED(m_hResult = DirectInput8Create(
						m_hInstance, DIRECTINPUT_VERSION,
						IID_IDirectInput8, (VOID**)&m_pDI, NULL)))
			return DINPUT_ERROR_NO_INIT;
	}
	else
		return DINPUT_ERROR_DINPUT_EXIST;



	// -------------------------------- Init Direct Keyboard ------------------------------
	if(dwFlagsEnumDevice & DINPUT_INIT_KEYBOARD)
	{
		if(!m_pDI_Keyboard)
		{
			if(FAILED(m_hResult = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pDI_Keyboard, NULL)))
				return DINPUT_ERROR_NO_KEYBOARD_INIT;

			// Create buffer for save data keyboard
			DIPROPDWORD dipDW;
			ZeroMemory(&dipDW, sizeof(DIPROPDWORD));
			dipDW.diph.dwSize		= sizeof(DIPROPDWORD);
			dipDW.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
			dipDW.diph.dwObj		= 0;
			dipDW.diph.dwHow		= DIPH_DEVICE;
			dipDW.dwData			= DINPUT_KEYBOARD_BUFFERSIZE;


			// Set size buffer
			if(FAILED(m_hResult = m_pDI_Keyboard->SetProperty(DIPROP_BUFFERSIZE, &dipDW.diph)))
				return DINPUT_ERROR_NO_KEYBOARD_INIT;

			// Set format data keyboard
			if(FAILED(m_hResult = m_pDI_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
				return DINPUT_ERROR_NO_KEYBOARD_INIT;

			// Set cooperative level for not exlusive access
			if(FAILED(m_hResult = m_pDI_Keyboard->SetCooperativeLevel(m_hWnd, 
														DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
				return DINPUT_ERROR_NO_KEYBOARD_INIT;

			// Acquire device keyboard
			m_pDI_Keyboard->Acquire();

			// Get layout keyboard
			m_HKL_Layout = GetKeyboardLayout(0);
		}
		else
			return DINPUT_ERROR_KEYBOARD_EXIST;
	}



	// -------------------------------- Init Direct Mouse ----------------------------------
	if(dwFlagsEnumDevice & DINPUT_INIT_MOUSE)
	{
		if(!m_pDI_Mouse)
		{
			if(FAILED(m_hResult = m_pDI->CreateDevice(GUID_SysMouse, &m_pDI_Mouse, NULL)))
				return DINPUT_ERROR_NO_MOUSE_INIT;

			// Create buffer for save data mouse
			DIPROPDWORD dipDW;
			ZeroMemory(&dipDW, sizeof(DIPROPDWORD));
			dipDW.diph.dwSize		= sizeof(DIPROPDWORD);
			dipDW.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
			dipDW.diph.dwObj		= 0;
			dipDW.diph.dwHow		= DIPH_DEVICE;
			dipDW.dwData			= DINPUT_MOUSE_BUFFERSIZE;


			// Set size buffer
			if(FAILED(m_hResult = m_pDI_Mouse->SetProperty(DIPROP_BUFFERSIZE, &dipDW.diph)))
				return DINPUT_ERROR_NO_MOUSE_INIT;

			// Set format data mouse
			if(FAILED(m_hResult = m_pDI_Mouse->SetDataFormat(&c_dfDIMouse)))
				return DINPUT_ERROR_NO_MOUSE_INIT;

			// Set cooperative level for not exlusive access
			if(FAILED(m_hResult = m_pDI_Mouse->SetCooperativeLevel(m_hWnd, 
				DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
				return DINPUT_ERROR_NO_MOUSE_INIT;

			// Acquire device mouse
			m_pDI_Mouse->Acquire();
		}
		else
			return DINPUT_ERROR_MOUSE_EXIST;
	}


	return DINPUT_SUCCESS;
}


//--------------------------------------------------------------------------------------
// Name: iReadKeyboardBufferedData
// Desc:
//--------------------------------------------------------------------------------------
int CDirectInput::iReadKeyboardBufferedData(void)
{
	if( (!m_pDI) & (!m_pDI_Keyboard) )
		return DINPUT_ERROR_NO_INIT;

	m_dwItemsKeyboard = DINPUT_KEYBOARD_BUFFERSIZE;
	m_hResult = m_pDI_Keyboard->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
												m_didKeyboardBuffer,
												&m_dwItemsKeyboard,
												0);
	// if keyboard been lost then reacquire it
	if(FAILED(m_hResult)) 
	{
		m_pDI_Keyboard->Acquire();
		return DINPUT_SUCCESS;
	}

	// Read keyboard data
	if(m_dwItemsKeyboard)
		for(DWORD dwCurBuffer = 0; dwCurBuffer < m_dwItemsKeyboard; dwCurBuffer++)
		{
			m_byteASCII = CDirectInput::byteScanToASCII( m_didKeyboardBuffer[dwCurBuffer].dwOfs );

			if(m_didKeyboardBuffer[dwCurBuffer].dwData & 0x80)
			{
				m_bDIK_key[m_didKeyboardBuffer[dwCurBuffer].dwOfs][dwCurBuffer] = true;
				m_bASCII_key[m_byteASCII][dwCurBuffer] = true;
			}
			else
			{
				m_bDIK_key[m_didKeyboardBuffer[dwCurBuffer].dwOfs][dwCurBuffer] = false;
				m_bASCII_key[m_byteASCII][dwCurBuffer] = false;
			}
		}

	return (m_dwItemsKeyboard);
}


//--------------------------------------------------------------------------------------
// Name: iReadMouseBufferedData
// Desc:
//--------------------------------------------------------------------------------------
int CDirectInput::iReadMouseBufferedData(void)
{
	if( (!m_pDI) & (!m_pDI_Mouse) )
		return DINPUT_ERROR_NO_INIT;

	m_dwItemsMouse = DINPUT_MOUSE_BUFFERSIZE;
	m_hResult = m_pDI_Mouse->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
											m_didMouseBuffer,
											&m_dwItemsMouse,
											0);

	// if mouse been lost then reacquire it
	if(FAILED(m_hResult)) 
	{
		m_pDI_Mouse->Acquire();
		return DINPUT_SUCCESS;
	}

	// Read mouse data
	m_iMouseDelta_X = 0;
	m_iMouseDelta_Y = 0;
	m_iMouseDelta_Z = 0;

	if(m_dwItemsMouse)
		for(DWORD dwCurBuffer = 0; dwCurBuffer < m_dwItemsMouse; dwCurBuffer++)
		{
			switch(m_didMouseBuffer[dwCurBuffer].dwOfs)
			{
				case DIMOFS_BUTTON0:
						if(m_didMouseBuffer[dwCurBuffer].dwData & 0x80)
							m_bMouseButton_L = true;
						else
							m_bMouseButton_L = false;
					break;
				case DIMOFS_BUTTON1:
						if(m_didMouseBuffer[dwCurBuffer].dwData & 0x80)
							m_bMouseButton_R = true;
						else
							m_bMouseButton_R = false;
					break;
				case DIMOFS_BUTTON2:
						if(m_didMouseBuffer[dwCurBuffer].dwData & 0x80)
							m_bMouseButton_M = true;
						else
							m_bMouseButton_M = false;
					break;
				case DIMOFS_BUTTON3:
						if(m_didMouseBuffer[dwCurBuffer].dwData & 0x80)
							m_bMouseButton_4 = true;
						else
							m_bMouseButton_4 = false;
					break;


				case DIMOFS_X:
						m_iMouseDelta_X += (int)m_didMouseBuffer[dwCurBuffer].dwData * m_fMouseSensitivity_X;
					break;

				case DIMOFS_Y:
						m_iMouseDelta_Y += (int)m_didMouseBuffer[dwCurBuffer].dwData * m_fMouseSensitivity_Y;
					break;

				case DIMOFS_Z:
						m_iMouseDelta_Z += (int)m_didMouseBuffer[dwCurBuffer].dwData * m_fMouseSensitivity_Z;
					break;
			}
		}



		SetCursorPos(m_iCursorCenterPos_X, m_iCursorCenterPos_Y);

		m_iMousePos_X += m_iMouseDelta_X;
		m_iMousePos_Y += m_iMouseDelta_Y;
		m_iMousePos_Z += m_iMouseDelta_Z;



		// если вышли за пределы клиентской части окна
		if(m_iMousePos_X < m_rectClientWindow.left ) m_iMousePos_X = m_rectClientWindow.left;
		if(m_iMousePos_Y < m_rectClientWindow.top ) m_iMousePos_Y = m_rectClientWindow.top;
		if(m_iMousePos_X > m_rectClientWindow.right ) m_iMousePos_X = m_rectClientWindow.right;
		if(m_iMousePos_Y > m_rectClientWindow.bottom ) m_iMousePos_Y = m_rectClientWindow.bottom;

	return (m_dwItemsMouse);
}


//--------------------------------------------------------------------------------------
// Name: vReadMouseImmediateData
// Desc:
//--------------------------------------------------------------------------------------
void CDirectInput::vReadMouseImmediateData(void)
{
	m_hResult = m_pDI_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_diMouseState);

	if(FAILED(m_hResult)) 
	{
		m_pDI_Mouse->Acquire();
		return;
	}
	//--------------------------

	SetCursorPos(m_iCursorCenterPos_X, m_iCursorCenterPos_Y);


	m_iMouseDelta_X = m_diMouseState.lX * m_fMouseSensitivity_X;
	m_iMouseDelta_Y = m_diMouseState.lY * m_fMouseSensitivity_Y;
	m_iMouseDelta_Z = m_diMouseState.lZ * m_fMouseSensitivity_Z;

	m_iMousePos_X += m_iMouseDelta_X;
	m_iMousePos_Y += m_iMouseDelta_Y;
	m_iMousePos_Z += m_iMouseDelta_Z;

	m_bMouseButton_L = m_diMouseState.rgbButtons[0]&0x80;
	m_bMouseButton_R = m_diMouseState.rgbButtons[1]&0x80;
	m_bMouseButton_M = m_diMouseState.rgbButtons[2]&0x80;
	m_bMouseButton_4 = m_diMouseState.rgbButtons[3]&0x80;


	// если вышли за пределы клиентской части окна
	if(m_iMousePos_X < m_rectClientWindow.left ) m_iMousePos_X = m_rectClientWindow.left;
	if(m_iMousePos_Y < m_rectClientWindow.top ) m_iMousePos_Y = m_rectClientWindow.top;
	if(m_iMousePos_X > m_rectClientWindow.right ) m_iMousePos_X = m_rectClientWindow.right;
	if(m_iMousePos_Y > m_rectClientWindow.bottom ) m_iMousePos_Y = m_rectClientWindow.bottom;
}



//--------------------------------------------------------------------------------------
// Name: vResizeWindow
// Desc:
//--------------------------------------------------------------------------------------
void CDirectInput::vResizeWindow()
{
	GetWindowRect(m_hWnd, &m_rectWindow);
	m_iCursorCenterPos_X = (m_rectWindow.right - m_rectWindow.left)/2;
	m_iCursorCenterPos_Y = (m_rectWindow.bottom - m_rectWindow.top)/2;

	GetClientRect(m_hWnd, &m_rectClientWindow);
}



//--------------------------------------------------------------------------------------
// Name: vSetMouseSensitivity
// Desc:
//--------------------------------------------------------------------------------------
void CDirectInput::vSetMouseSensitivity(float x, float y, float z)
{
	m_fMouseSensitivity_X = x;
	m_fMouseSensitivity_Y = y;
	m_fMouseSensitivity_Z = z;
}


//--------------------------------------------------------------------------------------
// Name: byteScanToASCII
// Desc:
//--------------------------------------------------------------------------------------
BYTE CDirectInput::byteScanToASCII(DWORD scancode)
{
	// convert scan code to ascii
	return MapVirtualKeyEx(scancode, 1, m_HKL_Layout);
}


//--------------------------------------------------------------------------------------
// Name: vRelease
// Desc:
//--------------------------------------------------------------------------------------
void CDirectInput::vRelease(void)
{
	// Mouse
	if (m_pDI_Mouse)
	{
		m_pDI_Mouse->Unacquire();
		m_pDI_Mouse->Release();
		m_pDI_Mouse = NULL;
	}

	// Keyboard
	if (m_pDI_Keyboard)
	{
		m_pDI_Keyboard->Unacquire();
		m_pDI_Keyboard->Release();
		m_pDI_Keyboard = NULL;
	}

	// DirectInput
	if(m_pDI)
	{
		m_pDI->Release();
		m_pDI = NULL;
	}
}