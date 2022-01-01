// CTab_Mesh.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "CTab_Mesh.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "FileInfo.h"
#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include "Export_Function.h"
#include "Scene1.h"

#include "CForm.h"

#include "MFC_ColliderSphere.h"


#include "CStatic_Object.h"
#include "Stone.h"
#include "Monster_draugr.h"
// CTab_Mesh 대화 상자

IMPLEMENT_DYNAMIC(CTab_Mesh, CDialogEx)

CTab_Mesh::CTab_Mesh(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
	, m_Radio_Object(0)
{

}

CTab_Mesh::~CTab_Mesh()
{
}

void CTab_Mesh::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);
	DDX_Control(pDX, IDC_LIST2, m_StaticList);
	DDX_Control(pDX, IDC_LIST3, m_DynamicList);
	DDX_Control(pDX, IDC_TREE2, m_ctrlTree_Static);
	DDX_Radio(pDX, IDC_RADIO3, m_Radio_Object);
}


BEGIN_MESSAGE_MAP(CTab_Mesh, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CTab_Mesh::OnTvnSelchangedTree1)
	ON_EN_CHANGE(IDC_EDIT1, &CTab_Mesh::OnEnChangeEdit1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CTab_Mesh::OnDeltaposSpin4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CTab_Mesh::OnDeltaposSpin5)
	ON_LBN_SELCHANGE(IDC_LIST3, &CTab_Mesh::OnLbnSelchangeDynamicList)
	ON_LBN_SELCHANGE(IDC_LIST2, &CTab_Mesh::OnLbnSelchangeStaticList)
	ON_BN_CLICKED(IDC_BUTTON4, &CTab_Mesh::OnBnClickedButton_Pos)
	ON_BN_CLICKED(IDC_BUTTON7, &CTab_Mesh::OnBnClickedButton_StaticListDelete)
	ON_BN_CLICKED(IDC_BUTTON8, &CTab_Mesh::OnBnClickedButton_DynamicListDelete)
	ON_BN_CLICKED(IDC_BUTTON5, &CTab_Mesh::OnBnClickedButton_SAVE)
	ON_BN_CLICKED(IDC_BUTTON6, &CTab_Mesh::OnBnClickedButton_LOAD)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CTab_Mesh::OnTvnSelchangedTree2_Static)
	ON_BN_CLICKED(IDC_BUTTON12, &CTab_Mesh::OnBnClickedButton1_Update)
END_MESSAGE_MAP()


// CTab_Mesh 메시지 처리기


