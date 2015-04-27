#include "CBase.h"

/////////////////////////////////////// Класс CLog ( Begin ) //////////////////////////////
CLog::CLog()
{
	m_bRunLog = false;
}

CLog::~CLog()
{
	if(m_bRunLog)
	{
		vAddString("Class CLog  = Destructor");
		fclose(m_fileLog);
	}
}

void CLog::vAddString(char *str)
{
	if(m_bRunLog)
		fprintf(m_fileLog,"%s\n", str);
}

void CLog::vInit()
{
	m_bRunLog = true;
	m_fileLog = fopen("Reports\\Log.txt","w");
	
	vAddString("Class CLog  = Init");
}
/////////////////////////////////////// Класс CLog ( End ) //////////////////////////////





/////////////////////////////////////// Класс CBase ( Begin ) //////////////////////////////
CBase::CBase()
{
	// Устанавливаем начальное псевдо-случаное число
	srand(time(0));
}

CBase::~CBase()
{
}

void CBase::vRead_Integer_InIni(char *pcSection, char *pcKey, char *pcPath_ini, int &iValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	char cIntValue[10]; // Для чисел
	GetPrivateProfileString(pcSection,pcKey,
		"not_ready",cIntValue, sizeof(cIntValue),pc_name_ini_file);
	sscanf(cIntValue,"%d",&iValue);

	delete[] pc_name_ini_file;
}

