#ifndef MFC_ColliderSphere_h__
#define MFC_ColliderSphere_h__

#include "Component.h"

BEGIN(Engine)

typedef struct _tagSphere2
{
	_vec3						vCenter;
	float						fRadius;
}Sphere2;


class  CMFC_ColliderSphere : public CComponent
{
private:
	explicit CMFC_ColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMFC_ColliderSphere(const CMFC_ColliderSphere& rhs);
	virtual ~CMFC_ColliderSphere(void);

public:
	const _matrix*			Get_CollWorldMatrix(void) { return &m_matCollWorld; }
	// _vec3*			Get_Center(void) { return &m_Sphere.vCenter; }
	// float&			Get_Radius(void) { return m_Sphere.fRadius; }

//	const _vec3*			Get_Max(void) { return &m_vMax; }

public:
	HRESULT			Ready_MFC_ColliderSphere(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwVtxSize);
	void			Render_MFC_ColliderSphere(COLLTYPE eType, const _matrix* pMFC_ColliderSphereMatrix);

	void Set_Pos(_vec3 Pos){
	
		m_matCollWorld._41 = Pos.x;
		m_matCollWorld._42 = Pos.y;
		m_matCollWorld._43 = Pos.z;

	
	}

private:
	//_vec3						vCenter;
	//float						fRadius;

#ifdef _DEBUG
	Sphere2					m_tSphere;
	LPD3DXMESH					m_pSphere;
	//////////////////////////////////////////////
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
#endif

	_matrix						m_matCollWorld;

public:
	static CMFC_ColliderSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwVtxSize);

	virtual CComponent* Clone(void);
	virtual void Free(void);
};

END
#endif // MFC_ColliderSphere_h__
