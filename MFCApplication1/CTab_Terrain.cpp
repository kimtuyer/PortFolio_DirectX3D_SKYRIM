// CTab_Terrain.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication1.h"
#include "CTab_Terrain.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "FileInfo.h"
#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"
#include "MFC_TerrainTex.h"
#include "MFC_Terrain.h"
#include "Optimization.h"
#include "Export_Function.h"

// CTab_Terrain 대화 상자

IMPLEMENT_DYNAMIC(CTab_Terrain, CDialogEx)

CTab_Terrain::CTab_Terrain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_RadioVtxedit(0)
{

}

CTab_Terrain::~CTab_Terrain()
{
}

void CTab_Terrain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT4, m_Itv);
	DDX_Control(pDX, IDC_LIST1, m_TextureListBox);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_EDIT8, m_VtxSize);
	DDX_Radio(pDX, IDC_RADIO1, m_RadioVtxedit);
	DDX_Control(pDX, IDC_EDIT7, m_Edit_Height);
	DDX_Control(pDX, IDC_SPIN7, m_Spin_Height);
}

void CTab_Terrain::OnEnChangeEdit1()
{
}


BEGIN_MESSAGE_MAP(CTab_Terrain, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &CTab_Terrain::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CTab_Terrain::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT4, &CTab_Terrain::OnEnChangeEdit4)
	ON_LBN_SELCHANGE(IDC_LIST1, &CTab_Terrain::OnLbnSelchangeList1)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON2, &CTab_Terrain::OnBnClickedSaved)
	ON_BN_CLICKED(IDC_BUTTON3, &CTab_Terrain::OnBnClickedLoad)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN7, &CTab_Terrain::OnDeltaposSpin_Height)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT8, &CTab_Terrain::OnEnChangeEdit_SideCtrl)
END_MESSAGE_MAP()


// CTab_Terrain 메시지 처리기






void CTab_Terrain::OnBnClickedButton1()
{
	CString str = _T("");
	GetDlgItemText(IDC_EDIT2, str);
	m_XCount = _ttoi(str);

	CString str2 = _T("");
	GetDlgItemText(IDC_EDIT3, str2);
	m_ZCount = _ttoi(str2);

	CString str3 = _T("");
	GetDlgItemText(IDC_EDIT4, str3);
	m_Itv = _ttoi(str3);

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	CComponent* pTerrainBuffer =pView->m_pManagementClass->Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_DYNAMIC);
	dynamic_cast<MFC_TerrainTex*>(pTerrainBuffer)->Ready_Buffer(m_XCount, m_ZCount, m_Itv);

	CGameObject* pTerrain= pView->m_pManagementClass->Get_Scene()->Get_Object(L"GameLogic", OBJECT_TERRAIN);
	
	 

	CComponent* pOptimizationCom = pView->m_pManagementClass->Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Optimization", ID_DYNAMIC);
	
	dynamic_cast<COptimization*>(pOptimizationCom)->Free();
	dynamic_cast<COptimization*>(pOptimizationCom)->Create(pView->m_pGraphicDev, true, m_XCount, m_ZCount);
	//if (FAILED(Ready_Proto(L"Proto_Optimization", dynamic_cast<COptimization*>(pOptimizationCom)->Create(pView->m_pGraphicDev,true, m_XCount, m_ZCount))))
	//	ERR_MSG(L"Proto_Optimization");

	//dynamic_cast<COptimization*>(pOptimizationCom)->Ready_Optimization(TRUE, m_XCount, m_ZCount);

	dynamic_cast<COptimization*>(pOptimizationCom)->Isin_FrustumForTerrain(
		dynamic_cast<MFC_TerrainTex*>(pTerrainBuffer)->Get_VtxPos(),
		dynamic_cast<MFC_TerrainTex*>(pTerrainBuffer)->Get_VtxCntX(),
		dynamic_cast<MFC_TerrainTex*>(pTerrainBuffer)->Get_VtxCntZ(),
		new INDEX32[dynamic_cast<MFC_TerrainTex*>(pTerrainBuffer)->Get_TriCnt()],
		dynamic_cast<MFC_Terrain*>(pTerrain)->Get_TriCnt()
	);


	


	//CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetApp());
	////NULL_CHECK_RETURN(pMainFrame, false);
	//
	//CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMainFrame->m_MainSplitter.GetPane(0, 1));
	//NULL_CHECK_RETURN(pView, false);

	//pView->Update

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Terrain::OnEnChangeEdit4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Terrain::OnLbnSelchangeList1()
{
	UpdateData(TRUE);
	int iIndex = m_TextureListBox.GetCurSel();
	CString wstrFileName;
	m_TextureListBox.GetText(iIndex, wstrFileName);
	int i = 0;
	//for (; i < wstrFileName.GetLength(); ++i)
	//{
	//	// isdigit 0~ 9까지의 숫자인지 아닌지를 판별해주는 함수. 
	//	if (isdigit(wstrFileName[i]))
	//		break;
	//}
	//wstrFileName.Delete(0, i);
	//m_byDrawID = _ttoi(wstrFileName.GetString());

	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	CMFCApplication1View* pView = dynamic_cast<CMFCApplication1View*>(pMain->m_MainSplitter.GetPane(0, 1));


	dynamic_cast<MFC_Terrain*>(pView->m_pManagementClass->Get_Scene()->Get_Object(L"GameLogic", OBJECT_TERRAIN))
		->Set_index(iIndex);//CComponent* pTerrainTexture = pView->m_pManagementClass->Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Texture", ID_DYNAMIC);
	//dynamic_cast<CTexture*>(pTerrainTexture)->Ready_Texture(wstrFileName, TYPE_NORMAL, 1);
	//dynamic_cast<CTexture*>(pTerrainTexture)->Set_TextureNumber(iIndex);
	//m_iIndex


	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Terrain::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iSize = DragQueryFile(hDropInfo, 0xffffffff, nullptr, 0);

	TCHAR szBuf[MAX_PATH] = L"";
	CString wstrRelativePath;
	for (int i = 0; i < iSize; ++i)
	{
		
		DragQueryFile(hDropInfo, i, szBuf, MAX_PATH);
		wstrRelativePath = CFileInfo::ConvertRelativePath(szBuf);
		CString wstrFileNameAndExtension = PathFindFileName(szBuf);
		TCHAR szFileName[MAX_PATH]{};
		lstrcpy(szFileName, wstrFileNameAndExtension.GetString());

		//PathRemoveExtension(szFileName);

		m_TextureListBox.AddString(wstrRelativePath);
	}
	m_TextureListBox.SetHorizontalExtent(500);
	UpdateData(FALSE);

	

	CDialogEx::OnDropFiles(hDropInfo);
}


