#include "stdafx.h"
#include "MarketNPC.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

CMarketNPC::CMarketNPC(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CMarketNPC::CMarketNPC(const CMarketNPC& rhs)
	: CGameObject(rhs)
{

}

CMarketNPC::~CMarketNPC(void)
{

}

HRESULT CMarketNPC::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	m_pTransformCom->Set_Pos(57.42, 7, 39.10);
	
	m_pNaviCom->Set_CellIndex(46);
	m_pMeshCom->Set_AnimationIndex(0);
	m_pColliderCom->Set_Radius(75);
	Set_HP(1);

	//m_pMeshCom->Set_AnimationIndex(13);

	return S_OK;
}

Engine::_int CMarketNPC::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);
	_vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
	CComponent* m_PlayerTransform = dynamic_cast<CPlayer*>(m_pPlayer)->Get_Component(L"Com_Transform", ID_DYNAMIC);
	const _vec3 vPlayerPos = dynamic_cast<CTransform*>(m_PlayerTransform)->m_vInfo[INFO::INFO_POS];

	m_pTransformCom->Chase_Target_Navi(&vPlayerPos, vPos); //현재 위치에 방향 회전만 계산


	
	
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



	if (true == m_pMeshCom->Is_AnimationsetFinish())
	{
		m_pMeshCom->Set_AnimationIndex(0);

	}

	m_pMeshCom->Play_Animation(fTimeDelta);

	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CMarketNPC::Render_Object(void)
{
	m_pColliderCom->Render_ColliderSphere(COLLTYPE(false), m_pColliderCom->Get_CollWorldMatrix());

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pNaviCom->Render_NaviMesh();


	m_pMeshCom->Render_Meshes();


}

HRESULT CMarketNPC::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<CDynamicMesh*>(Clone_Proto(L"Proto_Mesh_MarketFemale"));
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

void CMarketNPC::Key_Input(const _float& fTimeDelta)
{
	m_pTransformCom->Get_Info(INFO_LOOK, &m_vDir);





	if (Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir *fTimeDelta * 5.f)).z);


		m_pMeshCom->Set_AnimationIndex(54);
	}

	if (Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);
		


		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir *fTimeDelta * 5.f)).z);

		m_pMeshCom->Set_AnimationIndex(52);

		
		//m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
	}


	if (Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		_vec3 fY = { 0,1,0 };

		_vec3 vNormal;  D3DXVec3Cross(&vNormal, &vDir, &fY);



		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vNormal *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vNormal *fTimeDelta * 5.f)).z);



		//m_pTransformCom->Move_Pos(&vNormal, -10.f, fTimeDelta);
	
		m_pMeshCom->Set_AnimationIndex(54);

		//m_pTransformCom->m_vInfo[INFO::INFO_POS]
	
			//m_vInfo[INFO_POS] += *pDir * fSpeed *fTimeDelta;
	
	
	}
	if (Get_DIKeyState(DIK_A) & 0x80)
	{
	
		_vec3	vPos, vDir;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
		D3DXVec3Normalize(&vDir, &vDir);


		D3DXVec3Normalize(&m_vDir, &vDir);


		_vec3 fY = { 0,1,0 };

		_vec3 vNormal;  D3DXVec3Cross(&vNormal, &m_vDir, &fY);





		m_pTransformCom->Set_Pos(m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).x,
			SetUp_OnTerrain(), m_pNaviCom->Move_OnNaviMesh(&vPos, &(vNormal *fTimeDelta * 5.f)).z);


		//m_pTransformCom->Move_Pos(&vNormal, +10.f, fTimeDelta);
	
		m_pMeshCom->Set_AnimationIndex(54);

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
	//	m_pMeshCom->Set_AnimationIndex(54);
	//
	//	m_pMeshCom->Set_AnimationIndex(6);
	//}
	

	//if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	//{
	//	D3DXVec3Normalize(&m_vDir, &m_vDir);
	//	m_pTransformCom->Move_Pos(&m_vDir, -10.f, fTimeDelta);
	//	//m_pMeshCom->Set_AnimationIndex(7);
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

	if (Engine::Get_DIMouseState(DIM_RB) & 0X80)
	{
		m_pMeshCom->Set_AnimationIndex(30);

//		m_pMeshCom->Set_AnimationIndex(8);
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
		
	
	
	
		//m_pTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_MarketNPC, L"Com_Transform", ID_DYNAMIC));
		//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(dwMouseMove / 10.f) * fTimeDelta);
		////m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(180) * -fTimeDelta);
	
	
	
	












	if (true == m_pMeshCom->Is_AnimationsetFinish())
	{
		m_pMeshCom->Set_AnimationIndex(57);

		//m_pMeshCom->Set_AnimationIndex(38);
	}
}

_float CMarketNPC::SetUp_OnTerrain(void)
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

Engine::_vec3 CMarketNPC::PickUp_OnTerrain(void)
{
	CTerrainTex*		pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Buffer", ID_STATIC));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform*		pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"GameLogic", OBJECT_TERRAIN, L"Com_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());


	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

CMarketNPC* CMarketNPC::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMarketNPC*	pInstance = new CMarketNPC(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMarketNPC::Free(void)
{
	CGameObject::Free();
}

