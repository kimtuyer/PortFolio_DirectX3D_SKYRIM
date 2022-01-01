#pragma once


// CTab_Mesh 대화 상자

class CTab_Mesh : public CDialogEx
{
	DECLARE_DYNAMIC(CTab_Mesh)

public:
	CTab_Mesh(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab_Mesh();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	float m_Posx, m_Posy, m_Posz = 0;
	float m_RotX, m_RotY, m_RotZ = 0;

	float m_ScaleX, m_ScaleY, m_ScaleZ = 0;

public:
	vector<ObjectInfo*> m_vecObject;



public:
	CTreeCtrl m_ctrlTree;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_StaticList;
	afx_msg void OnLbnSelchangeDynamicList();
	CListBox m_DynamicList;
	afx_msg void OnLbnSelchangeStaticList();
	afx_msg void OnBnClickedButton_Pos();
	afx_msg void OnBnClickedButton_StaticListDelete();
	afx_msg void OnBnClickedButton_DynamicListDelete();
	afx_msg void OnBnClickedButton_SAVE();
	afx_msg void OnBnClickedButton_LOAD();
	CTreeCtrl m_ctrlTree_Static;
	afx_msg void OnTvnSelchangedTree2_Static(NMHDR *pNMHDR, LRESULT *pResult);
	int m_Radio_Object;
	afx_msg void OnBnClickedButton1_Update();
};
