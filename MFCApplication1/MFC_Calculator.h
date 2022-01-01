#ifndef MFC_Calculator_h__
#define MFC_Calculator_h__

#include "Component.h"

BEGIN(Engine)


class MFC_TerrainTex;
class CTerrainTex;
class CTransform;

class  CMFC_Calculator : public CComponent
{
private:
	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// 중점에서 각 면으로 투영시키는 벡터
		_vec3		vAxis[3];		// 박스의 세개의 면과 평행하는 벡터

	}OBB;

private:
	 CMFC_Calculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMFC_Calculator(const CMFC_Calculator& rhs);
	virtual ~CMFC_Calculator(void);

public:
	HRESULT			Ready_MFC_Calculator(void);
	_float			Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	_vec3			Picking_OnTerrain(HWND hWnd, const MFC_TerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransCom);


	void			Set_HeightOnTerrain(_vec3 vMouse, _ulong& dwHeight, _ulong& dwSize, MFC_TerrainTex* pTerrainBufferCom);





	_bool			Collision_AABB(const _vec3* pDestMin,
									const _vec3* pDestMax,
									const _matrix* pDestWorld,
									const _vec3* pSourMin,
									const _vec3* pSourMax,
									const _matrix* pSourWorld);

	_bool			Collision_OBB(const _vec3* pDestMin,
								const _vec3* pDestMax,
								const _matrix* pDestWorld,
								const _vec3* pSourMin,
								const _vec3* pSourMax,
								const _matrix* pSourWorld);

	_bool			Collision_Sphere(float& pDestRadius, _vec3* pDestCenter, const _matrix* pDestWorld
		, float& pSourRadius, _vec3* pSourCenter, const _matrix* pSourWorld);


private:
	VTXTEX*			m_pVertex = nullptr;

private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

public:
	static CMFC_Calculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

};

END
#endif // MFC_Calculator_h__
