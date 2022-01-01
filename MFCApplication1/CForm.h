#pragma once

#include "CTab_Terrain.h"
#include "CTab_Mesh.h"
#include "CTab_Navi.h"

// CForm 폼 보기
class CTab_Navi;
class CTab_Terrain;
class CTab_Mesh;
class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CForm };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CTab_Terrain m_pTerrainTab;
	CTab_Mesh m_pMeshTab;
	CTab_Navi m_pNaviTab;

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_mainTab;
	CWnd* m_pwndShow;
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


