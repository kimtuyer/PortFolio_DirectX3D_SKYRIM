#ifndef Skyheaventemple_h__
#define Skyheaventemple_h__

#include "GameObject.h"
#include "Define.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class COptimization;

END
class CSkyheaventemple : public CGameObject
{
private:
	explicit CSkyheaventemple(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyheaventemple(const CSkyheaventemple& rhs);
	virtual ~CSkyheaventemple(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);

private:	
	CStaticMesh*			m_pMeshCom			= nullptr;
	CTransform*				m_pTransformCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CCalculator*			m_pCalculatorCom	= nullptr;
	CCollider*				m_pColliderCom		= nullptr;
	COptimization*			m_pOptimizationCom	= nullptr;
	
	_bool					m_bColl = false;
	_uint					m_iFlag;
	_bool					m_bDraw = false;


public:
	static CSkyheaventemple*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Skyheaventemple_h__