BOOL CTab_Mesh::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	HTREEITEM hRoot;
	hRoot = m_ctrlTree.InsertItem(L"Texture", TVI_ROOT, TVI_LAST);
	

	HTREEITEM hChild2;
	hChild2 = m_ctrlTree.InsertItem(L"Dynamic", hRoot, TVI_LAST);
	HTREEITEM hDynastic1;
	hDynastic1 = m_ctrlTree.InsertItem(L"Player", hChild2, TVI_LAST);
	HTREEITEM hDynastic2;
	hDynastic2 = m_ctrlTree.InsertItem(L"Draugr", hChild2, TVI_LAST);

	HTREEITEM hDynastic21;
	hDynastic21 = m_ctrlTree.InsertItem(L"DraugrPriest", hChild2, TVI_LAST);

	HTREEITEM hDynastic3;
	hDynastic3 = m_ctrlTree.InsertItem(L"Giant", hChild2, TVI_LAST);

	HTREEITEM hDynastic4;
	hDynastic4 = m_ctrlTree.InsertItem(L"Dragon", hChild2, TVI_LAST);



	
	HTREEITEM hRoot2;
	hRoot = m_ctrlTree_Static.InsertItem(L"Texture", TVI_ROOT, TVI_LAST);
	HTREEITEM hChild;
	hChild = m_ctrlTree_Static.InsertItem(L"Static", hRoot, TVI_LAST);
	HTREEITEM hStatic;
	//hStatic = m_ctrlTree_Static.InsertItem(L"Nordic_Bridge", hChild, TVI_LAST);

	CStdioFile tip_file;

	CString Proto_name, str, Xname;

	if (tip_file.Open(L"Whiterun.txt", CFile::modeRead | CFile::typeText))
	{

		while (tip_file.ReadString(str))
		{
			Proto_name += str;

			//Xname += Proto_name;
			
			hStatic = m_ctrlTree_Static.InsertItem(Proto_name, hChild, TVI_LAST);

			Proto_name.Empty();
			str.Empty();

		}

		tip_file.Close();
	}




	//hStatic = m_ctrlTree_Static.InsertItem(L"House0", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"House1", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"House2", hChild, TVI_LAST);
	//
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock0", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock1", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock2", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock3", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock4", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Rock5", hChild, TVI_LAST);
	//
	//hStatic = m_ctrlTree_Static.InsertItem(L"Staris0", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Staris1", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Staris2", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Staris3", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Staris4", hChild, TVI_LAST);
	//
	//hStatic = m_ctrlTree_Static.InsertItem(L"Tree0", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Tree1", hChild, TVI_LAST);
	//hStatic = m_ctrlTree_Static.InsertItem(L"Tree2", hChild, TVI_LAST);
	//
	//
	//hStatic = m_ctrlTree_Static.InsertItem(L"Whiterun", hChild, TVI_LAST);















	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTab_Mesh::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int index = 0;

	HTREEITEM hItem = m_ctrlTree.GetSelectedItem();
	HTREEITEM hChild = m_ctrlTree.GetChildItem(NULL);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));



	while (hChild)
	{
		if (hChild == hItem)break;
		hChild = m_ctrlTree.GetNextItem(hChild, TVGN_NEXT);
		++index;
	}

	CString str_Mesh = m_ctrlTree.GetItemText(pNMTreeView->itemNew.hItem);

	if (str_Mesh == "Draugr")

	{

		dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(1,OBJECT_DRAUGR);
		m_DynamicList.AddString(L"Draugr");

	
	}

	else if (str_Mesh == "Player")
		dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(1,OBJECT_PLAYER);


	UpdateData(TRUE);
//	int iIndex = m_TextureListBox.GetCurSel();


	


	//CComponent* pTerrainTexture = pView->m_pManagementClass->Get_Component(L"GameLogic", L"Terrain", L"Com_Texture", ID_DYNAMIC);
	//dynamic_cast<CTexture*>(pTerrainTexture)->Ready_Texture(str_Mesh, TYPE_NORMAL, 1);
	//dynamic_cast<CTexture*>(pTerrainTexture)->Set_TextureNumber(iIndex);



	UpdateData(FALSE);





}


void CTab_Mesh::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Mesh::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CTab_Mesh::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CTab_Mesh::OnLbnSelchangeDynamicList()
{
	UpdateData(TRUE);
	int iIndex = m_DynamicList.GetCurSel();
	CString wstrFileName;
	m_DynamicList.GetText(iIndex, wstrFileName);
	
	CString str = _T("");
	GetDlgItemText(IDC_EDIT10, str);
	m_Posx = _ttoi(str);

	CString str2 = _T("");
	GetDlgItemText(IDC_EDIT11, str2);
	m_Posy = _ttoi(str2);

	CString str3 = _T("");
	GetDlgItemText(IDC_EDIT12, str3);
	m_Posz = _ttoi(str3);

	_vec3 vPos = { m_Posx,m_Posy,m_Posz };



	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

	//인덱스 숫자에 맞게 오브젝트 리스트[index]값 맞추어서 해당 넘버의 오브젝트를 불러와야함.
	if (wstrFileName == "Draugr")
	{
		int i = 0;
		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"GameLogic");
		list<CGameObject*> pList =pLayer->Get_ObjectList(OBJECT_DRAUGR);

		list<CGameObject*>::iterator iter = pList.begin();

		for (; iter != pList.end(); ++i)
			if (i == iIndex)
				dynamic_cast<CMonster_draugr*>(*iter)->Get_Transform()->Set_Pos(&vPos);
			else
				++iter;




		//dynamic_cast<CMonster_draugr*>(pObject)->Get_Transform()->Set_Pos(&vPos);
	}



	UpdateData(FALSE);

}


