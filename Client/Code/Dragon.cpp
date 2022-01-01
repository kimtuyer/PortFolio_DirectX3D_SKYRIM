#include "stdafx.h"
#include "Dragon.h"
#include "Breath.h"
#include "Export_Function.h"

CDragon::CDragon(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CDragon::CDragon(const CDragon& rhs)
	: CGameObject(rhs)
{

}

CDragon::~CDragon(void)
{
	delete m_pBreath;
	//m_pBreath->Free();
}

HRESULT CDragon::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.015f, 0.015f, 0.015f);
	m_pTransformCom->Set_Pos(114, 30.f, 2.19f);

	//m_pTransformCom->Set_Pos(62.8, 11.f, 65.85f);
	m_pBodyCollider->Set_Radius(100);
	m_pHeadCollider->Set_Radius(100);
	m_pLWingCollider->Set_Radius(100);
	m_pRWingCollider->Set_Radius(100);
	m_pTailCollider ->Set_Radius(100);

	m_iHP = 1;
	//m_pNaviCom->Set_CellIndex(1);
	m_pMeshCom->Set_AnimationIndex(32);

	

	return S_OK;
}

Engine::_int CDragon::Update_Object(const _float& fTimeDelta)
{

		m_fTime += fTimeDelta;

		CGameObject::Update_Object(fTimeDelta);

		Update_Collider(fTimeDelta);
		

		if (!m_bDeath)
		{


			if (!m_bLeverOn && m_Action_State != Current_State::CURRENT_DEATH)
				Action_Stage(fTimeDelta);


			if (m_bShotFull)
			{
				m_pMeshCom->Set_AnimationIndex(5);
				m_SetAniIndex = 5;

			}

			if (m_bLeverOn)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_alduinportaldeath.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(36);
				
			}

			if (m_Action_State == Current_State::CURRENT_DEATH)
			{
				if (m_SetAniIndex != 3 && m_SetAniIndex != 37)
				{
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_alduinportaldeath.wav", CSoundMgr::Dragon);


					m_pMeshCom->Set_AnimationIndex(18);
					m_SetAniIndex = 18;
				}
			}

			if (m_Action_State == Current_State::CURRENT_DEATH &&m_pMeshCom->Is_AnimationsetFinish()


				&& m_SetAniIndex == 18)
			{
				m_pMeshCom->Set_AnimationIndex(3);
				m_SetAniIndex = 3;

			}

			if (m_Action_State == Current_State::CURRENT_DEATH &&m_pMeshCom->Is_AnimationsetFinish()


				&& m_SetAniIndex == 3)
			{
				m_pMeshCom->Set_AnimationIndex(37);
				m_SetAniIndex = 37;

			}


			if (m_Action_State == Current_State::CURRENT_DEATH &&m_pMeshCom->Is_AnimationsetFinish()


				&& m_SetAniIndex == 37)
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_death_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(37);
				m_SetAniIndex = 37;

				m_bDeath = true;
				
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -7.5f);

			}





			/*
			1. 특정 퀘스트가 완료되었다고 확인되면,
			-Rendering on
			-드래곤의 포효 애니메이션(28) 발동
			-끝나는 대로 하늘위로 도약
			-먼저 성다리위로 날아와 착지
			-포효(28) 후 (27)건물or (29)벽 브레스 시전
			-다시 하늘로 도약
			-1층 여관위 안착
			-27 or 29 브레스 시전
			-1층중앙에 안착
			-근처 적이있으면 공격
			-다시 하늘로 도약
			-2층으로 날아가 플레이어 추적
			-플레이어는 2층에서 한바퀴 돌아 계단 올라옴
			-드래곤이 따라와 2층중앙 안착
			-트랩 발동
			-드래곤 묶인채로 공격
			-사망!



			*/





			//SetUp_OnTerrain();

			//NPC_Tongue2 bone 위치에서 브레스 발사!

			//충돌체 부착위치
			//NPC_LHand

			//NPC_RHand
			//>>좌우 날개공격위해 따로 충돌체 배치!

			//NPC_TailHub >>꼬리 공격 bone

			//NPC_Head_MagicNode__Hmag_ >>머리 공격 위해 충돌체 배치

			//NPC_Hub01 >>중심 잡고 몸통 충돌체 배치

			//if (m_bShot)
			//{
			//	m_pBreath->Update(fTimeDelta);
			//
			//}



			//Key_Input(fTimeDelta);

			//if (m_SetAniIndex != 3)
			m_pMeshCom->Play_Animation(fTimeDelta);



			if (m_bLeverOn &&m_pMeshCom->Is_AnimationsetFinish())
			{
				m_bLeverOn = false;
				dynamic_cast<CDynamicCamera*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"Environment", OBJECT_CAMERA))
					->Set_POV(OBJECT_PLAYER);
				//m_pMeshCom->Set_AnimationIndex(5);
			}


			if (m_SetAniIndex==5 && m_pMeshCom->Is_AnimationsetFinish())
			{
				m_bShotFull = false;
				m_ATT_Type = 0;
			}


		}
		else
		{

		m_pMeshCom = m_pMeshCom2;

		}
		Add_RenderGroup(RENDER_NONALPHA, this);

	
	return 0;
}

