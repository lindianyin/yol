#pragma once
#include "stdafx.h"
#include "Runtime/qstring.h"

//
//struct QRandomNameTemplate
//{
//	DWORD dwId;
//	QString strFamilyName;// 姓氏
//	QString strMaleName;// 男人名
//	QString strFemaleName;// 女人名
//};
//
//typedef std::map<DWORD, QRandomNameTemplate> QRandomNameMap;


class QRandomName
{
public:
	BOOL Init();
	BOOL UnInit();
	BOOL GetRandomName(CHAR* refNameStr, INT nSex);

private:
	std::vector<std::string*> m_vecFamilyNames;
	std::vector<std::string*> m_vecFemaleNames;
	std::vector<std::string*> m_vecMaleNames;

	BOOL GenerateRandomName(CHAR* refNameStr, INT nSex, BOOL bDobuleMode = FALSE);  // double模式，  单字名字重复两次！

};