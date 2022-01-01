#include "stdafx.h"
#include "Dialog.h"

#include "Export_Function.h"

CDialog::CDialog(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto)
	: CGameObject(pGraphicDev)
{
	Ready_CDialog_Object(Mesh_proto);

}

CDialog::CDialog(const CDialog& rhs)
	: CGameObject(rhs)
{

}

CDialog::~CDialog(void)
{

}

HRESULT CDialog::Ready_CDialog_Object(wstring Mesh_proto)
{
	FAILED_CHECK_RETURN(Add_Component(Mesh_proto), E_FAIL);

	m_fX = 200.f;
	m_fY = 300.f;

	m_fSizeX = 800.f;
	m_fSizeY = 100.f;

	m_iHP = 1;
	return S_OK;

}

HRESULT CDialog::Ready_Object(void)
{
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fX = 600.f;
	m_fY = 150.f;

	m_fSizeX = 800.f;
	m_fSizeY = 300.f;

	m_iHP = 1;

	return S_OK;
}

Engine::_int CDialog::Update_Object(const _float& fTimeDelta)
{
	if (m_bRenderOn) //대화창이 활성화 됐을시
	{
		m_fTime += fTimeDelta;

		CGameObject::Update_Object(fTimeDelta);

		//Add_RenderGroup(RENDER_UI, this);
		Add_RenderGroup(RENDER_NONALPHA, this);

		D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);


		if (m_bQuest) //이 키값이 true면 새로운 퀘스트가 활성화된것
		{
			for (int i = 0; i < m_vecQuestlist.size(); i++)
			{
				if (m_iNPC_type == m_vecQuestlist[i].NPC_Code)
				{
					m_vecQuestlist[i].Quest_State = QUEST_State::Quest_ON;
					//해당 퀘스트 상태가 진행중으로 변경!
				}
			}

			m_bQuest = false;
			//변경후 다시 해당 변수는 off

		}



		







		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_prevnext.wav", CSoundMgr::UI);
			if (m_iDialogSelect == 1 && m_iStage == 3)
			{
				--m_iCurrentDialogSelect;
				if (m_iCurrentDialogSelect < 0)
					m_iCurrentDialogSelect = 0;


			}
	
			if (m_iStage==0)
				{
					--m_iDialogSelect;

					if(m_iDialogSelect < 0)
					m_iDialogSelect = 0;
					//break;
				}

				
			

			




		}
		if (GetAsyncKeyState(VK_DOWN) & 0x0001)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"ui_menu_prevnext.wav", CSoundMgr::UI);

			if (m_iDialogSelect == 1 && m_iStage == 3)
			{
				++m_iCurrentDialogSelect;
				if (m_iCurrentDialogSelect >=2)
					m_iCurrentDialogSelect = 1;


			}



			if (m_iStage == 0 )
			{
				++m_iDialogSelect;

				if(m_iDialogSelect >= 3)
				m_iDialogSelect = 2;
			}








		}
		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			if (m_iDialogSelect		== 0 && m_iNPC_type==NPC_Blacksmith) //NPC의 인벤보기 선택

			{
				m_bInvenEnterKey = true; //다시 창 나갈시에 처리도 필요!
			}

			if (m_iDialogSelect == 0 && m_iStage == 0 &&  (m_iNPC_type==NPC_Guard || m_iNPC_type == NPC_Tulius || m_iNPC_type==NPC_Yarl)) //NPC만의 개인 퀘스트-1단계
			{
				m_iStage++;



			}



			if (m_iDialogSelect == 1 && m_iStage==0) //NPC만의 개인 퀘스트-1단계
			{
				m_iStage++;



			}
			else if (m_iDialogSelect == 1 && m_iStage == 1) //NPC만의 개인 퀘스트

			{
				m_iStage++;

			}
			else if (m_iDialogSelect == 0 && m_iStage == 1 && (m_iNPC_type == NPC_Guard || m_iNPC_type == NPC_Tulius || m_iNPC_type == NPC_Yarl)) //NPC만의 개인 퀘스트

			{
				m_iStage++;

			}

			else if (m_iDialogSelect == 1 && m_iStage == 2) //NPC만의 개인 퀘스트

			{
				m_iStage++;
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
				CSoundMgr::Get_Instance()->PlaySound(L"ui_quest_update.wav", CSoundMgr::UI);
			}
			//else if (m_iDialogSelect == 1 && m_iStage == 2 && m_iNPC_type == NPC_Forge)
			//{
			//
			//}


			else if (m_iDialogSelect == 0 && m_iStage == 2 && (m_iNPC_type == NPC_Guard || m_iNPC_type == NPC_Tulius || m_iNPC_type == NPC_Yarl)) //NPC만의 개인 퀘스트

			{
				m_iStage++;

			}

			else if (m_iDialogSelect == 0 && m_iStage == 3 && (m_iNPC_type == NPC_Guard || m_iNPC_type == NPC_Tulius || m_iNPC_type == NPC_Yarl)) //NPC만의 개인 퀘스트

			{
				m_iStage++;

			}



			else if (m_iDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Forge) //최종 퀘스트 수락여부

			{
				m_iStage++;

			}

			else	if (m_iDialogSelect == 1 && m_iStage == 3) //최종 퀘스트 수락여부

			{
				m_iStage++;

				//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
				//CSoundMgr::Get_Instance()->PlaySound(L"ui_quest_update.wav", CSoundMgr::UI);
				

			}
			else	if (m_iCurrentDialogSelect == 0 && m_iStage == 3 && m_iNPC_type==NPC_Forge) //최종 퀘스트 수락여부
			{
			
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
				CSoundMgr::Get_Instance()->PlaySound(L"ui_quest_update.wav", CSoundMgr::UI);
			
				m_iCurrentDialogSelect = 0;
				m_iStage++;
			
			
			}
			else if (m_iCurrentDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Tulius)


			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
				CSoundMgr::Get_Instance()->PlaySound(L"ui_quest_update.wav", CSoundMgr::UI);


			}
			//else	if (m_iCurrentDialogSelect == 0 && m_iStage == 3) //최종 퀘스트 수락여부
			//{
			//
			//	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::UI);
			//	CSoundMgr::Get_Instance()->PlaySound(L"ui_quest_update.wav", CSoundMgr::UI);
			//
			//	
			//
			//}
			else if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Guard)
			{
				CSoundMgr::Get_Instance()->StopAll();
				CSoundMgr::Get_Instance()->PlayBGM(L"mus_combat_03.wav");
				m_iStage++;

			}
			

			else	if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Blacksmith) //최종 퀘스트 수락여부
			{
				m_iCurrentDialogSelect = 0;

				//퀘스트 수락! >>이때 다시 나중 말걸시 대화는 원점으로 돌아감!
				m_iStage=0;

				m_bTabKey = true;
				m_bInvenEnterKey = false;

				//m_iStage = 0;	
				m_iCurrentDialogSelect = 0;
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

				m_bRenderOn = false;


			}
			else	if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type==NPC_Forge) //최종 퀘스트 수락여부
			{
			
			////퀘스트 수락! >>이때 다시 나중 말걸시 대화는 원점으로 돌아감!
			//m_iStage++;
			//
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			////m_iStage = 0;	
			//m_iCurrentDialogSelect = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;


			}

			else if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Yarl)
			{
			dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Set_PullOn1(true);

			dynamic_cast<CDynamicCamera*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"Environment", OBJECT_CAMERA))
				->Set_POV(OBJECT_DRAGON);

			CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_flightroar_08.wav", CSoundMgr::Dragon_Effect);




			list<CGameObject*> Groundfire = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Ground);

			list<CGameObject*>::iterator iter = Groundfire.begin();
			for (; iter != Groundfire.end(); iter++)
			{
				dynamic_cast<CEffect*>(*iter)->Set_RenderOn(true);
			}




			++m_iStage;
			}
			
			else	if (m_iCurrentDialogSelect ==0 && m_iStage == 4) //최종 퀘스트 수락여부
			{
				m_iCurrentDialogSelect = 0;

				//퀘스트 수락! >>이때 다시 나중 말걸시 대화는 원점으로 돌아감!
				m_iStage++;

				m_bTabKey = true;
				m_bInvenEnterKey = false;	

				//m_iStage = 0;	
				m_iCurrentDialogSelect = 0;
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

				m_bRenderOn = false;


			}
			else	if (m_iCurrentDialogSelect == 1 && m_iStage == 4) //최종 퀘스트 수락여부
			{
				m_iCurrentDialogSelect = 0;

				//퀘스트 수락! >>이때 다시 나중 말걸시 대화는 원점으로 돌아감!
				m_iStage++;

				m_bTabKey = true;
				m_bInvenEnterKey = false;

				//m_iStage = 0;
				m_iCurrentDialogSelect = 0;
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

				m_bRenderOn = false;


			}
			else	if (m_iDialogSelect == 1 && m_iStage == 4) //최종 퀘스트 수락여부
			{
			



				m_iStage++;

			}
			else if (m_iCurrentDialogSelect == 0 && m_iStage == 5 && m_iNPC_type == NPC_Guard)

			{

			m_iDialogSelect = 0;
			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_iCurrentDialogSelect = 0;
			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			}
			else	if (m_iDialogSelect == 1 && m_iStage == 5) //최종 퀘스트 수락여부
			{

				m_iDialogSelect = 0;
				m_bTabKey = true;
				m_bInvenEnterKey = false;

				m_iStage = 0;
				m_iCurrentDialogSelect = 0;
				dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

				m_bRenderOn = false;

			}

			if (m_iDialogSelect == 2) // 
			{

			}


		}
		if ( GetAsyncKeyState(VK_F1) & 0x0001) //tab 키 누르면 대화문 종료
		{//!dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Get_DialogCheck() &&
			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_iCurrentDialogSelect = 0;
			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic",OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			dynamic_cast<CDynamicCamera*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"Environment", OBJECT_CAMERA)
				)->Set_POV(OBJECT_PLAYER);

		}




	}
	return 0;
}

