#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CComponent(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pMtrl(rhs.m_pMtrl)
	, m_pSubset(rhs.m_pSubset)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_dwVtxSize(rhs.m_dwVtxSize)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwNumVtx(rhs.m_dwNumVtx)
{
	m_ppTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTexture[i] = rhs.m_ppTexture[i];
		m_ppTexture[i]->AddRef();
	}

	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pOriMesh->AddRef();
	m_pMesh->AddRef();

}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[MAX_PATH] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath, 
								D3DXMESH_MANAGED, 
								m_pGraphicDev,
								&m_pAdjacency,
								&m_pSubset,
								NULL, 
								&m_dwSubsetCnt, 
								&m_pOriMesh)))
								return E_FAIL;

	// �޽��� ���� ���� ���������� ������ �Լ�
	_ulong	dwFVF = m_pOriMesh->GetFVF();

	// �븻 ������ ���ԵǾ� ���� �ʴٸ�
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		// fvf ������ �����ϴ� �Լ�(���� �븻 ������ ������������ �ʾҴ�)
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		// ������ �������� ������ ���� ���� ������ �����ϴ� �Լ�
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}

	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}
	
	void*		pVertex = NULL;


	// �޽��� ���� ���ؽ��� ������ ��ȯ
	m_dwNumVtx = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumVtx];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte		byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	// fvf ������ �ٰŷ� ������ ũ�⸦ ����ϴ� �Լ�
	m_dwVtxSize = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwVtxSize * i + byOffset)));
	}

	// ����� ���۰� ���� ���� ���� �ּҸ� �����ϴ� �Լ�
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	// �޽��� ���� �ؽ�ó �İ�ü�� ����
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar	szFileName[MAX_PATH] = L"";

		lstrcpy(szFullPath, pFilePath);

		// ��Ƽ����Ʈ ����� ���ڿ��� ���̵����Ʈ ����� ���ڿ��� ġȯ���ִ� �Լ�
		MultiByteToWideChar(CP_ACP, 0, 
							m_pMtrl[i].pTextureFilename, 
							strlen(m_pMtrl[i].pTextureFilename),
							szFileName, 
							MAX_PATH);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTexture[i])))
			return E_FAIL;
	}

	m_pMesh->UnlockVertexBuffer();
	
	return S_OK;
 }

void Engine::CStaticMesh::Render_Meshes(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTexture[i]);
		m_pMesh->DrawSubset(i);
	}
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTexture[i]);

	Safe_Delete_Array(m_ppTexture);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pAdjacency);
	Safe_Release(m_pSubset);
	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);

	CComponent::Free();
}