void CDragon::Render_Object(void)
{
	//m_pBodyCollider->Render_ColliderSphere(COLLTYPE(false), m_pBodyCollider->Get_CollWorldMatrix());

	//if (m_bRenderOn)
	{



		//m_pHeadCollider->Render_ColliderSphere(COLLTYPE(false), m_pHeadCollider->Get_CollWorldMatrix());
		//m_pLWingCollider->Render_ColliderSphere(COLLTYPE(false), m_pLWingCollider->Get_CollWorldMatrix());
		//m_pRWingCollider->Render_ColliderSphere(COLLTYPE(false), m_pRWingCollider->Get_CollWorldMatrix());
		//m_pTailCollider->Render_ColliderSphere(COLLTYPE(false), m_pTailCollider->Get_CollWorldMatrix());
		//



		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		if (m_bShot)
		{
			m_pBreath->Render();

		}

		//m_pNaviCom->Render_NaviMesh();

		m_pMeshCom->Render_Meshes();

	}
}

_int CDragon::Update_Collider(const _float & fTimeDelta)
{

	_vec3 vDir, vPos;

	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vDir);

	for (int i = 0; i < 5; i++)
	{

		const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName(m_wFrameBoneName[i]);

		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);

		m_pBoneMatrix = &pFrame->CombinedTransformMatrix;


		_matrix matScale, matScale2, matRev, matTrans, matTrans2, matRot, m_matCollWorld, m_matCollWorld2;
		_vec3 worldbone = { m_pBoneMatrix->_41,m_pBoneMatrix->_42,m_pBoneMatrix->_43 };
		D3DXVec3TransformCoord(&worldbone, &worldbone, m_pTransformCom->Get_WorldMatrix());

		if (i == 0)
			D3DXMatrixScaling(&matScale, 0.05, 0.05, 0.05);
		else if (i == 1 || i==2)
			D3DXMatrixScaling(&matScale, 0.03, 0.03, 0.03);
		else if(i==3)
			D3DXMatrixScaling(&matScale, 0.02, 0.02, 0.02);
		else
		D3DXMatrixScaling(&matScale, 0.01, 0.01, 0.01);


	D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);

	D3DXMatrixTranslation(&matTrans, worldbone.x, worldbone.y, worldbone.z);
	//D3DXMatrixTranslation(&matTrans2, worldbone.x + 10 * vDir.x, worldbone.y + 10 * vDir.y, worldbone.z + 10 * vDir.z);


	m_matCollWorld = matScale * matRot*matTrans;//*matRev;

	if(i==0)
		m_pBodyCollider->Set_Matrix(m_matCollWorld);

	if (i == 1)
		m_pLWingCollider->Set_Matrix(m_matCollWorld);

	if (i == 2)
		m_pRWingCollider->Set_Matrix(m_matCollWorld);

	if (i == 3)
		m_pTailCollider->Set_Matrix(m_matCollWorld);

	if (i == 4)
		m_pHeadCollider->Set_Matrix(m_matCollWorld);


	
}

	const D3DXFRAME_DERIVED*		pFrame = m_pMeshCom->Get_FrameByName("NPC_Head_MagicNode__Hmag_");

	m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
	D3DXVec3Normalize(&vDir, &vDir);
	m_pBoneMatrix = &pFrame->CombinedTransformMatrix;


	_matrix matScale, matScale2, matRev, matTrans, matTrans2, matRot, m_matCollWorld, m_matCollWorld2;
	_vec3 worldbone = { m_pBoneMatrix->_41,m_pBoneMatrix->_42,m_pBoneMatrix->_43 };
	D3DXVec3TransformCoord(&worldbone, &worldbone, m_pTransformCom->Get_WorldMatrix());
	//if (GetAsyncKeyState('T') & 0x8000)
	//{
	//
	//	
	//
	//	m_pBreath = new CBreath(&worldbone, vDir, 1);
	//	m_pBreath->Set_Target((dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER)))->Get_Transform()->m_vInfo[INFO::INFO_POS]);
	//	m_pBreath->init(m_pGraphicDev, L"../Bin/Resource/Texture/effect/Breath/fxfire/fxfirecolumn8.png");
	//
	//	//iCount++;
	//	m_bShot = true;
	//	  
	//
	//}

	
	return _int();
}

