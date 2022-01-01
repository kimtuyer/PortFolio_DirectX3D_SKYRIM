#pragma once
#include "Export_Function.h"


// CTab_Terrain 대화 상자

class CTab_Terrain : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Terrain)

public:
	CTab_Terrain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	int m_XCount, m_ZCount ,m_Itv= 0;
	CManagement*			m_pManagementClass = nullptr;
	BYTE m_byDrawID;


public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	//CEdit m_Itv;
	afx_msg void OnEnChangeEdit4();
	CListBox m_TextureListBox;
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedSaved();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnDeltaposSpin_Height(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_Slider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit m_VtxSize;
	afx_msg void OnEnChangeEdit_SideCtrl();
	int m_RadioVtxedit;
	CEdit m_Edit_Height;
	CSpinButtonCtrl m_Spin_Height;
};
