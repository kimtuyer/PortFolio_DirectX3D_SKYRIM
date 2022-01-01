// CForm.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "CForm.h"

// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_CForm)
{
	
}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_mainTab);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//CSize sizeCroll;
	//
	//sizeCroll.cx = 0;
	//sizeCroll.cy = 0;
	//SetScrollSizes(MM_TEXT, sizeCroll);


	CRect Rect;
	m_mainTab.GetClientRect(&Rect);
	m_mainTab.InsertItem(0, L"Terrain", 0);
	m_mainTab.InsertItem(1, L"Mesh", 0);
	m_mainTab.InsertItem(2, L"NAVI", 0);

	
	m_pTerrainTab.Create(IDD_DIALOG1, &m_mainTab);
	//m_pTerrainTab->SetWindowPos(NULL, 0, 25, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pTerrainTab.SetWindowPos(NULL, 0, 20, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pwndShow = &m_pTerrainTab;

	m_pMeshTab.Create(IDD_DIALOG2, &m_mainTab);
	//m_pTerrainTab->SetWindowPos(NULL, 0, 25, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pMeshTab.SetWindowPos(NULL, 0, 20, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	//m_pwndShow = &m_tab1dialog;

	m_pNaviTab.Create(IDD_DIALOG3, &m_mainTab);
	//m_pTerrainTab->SetWindowPos(NULL, 0, 25, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	m_pNaviTab.SetWindowPos(NULL, 0, 20, Rect.Width() - 10, Rect.Height() - 30, SWP_SHOWWINDOW | SWP_NOZORDER);
	//m_pwndShow = &m_tab1dialog;

	


	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_pwndShow != NULL)
	{
		m_pwndShow->ShowWindow(SW_HIDE);
		//m_pwndShow = NULL;
	}
	int itabIndex = m_mainTab.GetCurSel();
	switch (itabIndex)
	{
	case 0:
		m_pwndShow = &m_pTerrainTab;
		m_pTerrainTab.ShowWindow(SW_SHOW);
		m_pMeshTab.ShowWindow(SW_HIDE);
		m_pNaviTab.ShowWindow(SW_HIDE);

		break;

	case 1:
		m_pwndShow = &m_pMeshTab;
		m_pMeshTab.ShowWindow(SW_SHOW);
		m_pTerrainTab.ShowWindow(SW_HIDE);
		m_pNaviTab.ShowWindow(SW_HIDE);

		break;
	case 2:
		m_pNaviTab.ShowWindow(SW_SHOW);
		m_pTerrainTab.ShowWindow(SW_HIDE);
		m_pMeshTab.ShowWindow(SW_HIDE);

		break;
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
