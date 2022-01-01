#include "NaviMesh.h"

USING(Engine)

Engine::CNaviMesh::CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CComponent(pGraphicDev)
	, m_dwIndex(0)	
{

}

Engine::CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	:CComponent(rhs)
	, m_dwIndex(rhs.m_dwIndex)
	, m_vecCell(rhs.m_vecCell)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

Engine::CNaviMesh::~CNaviMesh(void)
{
	CNaviMesh::Free();
}

HRESULT Engine::CNaviMesh::Ready_NaviMesh(const wstring & wstrFilePath)
{

	HANDLE hFile = CreateFile(wstrFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwbyte = 0;
	int pTileCount = 0;

	Engine::CellInfo* pTile = nullptr;
	//TILE* pTile = nullptr; 
	while (true)
	{
		pTile = new CellInfo;

		ReadFile(hFile, pTile, sizeof(CellInfo), &dwbyte, nullptr);
		if (0 == dwbyte)
		{
			Safe_Delete(pTile);
			break;
		}
		

		CCell* pCell = CCell::Create(m_pGraphicDev, pTile->CellIndex, &pTile->vPoint[0], &pTile->vPoint[1], &pTile->vPoint[2]);
		//pView->m_pNaviMesh->Insert_Sell(pCell);

		m_vecCell.emplace_back(pCell);

		//++pTileCount;
	}
	CloseHandle(hFile);

	FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);


	
	//m_vecCell.reserve(4);
	//
	//CCell*	pCell = nullptr;
	//
	//// 0
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	//
	//// 1
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	//
	//// 2
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	//
	//// 3
	//pCell = CCell::Create(m_pGraphicDev, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	//NULL_CHECK_RETURN(pCell, E_FAIL);
	//m_vecCell.push_back(pCell);
	//
	//FAILED_CHECK_RETURN(Link_Cell(), E_FAIL);

	return S_OK;
}

HRESULT CNaviMesh::MFC_Ready_NaviMesh(void)
{
	return S_OK;
}

void Engine::CNaviMesh::Render_NaviMesh(D3DXCOLOR color)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell(color);
}


CNaviMesh* Engine::CNaviMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_NaviMesh(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CNaviMesh * CNaviMesh::MFC_Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNaviMesh*	pInstance = new CNaviMesh(pGraphicDev);

	if (FAILED(pInstance->MFC_Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CNaviMesh::Clone(void)
{
	return new CNaviMesh(*this);
}

void Engine::CNaviMesh::Free(void)
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();

	CComponent::Free();
}

HRESULT Engine::CNaviMesh::Link_Cell(void)
{

	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			// 1. 이웃하는 셀 정보가 없어서 내가 연결해주고자 하는 셀을 넣을 수 있는지
			// 2. 이웃하는 셀과 기본 셀의 정점 두 개가 같은 점인지

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true    == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
														m_vecCell[i]->Get_Point(CCell::POINT_B), 
														m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
													m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
													m_vecCell[i]->Get_Point(CCell::POINT_A),
													m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}
	
	return S_OK;
}

Engine::_vec3 Engine::CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos,
												const _vec3* pTargetDir)
{

	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return vEndPos;

	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex))
		return *pTargetPos;

}