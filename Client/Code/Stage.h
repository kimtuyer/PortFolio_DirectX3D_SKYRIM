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
	_bool			InvenKey = false;	//인벤토리창 활성화 여부 식별
	_bool			DialogInvenKey = false; //npc 대화창 통한 인벤토리창 활성화 여부 >> npc,플레이어 둘다 출력해야함
	_bool			QuestKey = false;	//퀘스트창 활성화 여부 식별!
	_bool			CraftKey = false; //C키 누를시 제련창 활성화 위해
	_bool			CraftOn = false;	//제련오브젝트와 충돌체크된 상황에서 Craftkey를 눌러 제련창 활성화!
	_bool			m_bEKey = false;
	_bool			PullOn1 = false; //레버와 충돌시 체크!
	_bool			PullOn2 = false; //레버와 충돌시 체크!
	_bool			PullOn3 = false; //레버와 충돌시 체크!
	_bool			Quest6_On = false;
	_bool			Searchkey = false;
	_bool			DragonInvenKey = false;


	float			PullDelayTime = 0;//레버당긴후 일정시간지나면 카메라시점이동


	
	int				InvenKeyCount = 0;
	int				QuestKeyCount = 0;
	int				InvenSelectKey = 0;  //인벤의 윈도우0번창 각 텍스처  선택 식별
	int				SmithKeyCount = 0;
	int				ItemPopCount = 0;
	int				Monster_InvenKeyCount = 0;



	int				InvenItemSelectKey = 0;  //인벤의 윈도우1번창  어떤 아이템 선택중인지 식별
	int				InvenItemCount = 0;		//현재 선택한 인물 인벤토리의 총 아이템 개수 size()

	int				Inven_Window_SelectKey = 0; //인벤의 윈도우 창 0,1,2번 선택 식별
	int				Inven_Window_PreSelectKey = 0; //이전의 인벤 윈도우창 값 기억, 다시 left해 돌아올때 값을 줄 수 있도록!


	int				Inven_ItemWindow_Type = UI_INVENTORY_Player; // 아이템 윈도우창이 보여주는 아이템리스트가 플레이어 인지 NPC 인지 식별여부!



	int				InvenItemSelectCount = 0; //최초 Right 키로 아이템 창 정보 생성시 , 아이템 텍스처 출력 클래스 생성용


	_bool			m_nIkey = false; //i 키값 눌렷나 체크

	int				m_iSelectItemGold = 0;
	int				m_ibgmcount1 = 0;
	int				m_ibgmcount2 = 0;
	int				m_ibgmcount3 = 0;
	int				m_ibgmcount4 = 0;



	//아이템 클래스 보유
	CGameObject*	m_pItemList = nullptr;

	CGameObject*			m_pDialog;	//대화창 클래스 보유해 대화창의 몇번 대화문 선택했는지 전달해주기 위해 보유



public:
	static CStage*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


};

#endif // Stage_h__
