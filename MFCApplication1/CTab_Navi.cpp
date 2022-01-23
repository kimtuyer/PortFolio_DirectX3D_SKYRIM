// CTab_Navi.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "CTab_Navi.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "FileInfo.h"
#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#include "CForm.h"

#include "MFC_ColliderSphere.h"
// CTab_Navi 대화 상자

IMPLEMENT_DYNAMIC(CTab_Navi, CDialogEx)

CTab_Navi::CTab_Navi(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_iRadio(0)
{

}

CTab_Navi::~CTab_Navi()
{
}

void CTab_Navi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrl_NaviTree);
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Radio(pDX, IDC_RADIO1, m_iRadio);
	//DDX_Radio(pDX, IDC_RADIO3, m_iRadio_WP);

	DDX_Control(pDX, IDC_EDIT13, m_edit_CellIndex);
	DDX_Control(pDX, IDC_EDIT8, m_edit_X);
	DDX_Control(pDX, IDC_EDIT9, m_edit_Y);
	DDX_Control(pDX, IDC_EDIT10, m_edit_Z);
	DDX_Control(pDX, IDC_TREE3, m_ctrl_WayPointTree);
	DDX_Control(pDX, IDC_RADIO5, m_Radio_WP);
	DDX_Control(pDX, IDC_EDIT14, m_edit_WayPoint);
}


BEGIN_MESSAGE_MAP(CTab_Navi, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Navi::OnBnClickedButton_Delete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTab_Navi::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON9, &CTab_Navi::OnBnClickedButton_Save)
	ON_BN_CLICKED(IDC_BUTTON10, &CTab_Navi::OnBnClickedButton_Load)
	ON_BN_CLICKED(IDC_BUTTON2, &CTab_Navi::OnBnClickedButton_Update)
	ON_BN_CLICKED(IDC_RADIO1, &CTab_Navi::OnBnClickedRadio1_Select_CelltoPoint)
	ON_BN_CLICKED(IDC_BUTTON11, &CTab_Navi::OnBnClickedButton_PosEdit)
	ON_BN_CLICKED(IDC_BUTTON13, &CTab_Navi::OnBnClickedButton_WayPoint_Add)
	ON_BN_CLICKED(IDC_BUTTON14, &CTab_Navi::OnBnClickedButton_WayPoint_Delete)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE3, &CTab_Navi::OnTvnSelchangedTree_WayPoint)
	//ON_BN_CLICKED(IDC_RADIO5, &CTab_Navi::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_BUTTON15, &CTab_Navi::OnBnClickedButton_WP_Save)
	ON_BN_CLICKED(IDC_BUTTON16, &CTab_Navi::OnBnClickedButton_WP_Load)
END_MESSAGE_MAP()


// CTab_Navi 메시지 처리기


