#include "stdafx.h"
#include "GuardMan.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CGuardMan::CGuardMan(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh)
	: CGameObject(pGraphicDev)
{
	(Add_Component(Mesh), E_FAIL);

}

CGuardMan::CGuardMan(const CGuardMan& rhs)
	: CGameObject(rhs)
{

}

CGuardMan::~CGuardMan(void)
{

}

HRESULT CGuardMan::Ready_Object(void)
{
	//FAILED_CHECK_RETURN(Add_Component(Mesh), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(56.94f, 12, 66.9);
	
	m_pNaviCom->Set_CellIndex(130);
	m_pMeshCom->Set_AnimationIndex(6);
	m_pColliderCom->Set_Radius(50);

	
	//m_pMeshCom->Set_AnimationIndex(13);
	Set_HP(10);
	Set_AttackStat(3);
	Set_ActionState(CURRENT_IDLE);
	return S_OK;
}

Engine::_int CGuardMan::Update_Object(const _float& fTimeDelta)
{
	//CGameObject::Update_Object(fTimeDelta);

	m_fAniTime += fTimeDelta;
	//if (m_iID == 2 || m_iID == 3  )
	//{
	//	if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
	//		->Get_QuestState(6) == QUEST_State::Quest_ON
	//		) //3번 경비병싸움마무리후 4번 드래곤위협퀘스트 발동되면!
	//	{
	//		m_bRenderOn = true;
	//	}
	//
	//}


	if (m_bRenderOn)

	{


		m_fDeathTime += fTimeDelta;
		m_fTime += fTimeDelta;
		CGameObject::Update_Object(fTimeDelta);
		_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];


		const D3DXFRAME_DERIVED*	pFrame = m_pMeshCom->Get_FrameByName("SkirtFBone02");
		m_pBoneMatrix = &pFrame->CombinedTransformMatrix;


		_matrix matScale, matRev, matTrans, matRot, m_matCollWorld;
		_vec3 worldbone = { m_pBoneMatrix->_41,m_pBoneMatrix->_42,m_pBoneMatrix->_43 };
		D3DXVec3TransformCoord(&worldbone, &worldbone, m_pTransformCom->Get_WorldMatrix());



		D3DXMatrixScaling(&matScale, 0.01, 0.01, 0.01);
		D3DXMatrixRotationY(&matRot, m_pTransformCom->m_vAngle.y);

		D3DXMatrixTranslation(&matTrans, worldbone.x, worldbone.y, worldbone.z);
		//D3DXMatrixRotationAxis(&matRev, &m_pTransformCom->m_vInfo[INFO::INFO_UP], m_pTransformCom->m_vAngle.y);

		m_matCollWorld = matScale * matRot*matTrans;//*matRev;
		m_pColliderCom->Set_Matrix(m_matCollWorld);

		CComponent* m_PlayerTransform = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Component(L"Com_Transform", ID_DYNAMIC);
		const _vec3 vPlayerPos = dynamic_cast<CTransform*>(m_PlayerTransform)->m_vInfo[INFO::INFO_POS];

		_vec3 vPlayerPos0 = { vPlayerPos.x + 1,vPlayerPos.y ,vPlayerPos.z + 1 };
		_vec3 vPlayerPos1 = { vPlayerPos.x - 1,vPlayerPos.y ,vPlayerPos.z - 1 };


		_vec3	vDir = vPlayerPos - m_pTransformCom->m_vInfo[INFO_POS];
		_vec3 vDir0= vPlayerPos0 - m_pTransformCom->m_vInfo[INFO_POS];
		_vec3 vDir1 = vPlayerPos1 - m_pTransformCom->m_vInfo[INFO_POS];

		float fDist = D3DXVec3Length(&vDir); //1.12 이내


		if (m_iID == 0 || m_iID == 1) //전투후 hp 0일때 예외처리 필요!
		{
			if (dynamic_cast<CDialog*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"UI", OBJECT_Dialog))
				->Get_QuestState(4) == Quest_ON  && (m_Action_State != Current_State::CURRENT_DEATH))
			{
				
				//플레이어에게 공격태세! 공격시작
				if (m_iStart == 0)
				{

				m_Action_State = Current_State::CURRENT_ATTACK;
				m_iStart++;
				}



			}


		}













		if (dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))
			->Get_ActionStage() >=20
			) //드래곤이 여관아래 내리면
		{
			
			//m_Action_State = Current_State::CURRENT_ATTACK;

			_vec3 TargetPos;
			if (m_iID == 2)
			{

				TargetPos = { (dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_RwingCollider()->Get_CollWorldMatrix())->_41,
				(dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_RwingCollider()->Get_CollWorldMatrix())->_42,
					(dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_RwingCollider()->Get_CollWorldMatrix())->_43};
			
			}
			else if (m_iID == 3)
			{

				TargetPos = { (dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_HeadCollider()->Get_CollWorldMatrix())->_41,
				(dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_HeadCollider()->Get_CollWorldMatrix())->_42,
					(dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))->Get_HeadCollider()->Get_CollWorldMatrix())->_43 };

			}
			if (TargetPos.y <= 9)
			 {




				 vDir = TargetPos - m_pTransformCom->m_vInfo[INFO_POS];
				 fDist = D3DXVec3Length(&vDir); //1.12 이내


				 if (fDist < 1.2)
				 {



					 _matrix		matRot = *m_pTransformCom->Compute_LookAtTarget(&TargetPos);   //*Compute_LookAtTarget(pTargetPos);

					 _matrix		matScale, matTrans, matRot1, m_matWorld;

					 D3DXMatrixRotationY(&matRot1, acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
						 D3DXVec3Normalize(&m_pTransformCom->m_vInfo[INFO_UP], &m_pTransformCom->m_vInfo[INFO_UP]))));


					 D3DXMatrixScaling(&matScale, m_pTransformCom->m_vScale.x, m_pTransformCom->m_vScale.y, m_pTransformCom->m_vScale.z);
					 D3DXMatrixTranslation(&matTrans, m_pTransformCom->m_vInfo[INFO_POS].x,
						 m_pTransformCom->m_vInfo[INFO_POS].y,

						 m_pTransformCom->m_vInfo[INFO_POS].z);

					 m_matWorld = matScale * matRot* matTrans;//matTrans;// matRot * matTrans;


					 m_pTransformCom->Set_WorldMatrix(&m_matWorld);






					 //m_pTransformCom->Chase_Target_Navi(&vPlayerPos, vPos); //현재 위치에 방향 회전만 계산


					 //공격 애니메이션 전환, 좌-우 공격 애니메이션 끝날때마다 프레임시간 턴 둔 후 애니메이션 전환
					  //우단 긋기
					 if (m_iAttackCount == 0) //좌 우 공격 애니메이션 교체 위해서
					 {
						 m_pMeshCom->Set_AnimationIndex(8);//우단긋기

					 }

					 if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 0) //처음 8번공격 끝나면 9번공격 교체
					 {
						 m_pMeshCom->Set_AnimationIndex(9); //좌단긋기
						 m_iAttackCount++;

					 }
					 else if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 1) //두번재 9번공격이 끝나면 다시 8번 교체
					 {
						 m_pMeshCom->Set_AnimationIndex(8); //좌단긋기
						 m_iAttackCount = 0;

					 }


					 //if (m_fTime >= 1.f)
					 //{
					 //	m_pMeshCom->Set_AnimationIndex(9);
					 //
					 //
					 //
					 //}


				 }
				 else
				 {

					 //플레이어와 공격가능한 거리가 아닐 경우 플레이어방향으로 추적해야함
					 m_pMeshCom->Set_AnimationIndex(5);//무기 든 채로 달리기 모션
					 D3DXVec3Normalize(&vDir, &vDir);	

					 //_vec3 vNextPos = { m_pTransformCom->Move_Pos(&vPos, &(vDir *fTimeDelta * 1.5f)).x,
					//	 SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 1.5f)).z };

					 _vec3 vnextPos = { vPos + vDir * fTimeDelta * 1.5f };

					 //m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).x,
					 //	SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).z);


					 m_pTransformCom->Chase_Target_Navi(&TargetPos, vnextPos);

				 }





			 }



			 

		}








		if (m_Action_State == CURRENT_DEATH)
		{

			m_pMeshCom->Set_AnimationIndex(11);

			if (m_pMeshCom->Is_AnimationsetFinish())
			{
				m_pMeshCom->Set_AnimationIndex(12);

				//_vec3 vDir = m_pTransformCom->m_vInfo[INFO::INFO_LOOK];
				//D3DXVec3Normalize(&vDir, &vDir);

				//m_pTransformCom->Move_Pos(&vDir, -5.f, fTimeDelta);

				if (m_fDeathTime >= 3.f)
				{



					m_fDeathTime = 0;
					m_bRenderOn = false;
					//m_iHP = 0; //죽는 모션 보여준후 죽음 처리!
				}

			}

		}










		//공격상태로 전환, 애니메이션 Equip 세팅 후 끝나면 무기 든 상태 유지하면서 플레이어 추격
		//플레이어 객체를 추적, 플레이어 객체 충돌체구 와 몸체끼리 충돌시 공격 애니메이션 전환->
		if (m_Action_State == Current_State::CURRENT_ATTACK  &&  dynamic_cast<CDragon*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_DRAGON))
			->Get_ActionStage() < 20)
		{



			if (fDist <= 1.2) //플레이어가 공격 사정거리내 있는지 확인
			{
				m_Action_State = CURRENT_ATTACK;



				_matrix		matRot = *m_pTransformCom->Compute_LookAtTarget(&vPlayerPos);   //*Compute_LookAtTarget(pTargetPos);

				_matrix		matScale, matTrans, matRot1, m_matWorld;

				D3DXMatrixRotationY(&matRot1, acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir),
					D3DXVec3Normalize(&m_pTransformCom->m_vInfo[INFO_UP], &m_pTransformCom->m_vInfo[INFO_UP]))));


				D3DXMatrixScaling(&matScale, m_pTransformCom->m_vScale.x, m_pTransformCom->m_vScale.y, m_pTransformCom->m_vScale.z);
				D3DXMatrixTranslation(&matTrans, m_pTransformCom->m_vInfo[INFO_POS].x,
					m_pTransformCom->m_vInfo[INFO_POS].y,

					m_pTransformCom->m_vInfo[INFO_POS].z);

				m_matWorld = matScale * matRot* matTrans;//matTrans;// matRot * matTrans;


				m_pTransformCom->Set_WorldMatrix(&m_matWorld);






				//m_pTransformCom->Chase_Target_Navi(&vPlayerPos, vPos); //현재 위치에 방향 회전만 계산


				//공격 애니메이션 전환, 좌-우 공격 애니메이션 끝날때마다 프레임시간 턴 둔 후 애니메이션 전환
				 //우단 긋기
				if (m_iAttackCount == 0) //좌 우 공격 애니메이션 교체 위해서
				{
					m_pMeshCom->Set_AnimationIndex(8);//우단긋기

				}

				if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 0) //처음 8번공격 끝나면 9번공격 교체
				{
					m_pMeshCom->Set_AnimationIndex(9); //좌단긋기
					m_iAttackCount++;

				}
				else if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 1) //두번재 9번공격이 끝나면 다시 8번 교체
				{
					m_pMeshCom->Set_AnimationIndex(8); //좌단긋기
					m_iAttackCount = 0;

				}


				//if (m_fTime >= 1.f)
				//{
				//	m_pMeshCom->Set_AnimationIndex(9);
				//
				//
				//
				//}


			}
			else
			{


				//플레이어와 공격가능한 거리가 아닐 경우 플레이어방향으로 추적해야함
				m_pMeshCom->Set_AnimationIndex(5);//무기 든 채로 달리기 모션
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vDir0, &vDir0);
				D3DXVec3Normalize(&vDir1, &vDir1);

				if (m_iID == 0)
					vDir = vDir0;
				if (m_iID == 1)
					vDir = vDir1;


				_vec3 vNextPos = { m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 1.5f)).x,
					SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 1.5f)).z };



				//m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).x,
				//	SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).z);


				m_pTransformCom->Chase_Target_Navi(&vPlayerPos, vNextPos);

			}





		}

		//if (m_Action_State == Current_State::CURRENT_ATTACK)
		//{
		//	if(fDist > 1.2)
		//		m_Action_State = Current_State::CURRENT_ATTACK;
		//}


		//else if (m_Action_State == Current_State::CURRENT_ATTACK && (fDist <= 1.2))
		//{
		//
		//	m_pTransformCom->Chase_Target_Navi(&vPlayerPos, vPos); //현재 위치에 방향 회전만 계산
		//
		//
		//		//공격 애니메이션 전환, 좌-우 공격 애니메이션 끝날때마다 프레임시간 턴 둔 후 애니메이션 전환
		//		 //우단 긋기
		//	if (m_iAttackCount == 0) //좌 우 공격 애니메이션 교체 위해서
		//	{
		//		m_pMeshCom->Set_AnimationIndex(8);//우단긋기
		//
		//	}
		//
		//	if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 0) //처음 8번공격 끝나면 9번공격 교체
		//	{
		//		m_pMeshCom->Set_AnimationIndex(9); //좌단긋기
		//		m_iAttackCount++;
		//
		//	}
		//	else if (true == m_pMeshCom->Is_AnimationsetFinish() && m_iAttackCount == 1) //두번재 9번공격이 끝나면 다시 8번 교체
		//	{
		//		m_pMeshCom->Set_AnimationIndex(8); //좌단긋기
		//		m_iAttackCount = 0;
		//
		//	}
		//
		//
		//}

		if (m_Action_State == CURRENT_ATTACKED)
		{
			m_pMeshCom->Set_AnimationIndex(14);
			m_pTransformCom->Move_Pos(&vDir, -0.15f, fTimeDelta);
		}

		if (m_Action_State == CURRENT_POWERATTACKED)
		{

			m_pMeshCom->Set_AnimationIndex(15);
			m_pTransformCom->Move_Pos(&vDir, -0.3f, fTimeDelta);

		}


		if (m_pMeshCom->Is_AnimationsetFinish() &&(m_Action_State == CURRENT_ATTACKED || m_Action_State == CURRENT_POWERATTACKED))
		{
			m_Action_State = CURRENT_ATTACK;
		}



		if (m_pMeshCom->Is_AnimationsetFinish() && fDist > 1.2  && m_Action_State == CURRENT_ATTACK)
		{
			m_Action_State = Current_State::CURRENT_ATTACK;
			m_pMeshCom->Set_AnimationIndex(5);


		}
		else //경비병 공격상태 해제하려면 플레이어가 무기를 다시 집어넣으면 이를 체크해 공격상태 해제!
		{

		}


		//Key_Input(fTimeDelta);

		

		Add_RenderGroup(RENDER_NONALPHA, this);
	}
	return 0;

}