int CDragon::Action_Stage(const float & fTimeDelta)
{
	
	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
			->Get_QuestState(6) == QUEST_State::Quest_ON)
		
	{


		if (m_bActionStage == 0)
		{
			CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_flightroar_08.wav", CSoundMgr::Dragon);
			m_pMeshCom->Set_AnimationIndex(28); 

			
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(31); 

				m_bActionStage++;
			}

		}

		if (m_bActionStage == 1)
		{
			

			
			if (m_pMeshCom->Is_AnimationsetFinish()) //31번 건물위 도약시,
			{
				m_pMeshCom->Set_AnimationIndex(23); //제자리날개짓하며 부동

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);



				m_bActionStage++;


			}


		}
		if (m_bActionStage == 2)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(20);  //도약해 날기 시작!
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_03.wav", CSoundMgr::Dragon);


				m_bActionStage++;
			}


		}
		if (m_bActionStage == 3)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				if (m_SetAniIndex == 19)
				{
					m_pMeshCom->Set_AnimationIndex(19);  //고속 비행 시작!
					m_SetAniIndex = 19;
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_04.wav", CSoundMgr::Dragon);

				}
				else
				{
					m_pMeshCom->Set_AnimationIndex(19);  //고속 비행 시작!
					m_SetAniIndex = 19;
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				}

			}
			//m_bActionStage++;


			TargetPos = { 78.35f,33.f,107.29f };
			vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);


			m_pTransformCom->Move_Pos(&vDir, 10.f, fTimeDelta);

			_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

			float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
				(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
				(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

			if (fDist < 1) //영주 성 다리위 도달했을 경우
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //날다 멈추고 착지 시작모션
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);



				m_pTransformCom->Rotation(ROTATION::ROT_Y, 30.f);




			}

		}
		if (m_bActionStage == 4)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23);
				m_bActionStage++;
			}

		}
		if (m_bActionStage == 5)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				//CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(15);
				m_bActionStage++;

			}



		}
		if (m_bActionStage == 6)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_flightroar_08.wav", CSoundMgr::Dragon);
				
				m_pMeshCom->Set_AnimationIndex(28); //건물위에서 포효!
				m_bActionStage++;

			}



		}
		if (m_bActionStage == 7)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);

				
				m_pMeshCom->Set_AnimationIndex(27); //건물위에서 브레스

				m_bShotFull = true;
				m_ATT_Type = DRAGON_ATT_BREATH;

				list<CGameObject*>	 pBreathList =
					CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

				_vec3 vDir = { 1,0,0 };// m_pTransformCom->m_vInfo[INFO::INFO_LOOK];
				D3DXVec3Normalize(&vDir, &vDir);

				_vec3 vPos = { 69.6f,20.f,98.5f };// m_pTransformCom->m_vInfo[INFO::INFO_POS];// + 1.5*vDir;
				//vPos.x-=

				for (auto&p : pBreathList)
				{
					dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);

					dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(vPos);
				}

				m_bActionStage++;

			}



		}
		if (m_bActionStage == 8)
		{

			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(27); //건물위에서 브레스

				//CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon);


			}
			if (m_fTime >= 7.f)
			{
				m_bActionStage++;		
				m_fTime = 0.f;
			}



		}
		if (m_bActionStage == 9)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_bShotFull = false;
				m_ATT_Type = -1;
				list<CGameObject*>	 pBreathList =
					CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);


				for (auto&p : pBreathList)
				{
					dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(false);

					//dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(vPos);
				}

				m_pMeshCom->Set_AnimationIndex(31); //건물이륙
				m_bActionStage++;

			}



		}
		if (m_bActionStage == 10)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_takeofffx_01.wav", CSoundMgr::Dragon);

				
				m_pMeshCom->Set_AnimationIndex(23); //제자리 날개짓 부동

				if (m_pTransformCom->m_vInfo[INFO::INFO_POS].y < 58)
					m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 5.f*fTimeDelta;
				else
					m_bActionStage++;

				//m_bActionStage += 0.5;
			}


		}
		//if (m_bActionStage == 10.5)
		//{
		//	if (m_pMeshCom->Is_AnimationsetFinish())
		//	{
		//
		//		m_pMeshCom->Set_AnimationIndex(23); //제자리 날개짓 부동
		//
		//
		//
		//
		//
		//		m_bActionStage++;
		//	}
		//
		//
		//}

		if (m_bActionStage == 11)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_03.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(20); //성다리위에서 다시 날기 시작

				m_pTransformCom->Rotation(ROTATION::ROT_Y, 30.f); //20~30

				m_bActionStage++;

			}

		}
		if (m_bActionStage == 12)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(19); //2층 여관위 비행
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_03.wav", CSoundMgr::Dragon);

			}


			//m_bActionStage = 25;

			TargetPos = { 66.89f,12.f,30.8f };
			vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);




			m_pTransformCom->Move_Pos(&vDir, 8.f, fTimeDelta);




			_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

			float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
				(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
				(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

			if (fDist < 1) //여관건물위 도달했을 경우
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //날다 멈추고 착지 시작모션

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);





			}



		}
		if (m_bActionStage == 13)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_land_02.wav", CSoundMgr::Dragon);

				
				m_pMeshCom->Set_AnimationIndex(25); //여관 건물 위 안착!
				m_bActionStage++;
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -30.f);

			}
		}
		if (m_bActionStage == 14)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);

				m_pMeshCom->Set_AnimationIndex(27); //건물 위 브레스!
				m_bActionStage++;
			}
		}

		////////////////////////////////////////////////////////
		//2층 중앙위 

		if (m_bActionStage == 15)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				m_pMeshCom->Set_AnimationIndex(31); //건물이륙
				m_bActionStage++;
			}
		}

		if (m_bActionStage == 16)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23); //제자리날개짓부동
				m_bActionStage++;
			}
		}

		if (m_bActionStage == 17)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				//m_pMeshCom->Set_AnimationIndex(23); //제자리날개짓부동
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 18)
		{
			TargetPos = { 61.91f,7.f,31.04f };

			_vec3 vDir = TargetPos - m_pTransformCom->m_vInfo[INFO::INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);



			m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);

			_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];
			//
			float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
				(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
				(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

			if (fDist < 1) //1층 중앙우 ㅣ도달
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //날다 멈추고 착지 시작모션

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);


			}


		}
		if (m_bActionStage == 19)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(30); // 1층 중앙 착지
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_takeofffx_01.wav", CSoundMgr::Dragon);



				m_bActionStage++;
			}


		}


		if (m_bActionStage == 20)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				


				//if (m_fTime >= 5.f)
				{
					m_bActionStage++;
				//	m_fTime = 0.f;
				}

				//m_bActionStage++;
			}


		}
		if (m_bActionStage == 21)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				

				int GuardCount = 0;
				//주변 적 탐색, 적과 가장 가까운 부위와 거리 비교 해당 부위로 물리공격!
				//주변 적 모두 해치우거나 일정 시간 지난후 다시 이륙!
				list<CGameObject*> Elist =
					CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_GuardMan);
				if(!Elist.empty())
				for (auto p : Elist)
				{	
					if (dynamic_cast<CGuardMan*>(p)->Get_ID() == 2 || dynamic_cast<CGuardMan*>(p)->Get_ID() == 3)
					{

						


						//불능 상태가 아닌 경비병들 
						if (p->Get_ActionState() != Current_State::CURRENT_DEATH)
						{
							m_Action_State = Current_State::CURRENT_ATTACK; //드래곤도 공격상태 전환!
							GuardCount++;
							_vec3 TargetPos = dynamic_cast<CGuardMan*>(p)->Get_Transform()->m_vInfo[INFO::INFO_POS];


							_vec3 vPos = { m_pHeadCollider->Get_CollWorldMatrix()->_41, m_pHeadCollider->Get_CollWorldMatrix()->_42 
							,m_pHeadCollider->Get_CollWorldMatrix()->_43 };

							float fDistHead = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
								(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
								(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));
							
							vPos = { m_pLWingCollider->Get_CollWorldMatrix()->_41, m_pLWingCollider->Get_CollWorldMatrix()->_42
							,m_pLWingCollider->Get_CollWorldMatrix()->_43 };

							float fDistLwing = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
								(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
								(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

							vPos = { m_pRWingCollider->Get_CollWorldMatrix()->_41, m_pRWingCollider->Get_CollWorldMatrix()->_42
														,m_pRWingCollider->Get_CollWorldMatrix()->_43 };
							float fDistRwing = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
								(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
								(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

							vPos = { m_pTailCollider->Get_CollWorldMatrix()->_41, m_pTailCollider->Get_CollWorldMatrix()->_42
														,m_pTailCollider->Get_CollWorldMatrix()->_43 };

							float fDistTail =sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
								(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
								(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

							float Dist[5] = { fDistHead ,fDistLwing ,fDistRwing ,fDistTail };

							float Min = fDistHead;
							for (int i = 0; i < 3; i++)
							{
								if (Min > Dist[i + 1])
									Min = Dist[i + 1];
							}

							if (Min == Dist[0])//머리로 공격
							{
								m_ATT_Type = DRAGON_ATT_HEAD;
								m_pMeshCom->Set_AnimationIndex(0);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackbite_01.wav", CSoundMgr::Dragon);


							}
							if (Min == Dist[1])//머리로 공격
							{
								m_ATT_Type = DRAGON_ATT_LWING;
								m_pMeshCom->Set_AnimationIndex(9);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);


							}
							if (Min == Dist[2])//머리로 공격
							{
								m_ATT_Type = DRAGON_ATT_RWING;
								m_pMeshCom->Set_AnimationIndex(10);

								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);

							}
							if (Min == Dist[3])//머리로 공격
							{
								m_ATT_Type = DRAGON_ATT_TAIL;
								m_pMeshCom->Set_AnimationIndex(6);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_tail.wav", CSoundMgr::Dragon);


							}
							if (Min > 18.f)// 거리가 멀때는 브레스 공격
							{
								m_ATT_Type = DRAGON_ATT_BREATH;
								m_pMeshCom->Set_AnimationIndex(5);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);


							}



							break; //
						}


						

					}

				}
				if (GuardCount == 0) //모든 경비병이 전투불능일 경우!
				{
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_02.wav", CSoundMgr::Dragon);

					m_pMeshCom->Set_AnimationIndex(5); //지상 브레스 시전

					m_bShotFull = true;
					m_ATT_Type = DRAGON_ATT_BREATH;

					list<CGameObject*>	 pBreathList =
						CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

					_vec3 vPos = { 55.16f,7.f, 34.54f };// m_pTransformCom->m_vInfo[INFO::INFO_POS];// + 1.5*vDir;
					//vPos.x-=

					for (auto&p : pBreathList)
					{
						dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);
						dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
						dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(vPos);
					}

					if (m_fTime >= 3.f)
					{
					m_bActionStage++;
					m_fTime = 0.f;
					}
				}
				//m_bActionStage++;
			}


		}
		if (m_bActionStage == 22)
		{

			m_bShotFull = false;
			m_ATT_Type = -1;

			list<CGameObject*>	 pBreathList =
				CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

			_vec3 vPos = { 55.16f,7.f, 34.54f };// m_pTransformCom->m_vInfo[INFO::INFO_POS];// + 1.5*vDir;
			//vPos.x-=

			for (auto&p : pBreathList)
			{
				dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(false);
				dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
				//dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(vPos);
			}


			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				//주변 적 탐색, 적과 가장 가까운 부위와 거리 비교 해당 부위로 물리공격!
				//주변 적 모두 해치우거나 일정 시간 지난후 다시 이륙!

				//m_pMeshCom->Set_AnimationIndex(5);
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 23)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{


				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(30); //다시 이륙
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 24)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{


				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23); //날개짓 부동


				if (m_pTransformCom->m_vInfo[INFO::INFO_POS].y <= 31)
					m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 5.f*fTimeDelta;
				else
					m_bActionStage++;


				//m_bActionStage++;
			}


		}
		if (m_bActionStage == 25)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(20); //비행 준비
				m_pTransformCom->Rotation(ROTATION::ROT_Y, 20.f);
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 26)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				//m_bShotFull = true;
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_03.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(20); //비행

				//이때부터 플레이어 추적 공격 시작!
				TargetPos = { 19.f,7.f,33.76f };//dynamic_cast<CPlayer*>(m_pPlayer)->Get_Transform()->m_vInfo[INFO::INFO_POS];
				//TargetPos.x -= 1;
				//TargetPos.z -= 1;


				vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
				D3DXVec3Normalize(&vDir, &vDir);

				m_pTransformCom->m_vInfo[INFO::INFO_POS].x += vDir.x * 7.5*fTimeDelta;
				//m_pTransformCom->m_vInfo[INFO::INFO_POS].y += vDir.y * 7.5*fTimeDelta;

				m_pTransformCom->m_vInfo[INFO::INFO_POS].z += vDir.z * 7.5*fTimeDelta;


				//m_pTransformCom->Move_Pos(&vDir, 7.5f, fTimeDelta);

				_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

				float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				m_pTransformCom->Chase_Target_Navi(&TargetPos, vPos);


				if (25 > fDist) //여기서부터 일정 거리따라 브레스 발사 공격
				{
					m_bShotFull = true; //브레스 발사공격
					m_ATT_Type = DRAGON_ATT_BREATH;

					list<CGameObject*>Breath = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

					for (auto &p : Breath)
					{	
						dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);
						dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
						 dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(TargetPos);
					}

					if (m_fTime >= 5.f)
					{
					m_bActionStage++;
					m_fTime = 0.f;
					}

					//
					//m_pMeshCom->Set_AnimationIndex(26); //날다 멈추고 착지 시작모션

					//플레이어보고 트랩이 준비될동안 시간끌어달라 퀘스트로
					//일정시간 지나면 
					//최종적으로 플레이어가 한바퀴돌아 2층 중앙안쪽으로 들어와 드래곤 유인
					//드래곤이 따라 착지해 앞으로 기어서 들어올때 트랩 발동!



				}



			}


		}
		if (m_bActionStage == 27)
		{
			m_bShotFull = false; //브레스 발사공격
			m_ATT_Type = -1;

			list<CGameObject*>Breath = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);

			for (auto &p : Breath)
			{
				dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(false);

				//dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(TargetPos);
			}
		
		if (m_pMeshCom->Is_AnimationsetFinish())
		{
			m_pMeshCom->Set_AnimationIndex(26);

			TargetPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Transform()->m_vInfo[INFO::INFO_POS];
			TargetPos.x -= 1;
			TargetPos.z -= 1;
			vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
			D3DXVec3Normalize(&vDir, &vDir);

			m_pTransformCom->m_vInfo[INFO::INFO_POS].x += vDir.x * 7.5*fTimeDelta;
			m_pTransformCom->m_vInfo[INFO::INFO_POS].z += vDir.z * 7.5*fTimeDelta;


			//m_pTransformCom->Move_Pos(&vDir, 7.5f, fTimeDelta);

			_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

			float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
				(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
				(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

			m_pTransformCom->Chase_Target_Navi(&TargetPos, vPos);


			if (10 < fDist) //여기서부터 일정 거리따라 브레스 발사 공격
			{

				//m_bShotFull = true; //브레스 발사공격
				//m_ATT_Type = DRAGON_ATT_BREATH;
				//
				//list<CGameObject*>Breath = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);
				//
				//for (auto &p : Breath)
				//{
				//	dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(TargetPos);
				//	dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
				//	dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);
				//
				//}


				m_bActionStage++;
				//
				m_pMeshCom->Set_AnimationIndex(26); //날다 멈추고 착지 시작모션
				
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);


				m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 5.f;

				//m_pTransformCom->Rotation(ROTATION::ROT_Y, 15.f);

				//플레이어보고 트랩이 준비될동안 시간끌어달라 퀘스트로
				//일정시간 지나면 
				//최종적으로 플레이어가 한바퀴돌아 2층 중앙안쪽으로 들어와 드래곤 유인
				//드래곤이 따라 착지해 앞으로 기어서 들어올때 트랩 발동!



			}


			}
		}
		if (m_bActionStage == 28)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				m_bShotFull = false;

				m_pMeshCom->Set_AnimationIndex(19);

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_03.wav", CSoundMgr::Dragon);

				TargetPos = { 62.8f,11.f,65.85f };

				vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
				D3DXVec3Normalize(&vDir, &vDir);

				m_pTransformCom->m_vInfo[INFO::INFO_POS].x += vDir.x * 7.5*fTimeDelta;
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z += vDir.z * 7.5*fTimeDelta;

				if(m_pTransformCom->m_vInfo[INFO::INFO_POS].y<=20.f)
				m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 1 * 5 * fTimeDelta;
				//m_pTransformCom->Move_Pos(&vDir, 7.5f, fTimeDelta);

				_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

				float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				m_pTransformCom->Chase_Target_Navi(&TargetPos, vPos);

				//플레이어가 2층 중앙해당 네비 인덱스까지 이동했을 경우!
				if (dynamic_cast<CPlayer*>(m_pPlayer)->Get_Navi()->Get_CurrentIndex() == 87)
				{


					TargetPos = { 62.8f,11.f,65.85f };

					vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
					D3DXVec3Normalize(&vDir, &vDir);		

					m_pTransformCom->m_vInfo[INFO::INFO_POS].x += vDir.x * 7.5*fTimeDelta;
					m_pTransformCom->m_vInfo[INFO::INFO_POS].y += vDir.y * 7.5*fTimeDelta;
					m_pTransformCom->m_vInfo[INFO::INFO_POS].z += vDir.z * 7.5*fTimeDelta;

					_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

					m_pTransformCom->Chase_Target_Navi(&TargetPos, vPos);


					float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
						(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
						(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

					if (fDist < 4)
					{
						m_pMeshCom->Set_AnimationIndex(21);

						m_bActionStage++;
						CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);


					}

				}


			}
		}
		if (m_bActionStage == 29)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())

			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23);
				m_bActionStage++;


			}
		}
		if (m_bActionStage == 30)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())

			{

				TargetPos = { 62.8f,11.f,65.85f };

				vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
				D3DXVec3Normalize(&vDir, &vDir);

				m_pTransformCom->m_vInfo[INFO::INFO_POS].x += vDir.x * 7.5*fTimeDelta;
				m_pTransformCom->m_vInfo[INFO::INFO_POS].y += vDir.x * 7.5*fTimeDelta;
				m_pTransformCom->m_vInfo[INFO::INFO_POS].z += vDir.z * 7.5*fTimeDelta;

				_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];

				m_pTransformCom->Chase_Target_Navi(&TargetPos, vPos);

				float fDist = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				if (fDist < 4.1)
				{
					m_pMeshCom->Set_AnimationIndex(24);

					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_takeofffx_01.wav", CSoundMgr::Dragon);


					m_bActionStage++;


				}





			}
		}
		if (m_bActionStage == 31)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())	

			{	
				m_pTransformCom->Set_Pos(62.8f, 11.f, 65.85f);

				m_pMeshCom->Set_AnimationIndex(24);
				//m_pTransformCom->Rotation(ROTATION::ROT_Y, 45.5);
				m_bActionStage++;
						
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_takeofffx_01.wav", CSoundMgr::Dragon);

			}
		}
		if (m_bActionStage == 32)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())

			{
				//m_pTransformCom->Set_Pos(62.8f, 11.f, 65.85f);
				//트랩이 발동되면 해당애니메 발동!  if()
				m_pMeshCom->Set_AnimationIndex(32);
				m_bActionStage++;


			}
		}
		if (m_bActionStage == 33)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())

			{
				

				
				/*
				드래곤포박된상태에서 플레이어와
				
				*/

				m_Action_State = Current_State::CURRENT_ATTACK; //드래곤도 공격상태 전환!
				_vec3 TargetPos = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Transform()->m_vInfo[INFO::INFO_POS];


				_vec3 vPos = { m_pHeadCollider->Get_CollWorldMatrix()->_41, m_pHeadCollider->Get_CollWorldMatrix()->_42
				,m_pHeadCollider->Get_CollWorldMatrix()->_43 };

				float fDistHead = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				vPos = { m_pLWingCollider->Get_CollWorldMatrix()->_41, m_pLWingCollider->Get_CollWorldMatrix()->_42
				,m_pLWingCollider->Get_CollWorldMatrix()->_43 };

				float fDistLwing = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				vPos = { m_pRWingCollider->Get_CollWorldMatrix()->_41, m_pRWingCollider->Get_CollWorldMatrix()->_42
											,m_pRWingCollider->Get_CollWorldMatrix()->_43 };
				float fDistRwing = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				vPos = { m_pTailCollider->Get_CollWorldMatrix()->_41, m_pTailCollider->Get_CollWorldMatrix()->_42
											,m_pTailCollider->Get_CollWorldMatrix()->_43 };

				float fDistTail = sqrtf((TargetPos.x - vPos.x) *(TargetPos.x - vPos.x) +
					(TargetPos.y - vPos.y)*(TargetPos.y - vPos.y) +
					(TargetPos.z - vPos.z)*(TargetPos.z - vPos.z));

				float Dist[5] = { fDistHead ,fDistLwing ,fDistRwing ,fDistTail };

				float Min = fDistHead;
				for (int i = 0; i < 3; i++)
				{
					if (Min > Dist[i + 1])
						Min = Dist[i + 1];
				}

				if (Min == Dist[0])//머리로 공격
				{

					//if (m_SetAniIndex == 0)
					{
						//m_ATT_Type = DRAGON_ATT_BREATH;
						//m_SetAniIndex = 5;
						//m_pMeshCom->Set_AnimationIndex(5);
						//CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackbite_01.wav", CSoundMgr::Dragon);

					}
					//else
					{
						if (Min < 5.f)
						{

						m_ATT_Type = DRAGON_ATT_HEAD;
						m_pMeshCom->Set_AnimationIndex(0);
						m_SetAniIndex = 0;
						CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackbite_01.wav", CSoundMgr::Dragon);

						}
					}

					
				}
				if (Min == Dist[1])
				{
					m_ATT_Type = DRAGON_ATT_LWING;
					m_pMeshCom->Set_AnimationIndex(9);
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);

					m_SetAniIndex = 9;

				}
				if (Min == Dist[2])
				{
					m_ATT_Type = DRAGON_ATT_RWING;
					m_pMeshCom->Set_AnimationIndex(10);
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);

					m_SetAniIndex = 10;
				}
				if (Min == Dist[3])
				{
					m_ATT_Type = DRAGON_ATT_TAIL;
					m_pMeshCom->Set_AnimationIndex(6);
					m_SetAniIndex = 6;
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_tail.wav", CSoundMgr::Dragon);

				}
				if (Min > 18.f)// 거리가 멀때는 브레스 공격
				{
					m_ATT_Type = DRAGON_ATT_BREATH;
					m_pMeshCom->Set_AnimationIndex(5);
					m_SetAniIndex = 5;

					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon);

				}


				//if (m_ATT_Type == DRAGON_ATT_BREATH)
				//{
				//
				//m_bShotFull = true; //브레스 발사공격
				//
				//list<CGameObject*>Breath = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);
				//
				//for (auto &p : Breath)
				//{
				//	dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(true);
				//	dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
				//	dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(TargetPos);
				//}
				//
				//}
				//else
				//{
				//	m_bShotFull = false; //브레스 발사공격
				//
				//	list<CGameObject*>Breath = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_Breath);
				//
				//	for (auto &p : Breath)
				//	{
				//		dynamic_cast<CDragonEffect*>(p)->Set_RenderOn(false);
				//		dynamic_cast<CDragonEffect*>(p)->Set_m_IndexCount();
				//		//dynamic_cast<CDragonEffect*>(p)->Set_TargetPos(TargetPos);
				//	}
				//
				//}



				//m_pTransformCom->Set_Pos(62.8f, 11.f, 65.85f);
				//트랩이 발동되면 해당애니메 발동!  if()
				//m_pMeshCom->Set_AnimationIndex(32);
				//m_bActionStage++;


			}
		}


	}

	return 0;
}