void CTab_Navi::OnBnClickedButton_Delete()
{

	UpdateData(TRUE);
	//int iIndex = m_DynamicList.GetCurSel();
	
	HTREEITEM hItem = m_ctrl_NaviTree.GetSelectedItem();
	CString str_Mesh = m_ctrl_NaviTree.GetItemText(hItem);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

	pView->m_pNaviMesh->MFC_PopCell();
	m_ctrl_NaviTree.DeleteItem(hItem);


	//pView->m_vecSphere.back()->Release();
	//pView->m_vecSphere.pop_back();

	pView->m_iCellindex--;

	//if (pView->m_iCellindex < 0)
	//{
	//	pView->m_iCellindex = 0;
	//	for (auto &p : pView->m_vecSphere)
	//	{
	//		p->Release();
	//		
	//	}
	//	pView->m_vecSphere.clear();
	//	
	//}

	
	//while (hChild)
	//{
	//	if (hChild == hItem)break;
	//	hChild = m_ctrlTree.GetNextItem(hChild, TVGN_NEXT);
	//	++index;
	//}








	UpdateData(FALSE);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Navi::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	//child 역시 인식 가능!
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	CString str_cindex = m_ctrl_NaviTree.GetItemText(selectItem);
	//별도 적용 버튼 만들어, 입력시 선택된 인덱스의 수치값을 가져와
	//다시 Set_

	int index = _ttoi(str_cindex);


	//for (int i = 0; i < pView->m_iCellindex; ++i)
	//{
	//	pView->m_pNaviMesh->Get_Cell(i)->Set_color({ 255,0,0,0.8 });

	if (m_iRadio==0)
	{



		pView->m_pNaviMesh->Get_Cell(index)->Set_color({ 255,255,255,1 });
		//cellindex = -1;
		//childindex = -1;

	}
	else if(m_iRadio==1) //각 셀의 특정 인덱스 번호를 선택할시 
	{
		CString index;
		m_edit_CellIndex.GetWindowTextW(index);
		 cellindex = _ttoi(index);

		HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
		CString str_cindex = m_ctrl_NaviTree.GetItemText(selectItem); //자식의 인덱스 번호
		 childindex = _ttoi(str_cindex);
		//float fValue = _tstof(strValue);


		CString cPoint;
		_vec3 vPoint;

		switch (childindex)
		{
		case CCell::POINT::POINT_A:
			vPoint=pView->m_pNaviMesh->Get_Cell(cellindex)->MFC_Get_Point(CCell::POINT::POINT_A);
			
			cPoint.Format(_T("%f"), vPoint.x);
			m_edit_X.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.y);
			m_edit_Y.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.z);
			m_edit_Z.SetWindowTextW(cPoint);


			break;
		case CCell::POINT::POINT_B:
			vPoint=pView->m_pNaviMesh->Get_Cell(cellindex)->MFC_Get_Point(CCell::POINT::POINT_B);

			cPoint.Format(_T("%f"), vPoint.x);
			m_edit_X.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.y);
			m_edit_Y.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.z);
			m_edit_Z.SetWindowTextW(cPoint);


			break;
		case CCell::POINT::POINT_C:
			vPoint=pView->m_pNaviMesh->Get_Cell(cellindex)->MFC_Get_Point(CCell::POINT::POINT_C);

			cPoint.Format(_T("%f"), vPoint.x);
			m_edit_X.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.y);
			m_edit_Y.SetWindowTextW(cPoint);

			cPoint.Format(_T("%f"), vPoint.z);
			m_edit_Z.SetWindowTextW(cPoint);

			break;
		default:
			break;
		}


		//HTREEITEM Root = m_ctrl_NaviTree.GetItem()

	}





	//}



	






	UpdateData(FALSE);

}


BOOL CTab_Navi::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTab_Navi::OnBnClickedButton_Save()
{
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 

	TCHAR szCurDir[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

		vector<CellInfo*>vecCellObject = pView->m_vecPoint;

		DWORD dwByte = 0;
		for (auto& pTile : vecCellObject)
			WriteFile(hFile, pTile, sizeof(CellInfo), &dwByte, nullptr);

		CloseHandle(hFile);
	}


}


