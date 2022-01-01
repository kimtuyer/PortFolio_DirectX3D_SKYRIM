#ifndef GuardMan_h__
#define GuardMan_h__

#include "GameObject.h"
#include "Define.h"
#include "../Client/Code/imperialSword.h"
#include "../Client/Code/Player.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;

END
class CGuardMan : public CGameObject
{
private:
	explicit CGuardMan(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh);
	explicit CGuardMan(const CGuardMan& rhs);
	virtual ~CGuardMan(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	_bool					Collision_ToObject(const _tchar* pLayerTag, const OBJECT_ID pObjTag);


	CColliderSphere*	Get_ColliderCom() {
		return m_pColliderCom
			;
	}

	void	Set_IndexNumber(int i) { m_iIndex_number = i; }
	int		Get_IndexNumber() { return m_iIndex_number; }

	void	Set_Sword(CGameObject* pObj) { m_pSword = pObj; }
	CGameObject* Get_Sword() { return m_pSword; }


	void	Set_Player(CGameObject* pObj) { m_pPlayer = pObj; }
	CGameObject* Get_Player() { return m_pPlayer; }


	CTransform*		Get_Transform() { return m_pTransformCom; }
	CDynamicMesh*	Get_MeshCom() { return m_pMeshCom; }
	CNaviMesh*			Get_NaviCom() {
		return  m_pNaviCom
			;
	}


	//플레이어 객체끼리 맞닿은 상태인지 확인
	void	Set_ColliderCheck(bool check) { m_bColliderCheck = check; }
	int		Get_ColliderCheck() { return m_bColliderCheck; }

	void	Set_ID(int i) { m_iID = i; }
	int		Get_ID() { return m_iID; }

	void	Set_RenderOn(bool check) { m_bRenderOn = check; }
	_bool	Get_RenderOn() { return m_bRenderOn; }

	void	Set_CellIndex(int i) { m_Cellindex = i; }




private:
	HRESULT					Add_Component(wstring Mesh);
	void					Key_Input(const _float& fTimeDelta);
	_float					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshCom = nullptr;
	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;

	_vec3					m_vDir;
	CGameObject*			m_pSword;
	CGameObject*			m_pPlayer;
	_bool					m_bColliderCheck = false;
	int						m_iAttackCount = 0;
	float					m_fTime = 0;
	float					m_fAniTime = 0;
	float					m_fDeathTime = 0;



	const _matrix*			m_pBoneMatrix = nullptr;
	bool	m_bColl;
	int		m_iIndex_number;

	int						m_iID = -1;
	_bool					m_bRenderOn = false;
	int						m_Cellindex = 0;
	int						m_iStart = 0;


public:
	static CGuardMan*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh);

private:
	virtual void		Free(void);
};


#endif // GuardMan_h__
