#include "stdafx.h"
#include "Stage.h"
#include <fstream>
#include <string>
#include "Export_Function.h"
#include <fstream>

CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{

}

CStage::~CStage(void)
{

}

HRESULT CStage::Ready_Scene(void)
{
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"mus_town_day_01.wav");
	//CSoundMgr::Get_Instance()->PlayBGM(L"mus_town_day_02.wav");


	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_InventoryLayer(L"UI_Inventory"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_QuestLayer(L"UI_Quest"), E_FAIL);



	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));


	{
		wstring wstrFilePath = L"../../MFCApplication1/Data/Whiterun1554.dat";
		HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (INVALID_HANDLE_VALUE == hFile)
			return;


		DWORD dwByte = 0;
		int pTileCount = 0;
		ObjectInfo* pTile = nullptr;



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

			Load_Update_Scene(pTile);


			++pTileCount;

		}



		CloseHandle(hFile);



	}

	

	Load_ItemInfo();

	Load_QuestInfo();






	return S_OK;
}

Engine::_int CStage::Update_Scene(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;
	PullDelayTime += fTimeDelta;

	Collison_Scene(fTimeDelta);

	Change_BGM(fTimeDelta);


	_vec3 vPos;
	float radian;
	list<CGameObject*> pNPClist = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);

	vector<QuestInfo> vecQuest = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();
	int Next_QuestID = -1;
	int Next_NpcCode = -1;
	for (auto p : vecQuest)
	{



		if (p.Quest_State == Quest_ON)
		{
			Next_QuestID = p.Quest_ID + 1;
		}
		if (Next_QuestID == p.Quest_ID)
			Next_NpcCode = p.NPC_Code;

	}
	

	for (auto p : pNPClist)
		if (dynamic_cast<CNPC*>(p)->Get_NPC_Type() == Next_NpcCode)
		{
			CGameObject* pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_QuestBarMARKER);
			dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);

			vPos = dynamic_cast<CNPC*>(p)->Get_Transform()->m_vInfo[INFO::INFO_POS];
			
			_vec3 vUp = { 0,1,0 };

			_vec3 vPlayer = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Transform()->m_vInfo[INFO::INFO_POS];
			_vec3 vPlayerRight; 
			D3DXVec3Cross(&vPlayerRight,&vUp,&dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Transform()->m_vInfo[INFO::INFO_LOOK]) ;
			_vec3 vDir = vPlayer - vPos;
			//D3DXVec3Normalize(&vDir, &vDir);

			 radian = acos(D3DXVec3Dot(&vDir, &vPlayerRight))*(180 / 3.14f);
				
			 if (radian >= 70 && radian <= 120)
			 {
				 pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_DIR);
				 dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);
				 dynamic_cast<CUI*>(pMap)->Set_TexIndex(3);
				 dynamic_cast<CUI*>(pMap)->Set_SizeX(8);
				 dynamic_cast<CUI*>(pMap)->Set_SizeY(15);	

			 }
			 if (radian >= 0 && radian < 70)
			 {
				 pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_DIR);
				 dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);
				 dynamic_cast<CUI*>(pMap)->Set_TexIndex(0);

				 dynamic_cast<CUI*>(pMap)->Set_SizeX(6);
				 dynamic_cast<CUI*>(pMap)->Set_SizeY(15);
			 }
			 if (radian > 120 && radian <= 180)
			 {
				 pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_DIR);
				 dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);
				 dynamic_cast<CUI*>(pMap)->Set_TexIndex(1);
					
				 dynamic_cast<CUI*>(pMap)->Set_SizeX(12);
				 dynamic_cast<CUI*>(pMap)->Set_SizeY(15);
			 }
			 if ((vDir.x+vDir.z<0))
			 {
				 //if(radian >= 70 && radian <= 120)
				 //
				 //dynamic_cast<CUI*>(pMap)->Set_TexIndex(3);
				 //
				 //dynamic_cast<CUI*>(pMap)->Set_SizeX(8);
				 //dynamic_cast<CUI*>(pMap)->Set_SizeY(15);
			 }


		}


	




	//m_pDialog
	if (GetAsyncKeyState('I') & 0x0001) //인벤토리 키가 눌렸을 경우
	{
		CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_cancel.wav", CSoundMgr::UI);

		m_nIkey = true;
		Update_InventoryScene(fTimeDelta);





	}
	if (dynamic_cast<CDialog*>(m_pDialog)->Get_InvenEnterKey()) //npc 대화문에서 인벤창보기 클릭시 !
	{
		Update_NPC_InventoryScene(fTimeDelta);
		dynamic_cast<CDialog*>(m_pDialog)->Set_InvenEnterKey(false);

	}

	//퀘스트 창 현황 보기 !
	if (GetAsyncKeyState('Q') & 0x0001)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_cancel.wav", CSoundMgr::UI);

		Update_QuestWindow(fTimeDelta);
	}
	if (!InvenKey &&   !CraftKey && GetAsyncKeyState('E') & 0x0001)
	{
		if (PullOn1 || PullOn2 || PullOn3)
		{
			list<CGameObject*> pList = Get_Layer(L"Environment")->Get_ObjectList(OBJECT_WALL);

		
			//obj_pullchain_down_01

			int i = 0;
			for (auto&p : pList)
			{

				if (i == 0 && PullOn1)
				{
					dynamic_cast<CStone*>(p)->Set_ID(OBJECT_WALL);
					CSoundMgr::Get_Instance()->PlaySound(L"obj_pullchain_down_01.wav", CSoundMgr::UI);

				}
				if (i == 1 && PullOn2)
				{
					dynamic_cast<CStone*>(p)->Set_ID(OBJECT_WALL);
					CSoundMgr::Get_Instance()->PlaySound(L"obj_pullchain_down_01.wav", CSoundMgr::UI);

				}
				if (i == 2 && PullOn3)
				{
					dynamic_cast<CStone*>(p)->Set_ID(OBJECT_WALL);
					CSoundMgr::Get_Instance()->PlaySound(L"obj_pullchain_down_01.wav", CSoundMgr::UI);

				}

				i++;
			}
			//if (PullDelayTime >= 2)
			{
				dynamic_cast<CDynamicCamera*>(Get_Object(L"Environment", OBJECT_CAMERA))
					->Set_POV(OBJECT_DRAGON);
				dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))
					->Set_Lever(true);

			}

		



		}

	}


	//Smthing 제련 창 키
	if (CraftOn &&  GetAsyncKeyState('C') & 0x0001)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_cancel.wav", CSoundMgr::UI);
		Update_SmithingWindow(fTimeDelta);

	}
	if (CraftKey) //

	{
		CLayer*	pUILayer = Get_Layer(L"UI_Inventory");
		//dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))

		CGameObject*	pGameObject = nullptr;
		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{

			// if (Inven_Window_SelectKey == UI_INVENTORY_Window1)//윈도우 1번창 에서  아이템 선택시
			{


				--InvenItemSelectKey;

				if ((!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty()))
				{


					vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
					for (int i = 0; i < PlayerItem.size(); ++i)
					{
						if (PlayerItem[i].Item_Type == ITEM_Type::ITEM_WEAPON || PlayerItem[i].Item_Type == ITEM_Type::ITEM_MISC)
						{


							if (InvenItemSelectKey < 0)
								InvenItemSelectKey = 0;


							if (InvenItemSelectKey == i)
							{
								InvenItemSelectKey = i;

								(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
								dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(PlayerItem[i].Item_Code);


								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


								if (PlayerItem[i].Item_Code == 0)
								{


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

								}
								if (PlayerItem[i].Item_Code == 1)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

								}
								if (PlayerItem[i].Item_Code == 2)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);

								}
								if (PlayerItem[i].Item_Code == 3)
								{

									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);

								}
								if (PlayerItem[i].Item_Code == 4)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);

								}
								if (PlayerItem[i].Item_Code == 5)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

								}
								if (PlayerItem[i].Item_Code == 6)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

								}



							}


						}
					}
				}



			}




		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{

			// if (Inven_Window_SelectKey == UI_INVENTORY_Window1) //윈도우 두번째창 에서  아이템 선택시 >>여기서 아이템 정보창 + 아이템 텍스처 이미지에 필요한 정보 삽입해야함.
			{
				++InvenItemSelectKey;
				if ((!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())
					)
				{
					vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
					for (int i = 0; i < PlayerItem.size(); ++i)
					{
						if (PlayerItem[i].Item_Type == ITEM_Type::ITEM_WEAPON)
						{



							if (InvenItemSelectKey == PlayerItem.size())
								InvenItemSelectKey -= 1;


							if (InvenItemSelectKey == i)
							{
								InvenItemSelectKey = i;
								//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

								(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
								dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(PlayerItem[i].Item_Code);

								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


								if (PlayerItem[i].Item_Code == 0)
								{


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);
									break;

								}
								if (PlayerItem[i].Item_Code == 1)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(290);
									break;

								}
								if (PlayerItem[i].Item_Code == 2)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);
									break;

								}
								if (PlayerItem[i].Item_Code == 3)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);
									break;

								}
								if (PlayerItem[i].Item_Code == 4)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);
									break;

								}
								if (PlayerItem[i].Item_Code == 5)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
									break;

								}
								if (PlayerItem[i].Item_Code == 6)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
									break;

								}



							}

						}
					}

				}

			



			}


		}
		else if (GetAsyncKeyState('E') &0x0001 &&  CraftKey) //선댁한 아이템 제련할건지 물음!
		{


			m_bEKey = true;

			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_RenderOn(true);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_fX2(380);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_fY(330);


			pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter));
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(270);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

			pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab));
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(420);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);




		}

		else if (GetAsyncKeyState(VK_RETURN) & 0x0001 && m_bEKey) //선댁한 아이템 제련 진행!
		{
		CSoundMgr::Get_Instance()->PlaySound(L"npc_human_blacksmith_forge_take_01.wav", CSoundMgr::UI_Effect);

		++ItemPopCount;
		int itemcode = 0;
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_RenderOn(false);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter))->Set_RenderOn(false);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab))->Set_RenderOn(false);
		


		m_bEKey = false;

		dynamic_cast<CDialog*>(m_pDialog)->Set_QuestState(NPC_Type::NPC_Forge, QUEST_State::Quest_Complete);
	
		vector<QuestInfo> Questlist = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();

		dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_Forge, Quest_Complete);

		dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_Blacksmith, Quest_Complete);



		for (auto&p : Questlist)
		{
			if (p.NPC_Code == NPC_Forge)
			{
				p.Quest_State = Quest_Complete;
			}
		}



		vector<ItemInfo>pList = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
		for(auto p: pList)
			if (p.Item_Type == ITEM_Type::ITEM_WEAPON)
			{
				itemcode = p.Item_Code;
			}


		if (ItemPopCount == 1)
		{
		dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Pop_Item();
		dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Strength_Item(itemcode, 100, 1000);
		}

		//재 호출해서 현재 제련창 닫기!
		Update_SmithingWindow(fTimeDelta);
		CraftOn = false;




			//제련후 아이템 스탯, 가치 증가 , 아이템 윈도우 이름 옆 () 등급 추가!
			//퀘스트 1 : 완료되었다고 퀘스트상태 수정!



		}





	}
	//Smthing 제련 창 키
	if (DragonInvenKey &&  GetAsyncKeyState('A') & 0x0001)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_cancel.wav", CSoundMgr::UI);
		Update_Monster_InventoryWindow(fTimeDelta);

	}
	if (Searchkey) //

	{
		CLayer*	pUILayer = Get_Layer(L"UI_Inventory");
		//dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))

		CGameObject*	pGameObject = nullptr;
		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{

			// if (Inven_Window_SelectKey == UI_INVENTORY_Window1)//윈도우 1번창 에서  아이템 선택시
			{


				--InvenItemSelectKey;

				if ((!dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_Inventory().empty()))
				{


					vector<ItemInfo>& DragonItem = dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_Inventory();
					for (int i = 0; i < DragonItem.size(); ++i)
					{
						if (DragonItem[i].Item_Type == ITEM_Type::ITEM_WEAPON || DragonItem[i].Item_Type == ITEM_Type::ITEM_MISC)
						{


							if (InvenItemSelectKey < 0)
								InvenItemSelectKey = 0;


							if (InvenItemSelectKey == i)
							{
								InvenItemSelectKey = i;

								(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
								dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(DragonItem[i].Item_Code);


								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


								if (DragonItem[i].Item_Code == 0)
								{


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

								}
								if (DragonItem[i].Item_Code == 1)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

								}
								if (DragonItem[i].Item_Code == 2)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);

								}
								if (DragonItem[i].Item_Code == 3)
								{

									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);

								}
								if (DragonItem[i].Item_Code == 4)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);

								}
								if (DragonItem[i].Item_Code == 5)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

								}
								if (DragonItem[i].Item_Code == 6)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

								}



							}


						}
					}
				}



			}




		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{

			// if (Inven_Window_SelectKey == UI_INVENTORY_Window1) //윈도우 두번째창 에서  아이템 선택시 >>여기서 아이템 정보창 + 아이템 텍스처 이미지에 필요한 정보 삽입해야함.
			{
				++InvenItemSelectKey;
				if ((!dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_Inventory().empty())
					)
				{
					vector<ItemInfo>& DragonItem = dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_Inventory();
					for (int i = 0; i < DragonItem.size(); ++i)
					{
						if (DragonItem[i].Item_Type == ITEM_Type::ITEM_WEAPON)
						{



							if (InvenItemSelectKey == DragonItem.size())
								InvenItemSelectKey -= 1;


							if (InvenItemSelectKey == i)
							{
								InvenItemSelectKey = i;
								//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

								(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
								dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(DragonItem[i].Item_Code);

								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


								if (DragonItem[i].Item_Code == 0)
								{


									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);
									break;

								}
								if (DragonItem[i].Item_Code == 1)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(290);
									break;

								}
								if (DragonItem[i].Item_Code == 2)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);
									break;

								}
								if (DragonItem[i].Item_Code == 3)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);
									break;

								}
								if (DragonItem[i].Item_Code == 4)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);
									break;

								}
								if (DragonItem[i].Item_Code == 5)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
									break;

								}
								if (DragonItem[i].Item_Code == 6)
								{
									dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
									dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
									dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
									dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
									break;

								}



							}

						}
					}

				}





			}


		}
		else if (GetAsyncKeyState('E') & 0x0001 && Searchkey) //선댁한 아이템 인벤토리에 넣을건지
		{


			m_bEKey = true;

			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_RenderOn(true);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_fX2(380);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_fY(330);


			pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter));
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(270);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

			pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab));
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(420);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);




		}

		else if (GetAsyncKeyState(VK_RETURN) & 0x0001 && m_bEKey) //선댁한 아이템 제련 진행!
		{
			CSoundMgr::Get_Instance()->PlaySound(L"npc_human_blacksmith_forge_take_01.wav", CSoundMgr::UI_Effect);

			++ItemPopCount;
			int itemcode = 0;
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_CraftWindow))->Set_RenderOn(false);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter))->Set_RenderOn(false);
			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab))->Set_RenderOn(false);



			m_bEKey = false;

			dynamic_cast<CDialog*>(m_pDialog)->Set_QuestState(NPC_Type::NPC_Forge, QUEST_State::Quest_Complete);

			vector<QuestInfo> Questlist = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();

			dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_Forge, Quest_Complete);

			dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_Blacksmith, Quest_Complete);



			for (auto&p : Questlist)
			{
				if (p.NPC_Code == NPC_Forge)
				{
					p.Quest_State = Quest_Complete;
				}
			}



			vector<ItemInfo>pList = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
			for (auto p : pList)
				if (p.Item_Type == ITEM_Type::ITEM_WEAPON)
				{
					itemcode = p.Item_Code;
				}


			if (ItemPopCount == 1)
			{
				dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Pop_Item();
				dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Strength_Item(itemcode, 100, 1000);
			}

			//재 호출해서 현재 제련창 닫기!
			Update_SmithingWindow(fTimeDelta);
			CraftOn = false;




			//제련후 아이템 스탯, 가치 증가 , 아이템 윈도우 이름 옆 () 등급 추가!
			//퀘스트 1 : 완료되었다고 퀘스트상태 수정!



		}





	}







	if (QuestKey) //퀘스트창 열린 상태
	{




	}





	if (InvenKey)

	{

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");

		CGameObject*			pGameObject = nullptr;

		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ui_select_on.wav", CSoundMgr::UI);

			if (Inven_Window_SelectKey == 0) //0번 윈도우 창 일때 , 플레이어 혼자만 볼시
			{
				--InvenSelectKey;

				//if 현재 플레이어가 선택하고 있는창이 window0 인지 window1인지 식별후 각 창에 맞는 텍스처키 선택할수있게 해야함
				for (int i = UI_INVENTORY_ALL; i <= UI_INVENTORY_POTION; i++) //All~ Potion 까지 선택할수 있게

				{

					if (InvenSelectKey < UI_INVENTORY_ALL)
					{
						InvenSelectKey = UI_INVENTORY_ALL;
						break;
					}

					if (InvenSelectKey == i)
					{

						(pGameObject) = pUILayer->Get_UI_InvenObject(i);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);  //현재 선택한 창, 밝게

						(pGameObject) = pUILayer->Get_UI_InvenObject(i + 1);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(0); //이전 선택창 다시 어둡게!



					}

				}
			}
			else if (Inven_Window_SelectKey == UI_INVENTORY_Window3)// 0번 윈도우창이 npc/플레이어 인벤 정보 모두 포함하는 창일시

			{
				--InvenSelectKey;
				for (int i = UI_INVENTORY_NPC_ALL; i <= UI_INVENTORY_POTION; i++) //All~ Potion 까지 선택할수 있게
				{
					if (InvenSelectKey < UI_INVENTORY_NPC_ALL)
					{
						InvenSelectKey = UI_INVENTORY_NPC_ALL;
						break;
					}

					if (InvenSelectKey == i)
					{

						(pGameObject) = pUILayer->Get_UI_InvenObject(i);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);  //현재 선택한 창, 밝게

						(pGameObject) = pUILayer->Get_UI_InvenObject(i + 1);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(0); //이전 선택창 다시 어둡게!



					}


				}



			}
			else if (Inven_Window_SelectKey == UI_INVENTORY_Window1)//윈도우 1번창 에서  아이템 선택시
			{


				--InvenItemSelectKey;

				if (Inven_ItemWindow_Type == UI_INVENTORY_Player && (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty()))
				{


					vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
					for (int i = 0; i < PlayerItem.size(); ++i)
					{
						if (InvenItemSelectKey < 0)
							InvenItemSelectKey = 0;


						if (InvenItemSelectKey == i)
						{
							InvenItemSelectKey = i;
							//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

							(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
							dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(PlayerItem[i].Item_Code);


							dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
							dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


							if (PlayerItem[i].Item_Code == 0)
							{


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (PlayerItem[i].Item_Code == 1)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (PlayerItem[i].Item_Code == 2)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);

							}
							if (PlayerItem[i].Item_Code == 3)
							{

								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);

							}
							if (PlayerItem[i].Item_Code == 4)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);

							}
							if (PlayerItem[i].Item_Code == 5)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

							}
							if (PlayerItem[i].Item_Code == 6)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
								

							}



						}


					}

				}
				if (Inven_ItemWindow_Type == UI_INVENTORY_NPC)

				{
					list<CGameObject*> NpcList = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
					list<CGameObject*>::iterator iter = NpcList.begin();

					vector<ItemInfo> NPCitem;
					for (; iter != NpcList.end(); iter++)
					{
						if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type())
						{
							NPCitem = dynamic_cast<CNPC*>(*iter)->Get_Inventory();
						}
					}

					for (int i = 0; i < NPCitem.size(); ++i)
					{
						if (InvenItemSelectKey < 0)
							InvenItemSelectKey = 0;


						if (InvenItemSelectKey == i)
						{
							InvenItemSelectKey = i;
							//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

							(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
							dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(NPCitem[i].Item_Code);


							dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
							dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


							if (NPCitem[i].Item_Code == 0)
							{


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (NPCitem[i].Item_Code == 1)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (NPCitem[i].Item_Code == 2)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);

							}
							if (NPCitem[i].Item_Code == 3)
							{

								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);

							}
							if (NPCitem[i].Item_Code == 4)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);

							}
							if (NPCitem[i].Item_Code == 5)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

							}




						}


					}
				}


			}




		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ui_select_on.wav", CSoundMgr::UI);

			if (Inven_Window_SelectKey == 0) //0번 윈도우 창 일때
			{


				++InvenSelectKey;
				for (int i = UI_INVENTORY_ALL; i <= UI_INVENTORY_POTION; i++) //All~ Potion 까지 선택할수 있게

				{

					if (InvenSelectKey >= UI_INVENTORY_POTION)
					{
						InvenSelectKey = UI_INVENTORY_POTION;
						break;
					}

					if (InvenSelectKey == i)
					{
						(pGameObject) = pUILayer->Get_UI_InvenObject(i);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);  //현재 선택한 창, 밝게

						(pGameObject) = pUILayer->Get_UI_InvenObject(i - 1);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(0); //이전 선택창 다시 어둡게!



					}

				}
			}
			else if (Inven_Window_SelectKey == UI_INVENTORY_Window3)// 0번 윈도우창이 npc/플레이어 인벤 정보 모두 포함하는 창일시
			{
				++InvenSelectKey;
				for (int i = UI_INVENTORY_NPC_ALL; i <= UI_INVENTORY_POTION; i++) //All~ Potion 까지 선택할수 있게

				{

					if (InvenSelectKey >= UI_INVENTORY_POTION)
					{
						InvenSelectKey = UI_INVENTORY_POTION;
						break;
					}

					if (InvenSelectKey == i)
					{
						(pGameObject) = pUILayer->Get_UI_InvenObject(i);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);  //현재 선택한 창, 밝게

						(pGameObject) = pUILayer->Get_UI_InvenObject(i - 1);
						dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(0); //이전 선택창 다시 어둡게!



					}

				}


			}



			else if (Inven_Window_SelectKey == UI_INVENTORY_Window1) //윈도우 두번째창 에서  아이템 선택시 >>여기서 아이템 정보창 + 아이템 텍스처 이미지에 필요한 정보 삽입해야함.
			{
				++InvenItemSelectKey;
				if (Inven_ItemWindow_Type == UI_INVENTORY_Player && (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())
					)
				{
					vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
					for (int i = 0; i < PlayerItem.size(); ++i)
					{
						if (InvenItemSelectKey == PlayerItem.size())
							InvenItemSelectKey -= 1;


						if (InvenItemSelectKey == i)
						{
							InvenItemSelectKey = i;
							//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

							(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
							dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(PlayerItem[i].Item_Code);

							dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
							dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


							if (PlayerItem[i].Item_Code == 0)
							{


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);
								break;

							}
							if (PlayerItem[i].Item_Code == 1)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(290);
								break;

							}
							if (PlayerItem[i].Item_Code == 2)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);
								break;

							}
							if (PlayerItem[i].Item_Code == 3)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);
								break;

							}
							if (PlayerItem[i].Item_Code == 4)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);
								break;

							}
							if (PlayerItem[i].Item_Code == 5)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);
								break;

							}
							if (PlayerItem[i].Item_Code == 6)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

								break;

							}



						}


					}

				}
				if (Inven_ItemWindow_Type == UI_INVENTORY_NPC)

				{

					list<CGameObject*> NpcList = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
					list<CGameObject*>::iterator iter = NpcList.begin();

					vector<ItemInfo> NPCitem;
					for (; iter != NpcList.end(); iter++)
					{
						if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type())
						{
							NPCitem = dynamic_cast<CNPC*>(*iter)->Get_Inventory();
						}
					}

					for (int i = 0; i < NPCitem.size(); ++i)
					{
						if (InvenItemSelectKey < 0)
							InvenItemSelectKey = 0;


						if (InvenItemSelectKey == i)
						{
							InvenItemSelectKey = i;
							//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달

							(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
							dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(NPCitem[i].Item_Code);


							dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
							dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
							dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);


							if (NPCitem[i].Item_Code == 0)
							{


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(200);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (NPCitem[i].Item_Code == 1)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);


								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

							}
							if (NPCitem[i].Item_Code == 2)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(210);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(240);

							}
							if (NPCitem[i].Item_Code == 3)
							{

								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(320);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(190);

							}
							if (NPCitem[i].Item_Code == 4)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(120);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(580);

							}
							if (NPCitem[i].Item_Code == 5)
							{
								dynamic_cast<CItemObject*>(pGameObject)->Set_fX_Item(590);
								dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
								dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(90);
								dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(490);

							}




						}


					}


				}




			}


		}


	
		if (GetAsyncKeyState('E') & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"npc_human_combat_idleb.wav", CSoundMgr::UI_Equip);
			if (Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_Player) //아이템 선택창이면서 플레이어의 아이템 일 경우 장착키
			{
				//장착 기능
				//현재 선택된 아이템인덱스를 플레이어 리스트와 대조, 
				//해당 아이템이 장착 상태면 해제, 아니면 장착 
				//플레이어 각 부위의 아이템코드 불러와 -1이면 미장착, 아님 장착 상태로 간주
				//이를 구별해 윈도우 1번창의 해당 아이템 이름 옆에 현재 장착 중이라는 것을 표시하는 35.png 이미지 출력!!!
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
				pGameObject = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER));

				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_BODY)
				{
					//해당 아이템 타입의 아이템을 플레이어가 장착했는지 검사해 장착 상태면 선택한 아이템으로 교체 장착
					if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmorEquip())
					{
						//장착 상태면 장착한 아이템 코드와 현재 선택한 아이템의 코드를 비교, 같은거면 해제,  다르면 교체해 장착!

						if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipArmorCode() == PlayerItem[InvenItemSelectKey].Item_Code)
						{
							//같으면 해제 처리!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipArmorCode(-1);

						}
						else
						{ //다르면 장착!
							

							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipArmorCode(PlayerItem[InvenItemSelectKey].Item_Code);
						}


					}
					else
					{
						dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipArmorCode(PlayerItem[InvenItemSelectKey].Item_Code);

					}


				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_GAUNTLET)
				{
					if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmEquip())
					{
						//장착 상태면 장착한 아이템 코드와 현재 선택한 아이템의 코드를 비교, 같은거면 해제,  다르면 교체해 장착!

						if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipGauntletCode() == PlayerItem[InvenItemSelectKey].Item_Code)
						{
							//같으면 해제 처리!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipGauntletCode(-1);

						}
						else
						{ //다르면 장착!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipGauntletCode(PlayerItem[InvenItemSelectKey].Item_Code);
						}


					}
					else
					{
						dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipGauntletCode(PlayerItem[InvenItemSelectKey].Item_Code);

					}

				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_BOOTS)
				{
					if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_BootsEquip())
					{
						//장착 상태면 장착한 아이템 코드와 현재 선택한 아이템의 코드를 비교, 같은거면 해제,  다르면 교체해 장착!

						if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipBootsCode() == PlayerItem[InvenItemSelectKey].Item_Code)
						{
							//같으면 해제 처리!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipBootsCode(-1);

						}
						else
						{ //다르면 장착!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipBootsCode(PlayerItem[InvenItemSelectKey].Item_Code);
						}


					}
					else
					{
						dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipBootsCode(PlayerItem[InvenItemSelectKey].Item_Code);

					}
				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_HELMET)
				{
					if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bHelmetEquip())
					{
						//장착 상태면 장착한 아이템 코드와 현재 선택한 아이템의 코드를 비교, 같은거면 해제,  다르면 교체해 장착!

						if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipHelmetCode() == PlayerItem[InvenItemSelectKey].Item_Code)
						{
							//같으면 해제 처리!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipHelmetCode(-1);

						}
						else
						{ //다르면 장착!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipHelmetCode(PlayerItem[InvenItemSelectKey].Item_Code);
						}


					}
					else
					{
						dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipHelmetCode(PlayerItem[InvenItemSelectKey].Item_Code);

					}
				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_WEAPON)
				{
					if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bWeaponEquip())
					{
						//장착 상태면 장착한 아이템 코드와 현재 선택한 아이템의 코드를 비교, 같은거면 해제,  다르면 교체해 장착!

						if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipWeaponCode() == PlayerItem[InvenItemSelectKey].Item_Code)
						{
							//같으면 해제 처리!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipWeaponCode(-1);

						}
						else
						{ //다르면 장착!
							dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipWeaponCode(PlayerItem[InvenItemSelectKey].Item_Code);
						}


					}
					else
					{
						dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_EquipWeaponCode(PlayerItem[InvenItemSelectKey].Item_Code);

					}
				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_POTION)
				{

				}
				if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_MISC)
				{

				}



			}
			else if(Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_NPC) //Npc 의 아이템일 경우 구매 기능
			{

				//구매 기능
				//How many 창 출력,  여기서 방향키 좌우로 개수 조절 기능 필요
				
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_HowmanyWindow))->Set_RenderOn(true);
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(false);

				pGameObject =dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter));
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				dynamic_cast<CUI*>(pGameObject)->Set_fX2(520);
				dynamic_cast<CUI*>(pGameObject)->Set_fY(480);
				dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
				dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

				pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab));
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				dynamic_cast<CUI*>(pGameObject)->Set_fX2(670);
				dynamic_cast<CUI*>(pGameObject)->Set_fY(480);
				dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
				dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);




				//vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
				//pGameObject = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER));
				//
				//if (PlayerItem[InvenItemSelectKey].Item_Type == ITEM_Type::ITEM_BODY)	


				


				//플레이어 아이템 리스트에 구매한 아이템 info를 push 하면 될듯

			}
			




		}
		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_ok.wav", CSoundMgr::UI);

			//Howmanu 창이 열린 상태인지 확인후 구매 처리 되야함!
			if (dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_HowmanyWindow))->Get_RenderOn() == true)
			{


				list<CGameObject*> NpcList = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
				list<CGameObject*>::iterator iter = NpcList.begin();

				vector<ItemInfo> NPCitem;
				for (; iter != NpcList.end(); iter++)
				{
					if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type())
					{
						NPCitem = dynamic_cast<CNPC*>(*iter)->Get_Inventory();
					}
				}

				for (int i = 0; i < NPCitem.size(); ++i)
				{
					if (InvenItemSelectKey < 0)
						InvenItemSelectKey = 0;


					if (InvenItemSelectKey == i)
					{
						InvenItemSelectKey = i;
						//아이템 정보창으로 아이템 데이터 전달,  아이템 이름값을 mesh proto 텍스처 정보에도 전달



						ItemInfo tItem;
						tItem.Item_Code = NPCitem[i].Item_Code;
						strcpy_s(tItem.Item_Name, NPCitem[i].Item_Name);
						tItem.Item_Stat = NPCitem[i].Item_Stat;
						tItem.Item_Type = NPCitem[i].Item_Type;
						tItem.Item_Value = NPCitem[i].Item_Value;
						tItem.Item_Weight = NPCitem[i].Item_Weight;

						//여기서 원래 플레이어가 가진 골드가 충분한지 검사 필요!
						dynamic_cast<CPlayer*>(Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER))->Insert_Item(tItem);

						dynamic_cast<CPlayer*>(Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER))->Set_Gold(tItem.Item_Value);

						//엔터키 눌러 구매후 현재 구매창은 종료, 이후 다시 e키 눌러 구매할때 생성!
						pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_HowmanyWindow));
						dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

						pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Enter));
						dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
						pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Tab));
						dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

						pGameObject = dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow));
						dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);


						break;


					}
				}
			}

			










		}
		////좌,우 윈도우창 전환 키
		if (GetAsyncKeyState(VK_LEFT) & 0x0001)
		{
			
			if (Inven_Window_PreSelectKey == UI_INVENTORY_Window0)
			{
				Inven_Window_SelectKey = 0;
				InvenSelectKey = UI_INVENTORY_ALL;


			}
			else
			{
				Inven_Window_SelectKey = UI_INVENTORY_Window3;
				InvenSelectKey = UI_INVENTORY_NPC_ALL;


			}
			
				InvenItemSelectCount = 0;


				CLayer*		pLayer = Get_Layer(L"UI_Inventory");
				NULL_CHECK_RETURN(pLayer, E_FAIL);

				dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(false);
				dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_HowmanyWindow))->Set_RenderOn(false);

				dynamic_cast<CItemObject*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(false);


				//플레이어 인벤창 봤을경우
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_E);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_R);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);


			//if (InvenSelectKey <= 0)
			//{
			//	InvenSelectKey = 0;
			//}
		}
		if (GetAsyncKeyState(VK_RIGHT) & 0x0001)
		{
			if (Inven_Window_SelectKey == 0 )
			{
				
				Inven_Window_PreSelectKey = 0;
				Inven_Window_SelectKey = UI_INVENTORY_Window1; //선택창 윈도우1번 창 변경


				CLayer*		pLayer = Get_Layer(L"UI_Inventory");
				NULL_CHECK_RETURN(pLayer, E_FAIL);


				if (InvenSelectKey < UI_INVENTORY_ALL) //NPC의 인벤토리 아이템목록 을 선택 및 출력!
				{
					Inven_ItemWindow_Type = UI_INVENTORY_NPC;


					dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
					dynamic_cast<CItemObject*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);

				}
				else
				{

					Inven_ItemWindow_Type = UI_INVENTORY_Player;

					if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())
					{
						dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
						dynamic_cast<CItemObject*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);

					}


				}

				//최초 이동시 맨처음 자동 선택되는 0번째 인덱스의 아이템에 관해서 여기서 
				InvenItemSelectKey = 0;

				//if (InvenItemSelectCount == 0)
				{
					
					
				

					







					InvenItemSelectCount++;
				}

				
			
				//아니다 장착이미지는 미리 바디,손,발,머리,무기 총 5개를 생성해놓고 렌더 미처리
				//아이템 윈도우창 활성화시, 이중에 플레이어가 장착중인 부위를 검사, 그 부위 맞는 텍스처만 렌더링, 아이템 폰트 위치 얻어와 위치 세팅!
				//미장착 부위는 계속해서 다시 렌더링 off 해주면서
				//위 작업을 실시간으로 업데이트 해줘야 함!
				
				
				//CGameObject*			pGameObject = nullptr;
				//
				//pGameObject = CItemObject::Create(m_pGraphicDev, L"Proto_Mesh_HideArmorgnd");
				//
				//dynamic_cast<CItemObject*>(pGameObject)->Set_fX(500);
				//dynamic_cast<CItemObject*>(pGameObject)->Set_fY(350);
				//dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.4);
				//dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.4);
				//
				//dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(420);
				//dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(150);
				//
				//CGameObject*		pCamera = Get_Layer(L"Environment")->Get_Object(OBJECT_CAMERA);

			//	_vec3 vPos = dynamic_cast<CDynamicCamera*>(pCamera)->Get_At();

				//dynamic_cast<CTransform*>(pTranscom)->Get_Info(INFO_POS,&vPos);

				
				//dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Pos(vPos.x+1.5, vPos.y+0.2, vPos.z);//62.9, 17.5, 90.15
				//dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->m_vInfo[INFO::]
				//dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Rotation(ROTATION::ROT_X, 90.f);
				//dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Rotation(ROTATION::ROT_Y, 210.f);

				//dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Scale(0.025, 0.025, 0.025);
				//NULL_CHECK_RETURN(pGameObject, E_FAIL);
				//FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_ITEMMESH, pGameObject), E_FAIL);


			}
			if (Inven_Window_SelectKey == UI_INVENTORY_Window3)
			{
				Inven_Window_PreSelectKey = UI_INVENTORY_Window3;
				Inven_Window_SelectKey = UI_INVENTORY_Window1; //선택창 윈도우1번 창 변경


				CLayer*		pLayer = Get_Layer(L"UI_Inventory");
				NULL_CHECK_RETURN(pLayer, E_FAIL);


				if (InvenSelectKey < UI_INVENTORY_ALL) //NPC의 인벤토리 아이템목록 을 선택 및 출력!
				{
					Inven_ItemWindow_Type = UI_INVENTORY_NPC;


					dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
					dynamic_cast<CItemObject*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);

				}
				else
				{

					Inven_ItemWindow_Type = UI_INVENTORY_Player;

					if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())
					{
						dynamic_cast<CUI*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
						dynamic_cast<CItemObject*>(pLayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);

					}


				}

				//최초 이동시 맨처음 자동 선택되는 0번째 인덱스의 아이템에 관해서 여기서 
				InvenItemSelectKey = 0;

				//if (InvenItemSelectCount == 0)
				{












					InvenItemSelectCount++;
				}

			}



			if (InvenSelectKey <= 0)
			{
				InvenSelectKey = 0;
			}

			Inven_Window_SelectKey = UI_INVENTORY_Window1; //오른쪽 창으로 전환



		}

		if (Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_Player) //아이템 윈도우창 활성화면서 현재 플레이어의 인벤창을 볼시
		{
			pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_E);
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(20);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);


			pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_R);
			dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CUI*>(pGameObject)->Set_fX2(120);
			dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);

			


			if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmorEquip())
			{
				//플레이어 인벤 리스트중 아이템코드 일치하는 넘 찾아 그 아이템 코드 폰트 위치에 출력
					pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Body);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
				

				for (int i = 0; i < PlayerItem.size(); ++i)
				{
					if (PlayerItem[i].Item_Code == dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipArmorCode())
					{
						//lll
						dynamic_cast<CUI*>(pGameObject)->Set_fX2(230);
						dynamic_cast<CUI*>(pGameObject)->Set_fY(i * 30 + 200);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
						break;

					}

				}




			}
			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmorEquip())
			{
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Body);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
			}


			if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmEquip())
			{
				//플레이어 인벤 리스트중 아이템코드 일치하는 넘 찾아 그 아이템 코드 폰트 위치에 출력
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Gauntlet);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();


				for (int i = 0; i < PlayerItem.size(); ++i)
				{
					if (PlayerItem[i].Item_Code == dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipGauntletCode())
					{
						dynamic_cast<CUI*>(pGameObject)->Set_fX2(230);
						dynamic_cast<CUI*>(pGameObject)->Set_fY(i * 30 + 200);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
						break;

					}

				}




			}
			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_ArmEquip())
			{
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Gauntlet);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
			}



			if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_BootsEquip())
			{
				//플레이어 인벤 리스트중 아이템코드 일치하는 넘 찾아 그 아이템 코드 폰트 위치에 출력
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Boots);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();


				for (int i = 0; i < PlayerItem.size(); ++i)
				{
					if (PlayerItem[i].Item_Code == dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipBootsCode())
					{
						dynamic_cast<CUI*>(pGameObject)->Set_fX2(230);
						dynamic_cast<CUI*>(pGameObject)->Set_fY(i * 30 + 200);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
						break;

					}

				}




			}
			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_BootsEquip())
			{
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Boots);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
			}


			if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bHelmetEquip())
			{
				//플레이어 인벤 리스트중 아이템코드 일치하는 넘 찾아 그 아이템 코드 폰트 위치에 출력
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Helmet);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();


				for (int i = 0; i < PlayerItem.size(); ++i)
				{
					if (PlayerItem[i].Item_Code == dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipHelmetCode())
					{
						dynamic_cast<CUI*>(pGameObject)->Set_fX2(230);
						dynamic_cast<CUI*>(pGameObject)->Set_fY(i * 30 + 200);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
						break;

					}

				}




			}
			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bHelmetEquip())
			{
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Helmet);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

			}


			if (dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bWeaponEquip())
			{
				//플레이어 인벤 리스트중 아이템코드 일치하는 넘 찾아 그 아이템 코드 폰트 위치에 출력
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Weapon);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();


				for (int i = 0; i < PlayerItem.size(); ++i)
				{
					if (PlayerItem[i].Item_Code == dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_EquipWeaponCode())
					{
						dynamic_cast<CUI*>(pGameObject)->Set_fX2(230);
						dynamic_cast<CUI*>(pGameObject)->Set_fY(i * 30 + 200);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
						dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
						break;

					}

				}




			}
			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_bWeaponEquip())
			{
				pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Weapon);
				dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
			}










		}
		if (Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_NPC) //아이템 윈도우창 활성화면서 현재 플레이어의 인벤창을 볼시
		{

		pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_E);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
		dynamic_cast<CUI*>(pGameObject)->Set_fX2(20);
		dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);


		pGameObject = pUILayer->Get_UI_InvenObject(UI_INVENTORY_R);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);
		dynamic_cast<CUI*>(pGameObject)->Set_fX2(120);
		dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);

		}

	}

	


	_int iResult = 0;

	//if (!InvenKey)
	

		for (auto& iter : m_mapLayer)
		{

			if (InvenKey && iter.second->Get_LayerName() == L"UI_Inventory" || iter.second->Get_LayerName() == L"Environment")
			{
				iResult = iter.second->Update_UI_Inven_Layer(fTimeDelta);
				iResult = iter.second->Update_Layer(fTimeDelta);


			}
			else if (QuestKey && iter.second->Get_LayerName() == L"UI_Quest" || iter.second->Get_LayerName() == L"Environment")

			{
				iResult = iter.second->Update_UI_Inven_Layer(fTimeDelta);
				iResult = iter.second->Update_Layer(fTimeDelta);

			}
			else if (CraftKey && iter.second->Get_LayerName() == L"UI_Inventory" || iter.second->Get_LayerName() == L"Environment")

			{
				iResult = iter.second->Update_UI_Inven_Layer(fTimeDelta);
				iResult = iter.second->Update_Layer(fTimeDelta);

			}





			else if (!QuestKey && !InvenKey && iter.second->Get_LayerName() != L"UI_Inventory")
			{

				if (iter.second->Get_LayerName() == L"GameLogic")

				{
					int  Naviindex = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Navi()->Get_CurrentIndex();

					if (Naviindex >= 0 && Naviindex <= 18 || Naviindex >= 182 && Naviindex <= 199)
					{
						m_iCurrentArea = 1;


					}
					else if (Naviindex >= 19 && Naviindex <= 73)
					{
						m_iCurrentArea = 2;


					}
					else if (Naviindex >= 74 && Naviindex <= 128)
					{
						m_iCurrentArea = 3;


					}
					else if (Naviindex >= 129 && Naviindex <= 181)
					{
						m_iCurrentArea = 4;


					}

					list<CGameObject*> pNPClist = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
					list<CGameObject*>::iterator iter1 = pNPClist.begin();

					list<CGameObject*> pGuardlist = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_GuardMan);
					list<CGameObject*>::iterator iter2 = pGuardlist.begin();



					for (; iter1 != pNPClist.end(); iter1++)
					{
						int Naviindex = dynamic_cast<CNPC*>(*iter1)->Get_NaviCom()->Get_CurrentIndex();
					
						if (!Object_Culling(Naviindex, m_iCurrentArea))
							dynamic_cast<CNPC*>(*iter1)->Set_RenderOn(false);
						else
							dynamic_cast<CNPC*>(*iter1)->Set_RenderOn(true);
					
					
					}
					for (; iter2 != pGuardlist.end(); iter2++)
					{
						int Naviindex2 = dynamic_cast<CGuardMan*>(*iter2)->Get_NaviCom()->Get_CurrentIndex();
						
						
					
						if (!Object_Culling(Naviindex2, m_iCurrentArea))
						{
							dynamic_cast<CGuardMan*>(*iter2)->Set_RenderOn(false);
					
						}
							//dynamic_cast<CGuardMan*>(*iter2)->Set_RenderOn(false);
						else
							dynamic_cast<CGuardMan*>(*iter2)->Set_RenderOn(true);
					}

					


				}
				//{
				//	//드래곤 퀘스트 시작시 모든 비전투 npc 렌더 off!
				//	//마지막 5번퀘스트 드래곤잡기가 시작되었을경우
				//	list<CGameObject*> NPClist = iter.second->Get_ObjectList(OBJECT_NPC);
				//	if(dynamic_cast<CDialog*>(m_pDialog)->Get_QuestState(5)==QUEST_State::Quest_ON)
				//		for (auto&p : NPClist)
				//		{
				//		if(dynamic_cast<CNPC*>(p)->Get_NPC_Type()!=NPC_Yarl) //영주제외 비전투npc모두 렌더off
				//		dynamic_cast<CNPC*>(p)->Set_RenderOn(false);
				//		}
				//	
				//
				//	
				//}


				iResult = iter.second->Update_Layer(fTimeDelta);

			}

			if (iResult & 0x80000000)
				return iResult;

		}
	



	return iResult;
	
	
	//return CScene::Update_Scene(fTimeDelta);
}