void CBase::vRead_UINT_InIni(char *pcSection, char *pcKey, char *pcPath_ini, UINT &uintValue)
{
	GetCurrentDirectoryA(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	char cIntValue[10]; // Для чисел
	GetPrivateProfileStringA(pcSection,pcKey,
		"not_ready",cIntValue, sizeof(cIntValue),pc_name_ini_file);
	sscanf(cIntValue,"%u",&uintValue);

	delete[] pc_name_ini_file;
}

void CBase::vRead_Float_InIni(char *pcSection, char *pcKey, char *pcPath_ini, float &fValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	char cFloatValue[20]; // Для чисел
	GetPrivateProfileString(pcSection,pcKey,
		"not_ready",cFloatValue, sizeof(cFloatValue),pc_name_ini_file);
	sscanf(cFloatValue,"%f",&fValue);

	delete[] pc_name_ini_file;
}
void CBase::vRead_String_InIni(char *pcSection, char *pcKey, char *pcPath_ini, char *pcValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	//char *temp = new char[length]; //Для временного хранения строки
	char cText[100];

	GetPrivateProfileString(pcSection,pcKey,
		"not_ready",cText, sizeof(cText),pc_name_ini_file);

	sprintf(pcValue,"%s",cText);

	delete[] pc_name_ini_file;
}
void CBase::vRead_Bool_InIni(char *pcSection, char *pcKey, char *pcPath_ini, bool &bValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	char cBoolValue[10], *pcBool; // Для bool
	GetPrivateProfileString(pcSection,pcKey,
		"not_ready",cBoolValue, sizeof(cBoolValue),pc_name_ini_file);
	pcBool = strlwr(cBoolValue);
	if(strstr(pcBool,"true"))
		bValue = true;
	if(strstr(pcBool,"false"))
		bValue = false;

	delete[] pc_name_ini_file;
}
void CBase::vRead_BOOL_InIni(char *pcSection, char *pcKey, char *pcPath_ini, BOOL &bValue)
{
	GetCurrentDirectory(sizeof(m_cDirectory_exe), m_cDirectory_exe);

	char *pc_name_ini_file = new char[ strlen(m_cDirectory_exe) + 40];
	sprintf(pc_name_ini_file, "%s\\%s", m_cDirectory_exe,pcPath_ini);


	char cBoolValue[10], *pcBool; // Для bool
	GetPrivateProfileString(pcSection,pcKey,
		"not_ready",cBoolValue, sizeof(cBoolValue),pc_name_ini_file);
	pcBool = strlwr(cBoolValue);
	if(strstr(pcBool,"true"))
		bValue = TRUE;
	if(strstr(pcBool,"false"))
		bValue = FALSE;

	delete[] pc_name_ini_file;
}


// Сравнивает строки
bool CBase::bCompareString(char *str_1, char *str_2)
{
	if( (str_1 == NULL) || (str_2 == NULL) )
		return false;

	if( strlen(str_1) != strlen(str_2) )
		return  false;

	int iMinQuantSym =0;
	(strlen(str_1) < strlen(str_2)) ? (iMinQuantSym = (int)strlen(str_1)) : (iMinQuantSym = (int)strlen(str_2));

	for(int i =0; i < iMinQuantSym; i++)
	{
		if( str_1[i] != str_2[i] )
			return false;
	}

	return true;
}

//------------------------------------------------------
// Name: swap
// Desc: поменять местами два значения
//------------------------------------------------------
template <class T>
inline void CBase::swap(T &val1, T &val2)
{
	T temp = val1;

	val1 = val2;
	val2 = temp;
}


//------------------------------------------------------
// Name: vSortArrayIncrease
// Desc: Сортирует массив целых чисел по возрастанию
//------------------------------------------------------
void CBase::vSortArrayIncrease(int* s_arr,int first, int last)
{
	int i = first, j = last, x = s_arr[(first + last) / 2];

	do {
		while (s_arr[i] < x) i++;
		while (s_arr[j] > x) j--;

		if(i <= j) {
			if (i < j) swap(s_arr[i], s_arr[j]);
			i++;
			j--;
		}
	} while (i <= j);

	if (i < last)
		vSortArrayIncrease(s_arr, i, last);
	if (first < j)
		vSortArrayIncrease(s_arr, first,j);
}

//------------------------------------------------------
// Name: iConvert_volume_to_squall
// Desc: Перевод громкости от 0..1 в 0..100 
//------------------------------------------------------
int CBase::iConvert_volume_to_squall(int iVolInProz) 
{ 
	float fVol = (float)iVolInProz/100;
	if (fVol <= 0) 
		return 0;

	const float steepness = 2.f; 

	float res = (log10f(fVol * 10.f) + steepness) / (steepness + 1.f); 
	//saturate(res); // clamp 0..1
	if(res > 1.0f)
		res = 1.0f;

	return int(res * 100.f); 
}

//------------------------------------------------------
// Name: dwFloat_To_DWORD
// Desc: Переводит Float в DWORD
//------------------------------------------------------
DWORD CBase::dwFloat_To_DWORD(float f)
{
	return *((DWORD*)&f);
}

//------------------------------------------------------
// Name: vPreCalculate
// Desc: вызывать перед расчётами
//		 Заполняет общие данные (например GetTickCount)
//------------------------------------------------------
void CBase::vPreCalculate()
{
	m_dwTickCount = GetTickCount();

	// Заодно посчитаем FPS
	m_stFPS.iFPS++;
	m_stFPS.bModified = false;
	if(m_dwTickCount - m_stFPS.dwBegTime >= 1000)
	{
		m_stFPS.dwBegTime = m_dwTickCount;
			sprintf_s(m_stFPS.cFPS,"FPS: %d", m_stFPS.iFPS);
		m_stFPS.iFPS = 0;
		m_stFPS.bModified = true;
	}
}

//------------------------------------------------------
// Name: bPointHitRectangle
// Desc: Проверяет попадание точки в прямоугольник
//------------------------------------------------------
bool CBase::bPointHitRectangle(resource_h::stSIZE pointPos, resource_h::stRECT rectPos)
{
	// Попадание точки в прямоугольник
	if( (pointPos.cx > rectPos.iPosX) && (pointPos.cx < rectPos.iPosX_End) &&
		(pointPos.cy > rectPos.iPosY) && (pointPos.cy < rectPos.iPosY_End) )
		return true;
	return false;
}
/////////////////////////////////////// Класс CBase ( End ) //////////////////////////////