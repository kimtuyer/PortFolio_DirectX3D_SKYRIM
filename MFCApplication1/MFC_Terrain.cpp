#include "pch.h"
#include "MFC_Terrain.h"
#include "Export_Function.h"
#include "MFC_TerrainTex.h"
MFC_Terrain::MFC_Terrain(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

MFC_Terrain::MFC_Terrain(const MFC_Terrain& rhs)
	: CGameObject(rhs)
{

}

MFC_Terrain::~MFC_Terrain(void)
{

}

HRESULT MFC_Terrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);
	//m_pIndex = new INDEX32[m_pBufferCom->Get_TriCnt()];
	Set_HP(1);
	return S_OK;
}

Engine::_int MFC_Terrain::Update_Object(const _float& fTimeDelta)
{
		CGameObject::Update_Object(fTimeDelta);

	//m_pOptimizationCom->Isin_FrustumForTerrain(m_pBufferCom->Get_VtxPos(),
	//	m_pBufferCom->Get_VtxCntX(),
	//	m_pBufferCom->Get_VtxCntZ(),
	//	m_pIndex,
	//	&m_dwTriCnt);


	//Engine::Add_RenderGroup(RENDER_PRIORITY, this);
	Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void MFC_Terrain::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pTextureCom->Render_Texture(m_iIndex);

	//FAILED_CHECK_RETURN(SetUp_Material(), );

	//m_pBufferCom->Copy_Index(m_pIndex, m_dwTriCnt);

	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT MFC_Terrain::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	

		// buffer
	//pComponent = m_pBufferCom2 = dynamic_cast<CTerrainTex*>(Clone_Proto(L"Proto_Buffer_CTerrainTex"));
	//NULL_CHECK_RETURN(pComponent, E_FAIL);

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<MFC_TerrainTex*>(Clone_Proto(L"Proto_Buffer_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);
	
	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Texture", pComponent);

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Transform", pComponent);

	// renderer
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_mapComponent[ID_STATIC].emplace(L"Com_Renderer", pComponent);

	//// Optimization
	pComponent = m_pOptimizationCom = dynamic_cast<COptimization*>(Clone_Proto(L"Proto_Optimization"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Optimization", pComponent);
	
	return S_OK;
}

HRESULT MFC_Terrain::SetUp_Material(void)
{
	D3DMATERIAL9		tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrl.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

MFC_Terrain* MFC_Terrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	MFC_Terrain*	pInstance = new MFC_Terrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void MFC_Terrain::Free(void)
{
	CGameObject::Free();
}