void CStage::Render_Scene(void)
{
	// debug 용
	++m_dwRenderCnt;

	if (m_fTime >= 1.f)
	{
		wsprintf(m_szFPS, L"FPS : %d", m_dwRenderCnt);
		m_fTime = 0.f;
		m_dwRenderCnt = 0;
	}

	Render_Font(L"Font_Jinji", m_szFPS, &_vec2(400.f, 20.f), D3DXCOLOR(0.f, 0.f, 0.f, 1.f));

	//드래곤 죽은뒤 충돌처리해 인벤 열수있는 대화문 출력!
	if (DragonInvenKey)
	{

		Render_Font(L"Font_Skyrim", L"A: Search", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));

	}



	if (InvenKey) //아이템 세부창 활성화시, npc + player인지 플레이어 혼자인지  
	{

		if (Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_Player)		//플레이어의 아이템 창 
		{

			TCHAR tPlayergold[50] = { '\0', };
			wsprintf(tPlayergold, L"%d", dynamic_cast<CPlayer*>(Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER))->Get_Gold());

			//여기선 플레이어의 인벤토리 무게량, 골드값 출력
			Render_Font(L"Font_Window2", L"Carry Weight", &_vec2(480, 550), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

			Render_Font(L"Font_Window2", L"24/300", &_vec2(610, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색


			Render_Font(L"Font_Window2", L"Gold", &_vec2(680, 550), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

			//dynamic_cast<CPlayer*>(Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER))

			Render_Font(L"Font_Window2", tPlayergold, &_vec2(730, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색




			if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())
			{

				vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
				TCHAR tStat[50] = { '\0', };
				TCHAR tWeight[50] = { '\0', };
				TCHAR tValue[50] = { '\0', };

				//윈도우2번창 E 키,  R 키 출력!
				//dynamic_cast<CUI*>(pGameObject)->Set_fX2(20);
				//dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
				Render_Font(L"Font_Window2", L"Equip", &_vec2(40, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				Render_Font(L"Font_Window2", L"Drop", &_vec2(140, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색




				for (int i = 0; i < PlayerItem.size(); ++i)
				{

					wstring wstr(PlayerItem[i].Item_Name, &PlayerItem[i].Item_Name[50]);
					wsprintf(tStat, L"%d", PlayerItem[i].Item_Stat);
					wsprintf(tWeight, L"%d", PlayerItem[i].Item_Weight);
					wsprintf(tValue, L"%d", PlayerItem[i].Item_Value);


					if (InvenItemSelectKey == i)
					{
						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						//아이템 윈도우창의 아이템 이름 출력


						Render_Font(L"Font_ItemName", wstr.c_str(), &_vec2(525, 430), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						////아이템 정보창의 아이템 이름 출력
						//
						//
						Render_Font(L"Font_ItemName", tStat, &_vec2(500, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						////아이템 정보창의 아이템 스탯 출력
						//
						//
						Render_Font(L"Font_ItemName", tWeight, &_vec2(630, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						//
						////아이템 정보창의 아이템 무게 출력
						//
						Render_Font(L"Font_ItemName", tValue, &_vec2(720, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						//아이템 정보창의 아이템 가치 출력






					}
					else // 선택안 된 나머지 아이템 이름은 회색 출력
					{
						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

					}
					//y값 490까지가 마지노선!

					//일단 i=0번째부터 현재 선택한 아이템 표시-> selectkey를 설정해 해당하는 번째의 font만 흰색으로 출력!



				}
			}
		}
		if (Inven_Window_SelectKey == UI_INVENTORY_Window1 && Inven_ItemWindow_Type == UI_INVENTORY_NPC)	//NPC의 아이템 창 
		{
			TCHAR tPlayergold[50] = { '\0', };
			wsprintf(tPlayergold, L"%d", dynamic_cast<CPlayer*>(Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER))->Get_Gold());

			Render_Font(L"Font_Window2", L"Player Gold", &_vec2(410, 550), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

			Render_Font(L"Font_Window2", tPlayergold, &_vec2(540, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색


			Render_Font(L"Font_Window2", L"NPC Gold", &_vec2(650, 550), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

			Render_Font(L"Font_Window2", L"1000", &_vec2(750, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색


			//대화중인 npc의 아이템 목록을 출력!!! 
			//대화중인 npc가 누군지 알아와  클래스는 NPC로 통일, Object_NPC의 리스트를 불러와 변수중에 npc타입이 먼지 보고 판별

			list<CGameObject*> NPClist = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
			list<CGameObject*>::iterator iter = NPClist.begin();
			vector<ItemInfo> NPCItem;
			for (; iter != NPClist.end(); iter++)
			{
				if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type())
					NPCItem = dynamic_cast<CNPC*>(*iter)->Get_Inventory();
			}

			{

				//vector<ItemInfo>& NPCItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
				TCHAR tStat[50] = { '\0', };
				TCHAR tWeight[50] = { '\0', };
				TCHAR tValue[50] = { '\0', };
				TCHAR tGold[50] = { '\0', };

				//윈도우2번창 E 키,  R 키 출력!
				//dynamic_cast<CUI*>(pGameObject)->Set_fX2(20);
				//dynamic_cast<CUI*>(pGameObject)->Set_fY(550);
				Render_Font(L"Font_Window2", L"Buy", &_vec2(40, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				Render_Font(L"Font_Window2", L"Exit", &_vec2(140, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색




				for (int i = 0; i < NPCItem.size(); ++i)
				{

					wstring wstr(NPCItem[i].Item_Name, &NPCItem[i].Item_Name[50]);
					wsprintf(tStat, L"%d", NPCItem[i].Item_Stat);
					wsprintf(tWeight, L"%d", NPCItem[i].Item_Weight);
					wsprintf(tValue, L"%d", NPCItem[i].Item_Value);




					if (InvenItemSelectKey == i)
					{
						wsprintf(tGold, L"%d", NPCItem[InvenItemSelectKey].Item_Value);


						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						//y값 490까지가 마지노선!




						if (dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_HowmanyWindow))->Get_RenderOn() == false)
						{

							Render_Font(L"Font_ItemName", wstr.c_str(), &_vec2(525, 430), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색




							Render_Font(L"Font_ItemName", tStat, &_vec2(500, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
							////아이템 정보창의 아이템 스탯 출력
							//
							//
							Render_Font(L"Font_ItemName", tWeight, &_vec2(630, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
							//
							////아이템 정보창의 아이템 무게 출력
							//
							Render_Font(L"Font_ItemName", tValue, &_vec2(720, 460), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							//아이템 정보창의 아이템 가치 출력

						}
						else
						{
							Render_Font(L"Font_ItemName", L"How many?", &_vec2(525, 410), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							Render_Font(L"Font_Window2", L"Yes", &_vec2(550, 480), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							Render_Font(L"Font_Window2", L"No", &_vec2(700, 480), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							//아래 윈도우2번창에 현재 선택한 아이템의 골드값을 플레이어 골드 옆에 표시해준다!
							Render_Font(L"Font_Window2", L"(-", &_vec2(585, 550), D3DXCOLOR(1, 0, 0, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							Render_Font(L"Font_Window2", tGold, &_vec2(605, 550), D3DXCOLOR(1, 0, 0, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							Render_Font(L"Font_Window2", L")", &_vec2(630, 550), D3DXCOLOR(1, 0, 0, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

							//Render_Font(L"Font_Window2", L"1000", &_vec2(540, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색


						}




					}
					else // 선택안 된 나머지 아이템 이름은 회색 출력
					{
						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

					}

				}


			}
		}





	}
	if (CraftKey)
	{

		if (!dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory().empty())

		{
			vector<ItemInfo>& PlayerItem = dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Inventory();
			TCHAR tStat[50] = { '\0', };
			TCHAR tWeight[50] = { '\0', };
			TCHAR tValue[50] = { '\0', };


			Render_Font(L"Font_Window2", L"Craft", &_vec2(40, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

			Render_Font(L"Font_Window2", L"Exit", &_vec2(140, 550), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색



			for (int i = 0; i < PlayerItem.size(); ++i)
			{
				Render_Font(L"Font_Window2", L"Requires :", &_vec2(550, 480), D3DXCOLOR(1, 1, 1, 1.f)); 

				Render_Font(L"Font_Window2", L"MoonStone", &_vec2(550, 500), D3DXCOLOR(1, 1, 1, 1.f));



				if (PlayerItem[i].Item_Type == ITEM_Type::ITEM_WEAPON || PlayerItem[i].Item_Type == ITEM_Type::ITEM_MISC)
				{

					wstring wstr(PlayerItem[i].Item_Name, &PlayerItem[i].Item_Name[50]);
					wsprintf(tStat, L"%d", PlayerItem[i].Item_Stat);
					wsprintf(tWeight, L"%d", PlayerItem[i].Item_Weight);
					wsprintf(tValue, L"%d", PlayerItem[i].Item_Value);


					if (InvenItemSelectKey == i)
					{

						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						//아이템 윈도우창의 아이템 이름 출력

						Render_Font(L"Font_ItemName", wstr.c_str(), &_vec2(475, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						if (PlayerItem[i].Item_Type == ITEM_Type::ITEM_WEAPON)
						Render_Font(L"Font_ItemName",L"(Legendary)", &_vec2(650, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						
						//Render_Font(L"Font_ItemName", wstr.c_str(), &_vec2(525, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

																												
																												
						////아이템 정보창의 아이템 이름 출력
						//
						//
						Render_Font(L"Font_ItemName", tStat, &_vec2(500, 410), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						if(PlayerItem[i].Item_Type==ITEM_Type::ITEM_WEAPON)
						Render_Font(L"Font_ItemName", L"+100", &_vec2(500, 425), D3DXCOLOR(1, 0, 0, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						////아이템 정보창의 아이템 스탯 출력
						//
						//
						Render_Font(L"Font_ItemName", tWeight, &_vec2(630, 410), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색
						
					
																										   //
						////아이템 정보창의 아이템 무게 출력
						//
						Render_Font(L"Font_ItemName", tValue, &_vec2(720, 410), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						if (PlayerItem[i].Item_Type == ITEM_Type::ITEM_WEAPON)
							Render_Font(L"Font_ItemName", L"+1000", &_vec2(720, 425), D3DXCOLOR(1, 0, 0, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

						//아이템 정보창의 아이템 가치 출력


					}
					else
					{
						Render_Font(L"Font_Inven", wstr.c_str(), &_vec2(240, i * 30 + 200), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색



					}



				}
			}






		}




	}


	if (QuestKey)
	{
		int QuestCount = 0;


		Render_Font(L"Font_Quest", L"QUESTS", &_vec2(140, 120), D3DXCOLOR(1, 1, 1, 1.f));


		Render_Font(L"Font_Quest", L"OBJECTIVES", &_vec2(447, 347), D3DXCOLOR(1, 1, 1, 1.f));

		vector<QuestInfo> Questlist = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();

		int QuestCompleteCount = 0;
		for (auto p : Questlist)
		{
			

			if (p.Quest_State == QUEST_State::Quest_ON)
			{
				TCHAR Name[100] = { 0, };

				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, p.Quest_Name, strlen(p.Quest_Name), Name, 100);


				Render_Font(L"Font_Quest", Name, &_vec2(100, 330 - QuestCount * 50), D3DXCOLOR(1, 1, 1, 1.f));

				Render_Font(L"Font_ItemName", Name, &_vec2(400, 195), D3DXCOLOR(1, 1, 1, 1.f));

				QuestCount++;

				if (p.Quest_ID == 0)
				{
					Render_Font(L"Font_Inven", L"Go to SkyForge and deliver the BlackSmith's item", &_vec2(320, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}
				if (p.Quest_ID == 1)
				{
					Render_Font(L"Font_Inven", L"Use reward items to strengthen your Weapons!", &_vec2(320, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}
				if (p.Quest_ID == 2)
				{
					Render_Font(L"Font_Inven", L"Bring the unpaid money to the Guard", &_vec2(320,380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}
				if (p.Quest_ID == 3)
				{
					Render_Font(L"Font_Inven", L"Defeat the guards", &_vec2(320, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}
				if (p.Quest_ID == 4)
				{
					Render_Font(L"Font_Inven", L"Defeat the Dragon", &_vec2(320, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}
				if (p.Quest_ID == 5)
				{
					Render_Font(L"Font_Inven", L"Lure the Dragon into the Trap", &_vec2(320, 380), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능 //0.5 0.5 0.5  1 회색

				}



			}
			if (p.Quest_State == QUEST_State::Quest_Complete)
			{
				QuestCompleteCount++;

				TCHAR Name[100] = { 0, };

				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, p.Quest_Name, strlen(p.Quest_Name), Name, 100);


				Render_Font(L"Font_Quest", Name, &_vec2(100, 330 + QuestCompleteCount * 20), D3DXCOLOR(0.5, 0.5, 0.5, 1.f));

			}
		}


	}


	if (!InvenKey)
	{
		int count = 0;
		for (auto p : m_vecNPC_ColliderList)
		{
			if (p == true)
			{
				DialogCheck = true;
				Render_Font(L"Font_Skyrim", L"Talk", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));
				Render_Font(L"Font_Skyrim", dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Name().c_str(), &_vec2(420, 380), D3DXCOLOR(1, 1, 1, 1.f));

				dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogON(true);

				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_F))->Set_RenderOn(true);
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_F))->Set_fX2(420);
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_F))->Set_fY(350);

				//InvenKey = true;
				//Render_Font(L"Font_Skyrim", L"Talk", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));

				count++;
			}

		}
		if (count == 0)
		{
			DialogCheck = false;
			dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogON(false);

			dynamic_cast<CDialog*>(dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Get_Dialog())->Set_RenderOn(false);


			dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_F))->Set_RenderOn(false);

		}


	}
	if (CraftOn) //제련 오브젝트와 충돌 상태시 메시지 출력
	{

		Render_Font(L"Font_Skyrim", L"Craft", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));


	}
	if (m_bEKey && CraftKey) //강화 의사 묻는 윈도우창 들어갈 폰트 출력

	{
		Render_Font(L"Font_Quest", L"Do you want to strengthen this item?", &_vec2(250, 300), D3DXCOLOR(1, 1, 1, 1.f));

		Render_Font(L"Font_Skyrim", L"Yes", &_vec2(300, 350), D3DXCOLOR(1, 1, 1, 1.f));

		Render_Font(L"Font_Skyrim", L"No", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));

	}

	if (PullOn1 || PullOn2 || PullOn3)	//레버 접촉시 출력
	{

		Render_Font(L"Font_Skyrim", L"E: PULL", &_vec2(450, 350), D3DXCOLOR(1, 1, 1, 1.f));


	}


}

_bool CStage::Collison_Scene(const _float & ftimeDelta)
{
	m_fColliTime += ftimeDelta;
	m_fColliDragonTime += ftimeDelta;
	m_PlayerSPTime += ftimeDelta;
	CLayer*		pLayer = Get_Layer(L"GameLogic");


	CLayer*		pLayer_UI = Get_Layer(L"UI");


	m_pDialog = pLayer_UI->Get_Object(OBJECT_Dialog
	);

	CGameObject* pPlayer = pLayer->Get_Object(OBJECT_PLAYER);

	CGameObject* pMonsterBar = pLayer_UI->Get_Object(OBJECT_MONSTERBAR);

	CGameObject* pDragonBar = pLayer_UI->Get_Object(OBJECT_DragonBar);



	CColliderSphere*		pPlayerWeapon_ColliderCom = nullptr;

	if (dynamic_cast<CPlayer*>(pPlayer)->Get_EquipState())
	{

	pPlayerWeapon_ColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER_WEAPON, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerWeapon_ColliderCom, FALSE);
	}
	




	CColliderSphere* pEnemyWeaponCollider = nullptr;
	
	CColliderSphere* pPlayerCollider = dynamic_cast<CPlayer*>(pPlayer)->Get_ColliderCom();


	list<CGameObject*> PlayerHP = pLayer_UI->Get_ObjectList(OBJECT_HP);
	list<CGameObject*> PlayerSP = pLayer_UI->Get_ObjectList(OBJECT_SP);


	list<CGameObject*> MonsterHP0 = pLayer_UI->Get_ObjectList(OBJECT_MONSTERHP);
	list<CGameObject*> MonsterHP1 = pLayer_UI->Get_ObjectList(OBJECT_MONSTERHP2);
	list<CGameObject*> MonsterHP2 = pLayer_UI->Get_ObjectList(OBJECT_MONSTERHP3);
	list<CGameObject*> MonsterHP3 = pLayer_UI->Get_ObjectList(OBJECT_MONSTERHP4);


	list<CGameObject*> DrognHP = pLayer_UI->Get_ObjectList(OBJECT_DragonHP);


	
	//CColliderSphere*		pBSNPC_ColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(L"GameLogic", OBJECT_NPC, L"Com_Collider", ID_STATIC));
	CColliderSphere* pGuardManCollider=nullptr;



	list<CGameObject*> GuardManList = pLayer->Get_ObjectList(OBJECT_GuardMan);

	list<CGameObject*> NPCList = pLayer->Get_ObjectList(OBJECT_NPC);


	CGameObject* pDragon = Get_Object(L"GameLogic", OBJECT_DRAGON);



	list<CGameObject*>::iterator iterHP = PlayerHP.begin();
	if (m_PlayerSPTime >= 1.5)
		if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BLOCK)
		{
			int count = 0;
			for (int i = 0; iterHP != PlayerHP.end(); i++, iterHP++)
			{
				if (dynamic_cast<CHp*>(*iterHP)->Get_RenderOn() == false)
				{
					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
					CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__00009FA5_1.wav", CSoundMgr::PLAYER);
					dynamic_cast<CHp*>(*iterHP)->Set_RenderOn(true);
					break;
				}
			}
			m_PlayerSPTime = 0;
		}




	list<CGameObject*>::iterator iterSP = PlayerSP.begin();
	if(m_PlayerSPTime>=1.5)
	if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_IDLE)
	{
		int count = 0;
		for (int i = 0; iterSP != PlayerSP.end(); i++, iterSP++)
		{
			if (dynamic_cast<CSp*>(*iterSP)->Get_RenderOn() == false)
			{
				dynamic_cast<CSp*>(*iterSP)->Set_RenderOn(true);
				break;
			}
		}
		m_PlayerSPTime = 0;
	}

	list<CGameObject*>::iterator iterSP2 = PlayerSP.end();
	if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BASH)
	{
		
		for (int i = 0; iterSP2 != PlayerSP.begin(); i++, iterSP2--)
				if (!PlayerSP.empty())
				{
					if(i==0)
					dynamic_cast<CSp*>(*(--iterSP2))->Set_RenderOn(false);
					else
						dynamic_cast<CSp*>(*(iterSP2))->Set_RenderOn(false);

				}
	}
		
	
	
	list<CGameObject*>::iterator iterSP3 = PlayerSP.end();
	if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_POWERATTACK)
	{
		for (int i = 0; iterSP3 != PlayerSP.begin(); i++, iterSP3--)
			if (!PlayerSP.empty())
			{		

				if (i == 0)
					dynamic_cast<CSp*>(*(--iterSP3))->Set_RenderOn(false);
				else
					dynamic_cast<CSp*>(*(iterSP3))->Set_RenderOn(false);

			}
	}




	list<CGameObject*>::iterator iter = GuardManList.begin();
	CGameObject* pEffect = nullptr;
	//pEffect = (Get_Object(L"GameLogic", OBJECT_Blood));
	if(m_fColliTime >=0.5f)
	if(dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == -1) //드래곤과 전투시에는 서로 충돌x
	for (int i = 0; iter != GuardManList.end(); i++,iter++)	{
		
		if (dynamic_cast<CGuardMan*>(*iter)  ->Get_RenderOn())
		{


			pGuardManCollider = dynamic_cast<CGuardMan*>(*iter)->Get_ColliderCom();
			//플레이어 무기와 경비병과의 충돌체크

			if (dynamic_cast<CPlayer*>(pPlayer)->Get_EquipState() && (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BASH ||
				dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_POWERATTACK || dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_ATTACK ||
				dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BLOCK))
			{
				if (m_pCalculatorCom->Collision_Sphere(pPlayerWeapon_ColliderCom->Get_Radius(), pPlayerWeapon_ColliderCom->Get_Center(), pPlayerWeapon_ColliderCom->Get_CollWorldMatrix(),
					0.02, pGuardManCollider->Get_Radius(), pGuardManCollider->Get_Center(), pGuardManCollider->Get_CollWorldMatrix(), 0.01)
					)//  플레이어가 공격상태일 경우 플레이어 무기와 충돌체크
				{

					OutputDebugString(L"플레이어 무기와 npc 충돌!!!");
					for (auto p : MonsterHP0)
						dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);

					for (auto p : MonsterHP1)
						dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);


					//(*iter)->Set_ActionState(CURRENT_ATTACK); //공격당한 대상도 공격상태로 전환!


					if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BASH)
					{
						(*iter)->Set_ActionState(CURRENT_ATTACKED);
						CSoundMgr::Get_Instance()->PlaySound(L"wpn_bash_blade_01.wav", CSoundMgr::EFFECT);

						if (!PlayerSP.empty())
						{
							dynamic_cast<CSp*>(PlayerSP.back())->Set_RenderOn(false);

						}

					}

					else if (
						dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_POWERATTACK)
					{
						(*iter)->Set_ActionState(CURRENT_POWERATTACKED);

						
						if (!PlayerSP.empty())
						{
							dynamic_cast<CSp*>(PlayerSP.back())->Set_RenderOn(false);

						}
						//CSoundMgr::Get_Instance()->PlaySound(L"npc_kill_slash_01.wav", CSoundMgr::EFFECT);


					}
					else if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_ATTACK )
						(*iter)->Set_ActionState(CURRENT_ATTACKED);


					else if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() == CURRENT_BLOCK)
					{
						CSoundMgr::Get_Instance()->PlaySound(L"wpn_bash_blade_01.wav", CSoundMgr::EFFECT);
					
						//(*iter)->Set_ActionState(CURRENT_POWERATTACKED);
					
					}


					dynamic_cast<CMonsterBar*>(pMonsterBar)->Set_RenderOn(true);



					if (pPlayer->Get_ActionState() != CURRENT_BLOCK)
					{
						CSoundMgr::Get_Instance()->PlaySound(L"npc_kill_slash_01.wav", CSoundMgr::Blood);


						if (dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 0)
						{

							


							if (!MonsterHP0.empty())
							{
								MonsterHP0.back()->Set_State(1);

							}
							else
							{
								dynamic_cast<CGuardMan*>(*iter)->Set_ActionState(Current_State::CURRENT_DEATH);

								//(*iter)->Set_State(1);
							}
						}
						else if (dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 1)
						{

						


							if (!MonsterHP1.empty())
							{
								MonsterHP1.back()->Set_State(1);

							}
							else
							{
								dynamic_cast<CGuardMan*>(*iter)->Set_ActionState(Current_State::CURRENT_DEATH);

								//(*iter)->Set_State(1);
							}
						}
					}
					else if (dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 2)
					{

						for (auto p : MonsterHP2)
							dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);



						if (!MonsterHP2.empty())
						{
							MonsterHP2.back()->Set_State(1);

						}
						else
						{
							dynamic_cast<CGuardMan*>(*iter)->Set_ActionState(Current_State::CURRENT_DEATH);

							//(*iter)->Set_State(1);
						}
					}


					else if (dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 3)
					{

						for (auto p : MonsterHP3)
							dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);



						if (!MonsterHP3.empty())
						{
							MonsterHP3.back()->Set_State(1);

						}
						else
						{
							dynamic_cast<CGuardMan*>(*iter)->Set_ActionState(Current_State::CURRENT_DEATH);

							//(*iter)->Set_State(1);
						}
					}




					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Get_Transform()->Set_Pos(pGuardManCollider->Get_Center());

					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(true);


					//(*iter)->Set_HPtoDamage(pPlayer->Get_AttackStat());


					m_iCount++;
				}
				else
				{
					m_iCount = 0;
					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(false);

				}


			}


			//적도 공격상태일 경우  경비병 무기와 플레이어 와의 충돌체크

			if ((*iter)->Get_ActionState() == CURRENT_ATTACK)
			{
				//dynamic_cast<CMonsterBar*>(pMonsterBar)->Set_RenderOn(true);
				//for (auto p : MonsterHP)
				//	dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);

				pEnemyWeaponCollider = dynamic_cast<CimperialSword*>(dynamic_cast<CGuardMan*>(*iter)->Get_Sword())->Get_ColliderCom();

				//-> 플레이어 몸체와 적 무기간의 충돌처리
				if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
					0.01, pEnemyWeaponCollider->Get_Radius(), pEnemyWeaponCollider->Get_Center(), pEnemyWeaponCollider->Get_CollWorldMatrix(), 0.02))

				{
					//상대 데미지 플레이어 체력에 전달
					//pPlayer->Set_HPtoDamage((*iter)->Get_AttackStat());



						
					list<CGameObject*>::iterator iterHP1 = PlayerHP.end();
					if (dynamic_cast<CPlayer*>(pPlayer)->Get_ActionState() != CURRENT_BLOCK) //방어상태는 공격불가판정
					{
						CSoundMgr::Get_Instance()->PlaySound(L"npc_kill_slash_03.wav", CSoundMgr::MONSTER);
						(pPlayer)->Set_ActionState(CURRENT_ATTACKED);

						for (int i = 0; iterHP1 != PlayerHP.begin(); i++, iterHP1--)
					{
								
						if (!PlayerHP.empty())
						{
							if (i == 0)
								dynamic_cast<CHp*>(*(--iterHP1))->Set_RenderOn(false);
							else
								dynamic_cast<CHp*>(*(iterHP1))->Set_RenderOn(false);

						}
						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Get_Transform()->Set_Pos(pPlayerCollider->Get_Center());

						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(true);
					}


					
					


					}

				}
				else
				{
					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(false);

				}




			}
			else if ((*iter)->Get_ActionState() == CURRENT_DEATH)
			{
				dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(false);

			}



		}
		else if ((*iter)->Get_ActionState() == CURRENT_DEATH)
		{
		dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(false);
		//dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(false);
		//
		//dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(false);
		}
		m_fColliTime = 0.f;

	}
	

	//드래곤과 경비병 충돌 처리 & 플레이어 충돌처리
	if (m_fColliDragonTime >= 0.5f)
	if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() != -1)
	{

		dynamic_cast<CMonsterBar*>(pDragonBar)->Set_RenderOn(true);

		for (auto &p : DrognHP)
			dynamic_cast<CMonsterHP*>(p)->Set_RenderOn(true);


		CColliderSphere* pDragonBody = dynamic_cast<CDragon*>(pDragon)->Get_BodyCollider();
		CColliderSphere* pDragonLWing = dynamic_cast<CDragon*>(pDragon)->Get_LwingCollider();
		CColliderSphere* pDragonRWing = dynamic_cast<CDragon*>(pDragon)->Get_RwingCollider();
		CColliderSphere* pDragonTail = dynamic_cast<CDragon*>(pDragon)->Get_TailCollider();



		CColliderSphere* pCollider = nullptr;



		//충돌 검사시작
		if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_HEAD)
		{
			
			 pCollider = dynamic_cast<CDragon*>(pDragon)->Get_HeadCollider();

		}
		else if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_LWING)
		{
			
			 pCollider = dynamic_cast<CDragon*>(pDragon)->Get_LwingCollider();



		}

		else if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_RWING)
		{
			
			 pCollider = dynamic_cast<CDragon*>(pDragon)->Get_RwingCollider();

		}

		else if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_TAIL)
		{
			
			 pCollider = dynamic_cast<CDragon*>(pDragon)->Get_TailCollider();

		}
		else if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_BREATH)
		{
			
			

		}



		if(pCollider!=nullptr)
		for (int i = 0; iter != GuardManList.end(); i++, iter++) {

			if (dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 2 || dynamic_cast<CGuardMan*>(*iter)->Get_ID() == 3)
			{

				pGuardManCollider = dynamic_cast<CGuardMan*>(*iter)->Get_ColliderCom();
				
				_vec3 vPos = { pGuardManCollider->Get_CollWorldMatrix()->_41, pGuardManCollider->Get_CollWorldMatrix()->_42 ,pGuardManCollider->Get_CollWorldMatrix()->_43 };
				pGuardManCollider->Set_Center(vPos);
			

				if (m_pCalculatorCom->Collision_Sphere(pCollider->Get_Radius(), pCollider->Get_Center(), pCollider->Get_CollWorldMatrix(),
				0.01, pGuardManCollider->Get_Radius(), pGuardManCollider->Get_Center(), pGuardManCollider->Get_CollWorldMatrix(), 0.02))
				{

				//경비병 충돌시 한번에 죽음 처리!
				//
				//경비병 땅에 쓰러뜨리는 모션 발동!
				dynamic_cast<CGuardMan*>(*iter)->Set_ActionState(Current_State::CURRENT_DEATH);



				//일정 시간지난후 해당 경비병들은 Hp=0처리해 삭제 처리!

				}

			}
		}

		//플레이어와 충돌처리!
			if(dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() != DRAGON_ATT_BREATH && dynamic_cast<CDragon*>(pDragon)->Get_ActionState()!=CURRENT_DEATH )
		if (m_pCalculatorCom->Collision_Sphere(pCollider->Get_Radius(), pCollider->Get_Center(), pCollider->Get_CollWorldMatrix(),
			0.01, pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(), 0.02))
		{
			

			pPlayer->Set_ActionState(CURRENT_POWERATTACKED);

			list<CGameObject*>::iterator iterHP2 = PlayerHP.end();
			if (pPlayer->Get_ActionState() != CURRENT_BLOCK)
			{		

			for (int i = 0; iterHP2 != PlayerHP.begin(); i++, iterHP2--)
			{

				if (!PlayerHP.empty())
				{
					if (i == 0)
						dynamic_cast<CHp*>(*(--iterHP2))->Set_RenderOn(false);
					else
						dynamic_cast<CHp*>(*(iterHP2))->Set_RenderOn(false);

				}
			}
			dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Get_Transform()->Set_Pos(pPlayerCollider->Get_Center());

			dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(true);


			}

		}
		else
		{
			dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_PBlood)))->Set_RenderOn(false);

		}

		//if (pPlayer->Get_ActionState() != CURRENT_BLOCK)
		if (dynamic_cast<CDragon*>(pDragon)->Get_ATT_Type() == DRAGON_ATT_BREATH)
		{
			_vec3 vPos;
			_vec3 vPlayer = { pPlayerCollider->Get_CollWorldMatrix()->_41,pPlayerCollider->Get_CollWorldMatrix()->_42,pPlayerCollider->Get_CollWorldMatrix()->_43 };
			list<CGameObject*>Breathlist= Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);
			int i = 0;
			for (auto p : Breathlist)
			{
					//if (dynamic_cast<CDragonEffect*>(p)->Get_iIndex() == 39)
					vPos = dynamic_cast<CDragonEffect*>(p)->Get_Collider();

					float fDist = sqrtf((vPlayer.x - vPos.x) *(vPlayer.x - vPos.x) +
						(vPlayer.y - vPos.y)*(vPlayer.y - vPos.y) +
						(vPlayer.z - vPos.z)*(vPlayer.z - vPos.z));

					pPlayer->Set_ActionState(CURRENT_POWERATTACKED);



					list<CGameObject*>::iterator iterHP3 = PlayerHP.begin();
					if (pPlayer->Get_ActionState() != CURRENT_BLOCK)
					{

					for (int i = 0; iterHP3 != PlayerHP.begin(); i++, iterHP3--)
						{

						if (!PlayerHP.empty())
						{
							if (i == 0)
								dynamic_cast<CHp*>(*(--iterHP3))->Set_RenderOn(false);
							else
								dynamic_cast<CHp*>(*(iterHP3))->Set_RenderOn(false);

						}
						}
					}

			}

		

			




		}




		//플레이어 무기와 드래곤간 충돌처리!
		if (pPlayerWeapon_ColliderCom != nullptr && (pPlayer->Get_ActionState() == CURRENT_ATTACK || pPlayer->Get_ActionState() == CURRENT_POWERATTACK))
		{


			if (m_pCalculatorCom->Collision_Sphere(pDragonBody->Get_Radius(), pDragonBody->Get_Center(), pDragonBody->Get_CollWorldMatrix(),
				0.015, pPlayerWeapon_ColliderCom->Get_Radius(), pPlayerWeapon_ColliderCom->Get_Center(), pPlayerWeapon_ColliderCom->Get_CollWorldMatrix(), 0.02))
			{
				//상대 데미지 플레이어 체력에 전달
				//pPlayer->Set_HPtoDamage((*iter)->Get_AttackStat());



				if (pPlayer->Get_ActionState() == CURRENT_ATTACK || pPlayer->Get_ActionState() == CURRENT_POWERATTACK)
				{

					if (!DrognHP.empty())
					{
						DrognHP.back()->Set_State(1);

					}
					else
					{
						//드래곤의 애니메이션 죽는 처리!
						dynamic_cast<CDragon*>(pDragon)->Set_ActionState(Current_State::CURRENT_DEATH);

					}


					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Get_Transform()->Set_Pos(pDragonBody->Get_Center());

					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(true);

				}










			}
		else if (m_pCalculatorCom->Collision_Sphere(pDragonLWing->Get_Radius(), pDragonLWing->Get_Center(), pDragonLWing->Get_CollWorldMatrix(),
					0.015, pPlayerWeapon_ColliderCom->Get_Radius(), pPlayerWeapon_ColliderCom->Get_Center(), pPlayerWeapon_ColliderCom->Get_CollWorldMatrix(), 0.02))
			{

					if (pPlayer->Get_ActionState() == CURRENT_ATTACK || pPlayer->Get_ActionState() == CURRENT_POWERATTACK)
					{

						if (!DrognHP.empty())
						{
						DrognHP.back()->Set_State(1);

						}
						else
						{
						//드래곤의 애니메이션 죽는 처리!
						dynamic_cast<CDragon*>(pDragon)->Set_ActionState(Current_State::CURRENT_DEATH);

						}


						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Get_Transform()->Set_Pos(pDragonBody->Get_Center());

						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(true);

					}


			}
		else if (m_pCalculatorCom->Collision_Sphere(pDragonRWing->Get_Radius(), pDragonRWing->Get_Center(), pDragonRWing->Get_CollWorldMatrix(),
				0.015, pPlayerWeapon_ColliderCom->Get_Radius(), pPlayerWeapon_ColliderCom->Get_Center(), pPlayerWeapon_ColliderCom->Get_CollWorldMatrix(), 0.02))
			{

					if (pPlayer->Get_ActionState() == CURRENT_ATTACK || pPlayer->Get_ActionState() == CURRENT_POWERATTACK)
				{

						if (!DrognHP.empty())
					{
						DrognHP.back()->Set_State(1);

					}
						else
					{
						//드래곤의 애니메이션 죽는 처리!
						dynamic_cast<CDragon*>(pDragon)->Set_ActionState(Current_State::CURRENT_DEATH);

					}


						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Get_Transform()->Set_Pos(pDragonBody->Get_Center());

						dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(true);

				}


			}
		else if (m_pCalculatorCom->Collision_Sphere(pDragonTail->Get_Radius(), pDragonTail->Get_Center(), pDragonTail->Get_CollWorldMatrix(),
				0.015, pPlayerWeapon_ColliderCom->Get_Radius(), pPlayerWeapon_ColliderCom->Get_Center(), pPlayerWeapon_ColliderCom->Get_CollWorldMatrix(), 0.02))
			{


				if (pPlayer->Get_ActionState() == CURRENT_ATTACK || pPlayer->Get_ActionState() == CURRENT_POWERATTACK)
				{

					if (!DrognHP.empty())
					{
					DrognHP.back()->Set_State(1);

					}
					else
					{
					//드래곤의 애니메이션 죽는 처리!
					dynamic_cast<CDragon*>(pDragon)->Set_ActionState(Current_State::CURRENT_DEATH);

					}


					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Get_Transform()->Set_Pos(pDragonBody->Get_Center());

					dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(true);

				}

			}


		else
				dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(false);

		}
		else
		{
			dynamic_cast<CEffect*>((Get_Object(L"GameLogic", OBJECT_MBlood)))->Set_RenderOn(false);

		}







		m_fColliDragonTime = 0;
	}


			


	
	CColliderSphere*	 pCollider = dynamic_cast<CDragon*>(pDragon)->Get_BodyCollider();
	//용이 죽은뒤 용에게서 아이템 , 골드 습득 할수있는 충돌 처리!
	if (dynamic_cast<CDragon*>(pDragon)->Get_DeathCheck())
		if (m_pCalculatorCom->Collision_Sphere(pCollider->Get_Radius(), pCollider->Get_Center(), pCollider->Get_CollWorldMatrix(),
			0.01, pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(), 0.02))
		{

			//
			DragonInvenKey = true;





		}





	//임시로 현 npc와 플레이어간 충돌 체크해 대화문 띄우기

	list<CGameObject*>::iterator iter2 = NPCList.begin();

	//if (m_fColliTime >= 0.5f)

		
		for (int i = 0; iter2 != NPCList.end(); i++, iter2++) { //NPC 타입의 오브젝트 클래스 따로 생성해  충돌체 모아서 검사하기

			CColliderSphere* pNPCCollider = dynamic_cast<CNPC*>(*iter2)->Get_Collider();

			

			//if(dynamic_cast<CNPC*>(*iter2)->Get_RenderOn())
			if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
				0.01, pNPCCollider->Get_Radius(), pNPCCollider->Get_Center(), pNPCCollider->Get_CollWorldMatrix(), 0.01)
				)
			{

				if (dynamic_cast<CNPC*>(*iter2)->Get_NPC_Type() == NPC_Tulius && dynamic_cast<CDialog*>(m_pDialog)->Get_QuestState(4)==Quest_Complete)
				{
					//dynamic_cast<CDialog*>(m_pDialog)->Set_RenderOn(true);
					
					//dynamic_cast<CPlayer*>(pPlayer)->Set_m_bDialogState();

				}



				dynamic_cast<CDialog*>(m_pDialog)->Set_NPC_Type(dynamic_cast<CNPC*>(*iter2)->Get_NPC_Type());

				dynamic_cast<CDialog*>(m_pDialog)->Set_NPC_Name(dynamic_cast<CNPC*>(*iter2)->Get_NPC_Name());

				

				m_vecNPC_ColliderList[i] = true;
				//DialogCheck = true;

				//충돌한 NPC 클래스의 해당 npc 이름 출력하게 하기.
				



			}
			else
			{
				m_vecNPC_ColliderList[i] = false;

				//DialogCheck = false;




			}

		}




		list<CGameObject*>::iterator iter3 = GuardManList.begin();

		//if (m_fColliTime >= 0.5f)
		for (int i = 0; iter3 != GuardManList.end(); i++, iter3++) { //NPC 타입의 오브젝트 클래스 따로 생성해  충돌체 모아서 검사하기

			CColliderSphere* pNPCCollider = dynamic_cast<CGuardMan*>(*iter3)->Get_ColliderCom();
			if(dynamic_cast<CGuardMan*>(*iter3)->Get_RenderOn() && dynamic_cast<CGuardMan*>(*iter3)->Get_ActionState()==CURRENT_IDLE)
			if (dynamic_cast<CGuardMan*>(*iter3)->Get_ID() == 1)
			{


				if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
					0.01, pNPCCollider->Get_Radius(), pNPCCollider->Get_Center(), pNPCCollider->Get_CollWorldMatrix(), 0.01)
					)
				{



					//dynamic_cast<CDialog*>(dynamic_cast<CPlayer*>(pPlayer)->Get_Dialog())->Set_RenderOn(true);

					dynamic_cast<CDialog*>(m_pDialog)->Set_NPC_Type(NPC_Guard);
					
					dynamic_cast<CDialog*>(m_pDialog)->Set_NPC_Name(L"GuardMan");

					m_vecNPC_ColliderList[3] = true;
					//DialogCheck = true;

					//충돌한 NPC 클래스의 해당 npc 이름 출력하게 하기.




				}
				else
				{
					m_vecNPC_ColliderList[3] = false;

					//DialogCheck = false;




				}


			}
			else
			{
				m_vecNPC_ColliderList[3] = false;

			}

			

		}











		//퀘스트 1: 의 제련 퀘스트 위해 제련 오브젝트와 충돌처리 >> 퀘스트0단계 완수 + 퀘스트1단계 진행중일때만 충돌체크!
		if (
			dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist()[1].Quest_State == QUEST_State::Quest_ON)


		{
			CColliderSphere* pSmithingCollider = dynamic_cast<CStone*>(Get_Object(L"Environment", OBJECT_Bench))->Get_Collider();

			if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
				0.01, pSmithingCollider->Get_Radius(), pSmithingCollider->Get_Center(), pSmithingCollider->Get_CollWorldMatrix(), 0.01)
				)
			{


				CraftOn = true;

				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_C))->Set_RenderOn(true);
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_C))->Set_fX2(420);
				dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_C))->Set_fY(350);


			}
			else
			{
				CraftOn = false;

			}




		}

		//if (dynamic_cast<CDialog*>(m_pDialog)->Get_QuestState(5)==QUEST_State::Quest_ON)

		{
			list<CGameObject*> pList = Get_Layer(L"Environment")->Get_ObjectList(OBJECT_WALL);
			CColliderSphere* pLevelCollider1 = nullptr;
			CColliderSphere* pLevelCollider2 = nullptr;
			CColliderSphere* pLevelCollider3 = nullptr;

			int i = 0;
			for (auto&p : pList)
			{
				if (i == 0)
					pLevelCollider1 = dynamic_cast<CStone*>(p)->Get_Collider();


				if (i == 1)
					pLevelCollider2 = dynamic_cast<CStone*>(p)->Get_Collider();


				if (i == 2)
					pLevelCollider3 = dynamic_cast<CStone*>(p)->Get_Collider();

				i++;

			}

			if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
					0.01, pLevelCollider1->Get_Radius(), pLevelCollider1->Get_Center(), pLevelCollider1->Get_CollWorldMatrix(), 0.01)
					)
			{

					PullOn1 = true;
					

					//레버와 충돌 처리시
			}
			else
			{
					PullOn1 = false;
			}


			if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
				0.01, pLevelCollider2->Get_Radius(), pLevelCollider2->Get_Center(), pLevelCollider2->Get_CollWorldMatrix(), 0.01)
				)
			{

				PullOn2 = true;


				//레버와 충돌 처리시
			}
			else
			{
				PullOn2 = false;
			}




			if (m_pCalculatorCom->Collision_Sphere(pPlayerCollider->Get_Radius(), pPlayerCollider->Get_Center(), pPlayerCollider->Get_CollWorldMatrix(),
				0.01, pLevelCollider3->Get_Radius(), pLevelCollider3->Get_Center(), pLevelCollider3->Get_CollWorldMatrix(), 0.01)
				)
			{

				PullOn3 = true;


				//레버와 충돌 처리시
			}
			else
			{
				PullOn3 = false;
			}

			



			//레버 충돌 처리시작



		}

		//퀘스트4, 경비병과 싸움후 모두 죽을때 퀘스트 완료 시점 확인 위해
		//m_iQuest4가 On, 진행중일때 경비병 리스트에서 경비병 0,1번 모두 current_Death 일경우, 퀘스트를 Complete 처리, 이후 다음 퀘스트로 진행!
		list<CGameObject*> pGuardlist = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_GuardMan);
		vector<QuestInfo> Questlist = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();

		


		list<CGameObject*>::iterator iterGuard = pGuardlist.begin();
		list<CGameObject*>::iterator iterNPC = NPCList.begin();

		int count = 0;
		if ( dynamic_cast<CDialog*>(m_pDialog)->Get_QuestState(4)== QUEST_State::Quest_ON)
		{

			for (int i = 0; iterGuard != pGuardlist.end(); i++, iterGuard++)
			{
				if (dynamic_cast<CGuardMan*>(*iterGuard)->Get_ID() == 0 && !dynamic_cast<CGuardMan*>(*iterGuard)->Get_RenderOn())
					count++;
				if (dynamic_cast<CGuardMan*>(*iterGuard)->Get_ID() == 1 && !dynamic_cast<CGuardMan*>(*iterGuard)->Get_RenderOn())
					count++;

			}
			if (count == 2)
			{
				dynamic_cast<CMonsterBar*>(pMonsterBar)->Set_RenderOn(false);

				dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_market, Quest_Complete);
				dynamic_cast<CDialog*>(m_pDialog)->Set_QuestlistState(NPC_Guard, Quest_Complete);


				dynamic_cast<CDialog*>(m_pDialog)->Set_QuestState(3, Quest_Complete);

				dynamic_cast<CDialog*>(m_pDialog)->Set_QuestState(4, Quest_Complete);

				for (; iterNPC != NPCList.end(); iterNPC++)
				{
					if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Tulius)
					{
						dynamic_cast<CNPC*>(*iterNPC)->Set_RenderOn(true);
					}
					if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Yarl)
					{
						dynamic_cast<CNPC*>(*iterNPC)->Set_RenderOn(true);
					}



		if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Blacksmith)
		{
	(*iterNPC)->Set_HP(0);
	//dynamic_cast<CNPC*>(*iterNPC)->Set_RenderOn(false);
		}
		if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_market)
{
	(*iterNPC)->Set_HP(0);

	//dynamic_cast<CNPC*>(*iterNPC)->Set_RenderOn(false);
}
		if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Forge)
	{
	(*iterNPC)->Set_HP(0);

	//dynamic_cast<CNPC*>(*iterNPC)->Set_RenderOn(false);
}






				}





				//dynamic_cast<CDialog*>(m_pDialog)->Set_QuestState(NPC_Guard, Quest_Complete);
				dynamic_cast<CDialog*>(m_pDialog)->Set_NPC_Type(NPC_Tulius);


			}

		}
		if (dynamic_cast<CDialog*>(m_pDialog)->Get_QuestState(6) == QUEST_State::Quest_ON)
		{


		}





		return _bool();
}

int CStage::Battle_Scene(const _float & ftimeDelta)
{










	return 0;
}

void CStage::Change_BGM(const _float & ftimeDelta)
{
	list<CGameObject*> NpcList = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
	list<CGameObject*>::iterator iterNPC = NpcList.begin();

	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
		->Get_QuestState(4) == Quest_ON && m_ibgmcount1 == 0)
	{
		m_ibgmcount1++;
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"mus_combat_02.wav");

	}
	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
		->Get_QuestState(4) == Quest_Complete && m_ibgmcount2 == 0)
	{
		m_ibgmcount2++;
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"mus_town_day_02.wav");


	}

	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
		->Get_QuestState(6) == Quest_ON && m_ibgmcount3 == 0)
	{
		m_ibgmcount3++;
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"mus_combat_boss_02.wav");

		//for (; iterNPC != NpcList.end(); iterNPC++)
		//{
		//	if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Tulius)
		//	{
		//		dynamic_cast<CNPC*>(*iterNPC)->Set_HP(0);
		//	}
		//}



		

	}
	if (dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
		->Get_Navi()->Get_CurrentIndex() == 90)
	{
		for (; iterNPC != NpcList.end(); iterNPC++)
		{
			if (dynamic_cast<CNPC*>(*iterNPC)->Get_NPC_Type() == NPC_Tulius)
			{
				dynamic_cast<CNPC*>(*iterNPC)->Set_HP(0);
			}
		}
	}



	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
		->Get_QuestState(6) == Quest_Complete && m_ibgmcount4 == 0)
	{
		m_ibgmcount4++;
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"mus_combat_boss_02_finale.wav");


	}



	if (dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type() == NPC_Yarl && dynamic_cast<CDialog*>(m_pDialog)->Get_QuestStage() == 1)
	{
		//이때 드래곤에게 줌인
		Quest6_On = true;
	}
	if (dynamic_cast<CDialog*>(m_pDialog)->Get_NPC_Type() == NPC_Yarl && dynamic_cast<CDialog*>(m_pDialog)->Get_QuestStage() == 2)
	{
		//이때 드래곤에게 줌인
		Quest6_On = false;
	}



}

