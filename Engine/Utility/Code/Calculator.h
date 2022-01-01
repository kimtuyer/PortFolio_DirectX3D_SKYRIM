#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"

BEGIN(Engine)

class CTerrainTex;
class CTransform;

class ENGINE_DLL CCalculator : public CComponent
{
private:
	typedef struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// �������� �� ������ ������Ű�� ����
		_vec3		vAxis[3];		// �ڽ��� ������ ��� �����ϴ� ����

	}OBB;

private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCalculator(const CCalculator& rhs);
	virtual ~CCalculator(void);

public:
	HRESULT			Ready_Calculator(void);
	_float			Compute_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv = 1);
	
	_float			Compute_HeightOnNavi(const _vec3* pPos, const _vec3 A, const _vec3 B, const _vec3 C);

	
	
	
	_vec3			Picking_OnTerrain(HWND hWnd, const CTerrainTex* pTerrainBufferCom, const CTransform* pTerrainTransCom);

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

	_bool			Collision_Sphere(float& pDestRadius, _vec3* pDestCenter, const _matrix* pDestWorld, float DestScale
		, float& pSourRadius, _vec3* pSourCenter, const _matrix* pSourWorld, float SourScale);


private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

	bool		ColliderCheck = false;
	int			m_iColliderCount = 0;

public:
	static CCalculator*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CComponent*		Clone(void);
	virtual void			Free(void);

};

END
#endif // Calculator_h__
