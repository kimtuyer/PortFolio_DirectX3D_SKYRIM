#include "stdafx.h"
#include "Player.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(16.04, 7, 33.15);
	//m_pTransformCom->Rotation(ROT_X, D3DXToRadian(180.f));

	
	m_pNaviCom->Set_CellIndex(0);

	//m_pTransformCom->Set_Pos(56.94f, 12, 66.9);

	//m_pNaviCom->Set_CellIndex(130);


	//m_pMeshComNude->Set_AnimationIndex(54);
	m_pColliderCom->Set_Radius(75);
	m_pDialog_ColliderCom->Set_Radius(50);

	m_pMeshComNude->Set_AnimationIndex(12);
	m_iAniIndex = 12;

	Set_HP(100);
	Set_AttackStat(3);


	//m_vecInventory.emplace_back()


	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{

	if (m_bArmor &&
		m_bArm &&
		m_bBoots&&
		m_bHelmet)
	{
		m_pMeshComNude = m_pMeshComFull; //모든 부위 파트 장착시 원래 풀 장착된 메쉬로 교체!
		m_bArmorFull = true;
	}
	else
	{
		m_bArmorFull = false;
	}

	CGameObject::Update_Object(fTimeDelta);

	_vec3 vDir;
	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	
	const D3DXFRAME_DERIVED*	pFrame = m_pMeshComNude->Get_FrameByName("SkirtFBone02");
	m_pBoneMatrix = &pFrame->CombinedTransformMatrix;


	_matrix matScale, matScale2,matRev, matTrans, matTrans2, matRot, m_matCollWorld , m_matCollWorld2;
	_vec3 worldbone = { m_pBoneMatrix->_41,m_pBoneMatrix->_42,m_pBoneMatrix->_43 };
	D3DXVec3TransformCoord(&worldbone, &worldbone, m_pTransformCom->Get_WorldMatrix());

		

		D3DXMatrixScaling(&matScale, 0.01, 0.01, 0.01);
		D3DXMatrixScaling(&matScale2, 0.01, 0.01, 0.01);

		D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);

		D3DXMatrixTranslation(&matTrans, worldbone.x, worldbone.y, worldbone.z);
		D3DXMatrixTranslation(&matTrans2, worldbone.x+10*vDir.x, worldbone.y + 10 * vDir.y, worldbone.z + 10 * vDir.z);

		//D3DXMatrixRotationAxis(&matRev, &m_pTransformCom->m_vInfo[INFO::INFO_UP], m_pTransformCom->m_vAngle.y);
				
		m_matCollWorld = matScale * matRot*matTrans;//*matRev;
		m_matCollWorld2= matScale * matRot*matTrans2;
		
		m_pColliderCom->Set_Matrix(m_matCollWorld);
		m_pDialog_ColliderCom->Set_Matrix(m_matCollWorld2);


	if(!m_bDialogState)
	Key_Input(fTimeDelta);

	if (GetAsyncKeyState(VK_F4) & 0x0001)
	{
		m_bDialogState = false;
		dynamic_cast<CDynamicCamera*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"Environment", OBJECT_CAMERA)
			)->Set_POV(OBJECT_PLAYER);
	}


	if (m_Action_State == CURRENT_ATTACKED)
	{
		//CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__000053CB_1.wav", CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__000053D4_1.wav", CSoundMgr::PLAYER);

		
	}
	//	m_pMeshComFull->Set_AnimationIndex(23);

	if (m_Action_State == CURRENT_POWERATTACKED)
	{
		//CSoundMgr::Get_Instance()-> StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__000053D4_1.wav", CSoundMgr::PLAYER);
		m_pMeshComNude->Set_AnimationIndex(24);
	}


	m_pMeshComNude->Play_Animation(fTimeDelta); 

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CPlayer::Render_Object(void)
{
	//	m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());
	//m_pDialog_ColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pDialog_ColliderCom->Get_CollWorldMatrix());

	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pNaviCom->Render_NaviMesh();


	m_pMeshComNude->Render_Meshes();


}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshComNude = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_PlayerNude"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshNude", pComponent);

	pComponent = m_pMeshComFull = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_PlayerFull"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_MeshFull", pComponent);



	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<CNaviMesh*>(Clone_Proto(L"Proto_Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);
	

	
	//// Collider
	pComponent = m_pColliderCom = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Collider", pComponent);

	pComponent = m_pDialog_ColliderCom = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_DialogCollider", pComponent);

	

	return S_OK;
	
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	m_fSwingTime += fTimeDelta;
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);


	//if (Get_DIKeyState(DIK_E) & 0x80)
	//{
	//	m_bArmor = true;
	//	m_bArm = true;
	//	m_bBoots = true;
	//	m_bHelmet = true;
	//
	//	m_bArmorFull = true;
	//}

	if (GetAsyncKeyState('T') & 0x0001)
	{
		list<CGameObject*>	 pBreathList =
		CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

		CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);
		_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];
		
		dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))
			->Set_ShotFull(true);  //m_bShotFull = true;

		dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Set_ATT_Type(DRAGON_ATT_BREATH);
			//DRAGON_ATT_BREATH

		for (auto&p : pBreathList)
		{
			dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);

			dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(vPos);
		}
	
			//->Set_RenderOn(true);
	
	}

	if (GetAsyncKeyState('M') & 0x0001)
	{
		if (!m_bMapKey)
		{



			_vec3	vPos, vDir;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);

			//ScreenToClient()

			//_vec2 vMouse=GetCursorPos()


		m_bMapKey = true;
		CGameObject* pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_MAP);
		dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);


		 pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_WORLDMAP);
		 dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);


		pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_QuestMARKER);

		//vPos.x/128 x 800, vPos.y/128 x 600
		

		list<CGameObject*> pNPClist = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_NPC);

		vector<QuestInfo> vecQuest = dynamic_cast<CDialog*>(m_pDialog)->Get_Questlist();
		int Next_QuestID = -1;
		int Next_NpcCode = 0;
		for (auto p : vecQuest)
		{
			if (p.Quest_State==Quest_ON)
			{
				 Next_QuestID = p.Quest_ID + 1;
			}
			if (Next_QuestID == p.Quest_ID)
				Next_NpcCode = p.NPC_Code;

		}

		for(auto p:pNPClist)
			if (dynamic_cast<CNPC*>(p)->Get_NPC_Type() == Next_NpcCode)
			{
				vPos = dynamic_cast<CNPC*>(p)->Get_Transform()->m_vInfo[INFO::INFO_POS];
				dynamic_cast<CUI*>(pMap)->Set_fX2(((vPos.x / 128) * 320) + 240);
				dynamic_cast<CUI*>(pMap)->Set_fY(600 - (((vPos.z / 128) * 320) + 100));
				dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);

			}

		pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_PLAYERMARKER);
		dynamic_cast<CUI*>(pMap)->Set_RenderOn(true);

		vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

		dynamic_cast<CUI*>(pMap)->Set_fX2(((vPos.x / 128 ) *320)+240);
		dynamic_cast<CUI*>(pMap)->Set_fY(600-(((vPos.z / 128) * 320)+100));
		

			
		}
		else
		{

			m_bMapKey = false;
			CGameObject* pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_MAP);
			dynamic_cast<CUI*>(pMap)->Set_RenderOn(false);

			pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_WORLDMAP);
			dynamic_cast<CUI*>(pMap)->Set_RenderOn(false);

			pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_QuestMARKER);
			dynamic_cast<CUI*>(pMap)->Set_RenderOn(false);

			pMap = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI")->Get_Object(OBJECT_PLAYERMARKER);
			dynamic_cast<CUI*>(pMap)->Set_RenderOn(false);

		}
	}


	//if (Get_DIKeyState(DIK_Z) & 0x80)
	//{
	//	//_vec3 vdir={}
	//	m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 5.f*fTimeDelta;
	//
	//
	//}
	//if (Get_DIKeyState(DIK_X) & 0x80)
	//{
	//	//_vec3 vdir={}
	//	m_pTransformCom->m_vInfo[INFO::INFO_POS].y -= 5.f*fTimeDelta;
	//
	//
	//}


	if (Get_DIKeyState(DIK_W) & 0x80)	
	{	

		CSoundMgr::Get_Instance()->PlaySound(L"fst_dirt_run_player_01.wav", CSoundMgr::PLAYER);
		


		_vec3	vPos, vDir;		
		m_pTransformCom->Get_Info(INFO_POS, &vPos);	
		m_pTransformCom->Get_Info(INFO_LOOK,	 &vDir);	
		D3DXVec3Normalize(&vDir, &vDir);	

		m_pTransformCom->Set_Pos(m_pNaviCom	->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).z);

		//m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);

		if (vPos == m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f))) //전방이 막혀서 충돌처리되어 제자리 위치
		{
			m_bforwardStop = true;
		}
		else
		{
			m_bforwardStop = false;

		}

		//m_pMeshComNude->Set_AnimationIndex(54);

		if (m_bEquipOn)
		{
			m_pMeshComNude->Set_AnimationIndex(15);
			m_iAniIndex = 15;

		}
		else
		{
			m_pMeshComNude->Set_AnimationIndex(13);
			m_iAniIndex = 13;

		}
		m_bWKey = true;
		m_bAKey = false;
		m_bDKey = false;
		m_bSKey = false;

	}	
	if (GetAsyncKeyState('X') & 0x8000 && GetAsyncKeyState(VK_RBUTTON) & 0X0001 && m_Action_State != CURRENT_BASH)
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);

		CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__000053CA_1.wav", CSoundMgr::PLAYER);

		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWEREffect);

		CSoundMgr::Get_Instance()->PlaySound(L"fx_swing_blade2hand_01.wav", CSoundMgr::PLAYER_POWEREffect);
		m_Action_State = CURRENT_POWERATTACK;

		m_pMeshComNude->Set_AnimationIndex(1);
		m_iAniIndex = 1;
	}

	if (GetAsyncKeyState('S') & 0x8000 &&  (m_Action_State!= CURRENT_ATTACK || m_Action_State != CURRENT_POWERATTACK))
	{

		CSoundMgr::Get_Instance()->PlaySound(L"fst_dirt_run_player_01.wav", CSoundMgr::PLAYER);
	
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		
	
		//m_pTransformCom->Move_Pos(&vDir, -5.f, fTimeDelta);
	
		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir *fTimeDelta * 5.f)).z);
	
		if (vPos == m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f))) //후방이 막혀서 충돌처리되어 제자리 위치
		{
			m_bbacwardStop = true;
		}
		else
		{
			m_bbacwardStop = false;
	
		}
	
	
	
		if (m_bEquipOn)
		{
			m_pMeshComNude->Set_AnimationIndex(17);
			m_iAniIndex = 17;
	
		}
		else
		{
			m_pMeshComNude->Set_AnimationIndex(14);
			m_iAniIndex = 14;
	
		}
	
		//m_pMeshComNude->Set_AnimationIndex(52);
	
		
		//m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
		m_bWKey = false;
		m_bAKey = false;
		m_bDKey = false;
		m_bSKey = true;
	}


	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"fst_dirt_run_player_01.wav", CSoundMgr::PLAYER);

		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		_vec3 fY = { 0,1,0 };

		_vec3 vNormal;  D3DXVec3Cross(&vNormal, &vDir, &fY);



		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vNormal *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vNormal *fTimeDelta * 5.f)).z);


		if (vPos == m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f))) //후방이 막혀서 충돌처리되어 제자리 위치
		{
			m_bRightStop = true;
		}
		else
		{
			m_bRightStop = false;

		}


		if (m_bEquipOn)
		{
			m_pMeshComNude->Set_AnimationIndex(4);
			m_iAniIndex = 4;

		}
		else
		{
			m_pMeshComNude->Set_AnimationIndex(2);
			m_iAniIndex = 2;

		}


		//m_pTransformCom->Move_Pos(&vNormal, -10.f, fTimeDelta);
	
		//m_pMeshComNude->Set_AnimationIndex(54);

		//m_pTransformCom->m_vInfo[INFO::INFO_POS]
	
			//m_vInfo[INFO_POS] += *pDir * fSpeed *fTimeDelta;
		m_bWKey = false;
		m_bAKey = false;
		m_bDKey = true;
		m_bSKey = false;
	
	
	}
	if (Get_DIKeyState(DIK_A) & 0x80)
	{
		CSoundMgr::Get_Instance()->PlaySound(L"fst_dirt_run_player_01.wav", CSoundMgr::PLAYER);

		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		D3DXVec3Normalize(&m_vDir, &vDir);


		_vec3 fY = { 0,1,0 };

		_vec3 vNormal;  D3DXVec3Cross(&vNormal, &m_vDir, &fY);


		if (m_bEquipOn)
		{
			m_pMeshComNude->Set_AnimationIndex(5);
			m_iAniIndex = 5;

		}
		else
		{
			m_pMeshComNude->Set_AnimationIndex(3);
			m_iAniIndex = 3;

		}



		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).z);


		if (vPos == m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f))) //후방이 막혀서 충돌처리되어 제자리 위치
		{
			m_bLeftStop = true;
		}
		else
		{
			m_bLeftStop = false;

		}


		//m_pTransformCom->Move_Pos(&vNormal, +10.f, fTimeDelta);
	
		//m_pMeshComNude->Set_AnimationIndex(54);

		m_bWKey = false;
		m_bAKey = true;
		m_bDKey = false;
		m_bSKey = false;

	}

	if (GetAsyncKeyState(0x52) & 0x0001 ) //무기 탈장착 스위치
	{

		if (m_bEquipOn)//장착상태면
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_Effect);

			CSoundMgr::Get_Instance()->PlaySound(L"wpn_blade2hand_sheathe.wav", CSoundMgr::PLAYER_Effect);

			m_pMeshComNude->Set_AnimationIndex(10); //장착 해제WeaponBack
			dynamic_cast<CSword*>(m_pSword)->Set_BoneName("WeaponBack");
			m_bEquipOn = false;

		}
		else
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_Effect);

			CSoundMgr::Get_Instance()->PlaySound(L"wpn_blade2hand_sheathe.wav", CSoundMgr::PLAYER_Effect);
			m_bEquipOn = true;
			m_pMeshComNude->Set_AnimationIndex(11); //장착 
			dynamic_cast<CSword*>(m_pSword)->Set_BoneName("NPC_R_Finger02__RF02_");
		}



	}







	if (Get_DIKeyState(DIK_F) & 0x80 && m_bDialogOn) //상대 NPC 충돌구체와 접촉한 상황에서 대화창으로 넘어가기 위한 스위치
	{
	
	
		dynamic_cast<CDialog*>(m_pDialog)->Set_RenderOn(true);
		m_bDialogState = true;


		dynamic_cast<CDynamicCamera*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"Environment", OBJECT_CAMERA)
			)->Set_POV(OBJECT_NPC);
	
	
	}




	//if (Get_DIKeyState(DIK_UP) & 0x80)
	//{
	//	_vec3	vPos, vDir;
	//	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	//	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	//	D3DXVec3Normalize(&vDir, &vDir);
	//	
	//	m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).x,
	//		SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).z);
	//	
	//	
	//	m_pMeshComNude->Set_AnimationIndex(54);
	//
	//	m_pMeshComNude->Set_AnimationIndex(6);
	//}
	

	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	D3DXVec3Normalize(&m_vDir, &m_vDir);
	//	m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
	//	//m_pMeshComNude->Set_AnimationIndex(7);
	//
	//}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	}
	
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * fTimeDelta);
	}	

	//if (Engine::Get_DIMouseState(DIM_LB) & 0X80)
	//{
	//	_vec3	vPos = PickUp_OnTerrain();
	//
	//	m_pTransformCom->Move_PickingPos(&vPos, 10.f, fTimeDelta);
	//}

	if (m_bArmorFull)
	{


		if (Get_DIKeyState(DIK_W) && Engine::Get_DIMouseState(DIM_RB) & 0X80)
		{

			m_Action_State = CURRENT_ATTACK;
			//m_pMeshComNude->Set_AnimationIndex(8);

			m_pMeshComNude->Set_AnimationIndex(16);
			m_iAniIndex = 16;

		}
		if (GetAsyncKeyState(VK_LBUTTON) & 0X8000 && m_iAniIndex != 20) //방어상태
		{

			//방어상태도 만들기!
			m_Action_State = CURRENT_BLOCK;
			//m_pMeshComNude->Set_AnimationIndex(8);

			m_pMeshComNude->Set_AnimationIndex(22);
			m_iAniIndex = 22;

		}
		if (GetAsyncKeyState(VK_LBUTTON) & 0X8000 && GetAsyncKeyState(VK_RBUTTON) & 0x0001)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);

			CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__00004E4B_1.wav", CSoundMgr::PLAYER);


			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_Effect);
			CSoundMgr::Get_Instance()->PlaySound(L"fx_swing_blade2hand_02.wav", CSoundMgr::PLAYER_Effect);

			//방어상태도 만들기!
			m_Action_State = CURRENT_BASH;
			//m_pMeshComNude->Set_AnimationIndex(8);

			m_pMeshComNude->Set_AnimationIndex(20);
			m_iAniIndex = 20;

		}
		//if (GetAsyncKeyState(VK_LBUTTON) & 0x0001 )
		//{
		//	//방어상태도 만들기!
		//	m_Action_State = CURRENT_ATTACK;
		//	//m_pMeshComNude->Set_AnimationIndex(8);
		//
		//	m_pMeshComNude->Set_AnimationIndex(9);
		//	m_iAniIndex = 9;
		//
		//}

		


		if (GetAsyncKeyState(VK_RBUTTON) & 0X0001 && (m_Action_State != CURRENT_BASH || m_Action_State != CURRENT_POWERATTACK))
		{
			//if (m_fSwingTime >= 2)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);

				CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__00004E4B_1.wav", CSoundMgr::PLAYER);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_Effect);

				CSoundMgr::Get_Instance()->PlaySound(L"fx_swing_blade2hand_01.wav", CSoundMgr::PLAYER_Effect);
				//m_fSwingTime = 0;
			}

			m_Action_State = CURRENT_ATTACK;

			m_iAttackCount++;

			if (m_iAttackCount % 2 == 0)
			{
				m_pMeshComNude->Set_AnimationIndex(8);
				m_iAniIndex = 8;

			}
			else
			{
				m_pMeshComNude->Set_AnimationIndex(9);
				m_iAniIndex = 9;

			}

			m_fSwingTime = 0;
		}
		if (GetAsyncKeyState('F') & 0x8000 && GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			//if (m_fSwingTime >= 2)
			{
				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER_POWEREffect);

				CSoundMgr::Get_Instance()->PlaySound(L"GeraltDialogue__000053CA_1.wav", CSoundMgr::PLAYER_POWEREffect);

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);

				CSoundMgr::Get_Instance()->PlaySound(L"fx_swing_blade2hand_01.wav", CSoundMgr::PLAYER);
			
				//m_fSwingTime = 0;
			}

			m_Action_State = CURRENT_POWERATTACK;
			//m_pMeshComNude->Set_AnimationIndex(8);

			m_pMeshComNude->Set_AnimationIndex(7);
			m_iAniIndex = 7;
			m_fSwingTime = 0;
		}

		//if (GetAsyncKeyState('R') & 0x8000& GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		//{
		//	//if (m_fSwingTime >= 2)
		//	{
		//		CSoundMgr::Get_Instance()->PlaySound(L"fx_swing_blade2hand_01.wav", CSoundMgr::PLAYER);
		//		//m_fSwingTime = 0;
		//	}
		//
		//	m_Action_State = CURRENT_ATTACK;
		//	//m_pMeshComNude->Set_AnimationIndex(8);
		//
		//	m_pMeshComNude->Set_AnimationIndex(7);
		//	m_iAniIndex = 7;
		//	m_fSwingTime = 0;
		//}

	}




	int dwMouseMove = 0;

	if (dwMouseMove =Engine:: Get_DIMouseMove(DIMS_X))
	{
		
		
		if(dwMouseMove>0)
		
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * fTimeDelta);
		else
		{
			m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);

		}
		
		
		
		
		
		//m_bRotCheck = true;


		//vRot_Look = m_vAt - m_vEye;

		//_matrix		matRot;
		//D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));
		//D3DXVec3TransformNormal(&vRot_Look, &vRot_Look, &matRot);


		//m_vAt = m_vEye + vRot_Look;


		//D3DXVec3Normalize(&vRot_Look, &vRot_Look);
		//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(45) * fTimeDelta);

		//플레이어가 회전해 현재 바라보는 방향과 카메라가 회전해 바라보는
		//방향이 일치하려면?
		//m_pTransformCom->Rotation(ROTATION::Y,)


		//m_pTransformCom->m_vInfo[INFO::INFO_LOOK] += vRot_Look;


	}
	

	//_long	dwMouseMove = 0;
	//
	//if (dwMouseMove = Get_DIMouseMove(DIMS_Y))
	//{
	//	//if(dwMouseMove<0)
	//	//	m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	//	//
	//
	//	//else if(dwMouseMove>0)
	//	{
	//		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * fTimeDelta);
	//
	//	}
	//}
		
	
	
	
		//m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_PLAYER, L"Com_Transform", ID_DYNAMIC));
		//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f) * fTimeDelta);
		////m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	
	
	
	












	if (true == m_pMeshComNude->Is_AnimationsetFinish() && !m_bEquipOn)
	{
		m_pMeshComNude->Set_AnimationIndex(12);
		
		m_iAniIndex = 12;

		//m_pMeshComNude->Set_AnimationIndex(53);
	}
	else if (true == m_pMeshComNude->Is_AnimationsetFinish() && m_bEquipOn) //장착 완료후 
	{
		m_pMeshComNude->Set_AnimationIndex(0);
		m_iAniIndex = 0;

		m_Action_State =CURRENT_IDLE;


	}
	else if (true == m_pMeshComNude->Is_AnimationsetFinish() && m_Action_State== CURRENT_BASH) //장착 완료후 
	{
		m_pMeshComNude->Set_AnimationIndex(0);
		m_iAniIndex = 0;

		m_Action_State = CURRENT_IDLE;


	}

	

				
	//else if (true == m_pMeshComNude->Is_AnimationsetFinish() && m_iAniIndex==9) //장착 완료후 
	//{
	//	m_pMeshComNude->Set_AnimationIndex(8);
	//	m_iAniIndex = 8;
	//
	//	m_Action_State = CURRENT_ATTACK;
	//
	//
	//}
	//else if (true == m_pMeshComNude->Is_AnimationsetFinish() && m_iAniIndex == 8) //장착 완료후 
	//{
	//	m_pMeshComNude->Set_AnimationIndex(9);
	//	m_iAniIndex = 9;
	//
	//	m_Action_State = CURRENT_ATTACK;
	//
	//
	//}



}

_float CPlayer::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	_ulong Cellindex = m_pNaviCom->Get_CurrentIndex();
	
	_vec3 A = m_pNaviCom->Get_Cell(Cellindex)->MFC_Get_Point(CCell::POINT_A);
	_vec3 B = m_pNaviCom->Get_Cell(Cellindex)->MFC_Get_Point(CCell::POINT_B);
	_vec3 C = m_pNaviCom->Get_Cell(Cellindex)->MFC_Get_Point(CCell::POINT_C);


	return		m_pCalculatorCom->Compute_HeightOnNavi(&vPos, A, B, C);


	//CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic",OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	//NULL_CHECK(pTerrainBufferCom);
	//
	//const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();
	//
	//
	//_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	//m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_vec3 CPlayer::PickUp_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	CGameObject::Free();
}