_int CStage::Load_ItemInfo()
{

	m_pItemList = CItem::Create(m_pGraphicDev);

	vector<ItemInfo> vecItemList = dynamic_cast<CItem*>(m_pItemList)->Get_ItemLIst();

	NULL_CHECK_RETURN(m_pItemList, FALSE);

	

	fstream	fs;
	fs.open("ItemInfo.csv", ios::in);
	string str_buf;

	string str_name
	, Code
	, Type
	, Weight
	, Value;

	int count = 0;
	int rowCount = 0;
	while (!fs.eof()) {

		ItemInfo pItem;

		getline(fs, str_buf, ',');
		if (count >= 5)
		{
			if (count == 5)
			{
				int index=str_buf.find('\n');
				str_buf.erase(0, index+1);
				strcpy_s(pItem.Item_Name, str_buf.c_str());
			}
			else if (count  == 6)
			{
				pItem.Item_Code = stoi(str_buf);
				Code = str_buf;
			}
			else if (count  == 7)
			{
				pItem.Item_Type = stoi(str_buf);
				Type = str_buf;

			}
			else if (count  == 8)
			{
				pItem.Item_Weight = stoi(str_buf);
				Weight = str_buf;

			}
			else if (count  == 9)
			{
				pItem.Item_Value = stoi(str_buf);
				Value = str_buf;

			}
			else if (count  == 10)
			{
				int index = str_buf.find('\n');

				str_name = str_buf.substr(index + 1, str_buf.back());

				pItem.Item_Stat = stoi(str_buf);

				pItem.Item_Code = stoi(Code);
				pItem.Item_Type = stoi(Type);
				pItem.Item_Weight = stoi(Weight);
				pItem.Item_Value= stoi(Value);

				vecItemList.push_back(pItem);

				if (pItem.Item_Code == 12)
					dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Insert_Item(pItem);



				strcpy_s(pItem.Item_Name, str_name.c_str());

			}

			else if (count % 5==1)
			{
				pItem.Item_Code = stoi(str_buf);
				Code = str_buf;

			}
			else if (count % 5 == 2)
			{
				pItem.Item_Type = stoi(str_buf);
				Type = str_buf;

			}
			else if (count % 5== 3)
			{
				pItem.Item_Weight = stoi(str_buf);
				Weight = str_buf;

			}
			else if (count % 5 == 4)
			{
				pItem.Item_Value = stoi(str_buf);
				Value = str_buf;

			}
			else if (count % 5== 0)
			{
				int index = str_buf.find('\n');

				str_name=str_buf.substr(index+1, str_buf.back());

				pItem.Item_Stat = stoi(str_buf);

				pItem.Item_Code = stoi(Code);
				pItem.Item_Type = stoi(Type);
				pItem.Item_Weight = stoi(Weight);
				pItem.Item_Value = stoi(Value);


				vecItemList.emplace_back(pItem);				
				if (pItem.Item_Code == 12)
					dynamic_cast<CDragon*>(Get_Object(L"GameLogic", OBJECT_DRAGON))->Insert_Item(pItem);


				strcpy_s(pItem.Item_Name, str_name.c_str());

			}



		}


		count++;
	}
	fs.close();

	list<CGameObject*> NPC_list = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
	list<CGameObject*>::iterator iter = NPC_list.begin();

	int type = NPC_Type::NPC_Blacksmith;
	for(; iter!= NPC_list.end(); iter++)
	{
		if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == type)
		{
			dynamic_cast<CNPC*>(*iter)->Set_Inventory(vecItemList);
		}

		//dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_Inventory(vecItemList);

		//임시로 여기서 받은 인벤토리에서 무게량 계산
	}











	return _int();
}

