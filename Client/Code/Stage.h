#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"
#include "Define.h"
#include "BackGround.h"
#include "Player.h"
#include "Monster.h"

#include "Terrain.h"
#include "DynamicCamera.h"
#include "SkyBox.h"

#include "Skyheaventemple.h"

#include "Monster_draugr.h"
#include "Stone.h"
#include "Sword.h"
#include "Tree.h"
#include "Effect.h"
#include "CStatic_Object.h"
#include "Player.h"
#include"PlayerArmor.h"
#include"PlayerBoots.h"
#include "PlayerGauntlet.h"
#include"PlayerHelmet.h"


#include "GuardMan.h"
#include "BlacksmithNPC.h"
#include "imperialSword.h"
#include "UI.h"
#include "Hp.h"
#include "Mp.h"
#include "Sp.h"
#include "Dialog.h"
#include "MonsterBar.h"
#include "MonsterHP.h"

#include "MarketNPC.h"
#include "BarkeeperNPC.h"
#include "NPC.h"
#include "Dragon.h"

#include"Font_texture.h"
#include"CItem.h"
#include"CItemObject.h"

#include "StaticCamera.h"
#include"Effect.h"
#include "DragonEffect.h"
#include"Trap.h"

BEGIN(Engine)

class CDynamicMesh;
class CTransform;

class CCalculator;
class CCollider;
class CNaviMesh;
class CColliderSphere;



END

class CStage : public CScene
{
private:
	explicit CStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage(void);

public:
	virtual HRESULT Ready_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

	_bool	Collison_Scene(const _float& ftimeDelta);
	int		Battle_Scene(const _float& ftimeDelta);
	void	Change_BGM(const _float& ftimeDelta);



	_int				Load_ItemInfo();
	_int				Load_QuestInfo();
	_int				Update_InventoryScene(const _float& fTimeDelta);
	_int				Update_NPC_InventoryScene(const _float& fTimeDelta);
	_int				Update_QuestWindow(const _float& fTimeDelta);
	//_int				Update_QuestState(const _float& fTimeDelta);
	_int				Update_SmithingWindow(const _float& fTimeDelta);
	_int				Update_Monster_InventoryWindow(const _float& fTimeDelta);


	bool			Get_DialogCheck() { return DialogCheck; }


	_bool			Get_Quest6() { return Quest6_On; }

	_bool			Get_PullOn1() { return PullOn1; }
	_bool			Get_PullOn2() { return PullOn2; }
	_bool			Get_PullOn3() { return PullOn3; }

	void			Set_PullOn1(_bool check) { PullOn1 = check; }

	bool Object_Culling(int Naviindex, int CurrentArea)
	{
		int Area = 0;

		m_iCurrentArea = CurrentArea;

		if (Naviindex >= 0 && Naviindex <= 18 || Naviindex >= 182 && Naviindex <= 199)
		{
			Area = 1;


		}
		else if (Naviindex >= 19 && Naviindex <= 73)
		{
			Area = 2;


		}
		else if (Naviindex >= 74 && Naviindex <= 128)
		{
			Area = 3;


		}
		else if (Naviindex >= 129 && Naviindex <= 181)
		{
			Area = 4;


		}

		if (m_iCurrentArea == Area)
			return true;
		else
			return false;

	}
	void Set_CurrentArea(int i)
	{
		m_iCurrentArea = i;
	}



private:
	HRESULT			Ready_Environment_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_Layer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_InventoryLayer(const _tchar* pLayerTag);
	HRESULT			Ready_UI_QuestLayer(const _tchar* pLayeTag);
	HRESULT			Ready_UI_SmithingLayer(const _tchar* pLayeTag);


	HRESULT			Ready_LightInfo(void);
	
	_int Load_Update_Scene(ObjectInfo* pInfo);







private:
	_ulong			m_dwRenderCnt = 0;
	_tchar			m_szFPS[256];
	_float			m_fTime = 0.f;
	float			m_fColliTime = 0.f;
	float			m_fColliDragonTime = 0.f;
	float			m_PlayerSPTime = 0;

	int				m_iCount = 0;
	int				m_iCurrentArea = 0;

	CCalculator*			m_pCalculatorCom = nullptr;
	vector<CColliderSphere*> m_vecCollider;
	_bool			DialogCheck = false;
	int				ColliInit = 0;	


	vector<bool>	m_vecNPC_ColliderList;
	_bool			InvenKey = false;	//�κ��丮â Ȱ��ȭ ���� �ĺ�
	_bool			DialogInvenKey = false; //npc ��ȭâ ���� �κ��丮â Ȱ��ȭ ���� >> npc,�÷��̾� �Ѵ� ����ؾ���
	_bool			QuestKey = false;	//����Ʈâ Ȱ��ȭ ���� �ĺ�!
	_bool			CraftKey = false; //CŰ ������ ����â Ȱ��ȭ ����
	_bool			CraftOn = false;	//���ÿ�����Ʈ�� �浹üũ�� ��Ȳ���� Craftkey�� ���� ����â Ȱ��ȭ!
	_bool			m_bEKey = false;
	_bool			PullOn1 = false; //������ �浹�� üũ!
	_bool			PullOn2 = false; //������ �浹�� üũ!
	_bool			PullOn3 = false; //������ �浹�� üũ!
	_bool			Quest6_On = false;
	_bool			Searchkey = false;
	_bool			DragonInvenKey = false;


	float			PullDelayTime = 0;//��������� �����ð������� ī�޶�����̵�


	
	int				InvenKeyCount = 0;
	int				QuestKeyCount = 0;
	int				InvenSelectKey = 0;  //�κ��� ������0��â �� �ؽ�ó  ���� �ĺ�
	int				SmithKeyCount = 0;
	int				ItemPopCount = 0;
	int				Monster_InvenKeyCount = 0;



	int				InvenItemSelectKey = 0;  //�κ��� ������1��â  � ������ ���������� �ĺ�
	int				InvenItemCount = 0;		//���� ������ �ι� �κ��丮�� �� ������ ���� size()

	int				Inven_Window_SelectKey = 0; //�κ��� ������ â 0,1,2�� ���� �ĺ�
	int				Inven_Window_PreSelectKey = 0; //������ �κ� ������â �� ���, �ٽ� left�� ���ƿö� ���� �� �� �ֵ���!


	int				Inven_ItemWindow_Type = UI_INVENTORY_Player; // ������ ������â�� �����ִ� �����۸���Ʈ�� �÷��̾� ���� NPC ���� �ĺ�����!



	int				InvenItemSelectCount = 0; //���� Right Ű�� ������ â ���� ������ , ������ �ؽ�ó ��� Ŭ���� ������


	_bool			m_nIkey = false; //i Ű�� ���ǳ� üũ

	int				m_iSelectItemGold = 0;
	int				m_ibgmcount1 = 0;
	int				m_ibgmcount2 = 0;
	int				m_ibgmcount3 = 0;
	int				m_ibgmcount4 = 0;



	//������ Ŭ���� ����
	CGameObject*	m_pItemList = nullptr;

	CGameObject*			m_pDialog;	//��ȭâ Ŭ���� ������ ��ȭâ�� ��� ��ȭ�� �����ߴ��� �������ֱ� ���� ����



public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // Stage_h__
