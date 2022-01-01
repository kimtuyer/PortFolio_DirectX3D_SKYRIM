#include "ColliderSphere.h"

USING(Engine)

Engine::CColliderSphere::CColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pVB(nullptr)
	, m_pIB(nullptr)	
{
	D3DXMatrixIdentity(&m_matCollWorld);
}

Engine::CColliderSphere::CColliderSphere(const CColliderSphere& rhs)
	: CComponent(rhs)	
	//, m_vMin(rhs.m_vMin)
	//, m_vMax(rhs.m_vMax)
	, m_matCollWorld(rhs.m_matCollWorld)
{
#ifdef _DEBUG
	m_pVB = rhs.m_pVB;
	m_pVB->AddRef();
	m_pIB = rhs.m_pIB;
	m_pIB->AddRef();

	for (_ulong i = 0; i < COL_END; ++i)
	{
		m_pTexture[i] = rhs.m_pTexture[i];
		m_pTexture[i]->AddRef();
	}
	
#endif
}

Engine::CColliderSphere::~CColliderSphere(void)
{

}

CColliderSphere* Engine::CColliderSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwVtxSize)
{
	CColliderSphere*	pInstance = new CColliderSphere(pGraphicDev);

	if (FAILED(pInstance->Ready_ColliderSphere(pPos, dwNumVtx, dwVtxSize)))
		Safe_Release(pInstance);

	return pInstance;
}

CColliderSphere * CColliderSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderSphere*	pInstance = new CColliderSphere(pGraphicDev);

	return pInstance;
}

CComponent* Engine::CColliderSphere::Clone(void)
{
	return new CColliderSphere(*this);
}

void Engine::CColliderSphere::Free(void)
{
#ifdef _DEBUG
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	if (false == m_bClone)
	{
		for (_ulong i = 0; i < COL_END; ++i)
			Safe_Release(m_pTexture[i]);
	}
#endif
	CComponent::Free();

}

HRESULT Engine::CColliderSphere::Ready_ColliderSphere(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwVtxSize)
{
	//BYTE* pV;


	D3DXComputeBoundingSphere(pPos, dwNumVtx, sizeof(_vec3), &m_Sphere.vCenter, &m_Sphere.fRadius);

	//D3DXComputeBoundingBox(pPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax);

#ifdef _DEBUG
	if (FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXCUBE) * 8,	
													0,					
													FVF_CUBE,			
													D3DPOOL_MANAGED,	
													&m_pVB,
													NULL)))				
													return E_FAIL;

	if (FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 12,	
													0,					
													D3DFMT_INDEX16,			
													D3DPOOL_MANAGED,	
													&m_pIB,
													NULL)))				
													return E_FAIL;


	VTXCUBE*			pVertex = nullptr;

	//m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// �� 
	//pVertex[0].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	//pVertex[0].vTexUV = pVertex[0].vPos;
	//
	//pVertex[1].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	//pVertex[1].vTexUV = pVertex[1].vPos;
	//
	//pVertex[2].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	//pVertex[2].vTexUV = pVertex[2].vPos;
	//
	//pVertex[3].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	//pVertex[3].vTexUV = pVertex[3].vPos;
	//
	//// ��
	//pVertex[4].vPos = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	//pVertex[4].vTexUV = pVertex[4].vPos;
	//
	//pVertex[5].vPos = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	//pVertex[5].vTexUV = pVertex[5].vPos;
	//
	//pVertex[6].vPos = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	//pVertex[6].vTexUV = pVertex[6].vPos;
	//
	//pVertex[7].vPos = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	//pVertex[7].vTexUV = pVertex[7].vPos;
	
	//m_pVB->Unlock();

	INDEX16*		pIndex = NULL;

	//m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	//pIndex[0]._0 = 1;
	//pIndex[0]._1 = 5;
	//pIndex[0]._2 = 6;
	//
	//pIndex[1]._0 = 1;
	//pIndex[1]._1 = 6;
	//pIndex[1]._2 = 2;
	//
	//// X-
	//pIndex[2]._0 = 4;
	//pIndex[2]._1 = 0;
	//pIndex[2]._2 = 3;
	//
	//pIndex[3]._0 = 4;
	//pIndex[3]._1 = 3;
	//pIndex[3]._2 = 7;
	//
	//// Y+
	//pIndex[4]._0 = 4;
	//pIndex[4]._1 = 5;
	//pIndex[4]._2 = 1;
	//
	//pIndex[5]._0 = 4;
	//pIndex[5]._1 = 1;
	//pIndex[5]._2 = 0;
	//
	//// Y-
	//pIndex[6]._0 = 3;
	//pIndex[6]._1 = 2;
	//pIndex[6]._2 = 6;
	//
	//pIndex[7]._0 = 3;
	//pIndex[7]._1 = 6;
	//pIndex[7]._2 = 7;
	//
	//// Z+
	//pIndex[8]._0 = 7;
	//pIndex[8]._1 = 6;
	//pIndex[8]._2 = 5;
	//
	//pIndex[9]._0 = 7;
	//pIndex[9]._1 = 5;
	//pIndex[9]._2 = 4;
	//
	//// Z-
	//pIndex[10]._0 = 0;
	//pIndex[10]._1 = 1;
	//pIndex[10]._2 = 2;
	//
	//pIndex[11]._0 = 0;
	//pIndex[11]._1 = 2;
	//pIndex[11]._2 = 3;
	//
	//m_pIB->Unlock();


	//for (_ulong i = 0; i < COL_END; ++i)
	//{
	//	// 4��° ���ڰ��� 0�� ��� �Ϲ����� �ؽ�ó�� �����ϰڴٴ� �ɼ�
	//	// ���� 4��° ���ڰ��� D3DUSAGE_RENDERTARGET�̶� �ɼ��� ���� ��� ȭ�� ��¿��� �ؽ��ĸ� �����ϰڴٴ� �ɼ�
	//	m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);
	//
	//	D3DLOCKED_RECT		LockRect;
	//	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));
	//
	//	m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);
	//
	//	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1.f - i), 0.f, 1.f);
	//
	//	m_pTexture[i]->UnlockRect(0);
	//}

#endif
	return S_OK;
}

void Engine::CColliderSphere::Render_ColliderSphere(COLLTYPE eType, const _matrix* pColliderSphereMatrix)
{
	m_matCollWorld = *pColliderSphereMatrix;
	//m_matCollWorld._42 += 2;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matCollWorld);


	if (FAILED(D3DXCreateSphere(m_pGraphicDev, m_Sphere.fRadius, 10, 10, &m_pSphere, NULL)))
		return;//E_FAIL;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderSphereMatrix);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);

	//	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	//	m_pGraphicDev->SetFVF(FVF_CUBE);
		//m_pGraphicDev->SetIndices(m_pIB);

	m_pSphere->DrawSubset(0);
	//m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		//return E_FAIL;
//#ifdef _DEBUG

	
		
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderSphereMatrix);
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//
//	//m_pGraphicDev->SetTexture(0, m_pTexture[eType]);
	//
//	//m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
//	//m_pGraphicDev->SetFVF(FVF_CUBE);
	////m_pGraphicDev->SetIndices(m_pIB);
	//
	//m_pSphere->DrawSubset(0);
	////m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	//
	//m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
//#endif
}