_int CStage::Load_QuestInfo()
{

	vector<QuestInfo> vecQuestInfo;

	fstream	fs;
	fs.open("QuestInfo.csv", ios::in);

	string str_buf
	, str_name
	, Npc_Type
	, Npc_Code
	, Quest_id
	, Quest_State
	, Quest_Name;


	int count = 0;
	int rowCount = 0;
	while (!fs.eof()) {

		QuestInfo pQuest;

		getline(fs, str_buf, ',');
		if (count >= 4)
		{
			if (count == 4)
			{
				int index = str_buf.find('\n');
				str_buf.erase(0, index + 1);
				strcpy_s(pQuest.NPC_Type, str_buf.c_str());
			}
			else if (count == 5)
			{
				pQuest.NPC_Code = stoi(str_buf);
				Npc_Code = str_buf;
			}
			else if (count == 6)
			{
				pQuest.Quest_ID = stoi(str_buf);
				Quest_id = str_buf;

			}
			else if (count == 7)
			{
				pQuest.Quest_State = stoi(str_buf);
				Quest_State = str_buf;

			}

			else if (count == 8)
			{
				str_name = str_buf;

				int index = str_buf.find('\n');
				str_buf.erase(index, str_buf.back());

				index = str_name.find('\n');
				str_name.erase(0, index + 1);
				strcpy_s(pQuest.Quest_Name, str_buf.c_str());

				vecQuestInfo.push_back(pQuest);
		

			}
		
			else if (count % 4 == 1)
			{

				pQuest.NPC_Code = stoi(str_buf);
				Npc_Code = str_buf;

			}
			else if (count % 4 == 2)
			{

				pQuest.Quest_ID = stoi(str_buf);
				Quest_id = str_buf;

			}
			else if (count % 4 == 3)
			{

				pQuest.Quest_State = stoi(str_buf);
				Quest_State = str_buf;


			}
			else if (count % 4== 0)
			{
				strcpy_s(pQuest.NPC_Type, str_name.c_str());

				str_name = str_buf;

				int index = str_buf.find('\n');
				str_buf.erase(index, str_buf.back());

				strcpy_s(pQuest.Quest_Name, str_buf.c_str());


				pQuest.NPC_Code = stoi(Npc_Code);
				pQuest.Quest_ID = stoi(Quest_id);
				pQuest.Quest_State = stoi(Quest_State);
				
				vecQuestInfo.push_back(pQuest);

				index = str_name.find('\n');
				str_name.erase(0, index + 1);

			}


		}


		count++;
	}
	fs.close();


	dynamic_cast<CDialog*>(m_pDialog)->Set_Questlist(vecQuestInfo);
	m_vecNPC_ColliderList.resize(vecQuestInfo.size());
	







	//list<CGameObject*> NPC_list = Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
	//
	//list<CGameObject*>::iterator iter = NPC_list.begin();
	//int type = NPC_Type::NPC_Blacksmith;
	//for (; iter != NPC_list.end(); iter++)
	//	//for (int i = 0; i < 6; i++)
	//{
	//	if (dynamic_cast<CNPC*>(*iter)->Get_NPC_Type() == type)
	//	{
	//		dynamic_cast<CNPC*>(*iter)->Set_Inventory(vecItemList);
	//	}
	//
	//
	//
	//	//dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_Inventory(vecItemList);
	//
	//	//임시로 여기서 받은 인벤토리에서 무게량 계산
	//}



	return _int();
}

