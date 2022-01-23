#include "stdafx.h"
#include "SkyBox.h"

#include "Export_Function.h"

CSkyBox::CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CSkyBox::CSkyBox(const CSkyBox& rhs)
	: CGameObject(rhs)
{

}

CSkyBox::~CSkyBox(void)
{

}

HRESULT CSkyBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	FAILED_CHECK_RETURN(CGameObject::Ready_Object(), E_FAIL);

	m_pTransformCom->Set_Scale(40.f, 40.f, 40.f);
	Set_HP(1);

	return S_OK;
}

Engine::_int CSkyBox::Update_Object(const _float& fTimeDelta)
{
	m_fTime += fTimeDelta;
	CGameObject::Update_Object(fTimeDelta);

	if (m_fTime >= 5.f)
	{
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_After"));
	}
	if (m_fTime >= 10.f)
	{
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_Night"));

	}
	if (m_fTime >= 15.f){		
		m_fTime = 0.f;
		m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_Sun"));

	}

	_matrix		matCamWorld;
	// �佺���̽� ��ȯ ����� ����
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	//  �佺���̽� ��ȯ ���(ī�޶���� ����� �����)�� �ٽ� ����ķ� ���� ī�޶� ���� ��Ļ��·� ��ȯ
	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);

	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);
	
	Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CSkyBox::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

HRESULT CSkyBox::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	// buffer
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Clone_Proto(L"Proto_Buffer_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_Sun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_After"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture2", pComponent);

	pComponent = m_pTextureCom3 = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_SkyBox_Night"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture3", pComponent);


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

CSkyBox* CSkyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSkyBox*	pInstance = new CSkyBox(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkyBox::Free(void)
{
	CGameObject::Free();
}

