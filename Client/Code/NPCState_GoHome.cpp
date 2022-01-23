#include "stdafx.h"
#include "NPCState_GoHome.h"
#include "NPC.h"

//#include "Export_Function.h"

NPCState_GoHome::NPCState_GoHome()
{

}

NPCState_GoHome::NPCState_GoHome(const NPCState_GoHome& rhs)
	
{

}

NPCState_GoHome::~NPCState_GoHome(void)
{

}

HRESULT NPCState_GoHome::Ready_State(CNPC& pNPC)
{

	return S_OK;
}

Engine::_int NPCState_GoHome::Update_State(CNPC& pNPC, const _float& fTimeDelta)
{
	pNPC.Set_RenderOn(true);

	if (!pNPC.Get_FindPath()) //기존의 설정한 이동경로 없는 경우
		pNPC.Set_PathList(pNPC.Get_HomeLoc(), fTimeDelta);

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
			pNPC.m_pState = NPCState_Normal::Create();
			pNPC.m_pState->Ready_State(pNPC);
			pNPC.Set_RenderOn(false);
			pNPC.Set_FindPath();
		}
	}

	return 0;
}

_int NPCState_GoHome::Change_State(CNPC& pPlayer)
{
	return _int();
}





NPCState_GoHome* NPCState_GoHome::Create()
{
	static NPCState_GoHome	pInstance;

	

	return &pInstance;
}

void NPCState_GoHome::Free(void)
{

	//CGameObject::Free();
}

