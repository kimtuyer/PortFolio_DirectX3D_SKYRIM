#include "stdafx.h"
#include "NPCState_Mealtime.h"
#include "NPC.h"

//#include "Export_Function.h"

NPCState_Mealtime::NPCState_Mealtime()
{

}

NPCState_Mealtime::NPCState_Mealtime(const NPCState_Mealtime& rhs)
	
{

}

NPCState_Mealtime::~NPCState_Mealtime(void)
{

}

HRESULT NPCState_Mealtime::Ready_State(CNPC& pNPC)
{

	pNPC.Get_Transform()->Chase_Target_Navi(&pNPC.Get_BarLoc(), pNPC.Get_Transform()->m_vInfo[INFO_POS]);
	return S_OK;

}

Engine::_int NPCState_Mealtime::Update_State(CNPC& pNPC, const _float& fTimeDelta)
{
	if(!pNPC.Get_FindPath()) //기존의 설정한 이동경로 없는 경우
		pNPC.Set_PathList(pNPC.Get_BarLoc(), fTimeDelta);
	_vec3 vNextPos1;
	_vec3 vCurPos = pNPC.Get_Transform()->m_vInfo[INFO::INFO_POS];

	 vNextPos1= pNPC.Get_PathList()->front();

	_vec3 vDir = { vNextPos1-vCurPos };
	D3DXVec3Normalize(&vDir, &vDir);


		 _vec3 vNextPos2 = { pNPC.Get_NaviCom()->Move_OnNaviMesh(&vCurPos, &(vDir *fTimeDelta * 1.5f)).x,
					pNPC.SetUp_OnTerrain(), pNPC.Get_NaviCom()->Move_OnNaviMesh(&vCurPos, &(vDir *fTimeDelta * 1.5f)).z };
		
		//pNPC.Get_Transform()->Move_Pos(&vDir, fTimeDelta, 1.5f);
		// _vec3 vNextPos2 = { pNPC.Get_Transform()->m_vInfo[INFO::INFO_POS].x,pNPC.SetUp_OnTerrain(),
		//		 pNPC.Get_Transform()->m_vInfo[INFO::INFO_POS].z };

		 pNPC.Get_Transform()->Chase_Target_Navi(&vNextPos1, vNextPos2);
		 vNextPos1 -= vNextPos2;
		 float fDist = D3DXVec3Length(&vNextPos1);

	if (pNPC.Get_PathList()->size() > 1 &&  fDist <= 1.3)//다음 이동경로 새로 설정
	{
			 if (pNPC.Get_PathList()->size() >= 1)
				 pNPC.Get_PathList()->pop();

	 }


	if(pNPC.Get_PathList()->size()==1 && fDist<= 1.3f)
	{

		m_fTime += fTimeDelta;

		//시간 경과 체크- 오후가 되면 
		if (m_fTimeCycle <= m_fTime)
		{
			m_fTime = 0.f;
			pNPC.m_pState = NPCState_GoHome::Create();
			pNPC.Set_RenderOn(false);
			pNPC.Set_FindPath();
		}
	}

	return 0;
}

_int NPCState_Mealtime::Change_State(CNPC& pPlayer)
{
	return _int();
}





NPCState_Mealtime* NPCState_Mealtime::Create()
{
	static NPCState_Mealtime pInstance;
	//NPCState_Mealtime*	pInstance = new NPCState_Mealtime();

	//if (FAILED(pInstance->Ready_State()))
	//	Safe_Release(pInstance);

	return &pInstance;
}

void NPCState_Mealtime::Free(void)
{

	//CGameObject::Free();
}

