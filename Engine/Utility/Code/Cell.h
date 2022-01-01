#ifndef Cell_h__
#define Cell_h__

#include "Line.h"

BEGIN(Engine)

class ENGINE_DLL CCell :public CBase
{
public:
	enum POINT		{	POINT_A, POINT_B,POINT_C, POINT_END	};
	enum NEIGHBOR	{	NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};
	enum LINE		{	LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum MOVING		{	COMPARE_MOVE, COMPARE_STOP, COPARE_END};

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _vec3*		Get_Point(POINT eType) const { return &m_vPoint[eType]; }

	///////
	 _vec3		MFC_Get_Point(POINT eType)  { return m_vPoint[eType]; }
	 void		MFC_Set_Point(POINT eType, _vec3 vPos) { m_vPoint[eType] = vPos; }

	 void		MFC_Set_Point_X(POINT eType, float fX)
	 {
		 m_vPoint[eType].x = fX;
	 }
	 void		MFC_Set_Point_Y(POINT eType, float fX)
	 {
		 m_vPoint[eType].y = fX;
	 }
	 void		MFC_Set_Point_Z(POINT eType, float fX)
	 {
		 m_vPoint[eType].z = fX;
	 }


	 void		Set_color(D3DXCOLOR color) { m_color = color; }





	CCell*				Get_Neighbor(NEIGHBOR eType) const { return m_pNeighbor[eType]; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	const _ulong*		Get_CellIndex(void) { return &m_dwIndex; }





public:
	HRESULT			Ready_Cell(const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	_bool			Compare_Point(const _vec3* pPointFirst, const _vec3* pPointSecond, CCell* pCell);
	void			Render_Cell(D3DXCOLOR color);

	MOVING			Compare(const _vec3* pEndPos, _ulong* pCellIndex);

private:
	_vec3					m_vPoint[POINT_END];
	CCell*					m_pNeighbor[NEIGHBOR_END];
	CLine*					m_Line[LINE_END];

	///
	D3DXCOLOR				m_color = { 255.f, 0.f, 0.f, 0.8f };

	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXLINE				m_pD3DXLine;
	_ulong					m_dwIndex;

public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void		Free(void);
};


END
#endif // Cell_h__
