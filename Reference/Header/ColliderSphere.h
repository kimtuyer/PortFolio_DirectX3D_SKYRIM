#ifndef ColliderSphere_h__
#define ColliderSphere_h__

#include "Component.h"

BEGIN(Engine)

typedef struct _tagSphere
{
	_vec3						vCenter;
	float						fRadius;
}Sphere;


class ENGINE_DLL CColliderSphere : public CComponent
{
private:
	explicit CColliderSphere(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CColliderSphere(const CColliderSphere& rhs);
	virtual ~CColliderSphere(void);

public:
	const _matrix*			Get_CollWorldMatrix(void) { return &m_matCollWorld; }
	 _vec3*			Get_Center(void) { return &m_Sphere.vCenter; }
	 float&			Get_Radius(void) { return m_Sphere.fRadius; }

	 void			Set_Center(_vec3 vPos) { m_Sphere.vCenter = vPos; }

	 void			Set_Radius(float fRad) { m_Sphere.fRadius = fRad; }

	 void			Set_RadiusScale(float fRad) { m_Sphere.fRadius *= fRad; }

	 void			Set_Matrix(_matrix& mMatrix) {
		 m_matCollWorld = mMatrix
			 ;
	 }
	 void			Set_constMatrix(const _matrix* mMatrix) {
		 m_matCollWorld = *mMatrix
			 ;
	 }
	 void			Set_Pos(_vec3 vPos)
	 {
		 m_matCollWorld._41 = vPos.x;
		 m_matCollWorld._42 = vPos.y;
		 m_matCollWorld._43 = vPos.z;

	 }
//	const _vec3*			Get_Max(void) { return &m_vMax; }

public:
	HRESULT			Ready_ColliderSphere(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwVtxSize);
	void			Render_ColliderSphere(COLLTYPE eType, const _matrix* pColliderSphereMatrix);

private:
	//_vec3						vCenter;
	//float						fRadius;

#ifdef _DEBUG
	Sphere					m_Sphere;
	LPD3DXMESH					m_pSphere;
	//////////////////////////////////////////////
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];
#endif

	_matrix						m_matCollWorld;

public:
	static CColliderSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwVtxSize);

	static CColliderSphere*	Create(LPDIRECT3DDEVICE9 pGraphicDev);


	virtual CComponent* Clone(void);
	virtual void Free(void);
};

END
#endif // ColliderSphere_h__
