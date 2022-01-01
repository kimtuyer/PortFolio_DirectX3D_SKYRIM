#ifndef Dialog_h__
#define Dialog_h__

#include "GameObject.h"
#include "Define.h"
#include "Player.h"
#include "Stage.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END
class CDialog : public CGameObject
{
private:
	explicit CDialog(LPDIRECT3DDEVICE9 pGraphicDev, wstring Mesh_proto);
	explicit CDialog(const CDialog& rhs);
	virtual ~CDialog(void);

public:
	HRESULT Ready_CDialog_Object(wstring Mesh_proto);

	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void Render_Object(void) override;
	void Set_fX(float x) { m_fX = x; }
	void Set_fY(float y) { m_fY = y; }

	void Set_SizeX(float x) { m_fSizeX = x; }
	void Set_SizeY(float y) { m_fSizeY= y; }

	void Set_RenderOn(_bool state) { m_bRenderOn = state; }

	void Set_DialogSelect(int index) { m_iDialogSelect = index; } //대화문 0~2번중 어떤 대화문 선택했는지 식별
	int Get_DialogIndex() { return m_iDialogSelect; }
  

	_bool		Get_InvenEnterKey() { return m_bInvenEnterKey; } //물건 머 파냐는 대화문 위에 화살표있으면서 엔터키를 눌렀을시!
	void		Set_InvenEnterKey(_bool key) { m_bInvenEnterKey = key; }


	_bool		Get_TabKey() { return m_bTabKey; } //물건 머 파냐는 대화문 위에 화살표있으면서 엔터키를 눌렀을시!



	void	Render_Script();


	int			Get_NPC_Type() {
		return m_iNPC_type
			;
	}
	void		Set_NPC_Type(int i) { m_iNPC_type = i; }
	
	void		Set_NPC_Name(wstring name) { m_wNPC_Name = name; }
	wstring		Get_NPC_Name() {
		return m_wNPC_Name
			;
	}

	void		Set_Player(CGameObject* player) { m_pPlayer = player; }

	
	void		Set_Questlist(vector<QuestInfo> quest) { m_vecQuestlist = quest; }

	void		Set_QuestlistState(int NpcCode, int State)
	{
		for (int i = 0; i < m_vecQuestlist.size(); i++)
		{
			if (m_vecQuestlist[i].NPC_Code == NpcCode)
			{
				m_vecQuestlist[i].Quest_State = State;
			}
		}

	}




	void		Set_QuestState(int id,int state) {

		if (id == 1)
		{
			m_iQuest1 = state;;
		}
		if (id == 2)
		{
			m_iQuest2 = state;;
		}
		if (id == 3)
		{
			m_iQuest3 = state;;
		}
		if (id == 4)
		{
			m_iQuest4 = state;;
		}
		if (id == 5)
		{
			m_iQuest5 = state;;
		}
		if (id == 6)
		{
			m_iQuest6 = state;;
		}


	}











	//void		Set_QuestState(int size) { m_pQuest_State = new int[size]; 
	//
	//for (int i = 0; i < m_vecQuestlist.size(); i++)
	//	m_pQuest_State[i] = QUEST_State::Quest_Off;
	//
	//}
	//int			Get_QuestState(int index) { return m_pQuest_State[index]; }
	//void		Update_QuestState(int index, int state)
	//{
	//	m_pQuest_State[index] = state;
	//}


	
	vector<QuestInfo>	Get_Questlist() { return m_vecQuestlist; }


	int				Get_QuestState(int id) {

		if (id == 1)
		{
			return m_iQuest1;
		}
		if (id == 2)
		{
			return m_iQuest2;
		}
		if (id == 3)
		{
			return m_iQuest3;
		}
		if (id == 4)
		{
			return m_iQuest4;
		}
		if (id == 5)
		{
			return m_iQuest5;
		}
		if (id == 6)
		{
			return m_iQuest6;
		}
		

	}


	int Get_QuestStage() { return m_iStage; }

private:
	HRESULT					Add_Component(wstring Mesh);

private:
	CRcTex*					m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	CRenderer*				m_pRendererCom = nullptr;

	_matrix					m_matProj;

	_float					m_fX, m_fY;
	_float					m_fSizeX, m_fSizeY;
	_bool						m_bRenderOn = false;

	int						m_iNPC_type = NPC_Type::NPC_END;	//현재 대화중인 npc 가 누구인지 식별!
	wstring					m_wNPC_Name;


	int						m_iDialogSelect = 0;
	int						m_iCurrentDialogSelect = 0; //대화문의 마지막 선택지-퀘스트 수락 여부
	_bool					m_bInvenEnterKey = false;
	_bool					m_bTabKey = false;


	CGameObject*			m_pTargetArrow = nullptr;
	CGameObject*			m_pPlayer = nullptr;
	CGameObject*			m_F_Key = nullptr;


	float					m_fTime = 0;
	int						m_iStage = 0;	//퀘스트 대화문 진행단계 위해
	_bool					m_bQuest = false; //퀘스트 최종 수락 여부 식별

	vector<QuestInfo>		m_vecQuestlist;
	//int*					m_pQuest_State;
	int						m_iItemCount = 0;

	int						m_iQuest1 = QUEST_State::Quest_Off;
	int						m_iQuest2 = QUEST_State::Quest_Off;
	int						m_iQuest3 = QUEST_State::Quest_Off;
	int						m_iQuest4 = QUEST_State::Quest_Off;
	int						m_iQuest5 = QUEST_State::Quest_Off;
	int						m_iQuest6 = QUEST_State::Quest_Off;


	_bool					m_bItemUpdate = false;


public:
	static CDialog*		Create(LPDIRECT3DDEVICE9 pGraphicDev,wstring Mesh_proto);

private:
	virtual void		Free(void);
};


#endif // Dialog_h__
