#include "stdafx.h"
#include "Font_texture.h"

#include "Export_Function.h"

CFont_Texture::CFont_Texture(LPDIRECT3DDEVICE9 pGraphicDev , wstring Mesh_proto)
	: CGameObject(pGraphicDev)
{
	Ready_CFont_Texture_Object(Mesh_proto);
}

CFont_Texture::CFont_Texture(const CFont_Texture& rhs)
	: CGameObject(rhs)
{

}

CFont_Texture::~CFont_Texture(void)
{

}

HRESULT CFont_Texture::Ready_CFont_Texture_Object(wstring Mesh_proto)
{
	FAILED_CHECK_RETURN(Add_Component(Mesh_proto), E_FAIL);
	m_fX = 100.f;
	m_fY = 580.f;

	m_fSizeX = 800.f;
	m_fSizeY = 100.f;

	m_iHP = 1;

	m_fScalex = 0.1;
	m_fScaley = 0.1;
	return S_OK;
}

HRESULT CFont_Texture::Ready_Object(void)
{
	//FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_fX = 100.f;
	//m_fY = 580.f;
	//
	//m_fSizeX = 800.f;
	//m_fSizeY = 100.f;
	//
	//m_iHP = 1;
	//
	return S_OK;
}

Engine::_int CFont_Texture::Update_Object(const _float& fTimeDelta)
{
	CGameObject::Update_Object(fTimeDelta);

	//Add_RenderGroup(RENDER_Font_Texture, this);
	Add_RenderGroup(RENDER_NONALPHA, this);

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	return 0;
}

void CFont_Texture::Render_Object(void)
{
	if (m_bRenderOn)
	{

		_matrix		matWorld, matView, matOldView, matOldProj;

		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixIdentity(&matView);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
		m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

		matWorld._11 = m_fSizeX * m_fScalex; //0.1
		matWorld._22 = m_fSizeY * m_fScaley; //0.1
		matWorld._33 = 1.f;
		matWorld._41 = m_fX - WINCX * 0.5f;
		matWorld._42 = -m_fY + WINCY * 0.5f;

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

HRESULT CFont_Texture::Add_Component(wstring Mesh_proto)
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

CFont_Texture* CFont_Texture::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto)
{
	CFont_Texture*	pInstance = new CFont_Texture(pGraphicDev,  Mesh_proto);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFont_Texture::Free(void)
{
	CGameObject::Free();
}

