#ifndef Stone_h__
#define Stone_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;
class CColliderSphere;

END
class CStone : public CGameObject
{
private:
	explicit CStone(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);
	explicit CStone(const CStone& rhs);
	virtual ~CStone(void);

public:
	 HRESULT Ready_Static_Object(wstring Mesh_proto);


	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	CTransform*	Get_TransCom() { return m_pTransformCom; }

	CColliderSphere*	Get_Collider() { return m_pColliderCom; }

	void			Set_ID(int i) { m_id = i; }

private:
	HRESULT					Add_Component(wstring Mesh_proto);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);

private:	
	CStaticMesh*			m_pMeshCom			= nullptr;
	CStaticMesh*			m_pMeshLevelCom = nullptr;

	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	CColliderSphere*		m_pColliderCom		= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;
	
	_bool					m_bColl = false;
	//int			m_iHP = 3;
	_uint					m_iFlag;
	_bool					m_bDraw = false;

	int						m_id = 0;


public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);

private:
	virtual void		Free(void);
};


#endif // Stone_h__