void CTab_Terrain::OnBnClickedSaved()
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


		CComponent* pTerrainTex =
			pView->m_pManagementClass->Get_Component(L"GameLogic", OBJECT_ID::OBJECT_TERRAIN, L"Com_Buffer", COMPONENTID::ID_DYNAMIC);

		int Cntx = dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxCntX();

		int Cntz= dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxCntZ();
		DWORD dwByte = 0;


		int index = (Cntz - 1)*Cntx + (Cntx - 1);
		const _vec3* vPos=  dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxPos();


		for (int i = 0; i < index; i++)
		{

			

		

			WriteFile(hFile, vPos[i], sizeof(_vec3), &dwByte, nullptr);




		}
	
		CloseHandle(hFile);
	}
}


void CTab_Terrain::OnBnClickedLoad()
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
	
		
		CComponent* pTerrainTex =
			pView->m_pManagementClass->Get_Component(L"GameLogic", OBJECT_ID::OBJECT_TERRAIN, L"Com_Buffer", COMPONENTID::ID_DYNAMIC);


		int Cntx = dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxCntX();

		int Cntz = dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxCntZ();
		DWORD dwByte = 0;


		int index = (Cntz - 1)*Cntx + (Cntx - 1);
		_vec3* vPos=new _vec3[index];//= dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VtxPos();

		_vec3 pTile = {};



		VTXTEX*			pVertex = NULL;


		dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VB()->Lock(0, 0, (void**)&pVertex, 0);



		for(int i=0; i<index; i++)
		{
			ReadFile(hFile, pTile, sizeof(_vec3), &dwByte, nullptr);

			if (0 == dwByte)
			{
				break;
			}

			pVertex[i].vPos= pTile;
			//if (pTile.y > 0)
			//{
			//
			//	//return;
			//}



			vPos[i] = pTile;




		}
		dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->MFC_Set_VtxPos(vPos);
		CloseHandle(hFile);
		dynamic_cast<MFC_TerrainTex*>(pTerrainTex)->Get_VB()->Unlock();
		//delete pVertex;
		delete vPos;

		pView->Invalidate(FALSE);


	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTab_Terrain::OnDeltaposSpin_Height(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	float iVal = pNMUpDown->iPos + pNMUpDown->iDelta;


	if ((0 <= iVal))
	{
		CString sVal;
		sVal.Format(_T("%f"), iVal);

		m_Edit_Height.SetWindowTextW(sVal);

	}



}


void CTab_Terrain::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
	{

		int iSize = m_Slider.GetPos();

		CString cSize;
		cSize.Format(_T("%d"), iSize);

		m_VtxSize.SetWindowTextW(cSize);



	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CTab_Terrain::OnEnChangeEdit_SideCtrl()
{

	CString cSize;
	m_VtxSize.GetWindowTextW(cSize);
	int iSize = _ttoi(cSize);

	m_Slider.SetPos(iSize);

	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
