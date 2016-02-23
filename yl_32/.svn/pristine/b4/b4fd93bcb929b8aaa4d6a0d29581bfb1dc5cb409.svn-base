
#pragma once

#include "Misc/xml_i.h"
#include "tinyxml.h"
#include <list>

// -------------------------------------------------------------------------

template <class KNodeT>
class KXmlNodeHelper
{
public:
	KXmlNodeHelper()
	{

	}
	~KXmlNodeHelper()
	{
		Release();
	}
	IXmlNode* GetXmlNode(TiXmlNode* pNode)
	{
		if (NULL == pNode)
			return NULL;

		KNodeT* pTempNode = new KNodeT(pNode);
		m_lstNode.push_back(pTempNode);
		return pTempNode;
	}

private:
	BOOL Release()
	{
		typename NODE_LIST::iterator it = m_lstNode.begin();
		for (it; it != m_lstNode.end(); ++ it)
		{
			if (*it)
			{
				delete (*it);
				*it = NULL;
			}
		}
		m_lstNode.clear();

		return TRUE;
	}

private:
	typedef std::list<KNodeT*>		NODE_LIST;
	NODE_LIST						m_lstNode;
};

class QXmlElement : public IXmlNode
{
public:
	QXmlElement(TiXmlNode* pElement)
	{
		m_pElement = NULL;
		if (pElement)
			m_pElement = pElement->ToElement();
	}
	~QXmlElement()
	{
		m_pElement = NULL;
	}

public:
	virtual LPCTSTR GetValue(LPCTSTR szNodeName = NULL, LPTSTR pszValue = NULL)
	{
		if (m_pElement)
		{
			LPCTSTR pszElementValue = NULL;
			if (szNodeName != NULL)
			{
				TiXmlElement* pChildElement = m_pElement->FirstChildElement(szNodeName);
				if (pChildElement)
				{
					//zjq mod 08.11.15	获取节点内容的bug
					//pChildElement = pChildElement->FirstChildElement();
					//if (pChildElement)
					//{
					//	LPCTSTR pszElementValue = pChildElement->Value();

					//	if (pszElementValue && pszValue)
					//		strcpy(pszValue, pszElementValue);

					//	return pszElementValue;
					//}
					pszElementValue = pChildElement->GetText();
					//end mod
				}
			}
			else
			{
				pszElementValue = m_pElement->GetText();
			}
			if (pszElementValue && pszValue)
				strcpy(pszValue, pszElementValue);
			return pszElementValue;
		}
		return NULL;
	}
	virtual LPCTSTR GetValue(LPCTSTR szNodeName, int* pnValue)
	{
		LPCTSTR szValue = GetValue(szNodeName);
		if (szValue)
			*pnValue = atoi(szValue);
		return szValue;
	}
	virtual LPCTSTR GetValue(LPCTSTR szNodeName, double* pdValue)
	{
		LPCTSTR szValue = GetValue(szNodeName);
		if (szValue)
			*pdValue = atof(szValue);
		return szValue;
	}

	virtual IXmlNode* GetFirstChild(LPCTSTR szNodeName)
	{
		if (!m_pElement)
			return NULL;
		return m_elementHelper.GetXmlNode(m_pElement->FirstChildElement(szNodeName));
	}
	virtual IXmlNode* GetNextSibling(LPCTSTR szNodeName)
	{
		if (!m_pElement)
			return NULL;
		return m_elementHelper.GetXmlNode(m_pElement->NextSiblingElement(szNodeName));
	}

	virtual LPCTSTR GetAttribute(LPCTSTR szNodeName, LPTSTR pszValue = NULL)
	{
		if (!m_pElement)
			return NULL;

		LPCTSTR pszElementAttr = m_pElement->Attribute(szNodeName);
		if (pszElementAttr && pszValue)
			strcpy(pszValue, pszElementAttr);

		return pszElementAttr;
	}
	virtual LPCTSTR GetAttribute(LPCTSTR szNodeName, int* pnValue)
	{
		if (!m_pElement)
			return NULL;
		return m_pElement->Attribute(szNodeName, pnValue);
	}
	virtual LPCTSTR GetAttribute(LPCTSTR szNodeName, double* pdValue)
	{
		if (!m_pElement)
			return NULL;
		return m_pElement->Attribute(szNodeName, pdValue);
	}

	virtual VOID SetAttribute(LPCTSTR szNodeName, LPTSTR pszValue = NULL)
	{
		if (!m_pElement)
			return;
		m_pElement->SetAttribute(szNodeName, pszValue);
		return;
	}
	virtual VOID SetAttribute(LPCTSTR szNodeName, int pnValue)
	{
		if (!m_pElement)
			return;
		m_pElement->SetAttribute(szNodeName, pnValue);
	}
	virtual VOID SetAttribute(LPCTSTR szNodeName, double val)
	{
		if (!m_pElement)
			return;
		m_pElement->SetDoubleAttribute(szNodeName, val);
	}
private:
	TiXmlElement*				m_pElement;
	KXmlNodeHelper<QXmlElement>	m_elementHelper;
};

class KXMLDocument : public IXmlDocument
{
public:
	KXMLDocument()
	{

	}
	virtual ~KXMLDocument()
	{
		
	}

public:
	virtual BOOL Load(LPCTSTR pszXMLFileName);
	virtual BOOL LoadFromBuffer(LPCTSTR szBuffer, INT nLength);
	virtual UINT Release()
	{
		delete this;
		return 0;
	}
	virtual IXmlNode* GetNode(LPCTSTR szNodeName)
	{
		TiXmlElement* pElement = m_xmlDoc.FirstChildElement(szNodeName);
		return m_elementHelper.GetXmlNode(pElement);
	}
	virtual BOOL Push2Script(QLuaScript& sc);
	virtual BOOL Save(LPCSTR pszXmlFile)
	{
		return m_xmlDoc.SaveFile(pszXmlFile);
	}
	virtual BOOL SaveToBuffer(LPSTR szBuffer, INT nLength);
private:
	TiXmlDocument				m_xmlDoc;
	KXmlNodeHelper<QXmlElement>	m_elementHelper;
};

// -------------------------------------------------------------------------
