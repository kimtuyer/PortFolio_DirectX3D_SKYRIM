#include "stdafx.h"

#include "CItemObject.h"

//#include "Calculator.h"
//#include "TerrainTex.h"
#include "Export_Function.h"

CItemObject::CItemObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto) : CGameObject(pGraphicDev)
{
	Ready_CItemObject(Mesh_proto);
}



CItemObject::CItemObject(const CItemObject& rhs)
	: CGameObject(rhs)
{

}

CItemObject::~CItemObject()
{
}

HRESULT CItemObject::Ready_CItemObject(wstring Mesh_proto)
{
	
	FAILED_CHECK_RETURN(Add_Component(Mesh_proto), E_FAIL);


	m_fX = 100.f;
	m_fY = 580.f;

	m_fSizeX = 800.f;
	m_fSizeY = 100.f;


	m_fScalex = 0.1;
	m_fScaley = 0.1;
	Set_HP(1);

	//m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	//m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);
	//m_pTransformCom->Rotation(ROT_Y, D3DXToRadian(60.f));

	return S_OK;


}

HRESULT CItemObject::Ready_Object(void)
{
	return S_OK;
}

_int CItemObject::Update_Object(const _float & fTimeDelta)
{

	CGameObject::Update_Object(fTimeDelta);

	//SetUp_OnTerrain();

	//m_bColl = Collision_ToObject(L"GameLogic", L"Player");

	

	//m_bDraw = m_pOptimizationCom->Isin_FrustumForObject(&vPos);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;

}

void CItemObject::Render_Object(void)
{
	

	if (m_bRenderOn)
	{


		//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


		_matrix		matWorld, matView, matOldView, matOldProj;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._11 = m_fSizeX * m_fScalex; //0.1
		matWorld._22 = m_fSizeY * m_fScaley; //0.1
		matWorld._33 = 1.f;
		matWorld._41 = m_fX - 800 * 0.5f;
		matWorld._42 = -m_fY + 600 * 0.5f;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);



		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0x00000077); //0x00000077 //0xc0




		m_pTextureCom->Render_Texture(m_iTextureIndex);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}
}
HRESULT CItemObject::Add_Component(wstring Mesh_proto)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(Mesh_proto.c_str()));
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


void CItemObject::Release_CItemObject()
{
}




void CItemObject::Free(void)
{
	CGameObject::Free();

}

CItemObject * CItemObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto)
{
	CItemObject*	pInstance = new CItemObject(pGraphicDev, Mesh_proto);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);


	
	return pInstance;

}