_int CStage::Update_InventoryScene(const _float & fTimeDelta)
{
	if (InvenKeyCount == 0 && m_nIkey) //플레이어 개인의 인벤창 보기일시
	{
		InvenKey = true;
		InvenKeyCount++;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");



		CGameObject*			pGameObject = nullptr;
		CGameObject*			pWeaponObject = nullptr;
		CGameObject*			pPlayer = nullptr;



		Inven_Window_SelectKey = UI_INVENTORY_Window0;
		dynamic_cast<CUI*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(true);

		dynamic_cast<CUI*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window3))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC))->Set_RenderOn(false);

		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ALL))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_WEAPON))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ARMOR))->Set_RenderOn(false);



		InvenSelectKey = UI_INVENTORY_ALL;
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ALL);
		dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);








	}
	else if (InvenKeyCount != 0)
	{
		InvenKey = false;	//인벤창 종료
		InvenKeyCount = 0;
		InvenSelectKey = UI_INVENTORY_ALL;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");
		CGameObject*			pGameObject = nullptr;

		dynamic_cast<CUI*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(false);

		dynamic_cast<CUI*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window3))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC))->Set_RenderOn(true);

		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ALL))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_WEAPON))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ARMOR))->Set_RenderOn(true);





		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
		dynamic_cast<CItemObject*>(pGameObject)->Set_RenderOn(false);

		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);


		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Body);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Boots);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Helmet);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Gauntlet);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Weapon);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	}


	

	return _int();
}

