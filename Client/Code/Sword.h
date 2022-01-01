#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"
#include "Define.h"
#include "Player.h"
BEGIN(Engine)
class CDynamicMesh;

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CColliderSphere;

END
class CSword : public CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSword(const CSword& rhs);
	virtual ~CSword(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	void Set_BoneName( char*	 bone) { m_wFrameBoneName = bone; }


private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);
	_bool m_bColl;
private:	
	CStaticMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;


	 char*					m_wFrameBoneName;

	 CDynamicMesh*		pPlayerMeshCom = nullptr;
public:
	static CSword*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Sword_h__