void CTab_Mesh::OnLbnSelchangeStaticList()
{
	UpdateData(TRUE);
	
	int iIndex = m_StaticList.GetCurSel();
	CString wstrFileName;
	m_StaticList.GetText(iIndex, wstrFileName);


	_vec3 vPos = {};




	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	//if (wstrFileName == "Nordic_Bridge")
	{
		int i = 0;
		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();

		for (; iter != pList.end(); ++i)
			if (i == iIndex)
			{
				//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(&vPos);
				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Get_Info(INFO::INFO_POS, &vPos);

				CString cPoint;
				cPoint.Format(_T("%f"), vPos.x);
				SetDlgItemText(IDC_EDIT10, cPoint);

				cPoint.Format(_T("%f"), vPos.y);
				SetDlgItemText(IDC_EDIT11, cPoint);

				cPoint.Format(_T("%f"), vPos.z);
				SetDlgItemText(IDC_EDIT12, cPoint);



				_vec3 vScale =
					dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->m_vScale;
				
				cPoint.Format(_T("%f"), vScale.x);
				SetDlgItemText(IDC_EDIT3, cPoint);

				cPoint.Format(_T("%f"), vScale.y);
				SetDlgItemText(IDC_EDIT5, cPoint);

				cPoint.Format(_T("%f"), vScale.z);
				SetDlgItemText(IDC_EDIT6, cPoint);

				_vec3 vRot =
					dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->m_vAngle;

				cPoint.Format(_T("%f"), vRot.x);
				SetDlgItemText(IDC_EDIT7, cPoint);

				cPoint.Format(_T("%f"), vRot.y);
				SetDlgItemText(IDC_EDIT8, cPoint);

				cPoint.Format(_T("%f"), vRot.z);
				SetDlgItemText(IDC_EDIT9, cPoint);




			}
			else
				++iter;




		//dynamic_cast<CMonster_draugr*>(pObject)->Get_Transform()->Set_Pos(&vPos);
	}





	

	UpdateData(FALSE);

}


void CTab_Mesh::OnBnClickedButton_Pos()
{

	UpdateData(TRUE);

	int iIndex = m_StaticList.GetCurSel();
	CString wstrFileName;
	m_StaticList.GetText(iIndex, wstrFileName);
	int i = 0;
	//for (; i < wstrFileName.GetLength(); ++i)
	//{
	//	// isdigit 0~ 9까지의 숫자인지 아닌지를 판별해주는 함수. 
	//	if (isdigit(wstrFileName[i]))
	//		break;
	//}
	//wstrFileName.Delete(0, i);
	//m_byDrawID = _ttoi(wstrFileName.GetString());

	CString str = _T("");
	GetDlgItemText(IDC_EDIT10, str);
	m_Posx = _ttof(str);

	CString str2 = _T("");
	GetDlgItemText(IDC_EDIT11, str2);
	m_Posy = _ttof(str2);

	CString str3 = _T("");
	GetDlgItemText(IDC_EDIT12, str3);
	m_Posz = _ttof(str3);

	_vec3 vPos = { m_Posx,m_Posy,m_Posz };


	GetDlgItemText(IDC_EDIT7, str);
	m_RotX = _ttof(str);

	GetDlgItemText(IDC_EDIT8, str2);
	m_RotY = _ttof(str2);

	GetDlgItemText(IDC_EDIT9, str3);
	m_RotZ = _ttof(str3);

	_vec3 vRot = { m_RotX,m_RotY,m_RotZ };


	GetDlgItemText(IDC_EDIT3, str);
	m_ScaleX = _ttof(str);

	GetDlgItemText(IDC_EDIT5, str2);
	m_ScaleY = _ttof(str2);

	GetDlgItemText(IDC_EDIT6, str3);
	m_ScaleZ = _ttof(str3);

	_vec3 vScale = { m_ScaleX,m_ScaleY,m_ScaleZ };




	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

	//인덱스 숫자에 맞게 오브젝트 리스트[index]값 맞추어서 해당 넘버의 오브젝트를 불러와야함.
	//sring 값 int 변환후 오브젝트 리스트 순회하면서 해당 값과 일치하는 오브젝트 수정!
	//if (wstrFileName == "Nordic_Bridge")
	{
		int i = 0;
		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();

		for (; iter != pList.end(); ++i)
			if (i == iIndex)
			{

				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(&vPos);
				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Scale(m_ScaleX, m_ScaleY, m_ScaleZ);
				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_X, m_RotX);
				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Y, m_RotY);
				dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Z, m_RotZ);

				m_vecObject[i]->vPos = vPos;
				m_vecObject[i]->vScale = vScale;
				m_vecObject[i]->vRot = vRot;
				break;

			}

			else
				++iter;




		//dynamic_cast<CMonster_draugr*>(pObject)->Get_Transform()->Set_Pos(&vPos);
	}

	UpdateData(FALSE);

}