_int CStage::Update_NPC_InventoryScene(const _float & fTimeDelta)
{

	if (InvenKeyCount == 0 && dynamic_cast<CDialog*>(m_pDialog)->Get_InvenEnterKey()) //npc 플레이어 둘다 인벤창을 열어야 할시
	{
		
	InvenKey = true;
	InvenKeyCount++;

	CLayer*		pUILayer = Get_Layer(L"UI_Inventory");



	CGameObject*			pGameObject = nullptr;
	CGameObject*			pWeaponObject = nullptr;
	CGameObject*			pPlayer = nullptr;
	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window0);  //플레이어 개인의 인벤토리만 보여주는 윈도우창!
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);


	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_Window3); //플레이어+npc 둘다 포함하는 윈도우창!
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);


	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(true);


	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ALL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(true);

	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_WEAPON);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(true);

	(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_NPC_ARMOR);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(true);


	Inven_Window_SelectKey = UI_INVENTORY_Window3;

	InvenSelectKey = UI_INVENTORY_NPC_ALL;







	}
	else if (InvenKeyCount != 0)
	{
		InvenKey = false;	//인벤창 종료
		InvenKeyCount = 0;
		InvenSelectKey = UI_INVENTORY_ALL;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");
		CGameObject*			pGameObject = nullptr;


		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH);
		dynamic_cast<CItemObject*>(pGameObject)->Set_RenderOn(false);

		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);


		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Body);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Boots);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Helmet);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Gauntlet);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
		(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_EQUIP_Weapon);
		dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	}
	return _int();

}

