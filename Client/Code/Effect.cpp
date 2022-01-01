#include "stdafx.h"
#include "Effect.h"

#include "Export_Function.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh)
	: CGameObject(pGraphicDev)
{
	(Add_Component(Mesh), E_FAIL);

}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
{

}

CEffect::~CEffect(void)
{

}

HRESULT CEffect::Ready_Object(void)
{
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_iHP = 1;

	//m_pTransformCom->Set_Pos(&_vec3(rand() % 20, 0.f, rand() % 20));
	m_pTransformCom->Set_Scale(1, 1, 1);
	return S_OK;
}

Engine::_int CEffect::Update_Object(const _float& fTimeDelta)
{
	if (m_bRenderOn && m_ID==0	)
	{



		m_fFrame += 4.f * fTimeDelta;

		if (m_fFrame > m_fFrameSize)
			m_fFrame = 0.f;


		CGameObject::Update_Object(fTimeDelta);


		_matrix		matWorld, matView, matBill;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, NULL, &matBill);

		// 현재 이 코드는 문제의 소지가 있다.
		// 월드 행렬 = 스 * 자 * 이 * 공 * 부
		// 월 = 빌 * 월(스(I)) -> 빌(자^-1) * 스 * 자 

		m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		Compute_ViewZ(&vPos);




		Add_RenderGroup(RENDER_ALPHA, this);
	}
	else if (m_bRenderOn && m_ID == 1)
	{
		if(dynamic_cast<CPlayer*>(CManagement::GetInstance()->Get_Scene()->Get_Object(L"GameLogic", OBJECT_PLAYER))
			->Get_Navi()->Get_CurrentIndex()==132)
		{
			m_bRender2 = true;

			
		}
		if (m_bRender2)
		{
			m_fFrame += 4.f * fTimeDelta;

			if (m_fFrame > m_fFrameSize)
				m_fFrame = 0.f;


			CGameObject::Update_Object(fTimeDelta);


			_matrix		matWorld, matView, matBill;

			m_pTransformCom->Get_WorldMatrix(&matWorld);
			m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

			D3DXMatrixIdentity(&matBill);

			matBill._11 = matView._11;
			matBill._13 = matView._13;
			matBill._31 = matView._31;
			matBill._33 = matView._33;

			D3DXMatrixInverse(&matBill, NULL, &matBill);

			// 현재 이 코드는 문제의 소지가 있다.
			// 월드 행렬 = 스 * 자 * 이 * 공 * 부
			// 월 = 빌 * 월(스(I)) -> 빌(자^-1) * 스 * 자 

			m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);

			Compute_ViewZ(&vPos);




			Add_RenderGroup(RENDER_ALPHA, this);
		}



	}
	else if (m_bRenderOn && m_ID == 2)//헬파이어
	{



		_vec3 vDir = { 0,1,0 };
		m_pTransformCom->Move_Pos(&vDir, -8.f, fTimeDelta);

		m_fFrame += 4.f * fTimeDelta;

		if (m_fFrame > m_fFrameSize)
			m_fFrame = 0.f;


		CGameObject::Update_Object(fTimeDelta);


		_matrix		matWorld, matView, matBill;

		m_pTransformCom->Get_WorldMatrix(&matWorld);
		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, NULL, &matBill);

		// 현재 이 코드는 문제의 소지가 있다.
		// 월드 행렬 = 스 * 자 * 이 * 공 * 부
		// 월 = 빌 * 월(스(I)) -> 빌(자^-1) * 스 * 자 

		m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		Compute_ViewZ(&vPos);




		Add_RenderGroup(RENDER_ALPHA, this);

		if (m_pTransformCom->m_vInfo[INFO_POS].y <= 7)
		{
			m_pTransformCom->m_vInfo[INFO_POS] = vOrgin;;
			//m_bRenderOn = false;
		}


	}


	return 0;
}

void CEffect::Render_Object(void)
{
	if (m_bRenderOn)
	{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(_ulong(m_fFrame));
	m_pBufferCom->Render_Buffer();
	}
}

HRESULT CEffect::Add_Component(wstring Mesh)
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

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh)
{
	CEffect*	pInstance = new CEffect(pGraphicDev, Mesh);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect::Free(void)
{
	CGameObject::Free();
}