void CGuardMan::Render_Object(void)
{
	if (m_bRenderOn)
	{
		m_pMeshCom->Play_Animation(m_fAniTime);
		m_fAniTime = 0.f;


	//m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	//m_pNaviCom->Render_NaviMesh();


	m_pMeshCom->Render_Meshes();

	}

}

_bool CGuardMan::Collision_ToObject(const _tchar * pLayerTag, const OBJECT_ID pObjTag)
{
	CColliderSphere*		pPlayerColliderCom = dynamic_cast<CColliderSphere*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);

	/*return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(), pPlayerColliderCom->Get_Max(), pPlayerColliderCom->Get_CollWorldMatrix(),
											m_pColliderCom->Get_Min(), m_pColliderCom->Get_Max(), m_pColliderCom->Get_CollWorldMatrix());*/

	_vec3 vPos = m_pTransformCom->m_vInfo[INFO::INFO_POS];
	vPos.y += 1;
	return m_pCalculatorCom->Collision_Sphere(pPlayerColliderCom->Get_Radius(), pPlayerColliderCom->Get_Center(),
		pPlayerColliderCom->Get_CollWorldMatrix(), 0.01, m_pColliderCom->Get_Radius(), &vPos, m_pColliderCom->Get_CollWorldMatrix(), 0.01);

}

HRESULT CGuardMan::Add_Component(wstring Mesh)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(Mesh.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", pComponent);


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
	return S_OK;
	
}

void CGuardMan::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);



	if (true == m_pMeshCom->Is_AnimationsetFinish())
	{
		m_pMeshCom->Set_AnimationIndex(5);

		//m_pMeshCom->Set_AnimationIndex(38);
	}
}

_float CGuardMan::SetUp_OnTerrain(void)
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

Engine::_vec3 CGuardMan::PickUp_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

CGuardMan* CGuardMan::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh)
{
	CGuardMan*	pInstance = new CGuardMan(pGraphicDev, Mesh);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGuardMan::Free(void)
{
	CGameObject::Free();
}