_int CStage::Update_QuestWindow(const _float & fTimeDelta)
{

	if (QuestKeyCount==0) //플레이어 개인의 인벤창 보기일시
	{
		QuestKey = true;
		QuestKeyCount++;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");
		CGameObject*			pGameObject = nullptr;
	}
	else if (QuestKeyCount != 0)
	{
		QuestKey = false;	//인벤창 종료
		QuestKeyCount = 0;



	}




	return _int();
}

_int CStage::Update_SmithingWindow(const _float & fTimeDelta)
{
	InvenItemSelectKey = 0;
	if (SmithKeyCount == 0)
	{
		CraftKey = true;
		SmithKeyCount++;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");


		CGameObject* pGameObject = nullptr;
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window1))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window2))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window3))->Set_RenderOn(false);

		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_fY(400);

		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);
		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_fY(150);



		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_TextureIndex(1);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ALL))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ARMOR))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Player))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_POTION))->Set_RenderOn(false);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_ALL))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_ARMOR))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_WEAPON))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC))->Set_RenderOn(false);




		//Inven_Window_SelectKey = UI_INVENTORY_Window0;
		//InvenSelectKey = UI_INVENTORY_ALL;
		//(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ALL);
		//dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);








	}
	else if (SmithKeyCount != 0)
	{
		CraftKey = false;
		SmithKeyCount = 0;

		CGameObject* pGameObject = nullptr;
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window1))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window2))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(false);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_fY(450);




		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(false);
		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_fY(250);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_TextureIndex(0);



		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ALL))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ARMOR))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Player))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_POTION))->Set_RenderOn(true);



		m_bEKey = false;
	}


	return _int();
}

_int CStage::Update_Monster_InventoryWindow(const _float & fTimeDelta)
{
	InvenItemSelectKey = 0;
	if (Monster_InvenKeyCount == 0)
	{
		Searchkey = true;
		Monster_InvenKeyCount++;

		CLayer*		pUILayer = Get_Layer(L"UI_Inventory");


		CGameObject* pGameObject = nullptr;
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window1))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window2))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window3))->Set_RenderOn(false);

		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_fY(400);

		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(true);
		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_fY(150);



		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_TextureIndex(1);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ALL))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ARMOR))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Player))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_POTION))->Set_RenderOn(false);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_ALL))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_ARMOR))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC_WEAPON))->Set_RenderOn(false);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_NPC))->Set_RenderOn(false);




		//Inven_Window_SelectKey = UI_INVENTORY_Window0;
		//InvenSelectKey = UI_INVENTORY_ALL;
		//(pGameObject) = pUILayer->Get_UI_InvenObject(UI_INVENTORY_ALL);
		//dynamic_cast<CFont_Texture*>(pGameObject)->Set_TextureIndex(1);








	}
	else if (Monster_InvenKeyCount != 0)
	{
		Searchkey = false;
		Monster_InvenKeyCount = 0;

		CGameObject* pGameObject = nullptr;
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window0))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window1))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Window2))->Set_RenderOn(true);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_RenderOn(false);
		dynamic_cast<CUI*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMWindow))->Set_fY(450);




		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_RenderOn(false);
		dynamic_cast<CItemObject*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ITEMMESH))->Set_fY(250);

		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_WEAPON))->Set_TextureIndex(0);



		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ALL))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_ARMOR))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_Player))->Set_RenderOn(true);
		dynamic_cast<CFont_Texture*>(Get_Layer(L"UI_Inventory")->Get_UI_InvenObject(UI_INVENTORY_POTION))->Set_RenderOn(true);



		m_bEKey = false;
	}


	return _int();
}

//_int CStage::Update_QuestState(const _float & fTimeDelta)
//{
//	//dynamic_cast<CPlayer*>(Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_Inventory(vecItemList);
//
//	//vector<QuestInfo> Questlist= dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();
//
//
//
//	return _int();
//}

HRESULT CStage::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	pLayer->Set_LayerName(L"Environment");
	
	CGameObject*			pGameObject = nullptr;
	
	// DynamicCamera
	pGameObject = CDynamicCamera::Create(m_pGraphicDev, 
		&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f), 
		D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_CAMERA, pGameObject), E_FAIL);

	//// SkyBox
	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_SKY, pGameObject), E_FAIL);



	pGameObject = CStone::Create(m_pGraphicDev,L"Proto_Mesh_BlacksmithBench");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CStone*>(pGameObject)->Get_TransCom()->Set_Pos(58, 7, 25.41);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Bench, pGameObject), E_FAIL);

	pGameObject = CStone::Create(m_pGraphicDev,L"Proto_Mesh_BlacksmithWheel");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CStone*>(pGameObject)->Get_TransCom()->Set_Pos(59.73, 7, 27.14);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Wheel , pGameObject), E_FAIL);


	pGameObject = CStone::Create(m_pGraphicDev, L"Proto_Mesh_leveron");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CStone*>(pGameObject)->Get_TransCom()->Set_Pos(60.74f, 11.9f, 73.4f);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_WALL, pGameObject), E_FAIL);

	pGameObject = CStone::Create(m_pGraphicDev, L"Proto_Mesh_leveron");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CStone*>(pGameObject)->Get_TransCom()->Set_Pos(55.16f, 7.f, 34.54f);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_WALL, pGameObject), E_FAIL);


	pGameObject = CStone::Create(m_pGraphicDev, L"Proto_Mesh_leveron");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CStone*>(pGameObject)->Get_TransCom()->Set_Pos(19.f, 7.f, 33.76f);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_WALL, pGameObject), E_FAIL);
	
	

	
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CStage::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	pLayer->Set_LayerName(L"GameLogic");
	CGameObject*			pGameObject = nullptr;
	CGameObject*			pWeaponObject = nullptr;
	CGameObject*			pPlayer = nullptr;


	CLayer*		pUILayer = Get_Layer(L"UI");
	;


	// Terrain
	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_TERRAIN, pGameObject), E_FAIL);

	
	//pGameObject = CSkyheaventemple::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Map, pGameObject), E_FAIL);



//#pragma region PLAYER
//	// Player
	
//
//	// Sword
	pGameObject = CSword::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_PLAYER_WEAPON, pGameObject), E_FAIL);
   

	pPlayer = CPlayer::Create(m_pGraphicDev);
	dynamic_cast<CPlayer*>(pPlayer)->Set_Sword(pGameObject);
	dynamic_cast<CSword*>(dynamic_cast<CPlayer*>(pPlayer)->Get_Sword())->Set_BoneName("WeaponBack");
	dynamic_cast<CPlayer*>(pPlayer)->Set_Dialog(pUILayer->Get_Object(OBJECT_Dialog));
	NULL_CHECK_RETURN(pPlayer, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_PLAYER, pPlayer), E_FAIL);



	pGameObject = CDragon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDragon*>(pGameObject)->Set_Player(pPlayer);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DRAGON, pGameObject), E_FAIL);

	CGameObject* pEffect = nullptr;
	

	//for (int i = 0; i < 200; i++)
	//{
	////
	//	pEffect = CDragonEffect::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pEffect, E_FAIL);
	//dynamic_cast<CDragonEffect*>(pEffect)->Set_iIndex(i);
	//dynamic_cast<CDragonEffect*>(pEffect)->Set_BreathType(DraonBreath_Particle);
	//dynamic_cast<CDragonEffect*>(pEffect)->Set_DragonObject(pGameObject);
	//dynamic_cast<CDragonEffect*>(pEffect)->Set_TextureIndex(i%16);
	//if (40 - (i*0.2) <= 0)
	//{
	//	dynamic_cast<CDragonEffect*>(pEffect)->Set_Speed(0.2);
	//
	//}
	//else
	//{
	//
	//dynamic_cast<CDragonEffect*>(pEffect)->Set_Speed(40 - (i*0.2));
	//}
	//	
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Breath, pEffect), E_FAIL);
	////
	//}
	for (int i = 0; i < 40; i++)
	{
		pEffect = CDragonEffect::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pEffect, E_FAIL);
		dynamic_cast<CDragonEffect*>(pEffect)->Set_iIndex(i);
		dynamic_cast<CDragonEffect*>(pEffect)->Set_BreathType(DraonBreath_Beam);
		dynamic_cast<CDragonEffect*>(pEffect)->Set_DragonObject(pGameObject);
		dynamic_cast<CDragonEffect*>(pEffect)->Set_TextureIndex((i%16));
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Breath, pEffect), E_FAIL);
	



	}

	pEffect = CTrap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pEffect, E_FAIL);
	dynamic_cast<CTrap*>(pEffect)->Set_Dragon(pGameObject);
	dynamic_cast<CTrap*>(pEffect)->Set_ID(0);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Trap, pEffect), E_FAIL);


	pEffect = CTrap::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pEffect, E_FAIL);
	dynamic_cast<CTrap*>(pEffect)->Set_Dragon(pGameObject);
	dynamic_cast<CTrap*>(pEffect)->Set_ID(1);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Trap, pEffect), E_FAIL);






	pGameObject = CPlayerArmor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CPlayerArmor*>(pGameObject)->Set_Player(pPlayer);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Collider, pGameObject), E_FAIL);


	pGameObject = CPlayerBoots::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CPlayerBoots*>(pGameObject)->Set_Player(pPlayer);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Collider, pGameObject), E_FAIL);

	pGameObject = CPlayerGauntlet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CPlayerGauntlet*>(pGameObject)->Set_Player(pPlayer);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Collider, pGameObject), E_FAIL);

	pGameObject = CPlayerHelmet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CPlayerHelmet*>(pGameObject)->Set_Player(pPlayer);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Collider, pGameObject), E_FAIL);


//#pragma endregion PLAYER


	//추후에 툴에서 배치한것을 불러와 배치
	for (int i = 0; i < 9; i++)
	{
		////	// Sword
		
		
		
		
		if (i == 0 )
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			//dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(59.8f, 7.f, 37.7f);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}	
		if (i == 1)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(62.3f, 12.f, 65.19f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(84);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		if (i == 2)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(22.8f, 7.f, 34.24f);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			//dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(59.8f, 7.f, 37.7f);
			//dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(4);
			//dynamic_cast<CGuardMan*>(pGameObject)->Set_CellIndex(48);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);
	
	
			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);
	
	
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);
	
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);
	
	
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);
	
		}
		if (i == 3)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(55.4f, 7.f, 29.f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(51);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);
	
	
			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);
	
	
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);
	
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);
	
	
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);
	
		
		}
		if (i == 4)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(63.f, 7.f, 29.f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(63);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		if (i == 5)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(63.3f, 7.f, 33.6f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(65);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		if (i == 6)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(64.2f, 12.f, 58.7f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(83);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		if (i == 7)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(59.9f, 12.f, 73.f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(85);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		if (i == 8)
		{
			pGameObject = CGuardMan::Create(m_pGraphicDev, L"Proto_Mesh_GuardMan2");
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_IndexNumber(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_ID(i);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_Transform()->Set_Pos(65.29f, 12, 73.2f);
			dynamic_cast<CGuardMan*>(pGameObject)->Get_NaviCom()->Set_CellIndex(86);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_CellIndex(86);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_RenderOn(true);
			dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);


			pWeaponObject = CimperialSword::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pWeaponObject, E_FAIL);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_IndexNumber(i);
			dynamic_cast<CimperialSword*>(pWeaponObject)->Set_GuardMan(pGameObject);


			dynamic_cast<CGuardMan*>(pGameObject)->Set_Sword(pWeaponObject);

			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_GuardMan, pGameObject), E_FAIL);


			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Weapon, pWeaponObject), E_FAIL);


		}
		//dynamic_cast<CGuardMan*>(pGameObject)->Set_Player(pPlayer);
		
	
	
		
	
		
		
	
	
	
	
	}
	
	
	pGameObject = CNPC::Create(m_pGraphicDev,L"Proto_Mesh_BlackSmith");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_Blacksmith);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Adrianne Avenicci");
	dynamic_cast<CNPC*>(pGameObject)->Set_RenderOn(true);
	dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(22.54, 7, 30.2);
	dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(6);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);


	pGameObject = CNPC::Create(m_pGraphicDev, L"Proto_Mesh_Graymane");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_Forge);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Graymane");
	dynamic_cast<CNPC*>(pGameObject)->Set_RenderOn(true);

	dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(59.76, 7, 26.52);
	dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(54);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);


	pGameObject = CNPC::Create(m_pGraphicDev, L"Proto_Mesh_Market");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_market);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Carlotta");
	dynamic_cast<CNPC*>(pGameObject)->Set_RenderOn(true);

	dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(57.42, 7, 39.10);
	dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(46);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);



	pGameObject = CNPC::Create(m_pGraphicDev, L"Proto_Mesh_Yarl");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_Yarl);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Yarl");
	dynamic_cast<CNPC*>(pGameObject)->Set_RenderOn(true);

	dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(79, 23, 107);
	dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(124);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);



	pGameObject = CNPC::Create(m_pGraphicDev, L"Proto_Mesh_Tulius");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_Tulius);
	dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Tulius");
	dynamic_cast<CNPC*>(pGameObject)->Set_RenderOn(false);
	
	dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(62.43f, 12, 75.21f);
	dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(87);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);













	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Blood");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Scale(0.4, 0.4, 0.4);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_PBlood, pGameObject), E_FAIL);

	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Blood");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Scale(0.4, 0.4, 0.4);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MBlood, pGameObject), E_FAIL);	



	///////////////////////////////////////////////////////
	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(69.55, 16, 57.31);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);



	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(69.55, 50, 57.31);
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(69.55, 70, 57.31);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(
		72.42, 16, 58.26);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);


	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(
	//	72.42, 50, 58.26);	dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(72.42, 70, 58.26);
	//
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);









	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(72.17, 13.5, 34.55);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);






	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(72.17, 50.f, 34.55);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(72.17, 70, 34.55);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(72.38, 13.5, 36.57);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(72.38, 50, 36.57);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(72.38, 70, 36.57);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);






	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(63.77, 11, 26.72);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);






	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(63.77, 50, 26.72);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(63.77, 70, 26.72);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);








	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(60.78, 11, 24.89);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(60.78, 50, 24.89);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(60.78, 70, 24.89);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(58.25, 11, 22.23);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(58.25, 50, 22.23);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(58.25, 70, 22.23);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);









	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(55.12, 11, 26.22);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(55.12, 50, 26.22);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(55.12, 70, 26.22);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);








	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.11, 10.5, 36.27);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);


	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.11, 50, 36.27);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(65.11, 70, 36.27);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.68, 8.2, 32.53);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);




	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.68, 50, 32.53);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(65.68, 70, 32.53);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);








	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(56.68, 10.5, 37.74);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(56.68, 50, 37.74);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(56.68, 70, 37.74);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(50.63, 13, 36.07);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);



	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(50.63, 50, 36.07);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(50.63, 70, 36.07);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(50.89, 9.5, 32.52);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);



	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(50.89, 50, 32.52);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(50.89, 70, 32.52);
	//
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);





	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(32.97, 11.5, 24.71);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);


	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Hellfire");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(32.97, 70, 24.71);
	//dynamic_cast<CEffect*>(pGameObject)->Set_Orgin(32.97, 70, 24.71);
	//dynamic_cast<CEffect*>(pGameObject)->Set_ID(2);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(16);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);






	//Proto_Texture_Hellfire

	//2층 그라운드 불

	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(45.75, 17, 64.2);
	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);


	float _iCenterX = 63.1;
	float _iCenterY = 66.16;

	float endX = 65.96;
	float endZ = 72.44;

	int iDeltax = endX - 63.1;
	int iDeltaz = endZ - 66.16;
	int iRadius = (int)sqrt((iDeltax*iDeltax) + (iDeltaz*iDeltaz));

	float iDegree = 1;//3.14 / 180;

	float iX = 0 , iY =0;
	float Y = 12.8;
	//for (int iTheta = 0; iTheta <= 90; iTheta++)
	//{
	//	iX = _iCenterX + iRadius * cos(iTheta * iDegree);
	//	iY = _iCenterY + iRadius * sin(iTheta * iDegree);
	//	pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//	dynamic_cast<CEffect*>(pGameObject)->Set_ID(1);
	//	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Scale(1, 2, 1);
	//	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_PosX(iX);
	//	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_PosZ(iY);
	//	dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_PosY(Y);
	//
	//	dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//	
	//
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//
	//	//PUT_PIXEL(iX, iY);
	//}






	////2층 중앙광장 의 불
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.96, 12.6, 72.44);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(67.05, 12.6, 70.08);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(67.92, 12.6, 66.75);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(67.33, 12.6, 63.94);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(65.08, 12.6, 62.27);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(59.67, 12.6, 61.72);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(57.65, 12.6, 64.72);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(58.28, 12.6, 69.3
	//);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);
	//
	//pGameObject = CEffect::Create(m_pGraphicDev, L"Proto_Texture_Ground");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CEffect*>(pGameObject)->Set_RenderOn(true);
	//dynamic_cast<CEffect*>(pGameObject)->Get_Transform()->Set_Pos(59.55, 12.6, 71.54);
	//dynamic_cast<CEffect*>(pGameObject)->Set_FrameSize(21);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Ground, pGameObject), E_FAIL);

















	//pGameObject = CNPC::Create(m_pGraphicDev, L"Proto_Mesh_Graymane");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CNPC*>(pGameObject)->Set_Player(pPlayer);
	//dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Type(NPC_Type::NPC_Forge);
	//dynamic_cast<CNPC*>(pGameObject)->Set_NPC_Name(L"Graymane");
	//
	//dynamic_cast<CNPC*>(pGameObject)->Get_Transform()->Set_Pos(57.42, 7, 39.10);
	//dynamic_cast<CNPC*>(pGameObject)->Get_NaviCom()->Set_CellIndex(46);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);


	//pGameObject = CBlacksmithNPC::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CBlacksmithNPC*>(pGameObject)->Set_Player(pPlayer);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);
	//
	//
	//pGameObject = CMarketNPC::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CMarketNPC*>(pGameObject)->Set_Player(pPlayer);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);


	//pGameObject = CBarkeeperNPC::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CBarkeeperNPC*>(pGameObject)->Set_Player(pPlayer);
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_NPC, pGameObject), E_FAIL);


	//pGameObject = CMonster_draugr::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DRAUGR, pGameObject), E_FAIL);


	//// Stone
	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);

	//// Tree
	//pGameObject = CTree::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Tree", pGameObject), E_FAIL);

	//for (_ulong i = 0; i < 150; ++i)
	//{
	//	// effect
	//	pGameObject = CEffect::Create(m_pGraphicDev);
	//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Effect", pGameObject), E_FAIL);
	//}
	
	m_mapLayer.emplace(pLayerTag, pLayer);



	







	return S_OK;
}