void CDialog::Render_Object(void)
{
	if (m_bRenderOn)

	{


		_matrix		matWorld, matView, matOldView, matOldProj;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._11 = m_fSizeX * 0.4;
		matWorld._22 = m_fSizeY * 0.4;
		matWorld._33 = 1.f;
		matWorld._41 = m_fX - WINCX * 0.5f;
		matWorld._42 = -m_fY + WINCY * 0.5f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);




		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);




		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000077); //0xc0 //0x00000077

		m_pTextureCom->Render_Texture();
		m_pBufferCom->Render_Buffer();

		Render_Script();


		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}

}

void CDialog::Render_Script()
{
	if (m_iNPC_type == NPC_Barkeeper  || m_iNPC_type == NPC_Blacksmith )
	{
		if (m_iDialogSelect == 0 && m_iStage==0)
		{

			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the forge??", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

		}
		if (m_iDialogSelect == 1 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the forge??", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


		}
		if ( m_iDialogSelect == 2 && m_iStage == 0 )
		{
			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 0.5)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the forge??", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능
		}


		//대화문 2 스테이지 - 블랙스미스 퀘스트

		if (m_iDialogSelect == 1 && m_iStage == 1 && m_iNPC_type == NPC_Blacksmith)
		{
			for (auto p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					if (p.Quest_State == QUEST_State::Quest_Off)
					{

						Render_Font(L"Font_Dialog", L"Uh, I have something to deliver to \n my father. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

						Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능							
						
						
						break;
					}
					if (p.Quest_State == QUEST_State::Quest_ON)
					{
						Render_Font(L"Font_Dialog", L"Hurry up and deliver it \n to my father", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
						m_iStage = 0;
						break;

					}


				}

			}

		


		}
		if (m_iDialogSelect == 1 &&  m_iStage == 2 && m_iNPC_type == NPC_Blacksmith)
		{

			//if (m_fTime >= 1.5f)
			{
				//m_fTime = 0.f;

				Render_Font(L"Font_Dialog", L"Please go to Skyforge on the \n second floor and deliver it to me.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			}
		}
		if (m_iDialogSelect == 1 &&  m_iStage == 3 && m_iNPC_type == NPC_Blacksmith)
			//if (m_fTime >= 1.5f)
		{
			//m_fTime = 0.f;


			if (m_iCurrentDialogSelect == 0)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll deliver it for you.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Sorry, look for someone else.", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능


				


			}
			if (m_iCurrentDialogSelect == 1)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll deliver it for you.", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Sorry, look for someone else.", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				


			}






		}

		if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Blacksmith)
		{
			m_bQuest = true;


			Render_Font(L"Font_Dialog", L"All right, If you bring the Item,", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"father will give you a reward!", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


			Render_Font(L"Font_ItemName", L"Quest 0: Deliver the Item ", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

			

			++m_iItemCount;
			ItemInfo Ingot;
			Ingot.Item_Code = 6;
			Ingot.Item_Weight = 1;
			Ingot.Item_Value = 30;
			Ingot.Item_Type = ITEM_Type::ITEM_MISC;
			strcpy_s(Ingot.Item_Name, "MoonStone");

			//플레이어에게  엘븐 주괴 2개 삽입!
			if (m_iItemCount == 1 && !m_bItemUpdate)
			{
			m_bItemUpdate = true;
			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			m_iItemCount = 0;
			}

			//

			for (auto &p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					p.Quest_State = QUEST_State::Quest_ON;
					m_iQuest1= QUEST_State::Quest_ON;
				}

			}
			//m_iStage++;


			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;

			//return;



		}

		if ( m_iStage == 5 && m_iNPC_type == NPC_Blacksmith)
		{
			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_bItemUpdate = false;

			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			m_iItemCount = 0;
			return;


		}










	}
	if (m_iNPC_type == NPC_Forge )
	{
		for (auto p : m_vecQuestlist)
		{
			if (p.Quest_ID == 0 && p.Quest_State == QUEST_State::Quest_ON)
			{

				if (m_iDialogSelect == 0 && m_iStage == 0 && m_iQuest1 == Quest_ON)
				{

					Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"Here, Avenicci told me to give it to you.", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

				}
				if (m_iDialogSelect == 1 && m_iStage == 0 && m_iQuest1 == Quest_ON)
				{

					Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"Here, Avenicci told me to give it \nto you", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


				}
				if (m_iDialogSelect == 2 && m_iStage == 0 && m_iQuest1 == Quest_ON)
				{
					Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 0.5)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"Here, Avenicci told me to give it \n to you", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

					Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능
				}


				//대화문 2 스테이지 - 블랙스미스 퀘스트

				if (m_iDialogSelect == 1 && m_iStage == 1 && m_iNPC_type == NPC_Forge)
				{

					Render_Font(L"Font_Dialog", L"Oh, I just needed it!\n Thank you for giving it to me ", &_vec2(530, 330), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능



				}
				if (m_iDialogSelect == 1 && m_iStage == 2 && m_iNPC_type == NPC_Forge)
				{
					++m_iItemCount;
					
					if (m_iItemCount == 1 && !m_bItemUpdate)
					{
						dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Pop_Item();
						m_iItemCount = 0;
						m_bItemUpdate = true;
					}


					Render_Font(L"Font_Quest", L"One MoonStone has been removed from the Inventory ", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
					
					Render_Font(L"Font_Dialog", L"Just one is enough for me, Here. \n I'll give you this as a reward.\n Strengthen your sword.", &_vec2(530, 330), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


				}
				if (m_iCurrentDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Forge)
					//if (m_fTime >= 1.5f)
				{
					//m_fTime = 0.f;
					Render_Font(L"Font_Dialog", L"Sit here and strengthen it", &_vec2(530, 330), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					m_bQuest = true;

					Render_Font(L"Font_ItemName", L"Quest 1: Strengthen your weapons ", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					//Render_Font(L"Font_Quest", L"One MoonStone has been removed from the Inventory ", &_vec2(400, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					

					for (auto &p : m_vecQuestlist)
					{
						//if (p.Quest_ID == 0)
						//{
						//	p.Quest_State = QUEST_State::Quest_Complete;
						//	m_iQuest1= QUEST_State::Quest_Complete;
						//
						//}

						if (p.NPC_Code == m_iNPC_type)
						{
							p.Quest_State = QUEST_State::Quest_ON;
							m_iQuest2 = QUEST_State::Quest_ON;
						}

					}










				}//이후 여길 안지나고 스킵
				if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Forge)
				{
					//Render_Font(L"Font_Dialog", L"Sit here and strengthen it", &_vec2(530, 330), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
					//
					//
					//Render_Font(L"Font_ItemName", L"Quest 1: Strengthen your weapons ", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				
				}
				if (m_iStage == 5 && m_iNPC_type == NPC_Forge)
				{
					for (auto &p : m_vecQuestlist)
					{
						if (p.Quest_ID == 0)
						{
						p.Quest_State = QUEST_State::Quest_Complete;
						m_iQuest1 = QUEST_State::Quest_Complete;

						}

					}

					m_bQuest = true;

					m_bTabKey = true;
					m_bInvenEnterKey = false;

					m_iStage = 0;
					dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

					m_bRenderOn = false;

					m_iItemCount = 0;
					return;
				}



			}
		}
	

		




	}
	if (m_iNPC_type == NPC_market)
	{

		if (m_iDialogSelect == 0 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the market??", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

		}
		if (m_iDialogSelect == 1 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the market??", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


		}
		if (m_iDialogSelect == 2 && m_iStage == 0)
		{
			Render_Font(L"Font_Dialog", L"What have you got for sale?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 0.5)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"Need any help around the markets??", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능
		}


		//대화문 2 스테이지 - 블랙스미스 퀘스트

		if (m_iDialogSelect == 1 && m_iStage == 1 && m_iNPC_type == NPC_market)
		{
			for (auto p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)

				{
					p.Quest_State = Quest_ON;

					Render_Font(L"Font_Dialog", L"Help me, the security guard takes the goods\n from our store and doesn't pay \n back the money ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
					
					//m_iStage++;
					//break;

						//Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능			

					//if (p.Quest_State == QUEST_State::Quest_Off)
					//{
					//
					//	Render_Font(L"Font_Dialog", L"Uh, I have something to deliver to \n my father. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
					//
					//	Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능							
					//
					//
					//	break;
					//}
					//if (p.Quest_State == QUEST_State::Quest_ON)
					//{
					//	Render_Font(L"Font_Dialog", L"Hurry up and deliver it \n to my father", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
					//	m_iStage = 0;
					//	break;
					//
					//}


				}

			}




		}
		if (m_iDialogSelect == 1 && m_iStage == 2 && m_iNPC_type == NPC_market)
		{

			//if (m_fTime >= 1.5f)
			{
				//m_fTime = 0.f;

				Render_Font(L"Font_Dialog", L"Go to the security guard\n and get the money!", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			}
		}
		if (m_iDialogSelect == 1 && m_iStage == 3 && m_iNPC_type == NPC_market)
			//if (m_fTime >= 1.5f)
		{
			//m_fTime = 0.f;


			if (m_iCurrentDialogSelect == 0)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll go and get it.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Sorry, look for someone else.", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능





			}
			if (m_iCurrentDialogSelect == 1)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll go and get it.", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Sorry, look for someone else.", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능




			}






		}

		if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_market)
		{
			m_bQuest = true;


			//Render_Font(L"Font_Dialog", L"All right, If you bring the Item,", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			//
			//Render_Font(L"Font_Dialog", L"father will give you a reward!", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


			Render_Font(L"Font_ItemName", L"Quest 2: Take back the Money", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


		
			//++m_iItemCount;
			//ItemInfo Ingot;
			//Ingot.Item_Code = 6;
			//Ingot.Item_Weight = 1;
			//Ingot.Item_Value = 30;
			//Ingot.Item_Type = ITEM_Type::ITEM_MISC;
			//strcpy_s(Ingot.Item_Name, "MoonStone");
			//
			////플레이어에게  엘븐 주괴 2개 삽입!
			//if (m_iItemCount == 1 && !m_bItemUpdate)
			//{
			//	m_bItemUpdate = true;
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	m_iItemCount = 0;
			//}

			//

			for (auto &p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					p.Quest_State = QUEST_State::Quest_ON;
					m_iQuest3 = QUEST_State::Quest_ON;
				}

			}
			//m_iStage++;
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;

			//return;



		}

		if (m_iStage == 5 && m_iNPC_type == NPC_market)
		{
			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_bItemUpdate = false;

			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			m_iItemCount = 0;
			return;


		}



	}


	if (m_iNPC_type == NPC_Guard)
	{

		if (m_iDialogSelect == 0 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"Hey, why didn't you pay\n that store back?", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


		}
		if (m_iDialogSelect == 1 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"Hey, why didn't you pay\n that store back?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

			Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능



		}
		


		//대화문 2 스테이지 - 블랙스미스 퀘스트

		if (m_iDialogSelect == 0 && m_iStage == 1 && m_iNPC_type == NPC_Guard)
		{
			for (auto p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)

				{
					p.Quest_State = Quest_ON;

					Render_Font(L"Font_Dialog", L"I don't have money now.\n I'll pay you back later. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					//m_iStage++;
					break;

					//Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능			

				//if (p.Quest_State == QUEST_State::Quest_Off)
				//{
				//
				//	Render_Font(L"Font_Dialog", L"Uh, I have something to deliver to \n my father. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
				//
				//	Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능							
				//
				//
				//	break;
				//}
				//if (p.Quest_State == QUEST_State::Quest_ON)
				//{
				//	Render_Font(L"Font_Dialog", L"Hurry up and deliver it \n to my father", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
				//	m_iStage = 0;
				//	break;
				//
				//}


				}

			}




		}
		if (m_iDialogSelect == 0 && m_iStage == 2 && m_iNPC_type == NPC_Guard)
		{

			//if (m_fTime >= 1.5f)
			{
				//m_fTime = 0.f;

				Render_Font(L"Font_Dialog", L"Don't lie. Give me the money right now", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			}
		}
		if (m_iDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Guard)
			//if (m_fTime >= 1.5f)
		{
			//m_fTime = 0.f;


			if (m_iCurrentDialogSelect == 0)
			{
				Render_Font(L"Font_Dialog", L"Okay, If you lose in fight with me\n Give me the money right away.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Okay,I'll say that to the store owner ", &_vec2(530, 390), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능





			}
			if (m_iCurrentDialogSelect == 1)
			{
				Render_Font(L"Font_Dialog", L"Okay, If you lose in fight with me\n Give me the money right away.", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"Okay,I'll say that to the store owner ", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능




			}






		}

		if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Guard)
		{
			m_bQuest = true;


			//Render_Font(L"Font_Dialog", L"All right, If you bring the Item,", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			//
			//Render_Font(L"Font_Dialog", L"father will give you a reward!", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


			Render_Font(L"Font_ItemName", L"Quest 3: Defeat the Guard", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

			

			//++m_iItemCount;
			//ItemInfo Ingot;
			//Ingot.Item_Code = 6;
			//Ingot.Item_Weight = 1;
			//Ingot.Item_Value = 30;
			//Ingot.Item_Type = ITEM_Type::ITEM_MISC;
			//strcpy_s(Ingot.Item_Name, "MoonStone");
			//
			////플레이어에게  엘븐 주괴 2개 삽입!
			//if (m_iItemCount == 1 && !m_bItemUpdate)
			//{
			//	m_bItemUpdate = true;
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	m_iItemCount = 0;
			//}

			//

			for (auto &p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					p.Quest_State = QUEST_State::Quest_ON;
					m_iQuest4= QUEST_State::Quest_ON;
				}

			}
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//m_bItemUpdate = false;
			//
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;
			//m_iStage++;
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;

			//return;



		}

		if (m_iStage == 5 && m_iNPC_type == NPC_Guard)
		{
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//m_bItemUpdate = false;
			//
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;
			//
			//m_iItemCount = 0;
			//return;


		}




	}


	



	//툴리우스가 플레이어에게 접근,충돌처리된상황에서 
	if (m_iNPC_type == NPC_Tulius)
	{

		if (m_iDialogSelect == 0 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"Something bad happened to \n the Whiterun.\n Stop making a fuss and\n go to Yarl.", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

			//Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


		}
		//if (m_iDialogSelect == 1 && m_iStage == 0)
		//{
		//
		//	Render_Font(L"Font_Dialog", L"Hey, why didn't you pay\n that store back?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
		//
		//	Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능
		//
		//
		//
		//}




		if (m_iDialogSelect == 0 && m_iStage == 1 && m_iNPC_type == NPC_Tulius)
		{
			for (auto p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)

				{
					m_iQuest4 = Quest_Complete;
					p.Quest_State = Quest_ON;
					m_iQuest5 = Quest_ON;
					Render_Font(L"Font_Dialog", L"If you don't go, you'll go to jail\n for killing the guard.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					CSoundMgr::Get_Instance()->StopSound(CSoundMgr::Dragon_Effect);
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_flightroar_distant_b_01.wav", CSoundMgr::Dragon_Effect);



					//m_iStage++;
					break;

					//Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능			

				//if (p.Quest_State == QUEST_State::Quest_Off)
				//{
				//
				//	Render_Font(L"Font_Dialog", L"Uh, I have something to deliver to \n my father. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
				//
				//	Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능							
				//
				//
				//	break;
				//}
				//if (p.Quest_State == QUEST_State::Quest_ON)
				//{
				//	Render_Font(L"Font_Dialog", L"Hurry up and deliver it \n to my father", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
				//	m_iStage = 0;
				//	break;
				//
				//}


				}

			}




		}
		if (m_iDialogSelect == 0 && m_iStage == 2 && m_iNPC_type == NPC_Tulius)
		{

			////if (m_fTime >= 1.5f)
			//{
			//	//m_fTime = 0.f;
			//
				Render_Font(L"Font_Dialog", L"Decide whether to go to jail or to Yarl.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			//}
		}
		if (m_iDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Tulius)
			//if (m_fTime >= 1.5f)
		{
			//m_fTime = 0.f;


			if (m_iCurrentDialogSelect == 0)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll go to Yarl.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"No, even if I go to jail,\n I won't go. ", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능





			}
			if (m_iCurrentDialogSelect == 1)
			{
				Render_Font(L"Font_Dialog", L"Okay, I'll go to Yarl.", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"No, even if I go to jail,\n I won't go.  ", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능




			}






		}

		if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Tulius)
		{
			m_bQuest = true;


			//Render_Font(L"Font_Dialog", L"All right, If you bring the Item,", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			//
			//Render_Font(L"Font_Dialog", L"father will give you a reward!", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


			Render_Font(L"Font_ItemName", L"Quest 5: Dragon threat", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

			
			//++m_iItemCount;
			//ItemInfo Ingot;
			//Ingot.Item_Code = 6;
			//Ingot.Item_Weight = 1;
			//Ingot.Item_Value = 30;
			//Ingot.Item_Type = ITEM_Type::ITEM_MISC;
			//strcpy_s(Ingot.Item_Name, "MoonStone");
			//
			////플레이어에게  엘븐 주괴 2개 삽입!
			//if (m_iItemCount == 1 && !m_bItemUpdate)
			//{
			//	m_bItemUpdate = true;
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	m_iItemCount = 0;
			//}

			//

			for (auto &p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					p.Quest_State = QUEST_State::Quest_ON;
					m_iQuest5 = QUEST_State::Quest_ON;
				}

			}
			//m_iStage++;
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;

			//return;



		}

		if (m_iStage == 5 && m_iNPC_type == NPC_Tulius)
		{
			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_bItemUpdate = false;

			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			m_iItemCount = 0;
			return;


		}




	}




	if (m_iNPC_type == NPC_Yarl)
	{

		if (m_iDialogSelect == 0 && m_iStage == 0)
		{

			Render_Font(L"Font_Dialog", L"There's a Dragon in the mountain\n you can see far away.", &_vec2(530, 310), D3DXCOLOR(1.f, 1.f, 1.f, 1.f)); //\n 줄바꿈 가능

			//Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1.f)); //\n 줄바꿈 가능


		}
		//if (m_iDialogSelect == 1 && m_iStage == 0)
		//{
		//
		//	Render_Font(L"Font_Dialog", L"Hey, why didn't you pay\n that store back?", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
		//
		//	Render_Font(L"Font_Dialog", L"What can you tell me about Whiterun?", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1.f)); //\n 줄바꿈 가능
		//
		//
		//
		//}



		//대화문 2 스테이지 - 블랙스미스 퀘스트

		if (m_iDialogSelect == 0 && m_iStage == 1 && m_iNPC_type == NPC_Yarl)
		{
			for (auto p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)

				{
					m_iQuest5 = Quest_Complete;
					p.Quest_State = Quest_ON;
					m_iQuest6 = Quest_ON;
					Render_Font(L"Font_Dialog", L"If the Dragon comes into Whiterun,\n Please lure the Dragon.", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

					//m_iStage++;
					//break;

					//Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능			

				//if (p.Quest_State == QUEST_State::Quest_Off)
				//{
				//
				//	Render_Font(L"Font_Dialog", L"Uh, I have something to deliver to \n my father. ", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
				//
				//	Render_Font(L"Font_Dialog", L"It's an item for smithing.", &_vec2(530, 345), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능							
				//
				//
				//	break;
				//}
				//if (p.Quest_State == QUEST_State::Quest_ON)
				//{
				//	Render_Font(L"Font_Dialog", L"Hurry up and deliver it \n to my father", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능
				//	m_iStage = 0;
				//	break;
				//
				//}


				}

			}




		}
		if (m_iDialogSelect == 0 && m_iStage == 2 && m_iNPC_type == NPC_Yarl)
		{

			////if (m_fTime >= 1.5f)
			//{
			//	//m_fTime = 0.f;
			//
			Render_Font(L"Font_Dialog", L"There is a trap to catch a dragon\n in the center of the second floor \n of the village", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
		//}
			Render_Font(L"Font_Dialog", L"Pull the levers all over the village\n and lure them to the Second floor", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


		}



		if (m_iDialogSelect == 0 && m_iStage == 3 && m_iNPC_type == NPC_Yarl)
			//if (m_fTime >= 1.5f)
		{
			//m_fTime = 0.f;


			if (m_iCurrentDialogSelect == 0)
			{
				Render_Font(L"Font_Dialog", L"Okay,", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"No,", &_vec2(530, 350), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능





			}
			if (m_iCurrentDialogSelect == 1)
			{
				Render_Font(L"Font_Dialog", L"Okay, ", &_vec2(530, 310), D3DXCOLOR(0.5, 0.5, 0.5, 1)); //\n 줄바꿈 가능

				Render_Font(L"Font_Dialog", L"No, ", &_vec2(530, 390), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능




			}






		}

		if (m_iCurrentDialogSelect == 0 && m_iStage == 4 && m_iNPC_type == NPC_Yarl)
		{
			m_bQuest = true;


			Render_Font(L"Font_Dialog", L"There is one lever on the second floor\n and Two levers on the first floor", &_vec2(530, 310), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능
			//
			//Render_Font(L"Font_Dialog", L"father will give you a reward!", &_vec2(530, 350), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


			Render_Font(L"Font_ItemName", L"Quest 6: Beat the Dragon!", &_vec2(250, 100), D3DXCOLOR(1, 1, 1, 1)); //\n 줄바꿈 가능


		
			//++m_iItemCount;
			//ItemInfo Ingot;
			//Ingot.Item_Code = 6;
			//Ingot.Item_Weight = 1;
			//Ingot.Item_Value = 30;
			//Ingot.Item_Type = ITEM_Type::ITEM_MISC;
			//strcpy_s(Ingot.Item_Name, "MoonStone");
			//
			////플레이어에게  엘븐 주괴 2개 삽입!
			//if (m_iItemCount == 1 && !m_bItemUpdate)
			//{
			//	m_bItemUpdate = true;
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Insert_Item(Ingot);
			//	m_iItemCount = 0;
			//}

			//

			for (auto &p : m_vecQuestlist)
			{
				if (p.NPC_Code == m_iNPC_type)
				{
					p.Quest_State = QUEST_State::Quest_ON;
					m_iQuest6 = QUEST_State::Quest_ON;
				}

			}

		
			list<CGameObject*> NPCList = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);
			list<CGameObject*>::iterator iter2 = NPCList.begin();
			for (; iter2 != NPCList.end(); iter2++)
			{
				if (dynamic_cast<CNPC*>(*iter2)->Get_NPC_Type() == NPC_Tulius)
				{
					dynamic_cast<CNPC*>(*iter2)->Set_RenderOn(false);
				}
			}





			//m_iStage++;
			//m_bTabKey = true;
			//m_bInvenEnterKey = false;
			//
			//m_iStage = 0;
			//dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);
			//
			//m_bRenderOn = false;

			//return;



		}

		if (m_iStage == 5 && m_iNPC_type == NPC_Yarl)
		{
			dynamic_cast<CStage*>(CManagement::GetInstance()->Get_Scene())->Set_PullOn1(false);

			m_bTabKey = true;
			m_bInvenEnterKey = false;

			m_iStage = 0;
			m_bItemUpdate = false;

			dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))->Set_DialogState(false);

			m_bRenderOn = false;

			m_iItemCount = 0;
			return;


		}




	}






}

HRESULT CDialog::Add_Component(wstring Mesh)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(Mesh.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	return S_OK;

}

CDialog* CDialog::Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring Mesh_proto)
{
	CDialog*	pInstance = new CDialog(pGraphicDev, Mesh_proto);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDialog::Free(void)
{
	CGameObject::Free();
}

