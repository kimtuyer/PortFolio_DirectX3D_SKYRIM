#ifndef Trap_h__
#define Trap_h__

#include "GameObject.h"
#include "Define.h"
#include"Player.h"
class CBreath;

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;
class CStaticMesh;

END
class CTrap : public CGameObject
{
private:
	explicit CTrap(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrap(const CTrap& rhs);
	virtual ~CTrap(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	_int		Update_Collider(const _float& fTimeDelta);




	void		Set_RenderOn(_bool check) { m_bRenderOn = check; }
	void		Set_Dragon(CGameObject* p) { m_pDragon = p; }


	void		Set_ID(int i) { m_ID = i; }

private:
	HRESULT					Add_Component(void);
	void					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CStaticMesh*			m_pMeshCom1000 = nullptr;
	CStaticMesh*			m_pMeshCom800 = nullptr;
	CStaticMesh*			m_pMeshCom600 = nullptr;
	CStaticMesh*			m_pMeshCom400 = nullptr;
	CStaticMesh*			m_pMeshCom200 = nullptr;





	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;
	_bool					m_bRenderOn = false;



	CColliderSphere*		m_pHeadCollider = nullptr;
	CColliderSphere*		m_pBodyCollider = nullptr;
	CColliderSphere*		m_pTailCollider = nullptr;
	CColliderSphere*		m_pLWingCollider = nullptr;
	CColliderSphere*		m_pRWingCollider = nullptr;

	const _matrix*			m_pBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;

	CGameObject*		m_pDragon = nullptr;


	char*					m_wFrameBoneName[6] = {"NPC_Hub01","NPC_LFinger12","NPC_RFinger12","NPC_TailHub" ,"NPC_Head_MagicNode__Hmag_", "NPC_Tongue2" };

	
	int						m_ID = 0;	
	float					m_fTime = 0;

	_bool					height600 = false;
	_bool					height400 = false;
	_bool					height200 = false;

	

public:
	static CTrap*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Trap_h__
