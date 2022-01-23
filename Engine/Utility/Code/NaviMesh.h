#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Cell.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CComponent
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void				Set_CellIndex(const _ulong& dwIndex) { m_dwIndex = dwIndex; }

	///////////////////////////////////
	void				Insert_Sell(CCell* pCell) { m_vecCell.push_back(pCell); }
	void				Insert_WayPointSell(WayPointInfo* pCell) { m_vecWayPoint.emplace_back(pCell); }
	void				Insert_WayPointIndex(const _ulong& dwindex) { m_vecWayPointIndex.push_back(dwindex); }


	vector<int>*			Get_Wayvector() { return &m_vecWayPointIndex; }
	vector<WayPointInfo*>*		Get_WayPointlist() { return &m_vecWayPoint; }
	WayPointInfo*				Get_WayPoint(int WayPointIndex) {
		return m_vecWayPoint[WayPointIndex];
	}

	CCell*				Get_Cell(const _ulong& dwindex) {return m_vecCell[dwindex]; }
	void				MFC_PopCell() {	if(!m_vecCell.empty()) m_vecCell.pop_back(); }


	void				Set_PointPos(int cellIndex, CCell::POINT etype,_vec3 vPos) {


		m_vecCell[cellIndex]->MFC_Set_Point(etype, vPos);

	}
	void				Set_PointPosX(int cellIndex, CCell::POINT etype, float fx) {


		m_vecCell[cellIndex]->MFC_Set_Point_X(etype, fx);
	}
	void				Set_PointPosY(int cellIndex, CCell::POINT etype, float fx) {


		m_vecCell[cellIndex]->MFC_Set_Point_Y(etype, fx);
	}
	void				Set_PointPosZ(int cellIndex, CCell::POINT etype, float fx) {


		m_vecCell[cellIndex]->MFC_Set_Point_Z(etype, fx);
	}

	void				MFC_ReleaseCell()
	{
		for (auto&p : m_vecCell)
			p->Free();
		m_vecCell.clear();
	}



public:
	HRESULT				Ready_NaviMesh(const wstring & wstrFilePath, const wstring & wstrWayPointFilePath);




	/////////////////////////////////
	HRESULT				MFC_Ready_NaviMesh(void);
	_ulong	Get_CurrentIndex() {
		return m_dwIndex;
	}
	


	void				Render_NaviMesh(D3DXCOLOR color = { 255.f, 0.f, 0.f, 0.8f});

	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

private:
	HRESULT				Link_Cell(void);

private:
	vector<CCell*>			m_vecCell;
	vector<Engine::WayPointInfo*>m_vecWayPoint;
	vector<int>				m_vecWayPointIndex;

	_ulong					m_dwIndex;

public:
	static CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* wstrWayPointFilePath);

	static CNaviMesh*		MFC_Create(LPDIRECT3DDEVICE9 pGraphicDev);


	virtual CComponent*		Clone(void);
	virtual void			Free(void);
};


END
#endif // NaviMesh_h__
