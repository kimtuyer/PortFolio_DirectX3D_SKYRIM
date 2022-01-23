#include "stdafx.h"
#include "NPCState_Normal.h"
#include "NPC.h"
//#include "Export_Function.h"

NPCState_Normal::NPCState_Normal()
{

}

NPCState_Normal::NPCState_Normal(const NPCState_Normal& rhs)
	
{

}

NPCState_Normal::~NPCState_Normal(void)
{

}

HRESULT NPCState_Normal::Ready_State(CNPC& pNPC)
{
	pNPC.Set_RenderOn(true);
	return S_OK;
}

Engine::_int NPCState_Normal::Update_State(CNPC& pNPC, const _float& fTimeDelta)
{
	pNPC.Set_RenderOn(true);

	if (!pNPC.Get_FindPath()) //기존의 설정한 이동경로 없는 경우
		pNPC.Set_PathList(pNPC.Get_StartLoc(), fTimeDelta);

	_vec3 vNextPos1;
	_vec3 vCurPos = pNPC.Get_Transform()->m_vInfo[INFO::INFO_POS];

	vNextPos1 = pNPC.Get_PathList()->front();

	_vec3 vDir = { vNextPos1 - vCurPos };
	D3DXVec3Normalize(&vDir, &vDir);


	_vec3 vNextPos2 = { pNPC.Get_NaviCom()->Move_OnNaviMesh(&vCurPos, &(vDir *fTimeDelta * 1.5f)).x,
			   pNPC.SetUp_OnTerrain(), pNPC.Get_NaviCom()->Move_OnNaviMesh(&vCurPos, &(vDir *fTimeDelta * 1.5f)).z };


	pNPC.Get_Transform()->Chase_Target_Navi(&vNextPos1, vNextPos2);
	vNextPos1 -= vNextPos2;
	float fDist = D3DXVec3Length(&vNextPos1);

	if (pNPC.Get_PathList()->size() > 1 && fDist <= 1.3)//다음 이동경로 새로 설정
	{
		if (pNPC.Get_PathList()->size() >= 1)
			pNPC.Get_PathList()->pop();

	}


	if (pNPC.Get_PathList()->size() == 1 && fDist <= 1.3f)
	{

		m_fTime += fTimeDelta;

		//시간 경과 체크- 오후가 되면 
		if (m_fTimeCycle <= m_fTime)
		{
			m_fTime = 0.f;
			pNPC.m_pState = NPCState_Mealtime::Create();
			//pNPC.m_pState->Ready_State(pNPC);	ww		
			pNPC.Set_RenderOn(false);
			pNPC.Set_FindPath();
		}
		//npc 아이템 인벤토리가 텅빌경우,모두판매시
		//if (pNPC.Get_Inventory().empty())
		//{
		//
		//	pNPC.m_pState = NPCState_GoHome::Create();
		//}


		  //&CNPC_State::mealtime;
		//pPlayer.ChangeState(&CNPC_State::mealtime)
		return 0;
	}
}

_int NPCState_Normal::Change_State(CNPC& pPlayera)
{
	return _int();
}





NPCState_Normal* NPCState_Normal::Create()
{
	static NPCState_Normal	pInstance;

	//if (FAILED(pInstance->Ready_State()))
	//	Safe_Release(pInstance);

	return &pInstance;
}

void NPCState_Normal::Free(void)
{

	//CGameObject::Free();
}