void CTab_Navi::OnBnClickedButton_Load()
{
	CFileDialog Dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	//D:\박병건\118C\D2D\Framework_v1
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));
		CForm* pForm2 = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));


		
		DWORD dwByte = 0;
		int pTileCount = 0;
		CellInfo* pTile = nullptr;

		pView->m_pNaviMesh->MFC_ReleaseCell();
		pView->m_vecPoint.clear();
		pView->m_vecMousePos.clear(); //이전에 찍어서 가진 정보 있을경우 clear


		while (true)
		{
			pTile = new CellInfo;
			ReadFile(hFile, pTile, sizeof(CellInfo), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}

			pView->m_iCellindex = pTileCount;

			


			pView->m_vecPoint.push_back(pTile);

			CCell* pCell= CCell::Create(pView->m_pGraphicDev, pTile->CellIndex, &pView->m_vecPoint[pTileCount]->vPoint[0], &pView->m_vecPoint[pTileCount]->vPoint[1], &pView->m_vecPoint[pTileCount]->vPoint[2]);
			pView->m_pNaviMesh->Insert_Sell(pCell);


			//for (int i = 0; i < 3; i++)
			//{
			//	CMFC_ColliderSphere* pSphere = CMFC_ColliderSphere::Create(pView->m_pGraphicDev, &pTile->vPoint[i], 100, 100);
			//	pSphere->Set_Pos(pTile->vPoint[i]);
			//	pView->m_vecSphere.emplace_back(pSphere);
			//}
		




			CString a;
			a.Format(_T("%d"), pTileCount);

			HTREEITEM hRoot;
			hRoot = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(a, TVI_ROOT, TVI_LAST);


			HTREEITEM hChild;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"0_index", hRoot, TVI_LAST);
			//HTREEITEM hStatic;
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"1_index", hRoot, TVI_LAST);
			hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"2_index", hRoot, TVI_LAST);
		
			
			++pTileCount;

		}
		
		pView->m_iCellindex++;
		pView->Invalidate(FALSE);
		CloseHandle(hFile);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.}
}

void CTab_Navi::OnBnClickedButton_Update()
{

	UpdateData(TRUE);





	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM Parent, Child;
	Parent = m_ctrl_NaviTree.GetRootItem();
	m_ctrl_NaviTree.Expand(Parent, TVE_EXPAND);

	//Child = m_ctrl_NaviTree.GetNextItem(Parent, TVGN_CHILD);
	//m_ctrl_NaviTree.Expand(Child, TVE_EXPAND);


	while (Parent != NULL)
	{
		Parent = m_ctrl_NaviTree.GetNextSiblingItem(Parent);
		CString item= m_ctrl_NaviTree.GetItemText(Parent);

		m_ctrl_NaviTree.Expand(Parent, TVE_EXPAND);


		//Child = m_ctrl_NaviTree.GetNextItem(Child, TVGN_NEXT);
		//m_ctrl_NaviTree.Expand(Child, TVE_EXPAND);
	}


	UpdateData(FALSE);


}


void CTab_Navi::OnBnClickedRadio1_Select_CelltoPoint()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);


	
	


	UpdateData(FALSE);

}


void CTab_Navi::OnBnClickedButton_PosEdit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

	UpdateData(TRUE);

	
	if (m_iRadio == 1) //
	{
		CString cPoint;
		_vec3 vPoint;

		switch (childindex)
		{	
		case CCell::POINT::POINT_A:
			

			m_edit_X.GetWindowTextW(cPoint);
			vPoint.x = _ttof(cPoint);

			m_edit_Y.GetWindowTextW(cPoint);
			vPoint.y = _ttof(cPoint);

			m_edit_Z.GetWindowTextW(cPoint);
			vPoint.z = _ttof(cPoint);

			pView->m_pNaviMesh->Set_PointPos(cellindex, CCell::POINT::POINT_A, vPoint);
			pView->m_vecPoint[cellindex]->CellIndex = cellindex;
			pView->m_vecPoint[cellindex]->vPoint[CCell::POINT::POINT_A]= vPoint;
			


			break;
		case CCell::POINT::POINT_B:

			m_edit_X.GetWindowTextW(cPoint);
			vPoint.x = _ttof(cPoint);

			m_edit_Y.GetWindowTextW(cPoint);
			vPoint.y = _ttof(cPoint);

			m_edit_Z.GetWindowTextW(cPoint);
			vPoint.z = _ttof(cPoint);

			pView->m_pNaviMesh->Set_PointPos(cellindex, CCell::POINT::POINT_B, vPoint);
			pView->m_vecPoint[cellindex]->CellIndex = cellindex;
			pView->m_vecPoint[cellindex]->vPoint[CCell::POINT::POINT_B] = vPoint;
			break;
		case CCell::POINT::POINT_C:

			m_edit_X.GetWindowTextW(cPoint);
			vPoint.x = _ttof(cPoint);

			m_edit_Y.GetWindowTextW(cPoint);
			vPoint.y = _ttof(cPoint);

			m_edit_Z.GetWindowTextW(cPoint);
			vPoint.z = _ttof(cPoint);

			pView->m_pNaviMesh->Set_PointPos(cellindex, CCell::POINT::POINT_C, vPoint);
			pView->m_vecPoint[cellindex]->CellIndex = cellindex;
			pView->m_vecPoint[cellindex]->vPoint[CCell::POINT::POINT_C] = vPoint;
			break;
		default:
			break;
		}



	}
	UpdateData(FALSE);

}