void CTab_Mesh::OnBnClickedButton_StaticListDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	int iIndex = m_StaticList.GetCurSel();
	CString wstrFileName;
	//m_StaticList.GetText(iIndex, wstrFileName);


	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));



	

	//for (int i = 0; i < m_vecObject.size(); i++)
	//{
	//
	//	if (i == iIndex)
	//	{
	//
	//
	//		m_vecObject.pop_back();
	//	}
	//
	//}


	//if (wstrFileName == "Nordic_Bridge")
	
		int i = 0;
		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();
		for (; iter != pList.end(); ++i)
			if (i == iIndex)
			{

				(*iter)->Set_State(1);
				//Safe_Release(*iter);
				pList.erase(iter++);
				//pList.p

			}
			else
				++iter;			


		m_vecObject.erase(m_vecObject.begin()+iIndex);

			
		
	m_StaticList.DeleteString(m_StaticList.GetCurSel());
	UpdateData(FALSE);

}


void CTab_Mesh::OnBnClickedButton_DynamicListDelete()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int iIndex = m_DynamicList.GetCurSel();
	CString wstrFileName;
	m_DynamicList.GetText(iIndex, wstrFileName);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	if (wstrFileName == "Draugr")
	{
		int i = 0;
		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"GameLogic");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_DRAUGR);

		list<CGameObject*>::iterator iter = pList.begin();
		for (; iter != pList.end(); ++i)
			if (i == iIndex)
			{
				(*iter)->Set_State(1);
				//Safe_Release(*iter);
				pList.erase(iter++);
				//pList.p

			}
			else
				++iter;




		//dynamic_cast<CMonster_draugr*>(pObject)->Get_Transform()->Set_Pos(&vPos);
	}

	m_DynamicList.DeleteString(m_DynamicList.GetCurSel());
	
	
	
	
	
	
	
	
	UpdateData(FALSE);




}


void CTab_Mesh::OnBnClickedButton_SAVE()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

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
			


		DWORD dwByte = 0;

		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();
		_vec3 Info = {};


		//for (auto&p : m_vecObject)
		//{
		//	delete p;
		//}
		//m_vecObject.clear();
		//m_vecObject.resize(pList.size());

		for (int i=0; iter != pList.end(); i++,iter++)
		{
			m_vecObject[i]->vRot.y = dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->m_vAngle.y;
			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Get_Info(INFO::INFO_POS, &Info);
			m_vecObject[i]->vPos = Info;
				
		}

		

			


		for (auto& pTile : m_vecObject)
			WriteFile(hFile, pTile, sizeof(ObjectInfo), &dwByte, nullptr);

		CloseHandle(hFile);
	}
}


