#pragma once


// CTab_Navi 대화 상자

class CTab_Navi : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Navi)

public:
	CTab_Navi(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab_Navi();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton_Delete();
	CTreeCtrl m_ctrl_NaviTree;
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton_Save();
	afx_msg void OnBnClickedButton_Load();
	afx_msg void OnBnClickedButton_Update();
	afx_msg void OnBnClickedRadio1_Select_CelltoPoint();


	afx_msg void OnBnClickedButton_PosEdit();
	
	
	CButton m_Radio1;
	int m_iRadio;
	int m_iRadio_WP=0;
	int cellindex = -1;
	int childindex = -1;
	CEdit m_edit_CellIndex;
	CEdit m_edit_X;
	CEdit m_edit_Y;
	CEdit m_edit_Z;


	afx_msg void OnBnClickedButton_WayPoint_Add();
	afx_msg void OnBnClickedButton_WayPoint_Delete();
	CTreeCtrl m_ctrl_WayPointTree;
	afx_msg void OnTvnSelchangedTree_WayPoint(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadio5();
	CButton m_Radio_WP;
	afx_msg void OnBnClickedButton_WP_Save();
	afx_msg void OnBnClickedButton_WP_Load();
	CEdit m_edit_WayPoint;
};