HRESULT CStage::Ready_UI_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;
	// UI

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Map");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(660);	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(820);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.3);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MAP, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_WorldMap");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(1280);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(620);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_WORLDMAP, pGameObject), E_FAIL);


	


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_PlayerMarker");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(75);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(30);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.15);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.15);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_PLAYERMARKER, pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestMarker");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(23);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(43);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.3);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.3);
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_QuestMARKER, pGameObject), E_FAIL);




	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestMarker");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(60);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(23);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(43);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_QuestBarMARKER, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Direction");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(40);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(6);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(15);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.5);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.5);
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DIR, pGameObject), E_FAIL);

	







	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2(400);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(60);
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeX(23);
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeY(43);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.15);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.1);
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(true);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BAR, pGameObject), E_FAIL);





	

	for (int i = 0; i < 3; i++)
	{

	pGameObject = CUI::Create(m_pGraphicDev,L"Proto_Texture_UI");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(300 * i);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_UI, pGameObject), E_FAIL);

	}

	for (int i = 0; i < 10; i++)
	{
		pGameObject = CHp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CHp*>(pGameObject)->Set_fX(12*i);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_HP, pGameObject), E_FAIL);


		pGameObject = CMp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CMp*>(pGameObject)->Set_fX(12*i);

		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MP, pGameObject), E_FAIL);


		pGameObject = CSp::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CSp*>(pGameObject)->Set_fX(12 * i);
		dynamic_cast<CSp*>(pGameObject)->Set_Id(i);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_SP, pGameObject), E_FAIL);

	}
			
	pGameObject = CMonsterBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CMonsterBar*>(pGameObject)->Set_fX(300 );
	dynamic_cast<CMonsterBar*>(pGameObject)->Set_ID(OBJECT_GuardMan);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MONSTERBAR, pGameObject), E_FAIL);

	pGameObject = CMonsterBar::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CMonsterBar*>(pGameObject)->Set_fX(300 );
	//dynamic_cast<CMonsterBar*>(pGameObject)->Set_RenderOn(true);
	dynamic_cast<CMonsterBar*>(pGameObject)->Set_SizeX(1200);
	dynamic_cast<CMonsterBar*>(pGameObject)->Set_SizeY(100);

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DragonBar, pGameObject), E_FAIL);



	for (int i = 0; i < 7; i++)
	{

	pGameObject = CMonsterHP::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX(16 * i);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MONSTERHP, pGameObject), E_FAIL);

	}
	for (int i = 0; i < 7; i++)
	{

		pGameObject = CMonsterHP::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX(16 * i);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MONSTERHP2, pGameObject), E_FAIL);

	}
	for (int i = 0; i < 7; i++)
	{

		pGameObject = CMonsterHP::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX(16 * i);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MONSTERHP3, pGameObject), E_FAIL);

	}
	for (int i = 0; i < 7; i++)
	{

		pGameObject = CMonsterHP::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX(16 * i);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_MONSTERHP4, pGameObject), E_FAIL);

	}









	for (int i = 0; i < 19; i++)
	{

		pGameObject = CMonsterHP::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX2(255);

		dynamic_cast<CMonsterHP*>(pGameObject)->Set_fX(16 * i);
		//dynamic_cast<CMonsterHP*>(pGameObject)->Set_RenderOn(true);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DragonHP, pGameObject), E_FAIL);

	}


	//pGameObject = CDialog::Create(m_pGraphicDev, L"Proto_Texture_DialogBackGruond");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CDialog*>(pGameObject)->Set_SizeX(400);
	//
	//dynamic_cast<CDialog*>(pGameObject)->Set_SizeY(150);
	//
	//dynamic_cast<CDialog*>(pGameObject)->Set_fX(670.f);
	//
	//dynamic_cast<CDialog*>(pGameObject)->Set_fY(350.f);
	//
	//
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_UI, pGameObject), E_FAIL);
	
	pGameObject = CDialog::Create(m_pGraphicDev, L"Proto_Texture_Dialog");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CDialog*>(pGameObject)->Set_fX(810);
	
	dynamic_cast<CDialog*>(pGameObject)->Set_fY(330);
	
	dynamic_cast<CDialog*>(pGameObject)->Set_SizeY(500);
	//
	//dynamic_cast<CDialog*>(pGameObject)->Set_Player((Get_Layer(L"GameLogic")->Get_Object(OBJECT_PLAYER)));

	m_pDialog = pGameObject;

	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_Dialog, pGameObject), E_FAIL);

	pLayer->Set_LayerName(L"UI");


	

	m_mapLayer.emplace(pLayerTag, pLayer);


	return S_OK;
}

HRESULT CStage::Ready_UI_InventoryLayer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	pLayer->Set_LayerName(L"UI_Inventory");
	CGameObject*			pGameObject = nullptr;



	// UI
	
	
	//pGameObject = CStaticCamera::Create(m_pGraphicDev,
	//	&_vec3(0, 0, 0), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
	//	D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0, 1);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_E, pGameObject), E_FAIL);

		//





	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
		
		pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Window0");
		
		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		dynamic_cast<CUI*>(pGameObject)->Set_fX((225 * i));
		dynamic_cast<CUI*>(pGameObject)->Set_fY(0);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleX(1);
		dynamic_cast<CUI*>(pGameObject)->Set_scaleY(1);
		
		dynamic_cast<CUI*>(pGameObject)->Set_SizeX(100);
		dynamic_cast<CUI*>(pGameObject)->Set_SizeY(600);
		FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Window0, pGameObject), E_FAIL);
		
		}
		else if(i==1)
		{
			pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Window1");
		
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CUI*>(pGameObject)->Set_fX((225 * i));
			dynamic_cast<CUI*>(pGameObject)->Set_fY(0);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(1);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(1);
		
			dynamic_cast<CUI*>(pGameObject)->Set_SizeX(100);
			dynamic_cast<CUI*>(pGameObject)->Set_SizeY(600);
			FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Window1, pGameObject), E_FAIL);
		
		
		}
		else
		{
			pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Window2");
		
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			dynamic_cast<CUI*>(pGameObject)->Set_fX((0));
			dynamic_cast<CUI*>(pGameObject)->Set_fY(560);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleX(1);
			dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.2);
		
			dynamic_cast<CUI*>(pGameObject)->Set_SizeX(800);
			dynamic_cast<CUI*>(pGameObject)->Set_SizeY(190);
			FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Window2, pGameObject), E_FAIL);
		
		
		}
		



	}
	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Window3");
	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX((0));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(0);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(1);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(1);
	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(100);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(600);
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Window3, pGameObject), E_FAIL);



	pGameObject = CItemObject::Create(m_pGraphicDev, L"Proto_Texture_ITEM");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CItemObject*>(pGameObject)->Set_fX(500);
	dynamic_cast<CItemObject*>(pGameObject)->Set_fY(250);
	dynamic_cast<CItemObject*>(pGameObject)->Set_scaleX(0.3);
	dynamic_cast<CItemObject*>(pGameObject)->Set_scaleY(0.3);

	dynamic_cast<CItemObject*>(pGameObject)->Set_SizeX(284);
	dynamic_cast<CItemObject*>(pGameObject)->Set_SizeY(300);

	//dynamic_cast<CItemObject*>(pGameObject)->Set_TextureIndex(0);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_ITEMMESH, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_ItemWindow");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(420);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(150);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_ITEMWindow, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Howmany");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(420);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(150);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_HowmanyWindow, pGameObject), E_FAIL);



	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_CraftWindow");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(450);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(380);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.35);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(420);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(150);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_CraftWindow, pGameObject), E_FAIL);


	





	//아이템 윈도우창의 장착 아이템 나타내는 화살표 텍스처!
	//for (int i = 0; i < 5; i++)
	{

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Equip");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_ItemType(ITEM_BODY); //우선  몸통,머리,팔,다리, 무기 까지 타입만 생성>>해당 타입 아이템들만 장착 가능하므로

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_EQUIP_Body, pGameObject), E_FAIL);



	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Equip");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_ItemType(ITEM_BOOTS); //우선  몸통,머리,팔,다리, 무기 까지 타입만 생성>>해당 타입 아이템들만 장착 가능하므로

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_EQUIP_Boots, pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Equip");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_ItemType(ITEM_GAUNTLET); //우선  몸통,머리,팔,다리, 무기 까지 타입만 생성>>해당 타입 아이템들만 장착 가능하므로

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_EQUIP_Gauntlet, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Equip");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_ItemType(ITEM_HELMET); //우선  몸통,머리,팔,다리, 무기 까지 타입만 생성>>해당 타입 아이템들만 장착 가능하므로

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_EQUIP_Helmet, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Equip");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX(500);
	dynamic_cast<CUI*>(pGameObject)->Set_fY(450);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_ItemType(ITEM_WEAPON); //우선  몸통,머리,팔,다리, 무기 까지 타입만 생성>>해당 타입 아이템들만 장착 가능하므로

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_EQUIP_Weapon, pGameObject), E_FAIL);


	}


	//pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Decision"); //현재 InvenItemSelectKey 가리키는 위치 아이템 가리키기
	//
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CUI*>(pGameObject)->Set_fX((0));
	//dynamic_cast<CUI*>(pGameObject)->Set_fY(560);
	//dynamic_cast<CUI*>(pGameObject)->Set_scaleX(1);
	//dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.2);
	//
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeX(800);
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeY(190);
	//FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_DECISION, pGameObject), E_FAIL);



	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_NPC1");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-60));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(100);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.7);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.7);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_NPC, pGameObject), E_FAIL);



	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_All");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-60));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(140);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.7);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.7);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_NPC_ALL, pGameObject), E_FAIL);



	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Weapon");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(180);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(50);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_NPC_WEAPON, pGameObject), E_FAIL);




	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Armor");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(220);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(44);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_RenderOn(false);

	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_NPC_ARMOR, pGameObject), E_FAIL);

	//////////////////////////////////////////////////////////////////////////////
	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Player1");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(300);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.7);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.7);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(45);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Player, pGameObject), E_FAIL);






	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_All");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-60));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(350);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.7);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.7);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(21);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_ALL, pGameObject), E_FAIL);



	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Weapon");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(390);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);
	
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(50);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_WEAPON, pGameObject), E_FAIL);




	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Armor");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(430);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);
	
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(44);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_ARMOR, pGameObject), E_FAIL);
	//
	//
	//
	pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_Potion");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(470);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);

	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(48);
	dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_POTION, pGameObject), E_FAIL);



	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_E");
	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	
	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_E, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_R");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_R, pGameObject), E_FAIL);


	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Tab");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Tab, pGameObject), E_FAIL);



	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_Enter");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_Enter, pGameObject), E_FAIL);

	

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_F");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_F, pGameObject), E_FAIL);

		

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_C");

	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	dynamic_cast<CUI*>(pGameObject)->Set_RenderOn(false);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(32);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(32);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_C, pGameObject), E_FAIL);



	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;

}

HRESULT CStage::Ready_UI_QuestLayer(const _tchar * pLayerTag)
{

	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	pLayer->Set_LayerName(L"UI_Quest");

	CGameObject*			pGameObject = nullptr;


	//pGameObject = CFont_Texture::Create(m_pGraphicDev, L"Proto_Texture_QuestWindow");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_fX((-50));
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_fY(390);
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleX(0.6);
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_scaleY(0.6);
	//
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeX(50);
	//dynamic_cast<CFont_Texture*>(pGameObject)->Set_SizeY(15);
	//FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_INVENTORY_WEAPON, pGameObject), E_FAIL);

	

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestWindow");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2((395));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.35);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.35);

	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(960);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(550);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestWindow, pGameObject), E_FAIL);

	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestHorizon");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX((70));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.25);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.25);
	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(395);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(3);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestHorizon, pGameObject), E_FAIL);
	//
	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestVertical");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX2((195+80));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(320);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.34);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.34);
	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(24);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(487);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestVertical, pGameObject), E_FAIL);
	//
	//
	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestTitle");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX((400));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(200);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.45);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.45);
	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(520);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(40);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestTitle, pGameObject), E_FAIL);
	//
	//
	pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_QuestObjective");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	dynamic_cast<CUI*>(pGameObject)->Set_fX((400));
	dynamic_cast<CUI*>(pGameObject)->Set_fY(350);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.4);
	dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.4);
	
	dynamic_cast<CUI*>(pGameObject)->Set_SizeX(480);
	dynamic_cast<CUI*>(pGameObject)->Set_SizeY(15);
	FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestObjective, pGameObject), E_FAIL);


	

	m_mapLayer.emplace(pLayerTag, pLayer);

		return S_OK;
}

HRESULT CStage::Ready_UI_SmithingLayer(const _tchar * pLayeTag)
{

	

	//pGameObject = CUI::Create(m_pGraphicDev, L"Proto_Texture_CraftWindow");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CUI*>(pGameObject)->Set_fX2((395));
	//dynamic_cast<CUI*>(pGameObject)->Set_fY(300);
	//dynamic_cast<CUI*>(pGameObject)->Set_scaleX(0.35);
	//dynamic_cast<CUI*>(pGameObject)->Set_scaleY(0.35);
	//
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeX(960);
	//dynamic_cast<CUI*>(pGameObject)->Set_SizeY(550);
	//FAILED_CHECK_RETURN(pLayer->Add_UIInvenObject(UI_Quest_QuestWindow, pGameObject), E_FAIL);





	return S_OK;
}

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9			tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tLightInfo.Direction = _vec3(1.f, -1.f, 1.f);

	FAILED_CHECK_RETURN(Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

_int CStage::Load_Update_Scene(ObjectInfo * pInfo)
{
	CLayer*		pLayer = Get_Layer(L"Environment");

	
	CGameObject*			pGameObject = nullptr;
	int indexorder = 0;

	std::ifstream file("WhiterunIndex.txt");

	if (true == file.is_open())
	{
		string s;
		wstring ws;
		while (file)
		{

			getline(file, s);

			ws.assign(s.begin(), s.end());


			
			if (indexorder == pInfo->Object_ID)
			{
				pGameObject = CStatic_Object::Create(m_pGraphicDev, ws);
				dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Pos(pInfo->vPos.x, pInfo->vPos.y, pInfo->vPos.z);
				dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Scale(pInfo->vScale.x, pInfo->vScale.y, pInfo->vScale.z);
				dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Rotation(ROTATION::ROT_Y, pInfo->vRot.y);

				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
				//Proto_name.Empty();
				//str.Empty();



				break;
			}
			//Proto_name.Empty();
			//str.Empty();
			
			

			indexorder++;


			



		}
		file.close();
	}
	else
		return -1;


	return _int();
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage*	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage::Free(void)
{
	CScene::Free();
}
