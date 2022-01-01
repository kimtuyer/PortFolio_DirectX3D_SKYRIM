#ifndef Dragon_h__
#define Dragon_h__

#include "GameObject.h"
#include "Define.h"
#include"Player.h"
#include "GuardMan.h"
#include"DynamicCamera.h"
class CBreath;

BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;

END
class CDragon : public CGameObject
{
private:
	explicit CDragon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDragon(const CDragon& rhs);
	virtual ~CDragon(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;


	_int		Update_Collider(const _float& fTimeDelta);
	int			Action_Stage(const float& fTimeDelta);





	void		Set_ShotFull(bool check) { m_bShotFull = check; }
	bool		Get_ShorFull() { return m_bShotFull; }
	
	int			Get_EndIndex() { return m_iEndIndex; }
	void		Set_EndIndex(int i) { m_iEndIndex = i;
	
	m_bEndindexCheck = true;
	}

	bool			Get_EndIndexcheck() { return m_bEndindexCheck; }
	void		Set_EndIndexCheck(bool check) { m_bEndindexCheck = check; }
	
	void		Set_BreathRange(float dist) { m_frange = dist; }
	float		Get_BreathRange() { return m_frange; }

	void		Set_RenderOn(_bool check) { m_bRenderOn = check; }
	void		Set_Player(CGameObject* p) { m_pPlayer = p; }

	int			Get_ATT_Type() { return m_ATT_Type; }
	void		Set_ATT_Type(int i) { m_ATT_Type = i; }

	void		Set_Lever(bool check) { m_bLeverOn = check; }



	CColliderSphere*	Get_HeadCollider() { return m_pHeadCollider; }
	CColliderSphere*	Get_BodyCollider() { return m_pBodyCollider; }
	CColliderSphere*	Get_LwingCollider() { return m_pLWingCollider; }
	CColliderSphere*	Get_RwingCollider() { return m_pRWingCollider; }
	CColliderSphere*	Get_TailCollider() { return m_pTailCollider; }


	int			Get_ActionStage() { return m_bActionStage; }
	_bool		Get_DeathCheck() { return m_bDeath; }


	CTransform*			Get_Transform() { return m_pTransformCom; }

	vector<ItemInfo> Get_Inventory() { return m_vecInventory; }
	void			Set_Inventory(vector<ItemInfo> vecInven) {
		m_vecInventory = vecInven
			;
	}
	void			Insert_Item(ItemInfo titem) { m_vecInventory.push_back(titem); }
	void			Pop_Item(void) {
		if (!m_vecInventory.empty())
			m_vecInventory.pop_back();
	}


private:
	HRESULT					Add_Component(void);
	void					Key_Input(const _float& fTimeDelta);
	void					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CDynamicMesh*			m_pMeshCom2 = nullptr;

	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	_vec3					m_vDir;
	_bool					m_bRenderOn = false;

	CGameObject*			m_pPlayer=nullptr;


	CColliderSphere*		m_pHeadCollider = nullptr;
	CColliderSphere*		m_pBodyCollider = nullptr;
	CColliderSphere*		m_pTailCollider = nullptr;
	CColliderSphere*		m_pLWingCollider = nullptr;
	CColliderSphere*		m_pRWingCollider = nullptr;

	const _matrix*			m_pBoneMatrix = nullptr;
	const _matrix*			m_pParentWorldMatrix = nullptr;


	char*					m_wFrameBoneName[6] = {"NPC_Hub01","NPC_LFinger12","NPC_RFinger12","NPC_TailHub" ,"NPC_Head_MagicNode__Hmag_", "NPC_Tongue2" };

	
	vector<ItemInfo>		m_vecInventory;





	int							m_ATT_Type = -1;
	float						m_bActionStage = 0;
	int							m_SetAniIndex = 0;
	_vec3						TargetPos;
	_vec3						vDir;


	float						m_fTime = 0;
	float						m_fWait = 0;
	




	//드래곤 움직임 트리거!
	_bool						m_bLeverOn = false; //레버키당길때 발동되는 애니모션
	_bool						m_bDeath = false;





	//드래곤 브레스에 관한 변수
	CBreath*				m_pBreath;
	_bool					m_bShot = false;
	_bool					m_bShotFull = false; //멀티 파티클형태 0번째 이펙트텍스처가 사정거리끝에 도달시확인!
	int						iCount = 0;
	int						m_iEndIndex = 0;// 렌더링할 드래곤이펙트의 마지막번째 인덱스설정!
	_bool					m_bEndindexCheck = false;

	float					m_frange = 0;


public:
	static CDragon*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Dragon_h__
