#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CComponent(rhs)
	, m_pRootFrame(rhs.m_pRootFrame)
	, m_pLoader(rhs.m_pLoader)
	, m_MeshcontainerList(rhs.m_MeshcontainerList)
{
	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

void CDynamicMesh::Set_AnimationIndex(const _uint & iIndex)
{
	m_pAniCtrl->Set_AnimationIndex(iIndex);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(360.f)));
}

Engine::_bool Engine::CDynamicMesh::Is_AnimationsetFinish(void)
{
	return m_pAniCtrl->Is_AnimationsetFinish();
}

const	D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(char* pFrameName)
{	
	return(D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

HRESULT Engine::CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader,	// HierarchyLoader Ŭ���� ������
		NULL, 
		&m_pRootFrame, 
		&pAniCtrl)))	// aniCtrl ������		
		return E_FAIL;

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniCtrl, E_FAIL);
	
	Safe_Release(pAniCtrl);
	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixRotationY(&matTemp, D3DXToRadian(90.f)));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Meshes(void)
{
	
	for (auto& iter : m_MeshcontainerList)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = iter;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			pDerivedMeshContainer->pRenderingMatrix[i] = pDerivedMeshContainer->pFrameOffsetMatrix[i] * *pDerivedMeshContainer->ppFrameCombinedTransformMatrix[i];
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pDerivedMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pDerivedMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		// ����Ʈ���� ��Ű���� �����ϴ� �Լ�(��Ű�� �ִϸ��̼� �� �ƴ϶� ����� �������� ���� ���浵 �������ִ� �Լ�)
		// 1. ���� ���� ��ȯ ���� 2. ������� ������ ���� �����(�ȳ־��൵ �������) 3. ������ �ʴ� ������ ���� ���� 4. �ִϸ��̼ǿ� ���� ��ȯ�� ���� ����
		pDerivedMeshContainer->pSkinInfo->UpdateSkinnedMesh(pDerivedMeshContainer->pRenderingMatrix, NULL, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pDerivedMeshContainer->ppTexture[i]);
			pDerivedMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		
		pDerivedMeshContainer->pOriMesh->UnlockVertexBuffer();
		pDerivedMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}

}

// ��� �Լ�
void Engine::CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;
	
	// �θ�� �ڽ��� ����� ����� ���� = ���� ���� ���� ��� * �θ��� ���� ���
	// �ִϸ��̼ǰ��� ���� ����� ���� �θ�� �ڽ��� ���� ���� ���¸� �����ϱ� ���� ���� ����� ������ ���� �뵵
	// �������� �׸��� ��� = pFrame->CombinedTransformMatrix  * �ִϸ��̼� ���
	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * *pParentMatrix;

	// ���� ���� ������ �ִٸ�
	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	// �ڽ� ���� ������ �ִٸ�
	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);
}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*	pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
		
	}
	m_MeshcontainerList.clear();

	Safe_Release(m_pAniCtrl);

	CComponent::Free();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			// ���� �̸��� ������ �Լ�
			const char*		pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			// ��Ʈ������ ��ȸ�ϸ鼭 �̸��� �ش��ϴ� �� ������ ã���ִ� �Լ�
			D3DXFRAME_DERIVED*	pDerivedFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pDerivedMeshContainer->ppFrameCombinedTransformMatrix[i] = &pDerivedFrame->CombinedTransformMatrix;
		}

		m_MeshcontainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