HRESULT CDragon::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Dragon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pMeshCom2 = dynamic_cast<CDynamicMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Death"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


	// NaviMesh
	pComponent = m_pNaviCom = dynamic_cast<CNaviMesh*>(Engine::Clone_Proto(L"Proto_Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	// Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Calculator", pComponent);


	pComponent = m_pHeadCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_HeadCollider", pComponent);

	pComponent = m_pBodyCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BodyCollider", pComponent);

	pComponent = m_pTailCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_TailCollider", pComponent);

	pComponent = m_pLWingCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_LWingCollider", pComponent);

	pComponent = m_pRWingCollider = CColliderSphere::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_RWingCollider", pComponent);






	return S_OK;

}

void CDragon::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);


	if (Get_DIKeyState(DIK_UP) & 0x80)
	{
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Set_Pos(&m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)));
		m_pMeshCom->Set_AnimationIndex(54);
	}
	

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		D3DXVec3Normalize(&m_vDir, &m_vDir);
		m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * fTimeDelta);
	}	

	if (Get_DIMouseState(DIM_LB) & 0X80)
	{
		_vec3	vPos = PickUp_OnTerrain();

		m_pTransformCom->Move_PickingPos(&vPos, 10.f, fTimeDelta);
	}

	if (Get_DIMouseState(DIM_RB) & 0X80)
	{
		m_pMeshCom->Set_AnimationIndex(30);
	}

	if(true == m_pMeshCom->Is_AnimationsetFinish())
		m_pMeshCom->Set_AnimationIndex(57);
}

void CDragon::SetUp_OnTerrain(void)
{
	_vec3	vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK(pTerrainBufferCom);

	const _vec3*	ptPos = pTerrainBufferCom->Get_VtxPos();


	_float		fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight, vPos.z);
}

Engine::_vec3 CDragon::PickUp_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

CDragon* CDragon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDragon*	pInstance = new CDragon(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CDragon::Free(void)
{
	CGameObject::Free();
}