void CTab_Navi::OnBnClickedButton_WayPoint_Add()
{
	UpdateData(TRUE);
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString index;
	int wayindex = 0;
	m_edit_WayPoint.GetWindowTextW(index);
	wayindex = _ttoi(index);

	pView->m_pNaviMesh->Get_Cell(wayindex)->Set_color({ 255,0,255,1 });

	
	_vec3 A = pView->m_pNaviMesh->Get_Cell(wayindex)->MFC_Get_Point(CCell::POINT_A);
	_vec3 B = pView->m_pNaviMesh->Get_Cell(wayindex)->MFC_Get_Point(CCell::POINT_B);
	_vec3 C = pView->m_pNaviMesh->Get_Cell(wayindex)->MFC_Get_Point(CCell::POINT_C);

	_vec3 MidPoint = { (A.x + B.x + C.x) / 3  ,( A.y + B.y + C.y) / 3  ,( A.z + B.z + C.z )/ 3 };
	
	int size = pView->m_vecWayPoint.size();
	pView->m_vecWayPoint.push_back(WayPointInfo(MidPoint, size, wayindex));

	//pView->m_vecWayPointIndex.push_back(wayindex);


	CString b;
	b.Format(_T("%d"), wayindex);

	HTREEITEM hRoot2;
	hRoot2 = m_ctrl_WayPointTree.InsertItem(b, TVI_ROOT, TVI_LAST);



	UpdateData(FALSE);

}


void CTab_Navi::OnBnClickedButton_WayPoint_Delete()
{
	UpdateData(TRUE);
	//int iIndex = m_DynamicList.GetCurSel();

	HTREEITEM hItem = m_ctrl_WayPointTree.GetSelectedItem();
	CString str_Mesh = m_ctrl_WayPointTree.GetItemText(hItem);

	//HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	//CString str_cindex = m_ctrl_WayPointTree.GetItemText(selectItem);
	//별도 적용 버튼 만들어, 입력시 선택된 인덱스의 수치값을 가져와
	//다시 Set_

	int index = _ttoi(str_Mesh);

	
	//int index = 0;
	//str_Mesh.Format(_T("%d"), index);


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));




	vector<Engine::WayPointInfo>::iterator iter = pView->m_vecWayPoint.begin();
	for (int i=0; iter != pView->m_vecWayPoint.end(); i++)
	{
		if (index == (*iter).CellIndex)
		{
			iter=pView->m_vecWayPoint.erase(iter);
		}
		else
		{
			iter++;
		}


	}
	

	m_ctrl_WayPointTree.DeleteItem(hItem);
	pView->m_pNaviMesh->Get_Cell(index)->Set_color({ 255,255,255,1 });











	UpdateData(FALSE);
}