void CTab_Mesh::OnBnClickedButton_LOAD()
{
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
		ObjectInfo* pTile = nullptr;


		m_vecObject.clear();
		m_StaticList.ResetContent();


		CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
		list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);

		list<CGameObject*>::iterator iter = pList.begin();


		CStdioFile tip_file;

		CString Proto_name, str, Xname;

		for (; iter != pList.end();)
		{
				(*iter)->Set_State(1);
				pList.erase(iter++);

		}
				//Safe_Release(*iter);
				//pList.p
		int count = 0;
		while (true)
		{
			pTile = new ObjectInfo;
			ReadFile(hFile, pTile, sizeof(ObjectInfo), &dwByte, nullptr);

			if (0 == dwByte)
			{
				Safe_Delete(pTile);
				break;
			}
			
			



			if (pTile->Object_type == 0)// Static
			{

		
				if (tip_file.Open(L"Whiterun.txt", CFile::modeRead | CFile::typeText))
			{
					int index = 0;
					while (tip_file.ReadString(str))
				{

					if (index == pTile->Object_ID)
					{
						m_StaticList.AddString(str);
						break;
					}

					str.Empty();
					index++;
				}

				tip_file.Close();
			}

			}


			//_vec3 vPos = pTile->vPos;
			//_vec3 vScale = pTile->vScale; //{ 0, 0, 0 };
			//_vec3 vRot = pTile->vRot; //{ 0, 0, 0 };
			//
			
			m_vecObject.push_back(pTile);
			
			dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Load_Update_Scene(m_vecObject[pTileCount]);
			
			//
			//	CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
			//	list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);
			//	list<CGameObject*>::iterator iter = pList.begin();
			//	for (int i=0; iter != pList.end(); i++)	
			//	{
			//		if (i == pTileCount)
			//		{	
			//		
			//		
			//			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(m_vecObject[i]->vPos.x, m_vecObject[i]->vPos.y, m_vecObject[i]->vPos.z);
			//			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(&m_vecObject[i]->vPos);
			//			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Scale(pTile->vScale.x, pTile->vScale.y, pTile->vScale.z);
			//			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_X, pTile->vRot.x);
			//			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Y, m_vecObject[i]->vRot.y);
			//			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Z, pTile->vRot.z);
			//			
			//
			//		}
			//		else
			//			++iter;		
			//
			//	}
			//
			++pTileCount;

		}
			


	//	pView->m_iCellindex++;
		pView->Invalidate(FALSE);
		CloseHandle(hFile);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.}}
}

void CTab_Mesh::OnTvnSelchangedTree2_Static(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	//int index = 0;

	HTREEITEM hItem = m_ctrlTree_Static.GetSelectedItem();
	HTREEITEM hChild = m_ctrlTree_Static.GetChildItem(NULL);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));

	CString str_Mesh = m_ctrlTree_Static.GetItemText(pNMTreeView->itemNew.hItem);


	_vec3 vPos = {0,0,0};
	_vec3 vScale = {0.01,0.01,0.01};
	_vec3 vRot = {0,0,0};


	int index = _ttoi(str_Mesh);

	CStdioFile tip_file;

	CString Proto_name, str, Xname;

	int indexorder = 0;

	if (tip_file.Open(L"Whiterun.txt", CFile::modeRead | CFile::typeText))
	{

		while (tip_file.ReadString(str))
		{
			Proto_name += str;

			//Xname += Proto_name;
			if (str_Mesh == Proto_name)
			{
				dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, indexorder);
	//	
				m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot,0, indexorder));
	//

				m_StaticList.InsertString(m_vecObject.size()-1, str_Mesh);
				//m_StaticList.AddString(str_Mesh);
				break;
			}

			Proto_name.Empty();
			str.Empty();
			indexorder++;
		}

		tip_file.Close();
	}






	//if (str_Mesh == "Nordic_Bridge")
	//
	//
	//	dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, STATIC_OBJECT_ID::BRIDGE);
	//	
	//	m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot,0, STATIC_OBJECT_ID::BRIDGE));
	//
	//	m_StaticList.AddString(L"Nordic_Bridge");
	//
	//
	//if (str_Mesh == "House0")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, STATIC_OBJECT_ID::House0);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House0));
	//
	//	 m_StaticList.AddString(L"House0");
	//}
	//if (str_Mesh == "House1")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, House1);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House1));
	//
	//	 m_StaticList.AddString(L"House1");
	//}
	//
	//if (str_Mesh == "House2")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, House2);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::House2));
	//
	//	 m_StaticList.AddString(L"House2");
	//}
	//
	//
	//
	//if (str_Mesh == "Rock0")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock0);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock0));
	//
	//	 m_StaticList.AddString(L"Rock0");
	//}
	//
	//if (str_Mesh == "Rock1")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock1);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock1));
	//
	//	 m_StaticList.AddString(L"Rock1");
	//}
	//
	//if (str_Mesh == "Rock2")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock2);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock2));
	//
	//	 m_StaticList.AddString(L"Rock2");
	//}
	//
	//
	//
	//if (str_Mesh == "Rock3")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock3);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock3));
	//
	//	 m_StaticList.AddString(L"Rock3");
	//}
	//if (str_Mesh == "Rock4")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock4);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock4));
	//
	//	 m_StaticList.AddString(L"Rock4");
	//}
	//
	//if (str_Mesh == "Rock5")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Rock5);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Rock5));
	//
	//	 m_StaticList.AddString(L"Rock5");
	//}
	//
	//
	//
	//if (str_Mesh == "Staris0")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris0);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris0));
	//
	//	 m_StaticList.AddString(L"Staris0");
	//}
	//
	//if (str_Mesh == "Staris1")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris1);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris1));
	//
	//	 m_StaticList.AddString(L"Staris1");
	//}
	//
	//if (str_Mesh == "Staris2")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris2);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris2));
	//
	//	 m_StaticList.AddString(L"Staris2");
	//}
	//
	//if (str_Mesh == "Staris3")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris3);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris3));
	//
	//	 m_StaticList.AddString(L"Staris3");
	//}
	//
	//if (str_Mesh == "Staris4")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Staris4);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Staris4));
	//
	//	 m_StaticList.AddString(L"Staris4");
	//}
	//
	//
	//
	//if (str_Mesh == "Tree0")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree0);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree0));
	//
	//	 m_StaticList.AddString(L"Tree0");
	//}
	//
	//if (str_Mesh == "Tree1")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree1);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree1));
	//
	//	 m_StaticList.AddString(L"Tree1");
	//}
	//
	//if (str_Mesh == "Tree2")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Tree2);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Tree2));
	//
	//	 m_StaticList.AddString(L"Tree2");
	//}
	//if (str_Mesh == "Whiterun")
	//{
	//
	//	 dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, Whiterun);
	//
	//	 m_vecObject.push_back(new ObjectInfo(vPos, vScale, vRot, 0, STATIC_OBJECT_ID::Whiterun));
	//
	//	 m_StaticList.AddString(L"Whiterun");
	//}

	 








	UpdateData(TRUE);
	//	int iIndex = m_TextureListBox.GetCurSel();





		//CComponent* pTerrainTexture = pView->m_pManagementClass->Get_Component(L"GameLogic", L"Terrain", L"Com_Texture", ID_DYNAMIC);
		//dynamic_cast<CTexture*>(pTerrainTexture)->Ready_Texture(str_Mesh, TYPE_NORMAL, 1);
		//dynamic_cast<CTexture*>(pTerrainTexture)->Set_TextureNumber(iIndex);



	UpdateData(FALSE);

}


