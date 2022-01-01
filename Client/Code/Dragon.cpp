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
			1. Ư�� ����Ʈ�� �Ϸ�Ǿ��ٰ� Ȯ�εǸ�,
			-Rendering on
			-�巡���� ��ȿ �ִϸ��̼�(28) �ߵ�
			-������ ��� �ϴ����� ����
			-���� ���ٸ����� ���ƿ� ����
			-��ȿ(28) �� (27)�ǹ�or (29)�� �극�� ����
			-�ٽ� �ϴ÷� ����
			-1�� ������ ����
			-27 or 29 �극�� ����
			-1���߾ӿ� ����
			-��ó ���������� ����
			-�ٽ� �ϴ÷� ����
			-2������ ���ư� �÷��̾� ����
			-�÷��̾�� 2������ �ѹ��� ���� ��� �ö��
			-�巡���� ����� 2���߾� ����
			-Ʈ�� �ߵ�
			-�巡�� ����ä�� ����
			-���!



			*/





			//SetUp_OnTerrain();

			//NPC_Tongue2 bone ��ġ���� �극�� �߻�!

			//�浹ü ������ġ
			//NPC_LHand

			//NPC_RHand
			//>>�¿� ������������ ���� �浹ü ��ġ!

			//NPC_TailHub >>���� ���� bone

			//NPC_Head_MagicNode__Hmag_ >>�Ӹ� ���� ���� �浹ü ��ġ

			//NPC_Hub01 >>�߽� ��� ���� �浹ü ��ġ

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
			

			
			if (m_pMeshCom->Is_AnimationsetFinish()) //31�� �ǹ��� �����,
			{
				m_pMeshCom->Set_AnimationIndex(23); //���ڸ��������ϸ� �ε�

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);



				m_bActionStage++;


			}


		}
		if (m_bActionStage == 2)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(20);  //������ ���� ����!
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
					m_pMeshCom->Set_AnimationIndex(19);  //��� ���� ����!
					m_SetAniIndex = 19;
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_04.wav", CSoundMgr::Dragon);

				}
				else
				{
					m_pMeshCom->Set_AnimationIndex(19);  //��� ���� ����!
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

			if (fDist < 1) //���� �� �ٸ��� �������� ���
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //���� ���߰� ���� ���۸��
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
				
				m_pMeshCom->Set_AnimationIndex(28); //�ǹ������� ��ȿ!
				m_bActionStage++;

			}



		}
		if (m_bActionStage == 7)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);

				
				m_pMeshCom->Set_AnimationIndex(27); //�ǹ������� �극��

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
				m_pMeshCom->Set_AnimationIndex(27); //�ǹ������� �극��

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

				m_pMeshCom->Set_AnimationIndex(31); //�ǹ��̷�
				m_bActionStage++;

			}



		}
		if (m_bActionStage == 10)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_takeofffx_01.wav", CSoundMgr::Dragon);

				
				m_pMeshCom->Set_AnimationIndex(23); //���ڸ� ������ �ε�

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
		//		m_pMeshCom->Set_AnimationIndex(23); //���ڸ� ������ �ε�
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

				m_pMeshCom->Set_AnimationIndex(20); //���ٸ������� �ٽ� ���� ����

				m_pTransformCom->Rotation(ROTATION::ROT_Y, 30.f); //20~30

				m_bActionStage++;

			}

		}
		if (m_bActionStage == 12)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(19); //2�� ������ ����
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

			if (fDist < 1) //�����ǹ��� �������� ���
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //���� ���߰� ���� ���۸��

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);





			}



		}
		if (m_bActionStage == 13)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_land_02.wav", CSoundMgr::Dragon);

				
				m_pMeshCom->Set_AnimationIndex(25); //���� �ǹ� �� ����!
				m_bActionStage++;
				m_pTransformCom->Rotation(ROTATION::ROT_Y, -30.f);

			}
		}
		if (m_bActionStage == 14)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);

				m_pMeshCom->Set_AnimationIndex(27); //�ǹ� �� �극��!
				m_bActionStage++;
			}
		}

		////////////////////////////////////////////////////////
		//2�� �߾��� 

		if (m_bActionStage == 15)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{

				m_pMeshCom->Set_AnimationIndex(31); //�ǹ��̷�
				m_bActionStage++;
			}
		}

		if (m_bActionStage == 16)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23); //���ڸ��������ε�
				m_bActionStage++;
			}
		}

		if (m_bActionStage == 17)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);

				//m_pMeshCom->Set_AnimationIndex(23); //���ڸ��������ε�
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

			if (fDist < 1) //1�� �߾ӿ� �ӵ���
			{

				m_bActionStage++;

				m_pMeshCom->Set_AnimationIndex(21); //���� ���߰� ���� ���۸��

				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);


			}


		}
		if (m_bActionStage == 19)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(30); // 1�� �߾� ����
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
				//�ֺ� �� Ž��, ���� ���� ����� ������ �Ÿ� �� �ش� ������ ��������!
				//�ֺ� �� ��� ��ġ��ų� ���� �ð� ������ �ٽ� �̷�!
				list<CGameObject*> Elist =
					CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic")->Get_ObjectList(OBJECT_GuardMan);
				if(!Elist.empty())
				for (auto p : Elist)
				{	
					if (dynamic_cast<CGuardMan*>(p)->Get_ID() == 2 || dynamic_cast<CGuardMan*>(p)->Get_ID() == 3)
					{

						


						//�Ҵ� ���°� �ƴ� ��񺴵� 
						if (p->Get_ActionState() != Current_State::CURRENT_DEATH)
						{
							m_Action_State = Current_State::CURRENT_ATTACK; //�巡�ﵵ ���ݻ��� ��ȯ!
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

							if (Min == Dist[0])//�Ӹ��� ����
							{
								m_ATT_Type = DRAGON_ATT_HEAD;
								m_pMeshCom->Set_AnimationIndex(0);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackbite_01.wav", CSoundMgr::Dragon);


							}
							if (Min == Dist[1])//�Ӹ��� ����
							{
								m_ATT_Type = DRAGON_ATT_LWING;
								m_pMeshCom->Set_AnimationIndex(9);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);


							}
							if (Min == Dist[2])//�Ӹ��� ����
							{
								m_ATT_Type = DRAGON_ATT_RWING;
								m_pMeshCom->Set_AnimationIndex(10);

								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attackwing_01.wav", CSoundMgr::Dragon);

							}
							if (Min == Dist[3])//�Ӹ��� ����
							{
								m_ATT_Type = DRAGON_ATT_TAIL;
								m_pMeshCom->Set_AnimationIndex(6);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_tail.wav", CSoundMgr::Dragon);


							}
							if (Min > 18.f)// �Ÿ��� �ֶ��� �극�� ����
							{
								m_ATT_Type = DRAGON_ATT_BREATH;
								m_pMeshCom->Set_AnimationIndex(5);
								CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon_Effect);


							}



							break; //
						}


						

					}

				}
				if (GuardCount == 0) //��� ����� �����Ҵ��� ���!
				{
					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_02.wav", CSoundMgr::Dragon);

					m_pMeshCom->Set_AnimationIndex(5); //���� �극�� ����

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

				//�ֺ� �� Ž��, ���� ���� ����� ������ �Ÿ� �� �ش� ������ ��������!
				//�ֺ� �� ��� ��ġ��ų� ���� �ð� ������ �ٽ� �̷�!

				//m_pMeshCom->Set_AnimationIndex(5);
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 23)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{


				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(30); //�ٽ� �̷�
				m_bActionStage++;
			}


		}
		if (m_bActionStage == 24)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())
			{


				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_02.wav", CSoundMgr::Dragon);

				m_pMeshCom->Set_AnimationIndex(23); //������ �ε�


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

				m_pMeshCom->Set_AnimationIndex(20); //���� �غ�
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

				m_pMeshCom->Set_AnimationIndex(20); //����

				//�̶����� �÷��̾� ���� ���� ����!
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


				if (25 > fDist) //���⼭���� ���� �Ÿ����� �극�� �߻� ����
				{
					m_bShotFull = true; //�극�� �߻����
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
					//m_pMeshCom->Set_AnimationIndex(26); //���� ���߰� ���� ���۸��

					//�÷��̾�� Ʈ���� �غ�ɵ��� �ð�����޶� ����Ʈ��
					//�����ð� ������ 
					//���������� �÷��̾ �ѹ������� 2�� �߾Ӿ������� ���� �巡�� ����
					//�巡���� ���� ������ ������ �� ���ö� Ʈ�� �ߵ�!



				}



			}


		}
		if (m_bActionStage == 27)
		{
			m_bShotFull = false; //�극�� �߻����
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


			if (10 < fDist) //���⼭���� ���� �Ÿ����� �극�� �߻� ����
			{

				//m_bShotFull = true; //�극�� �߻����
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
				m_pMeshCom->Set_AnimationIndex(26); //���� ���߰� ���� ���۸��
				
				CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_wingflap_01.wav", CSoundMgr::Dragon);


				m_pTransformCom->m_vInfo[INFO::INFO_POS].y += 5.f;

				//m_pTransformCom->Rotation(ROTATION::ROT_Y, 15.f);

				//�÷��̾�� Ʈ���� �غ�ɵ��� �ð�����޶� ����Ʈ��
				//�����ð� ������ 
				//���������� �÷��̾ �ѹ������� 2�� �߾Ӿ������� ���� �巡�� ����
				//�巡���� ���� ������ ������ �� ���ö� Ʈ�� �ߵ�!



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

				//�÷��̾ 2�� �߾��ش� �׺� �ε������� �̵����� ���!
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
				//Ʈ���� �ߵ��Ǹ� �ش�ִϸ� �ߵ�!  if()
				m_pMeshCom->Set_AnimationIndex(32);
				m_bActionStage++;


			}
		}
		if (m_bActionStage == 33)
		{
			if (m_pMeshCom->Is_AnimationsetFinish())

			{
				

				
				/*
				�巡�����ڵȻ��¿��� �÷��̾��
				
				*/

				m_Action_State = Current_State::CURRENT_ATTACK; //�巡�ﵵ ���ݻ��� ��ȯ!
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

				if (Min == Dist[0])//�Ӹ��� ����
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
				if (Min > 18.f)// �Ÿ��� �ֶ��� �극�� ����
				{
					m_ATT_Type = DRAGON_ATT_BREATH;
					m_pMeshCom->Set_AnimationIndex(5);
					m_SetAniIndex = 5;

					CSoundMgr::Get_Instance()->PlaySound(L"npc_dragon_attack_03.wav", CSoundMgr::Dragon);

				}


				//if (m_ATT_Type == DRAGON_ATT_BREATH)
				//{
				//
				//m_bShotFull = true; //�극�� �߻����
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
				//	m_bShotFull = false; //�극�� �߻����
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
				//Ʈ���� �ߵ��Ǹ� �ش�ִϸ� �ߵ�!  if()
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

