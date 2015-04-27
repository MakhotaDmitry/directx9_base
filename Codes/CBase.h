#pragma once

#include "Resource.h"


/////////////////////////////////////// Класс CLog ( Begin ) //////////////////////////////
class CLog
{
	private:
		FILE	*m_fileLog;
		bool	m_bRunLog;  // Разрешить запись Log
	public:
		CLog();
		~CLog();

		void vAddString(char *str);
		void vInit();
};
/////////////////////////////////////// Класс CLog ( End ) //////////////////////////////




/////////////////////////////////////// Класс CBase ( Begin ) //////////////////////////////
class CBase
{
public:
	CLog				m_CLog;
	DWORD				m_dwTickCount;
	resource_h::stFPS	m_stFPS;
	char				m_cDirectory_exe [400];  // директория *.exe файла
public:
	CBase();
	~CBase();

	// Для работы с *.ini файлами  ( Считывание из директории *.EXE ФАЙЛА )
	void vRead_Integer_InIni(char *pcSection, char *pcKey, char *pcPath_ini, int &iValue);
	void vRead_UINT_InIni(char *pcSection, char *pcKey, char *pcPath_ini, UINT &uintValue);
	void vRead_Float_InIni(char *pcSection, char *pcKey, char *pcPath_ini, float &fValue);
	void vRead_String_InIni(char *pcSection, char *pcKey, char *pcPath_ini, char *pcValue);
	void vRead_Bool_InIni(char *pcSection, char *pcKey, char *pcPath_ini, bool &bValue);
	void vRead_BOOL_InIni(char *pcSection, char *pcKey, char *pcPath_ini, BOOL &bValue);


	template <class T> void swap(T &val1, T &val2);  // поменять местами два числа
	bool bCompareString(char *str_1, char *str_2);  // сравнивает строки
	void vSortArrayIncrease(int* s_arr,int first, int last); // Сортирует массив целых чисел по возрастанию


	DWORD dwFloat_To_DWORD(float f);
	int iConvert_volume_to_squall(int iVolInProz);
	void vPreCalculate();

	float fGradToRad(float fGrad) { return fGrad*3.141592654f/180.0f; }; // из градусов в радианы
	float fRadToGrad(float fRadian) { return fRadian*180.0f/3.141592654f; }; // из радиан в градусы

	// Пересечение поверхностей
	bool bPointHitRectangle(resource_h::stSIZE pointPos, resource_h::stRECT rectPos);
};
/////////////////////////////////////// Класс CBase ( End ) //////////////////////////////