void CTab_Mesh::OnBnClickedButton1_Update()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//_vec3 vPos = pTile->vPos;
	//_vec3 vScale = pTile->vScale; //{ 0, 0, 0 };
	//_vec3 vRot = pTile->vRot; //{ 0, 0, 0 };

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));
	
	//dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, pTile->Object_ID);

	CLayer* pLayer = pView->m_pManagementClass->Get_Scene()->Get_Layer(L"Environment");
	list<CGameObject*> pList = pLayer->Get_ObjectList(OBJECT_BRIDGE);
	list<CGameObject*>::iterator iter = pList.begin();



	for (; iter != pList.end();)
	{
		(*iter)->Set_State(1);
		pList.erase(iter++);

	}

	






	iter = pList.begin();
	for (int i = 0; iter != pList.end(); i++, ++iter)
	{
		//if (i == pTileCount)
		{
			dynamic_cast<Scene1*>(pView->m_pManagementClass->Get_Scene())->Late_Update_Scene(0, m_vecObject[i]->Object_ID);


			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(m_vecObject[i]->vPos.x, m_vecObject[i]->vPos.y, m_vecObject[i]->vPos.z);
			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Pos(&m_vecObject[i]->vPos);
			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Set_Scale(pTile->vScale.x, pTile->vScale.y, pTile->vScale.z);
			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_X, pTile->vRot.x);
			dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Y, m_vecObject[i]->vRot.y);
			//dynamic_cast<CStatic_Object*>(*iter)->Get_Transform()->Rotation(ROTATION::ROT_Z, pTile->vRot.z);


		}
		//else
			

	}

	


}