void CTab_Navi::OnTvnSelchangedTree_WayPoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	//child 역시 인식 가능!
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	CString str_cindex = m_ctrl_WayPointTree.GetItemText(selectItem);
	//별도 적용 버튼 만들어, 입력시 선택된 인덱스의 수치값을 가져와
	//다시 Set_

	int index = _ttoi(str_cindex);


	//for (int i = 0; i < pView->m_iCellindex; ++i)
	//{
	//	pView->m_pNaviMesh->Get_Cell(i)->Set_color({ 255,0,0,0.8 });

	if (m_iRadio == 0)
	{



		pView->m_pNaviMesh->Get_Cell(index)->Set_color({ 255,255,255,1 });
		//cellindex = -1;
		//childindex = -1;

	}
	//else if (m_iRadio == 1) //각 셀의 특정 인덱스 번호를 선택할시 
	//{
	//	CString index;
	//	m_edit_CellIndex.GetWindowTextW(index);
	//	cellindex = _ttoi(index);
	//
	//	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	//	CString str_cindex = m_ctrl_NaviTree.GetItemText(selectItem); //자식의 인덱스 번호
	//	childindex = _ttoi(str_cindex);
	//	//float fValue = _tstof(strValue);
	//
	//
	//	CString cPoint;
	//	_vec3 vPoint;
	//
	//	
	//
	//
	//	//HTREEITEM Root = m_ctrl_NaviTree.GetItem()
	//
	//}





	//}










	UpdateData(FALSE);
}


void CTab_Navi::OnBnClickedRadio5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Navi::OnBnClickedButton_WP_Save()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 

	TCHAR szCurDir[MAX_PATH]{};

	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");

	Dlg.m_ofn.lpstrInitialDir = szCurDir;
	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;

		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

		vector<WayPointInfo>vecCellObject = pView->m_vecWayPoint;

		DWORD dwByte = 0;
		for (auto& pTile : vecCellObject)
			WriteFile(hFile, &pTile, sizeof(WayPointInfo), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CTab_Navi::OnBnClickedButton_WP_Load()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, // 다른이름으로 저장. 만약 TRUE 파일 열기. 
		L"dat",// 디폴트 확장자 
		L"*.dat",// 디폴트 파일 이름 
		OFN_OVERWRITEPROMPT);// 덮어쓸때 경고 메시지 띄어주겠다. 
	TCHAR szCurDir[MAX_PATH]{};
	GetCurrentDirectory(MAX_PATH, szCurDir);
	PathRemoveFileSpec(szCurDir);
	lstrcat(szCurDir, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szCurDir;

	if (IDOK == Dlg.DoModal())
	{
		CString wstrFilePath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(wstrFilePath.GetString(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;
		CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
		CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));
		CForm* pForm2 = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));



		DWORD dwByte = 0;
		int pTileCount = 0;
		//int pTile = 0;

		pView->m_vecWayPoint.clear();
		//pView->m_pNaviMesh->MFC_ReleaseCell();
		//pView->m_vecPoint.clear();
		//pView->m_vecMousePos.clear(); //이전에 찍어서 가진 정보 있을경우 clear


		while (true)
		{
			_tagWayPointInfo pTile;
			ReadFile(hFile, &pTile, sizeof(WayPointInfo), &dwByte, nullptr);

			if (0 == dwByte)
			{
				//Safe_Delete(pTile);
				break;
			}

			//pView->m_iCellindex = pTileCount;

			pView->m_vecWayPoint.push_back(pTile);;
			pView->m_pNaviMesh->Get_Cell(pTile.CellIndex)->Set_color({ 255,0,255,1 });

		
			CString a;
			a.Format(_T("%d"), pTile.CellIndex);
				
			HTREEITEM hRoot;
			hRoot = pForm2->m_pNaviTab.m_ctrl_WayPointTree.InsertItem(a, TVI_ROOT, TVI_LAST);


			//HTREEITEM hChild;
			//hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"0_index", hRoot, TVI_LAST);
			////HTREEITEM hStatic;
			//hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"1_index", hRoot, TVI_LAST);
			//hChild = pForm2->m_pNaviTab.m_ctrl_NaviTree.InsertItem(L"2_index", hRoot, TVI_LAST);


			++pTileCount;

		}

		//pView->m_iCellindex++;
		pView->Invalidate(FALSE);
		CloseHandle(hFile);
	}
}
