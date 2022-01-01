#ifndef PlayerHelmet_h__
#define PlayerHelmet_h__

#include "GameObject.h"
#include "Define.h"
#include "Player.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CColliderSphere;
class CDynamicMesh;

END
class CPlayerHelmet : public CGameObject
{
private:
	explicit CPlayerHelmet(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayerHelmet(const CPlayerHelmet& rhs);
	virtual ~CPlayerHelmet(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	void Set_BoneName( char*	 bone) { m_wFrameBoneName = bone; }

	void Set_Player(CGameObject* pp) { m_pPlayer = pp; }


private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);
	_bool m_bColl;
private:	
	CDynamicMesh*			m_pMeshCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	const _matrix*			m_pParentBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;


	int						m_iPlayerAniIndex = 0;
	CGameObject*			m_pPlayer;
	 char*					m_wFrameBoneName;
public:
	static CPlayerHelmet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // PlayerHelmet_h__
