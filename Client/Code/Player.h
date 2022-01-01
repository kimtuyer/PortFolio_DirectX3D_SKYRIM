#ifndef Player_h__
#define Player_h__

#include "GameObject.h"
#include "Define.h"
#include "Sword.h"
#include "Dialog.h"
#include "DragonEffect.h"
BEGIN(Engine)

class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;

END
class CPlayer : public CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;

	CColliderSphere*	Get_ColliderCom() {
		return m_pColliderCom
			;
	}

	void Set_DialogON(_bool Check) { m_bDialogOn = Check; }
	_bool	Get_DialogOn() { return m_bDialogOn; }

	void	Set_DialogState(_bool check) { m_bDialogState = check; }




	void	Set_Sword(CGameObject* pObj) { m_pSword = pObj; }
	CGameObject* Get_Sword() { return m_pSword; }

	void	Set_Dialog(CGameObject* pObj) { m_pDialog = pObj; }
	CGameObject* Get_Dialog() { return m_pDialog; }


	bool	Get_EquipState() { return m_bEquipOn; }


	int		Get_AniIndex() { return m_iAniIndex; }

	bool	Get_forwardStop() {
		return m_bforwardStop
			;
	}
	bool	Get_bacwardStop() {
		return m_bbacwardStop
			;
	}

	bool	Get_LeftStop() {
		return m_bLeftStop
			;
	}
	bool	Get_RightStop() {
		return m_bRightStop
			;
	}








	bool	Get_ArmorFullEquip() { return m_bArmorFull; }

	vector<ItemInfo> Get_Inventory() { return m_vecInventory; }
	void			Set_Inventory(vector<ItemInfo> vecInven) {
		m_vecInventory = vecInven
			;
	}
	void			Insert_Item(ItemInfo titem) { m_vecInventory.push_back(titem); }
	void			Pop_Item(void) {
		if(!m_vecInventory.empty())
		m_vecInventory.pop_back(); }
	void		Strength_Item(int itemCode,int Stat, int Value)
	{
		for(auto&p:m_vecInventory)
			if (p.Item_Code == itemCode)
			{
				p.Item_Stat += Stat;
				p.Item_Value += Value;

			}

	}




	bool	Get_WeaponAttackEquip() { return m_bEquipOn
	;} //무기를 손에 들고있는지 검사



	void	Set_CarryWeight(int weight) { m_iCarryWeight = weight; }
	int		Get_CarryWeight() { return m_iCarryWeight; }

	void	Set_Gold(int gold) { m_iGold -= gold; }
	int		Get_Gold() { return m_iGold; }



	CTransform*		Get_Transform() { return m_pTransformCom; }
	CNaviMesh*		Get_Navi() { return m_pNaviCom; }
	void			Set_m_bDialogState() { m_bDialogState = true; }


private:
	HRESULT					Add_Component(void);
	void					Key_Input(const _float& fTimeDelta);
	_float					SetUp_OnTerrain(void);
	_vec3					PickUp_OnTerrain(void);

private:
	CDynamicMesh*			m_pMeshComNude = nullptr;
	CDynamicMesh*			m_pMeshComFull = nullptr;

	CNaviMesh*				m_pNaviCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;
	CColliderSphere*		m_pColliderCom = nullptr;


	int						m_iAniIndex = 0;

	CColliderSphere*		m_pDialog_ColliderCom = nullptr;
	CGameObject*			m_pDialog;


	CGameObject*			m_pSword;
	_vec3					m_vDir;

	const _matrix*			m_pBoneMatrix = nullptr;

	_bool					m_bDialogOn = false;		//대화창 활성화 여부 
	_bool					m_bDialogState = false;		//대화창이 열려있는지 여부/ >>대화시 플레이어움직임 제한!
	_bool					m_bEquipOn = false;			//현재 소지하고 있는 무기를 공격 모션 상태로 장착여부


	_bool					m_bforwardStop = false; //전방 달리기중 충돌인해 정지여부 -갑옷과 싱크 위해
	_bool					m_bbacwardStop = false; //전방 달리기중 충돌인해 정지여부 -갑옷과 싱크 위해

	_bool					m_bLeftStop = false; //전방 달리기중 충돌인해 정지여부 -갑옷과 싱크 위해
	_bool					m_bRightStop = false; //전방 달리기중 충돌인해 정지여부 -갑옷과 싱크 위해


	int						m_iCarryWeight = 0; //플레이어가 소지하고 있는 아이템들의 총 무게
	int						m_iGold = 1000; //기본 골드 1000 설정

	float					m_fSwingTime = 0;


	_bool					m_bLB = false;
	_bool					m_bRB = false;
	int						m_iAttackCount = 0;

	_bool					m_bMapKey = false;
public:



	void			Set_EquipArmorCode(int i)
	{
		m_iEquipArmor_Code = i;

		m_bArmor = true;
		if (i == -1)
			m_bArmor = false;

	}
	void			Set_EquipBootsCode(int i)
	{
		m_iEquipBoots_Code = i;
		m_bBoots = true;
		if (i == -1)
			m_bBoots = false;
	}
	void			Set_EquipGauntletCode(int i)
	{
		m_iEquipGauntlet_Code = i;
		m_bArm = true;
		if (i == -1)
			m_bArm = false;
	}
	void			Set_EquipHelmetCode(int i)
	{
		m_iEquipHelmet_Code = i;
		m_bHelmet = true;
		if (i == -1)
			m_bHelmet = false;
	}

	void			Set_EquipWeaponCode(int i)
	{
		m_iEquipWeapon_Code = i;
		m_bWeapon = true;
		if (i == -1)
		{
			m_bWeapon = false;
			m_bEquipOn = false; //무기 자체를 해제했기 때문에 공격상태의 무기장착도 해제 처리된다

		}
	}


	int			Get_EquipArmorCode(){ return m_iEquipArmor_Code;}
	int			Get_EquipBootsCode() { return m_iEquipBoots_Code; }
	int			Get_EquipGauntletCode() { return m_iEquipGauntlet_Code; }
	int			Get_EquipHelmetCode() { return m_iEquipHelmet_Code; }
	int			Get_EquipWeaponCode() { return m_iEquipWeapon_Code; }


	_bool		Get_ArmorEquip() {	return m_bArmor;}
	_bool		Get_ArmEquip() { return m_bArm; }
	_bool		Get_BootsEquip() { return m_bBoots; }
	_bool		Get_bHelmetEquip() { return m_bHelmet; }
	_bool		Get_bWeaponEquip() { return m_bWeapon; }


	_bool		Get_Wkey() { return m_bWKey; }
	_bool		Get_Skey() { return m_bSKey; }
	_bool		Get_Akey() { return m_bAKey; }
	_bool		Get_Dkey() { return m_bDKey; }



private:

	//각 부위별 파트 장착여부	식별
	_bool					m_bArmor	= false;
	_bool					m_bArm		= false;
	_bool					m_bBoots	= false;
	_bool					m_bHelmet	= false;
	_bool					m_bWeapon	= false;

	

	_bool					m_bArmorFull = false;		//아머 풀 장착 여부 식별


	vector<ItemInfo>		m_vecInventory;
	int						m_iEquipArmor_Code = -1;
	int						m_iEquipBoots_Code = -1;
	int						m_iEquipGauntlet_Code = -1;
	int						m_iEquipHelmet_Code = -1;
	int						m_iEquipWeapon_Code = -1;


	_bool					m_bWKey = false;
	_bool					m_bAKey = false;
	_bool					m_bSKey = false;
	_bool					m_bDKey = false;



public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free(void);
};


#endif // Player_h__
