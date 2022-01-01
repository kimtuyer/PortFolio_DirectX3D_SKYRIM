#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::CHierarchyLoader(const CHierarchyLoader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pPath(rhs.m_pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{

}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, 
												LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->CombinedTransformMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);
	
	*ppNewFrame = pDerivedFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData,
															CONST D3DXMATERIAL *pMaterials, 
															CONST D3DXEFFECTINSTANCE *pEffectInstances,
															DWORD NumMaterials,
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo, 
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*		pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH	pMesh = pMeshData->pMesh;
	
	// �޽��� ���� �������� ������ ��ȯ�ϴ� �Լ�
	_ulong	dwNumFaces = pMesh->GetNumFaces();

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);


	_ulong	dwFVF = pMesh->GetFVF();

	// �븻 ������ ���ԵǾ� ���� �ʴٸ�
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// fvf ������ �����ϴ� �Լ�(���� �븻 ������ ������������ �ʾҴ�)
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);

		// ������ �������� ������ ���� ���� ������ �����ϴ� �Լ�
		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	}

	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pDerivedMeshContainer->MeshData.pMesh);
	}

	// ���� ������
	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	// ���������� ������ ������ �ִ� ���
	if (0 != pDerivedMeshContainer->NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_uint i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar	szFileName[MAX_PATH] = L"";
			_tchar	szFullPath[MAX_PATH] = L"";

			// ��Ƽ����Ʈ ����� ���ڿ��� ���̵����Ʈ ����� ���ڿ��� ġȯ���ִ� �Լ�
			MultiByteToWideChar(CP_ACP, 0,
				pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename),
				szFileName,
				MAX_PATH);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
			{
				MSG_BOX("DynamicMesh Texture Create Failed");
				return E_FAIL;
			}
		}	
	}
	// ���� ������ ���� ��� ������ ���� ���� ���� �־� ��� ������� �����ϰ� �ϱ� ���� �ӽ� ����
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
	}

	// ���࿡ pSkinInfo�� ���ٸ� �� �޽��� ������� ���Ѵٰ� �����ϴ°� ����
	if (nullptr == pSkinInfo)
	{
		MSG_BOX("������");
		return S_OK;
	}

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();

	// ������ �ʴ� ���� ������ ���� �޽� �İ�ü�� ����������
	pDerivedMeshContainer->MeshData.pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), 
														pDerivedMeshContainer->MeshData.pMesh->GetFVF(), 
														m_pGraphicDev, 
														&pDerivedMeshContainer->pOriMesh);


	// �޽� �����̳ʿ� ������ ��ġ�� ���� ������ ������ �Լ�
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedTransformMatrix = new _matrix*[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedTransformMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
	{
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	}
	
	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if (nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);

	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pDerirvedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerirvedMeshContainer->NumMaterials; ++i)
		Safe_Release(pDerirvedMeshContainer->ppTexture[i]);
	
	Safe_Delete_Array(pDerirvedMeshContainer->ppTexture);

	Safe_Release(pDerirvedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerirvedMeshContainer->pOriMesh);
	Safe_Release(pDerirvedMeshContainer->pSkinInfo);
	
	Safe_Delete_Array(pDerirvedMeshContainer->Name);
	Safe_Delete_Array(pDerirvedMeshContainer->pAdjacency);
	Safe_Delete_Array(pDerirvedMeshContainer->pMaterials);

	Safe_Delete_Array(pDerirvedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerirvedMeshContainer->ppFrameCombinedTransformMatrix);
	Safe_Delete_Array(pDerirvedMeshContainer->pRenderingMatrix);

	Safe_Delete(pDerirvedMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	return new CHierarchyLoader(pGraphicDev, pPath);
}

Engine::_ulong Engine::CHierarchyLoader::Release(void)
{
	Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

