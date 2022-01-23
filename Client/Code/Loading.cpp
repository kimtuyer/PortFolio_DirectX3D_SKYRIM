#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"


CLoading::CLoading(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_bFinish(false)	
{
	m_pGraphicDev->AddRef();
	ZeroMemory(m_szLoading, sizeof(_tchar) * 256);
}

CLoading::~CLoading(void)
{

}

HRESULT CLoading::Ready_Loading(LOADINGID eID)
{
	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"mus_maintheme.wav");

	InitializeCriticalSection(&m_Crt);

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	m_eID = eID;

	


	return S_OK;
}

Engine::_uint CLoading::LoadingForStage(void)
{
	

	//lstrcpy(m_szLoading, L"Texture Loading.................");
	// 버퍼
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_TerrainTex", CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Buffer_CubeTex", CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	


	// 텍스쳐
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Map", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/whiterunmap3.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_WorldMap", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/Whiterun_location.bmp", TYPE_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_PlayerMarker", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/146.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestMarker", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Map/287.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Direction", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Direction/%d.png", TYPE_NORMAL, 4)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_W_Direction", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Direction/W.png", TYPE_NORMAL, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_S_Direction", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Direction/S.png", TYPE_NORMAL, 1)), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_N_Direction", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Direction/N.png", TYPE_NORMAL, 1)), E_FAIL);




	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/wrfieldHighgrass01.dds", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Terrain2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain/Terrain0.png", TYPE_NORMAL, 1)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox_Sun", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/sky_day_clear_09.dds", TYPE_CUBE, 1)), E_FAIL);  //sky_day_clear_09 //evening1 //night  //sunrise
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox_After", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/evening1.dds", TYPE_CUBE, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SkyBox_Night", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/SkyBox/night.dds", TYPE_CUBE, 1)), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_UI", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Bar.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_HPBar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/HpBar10.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_MPBar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/MpBar10.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_SPBar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/StaminatBar10.jpg", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_MonsterBar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/MonsterBar.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_MonsterHPBar", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/MonsterHp10.png", TYPE_NORMAL, 1)), E_FAIL);

	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Dialog", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/25.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_DialogBackGruond", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/13.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Window0", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/25.png", TYPE_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Window1", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Window.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Window2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/Windowrot.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Window3", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/26.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Player1", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/player.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_NPC1", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/UI/NPC.png", TYPE_NORMAL, 1)), E_FAIL);


	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Armor", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/Armor%d.png", TYPE_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_All", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/ALL%d.png", TYPE_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Weapon", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/Weapon%d.png", TYPE_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Potion", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/potion%d.png", TYPE_NORMAL, 2)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Decision", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/Decision.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Equip", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/35.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_ItemWindow", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/ItemWindow.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Howmany", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/howmany.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_HowmanyKey", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/28.png", TYPE_NORMAL, 1)), E_FAIL);

	


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_E", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/e.png.dds", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_R", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/r.png.dds", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_F", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/f.png.dds", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_C", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/c.png.dds", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_ITEMFont", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/Item.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Tab", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/tab.png.dds", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Enter", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/enter.png.dds", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestWindow", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Quest/577.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestHorizon", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Quest/12.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestVertical", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Quest/2.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestTitle", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Quest/1.png", TYPE_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_QuestObjective", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Quest/435.png", TYPE_NORMAL, 1)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_CraftWindow", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/58.png", TYPE_NORMAL, 1)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Blood", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/blood%d.png", TYPE_NORMAL, 4)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Breath", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/Breath/1/flamesmed01-crop%d.png", TYPE_NORMAL, 16)), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Breath2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/Breath/fxfire/fxfirecolumn%d.png", TYPE_NORMAL, 16)), E_FAIL);

	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Ground2", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/ground/FireB&d.png", TYPE_NORMAL, 21)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Ground", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/ground2/AnyConv.com__GroundFireC%d.png", TYPE_NORMAL, 21)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Hellfire", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/effect/Breath/fxfire/fxfirecolumn%d.png", TYPE_NORMAL, 16)), E_FAIL);



	//아이템 텍스처//////////////////////////////////////////////////////////////////////////////////////////////
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_ITEM", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Item/%d.png", TYPE_NORMAL, 8)), E_FAIL);
	

	

	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_DialogBackGruond", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/shapes/13.png", TYPE_NORMAL, 1)), E_FAIL);




	
	//burger%d
	//skyrimcloudslower00
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Texture_Effect", CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Explosion/Explosion%d.png", TYPE_NORMAL, 90)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Dragon", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Dragon/", L"Ohdaving.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Death", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Dragon/", L"Death.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player



	// 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Skyheaventemple", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Skyheaven/", L"SkyheavenTemple.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Stone", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/TombStone/", L"TombStone.X")), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Trap1000", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Trap/", L"Trap1000.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Trap800", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Trap/", L"Trap800.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Trap600", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Trap/", L"Trap600.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Trap400", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Trap/", L"Trap400.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Trap200", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Trap/", L"Trap200.X")), E_FAIL);


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_leveron", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/level/", L"pullon.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_leverdown", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/level/", L"pulldown.X")), E_FAIL);



	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Tree", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Tree/", L"Tree01.X")), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Stone",CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/bridge/", L"DragonBridge.X")), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Navi", CNaviMesh::Create(m_pGraphicDev,L"../../MFCApplication1/Data/NaviTest43.dat", L"../../MFCApplication1/Data/WP.dat")), E_FAIL);
//
//#pragma region PLAYER
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Sword", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Sword/", L"elvengreatSword.X")), E_FAIL); 
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_PlayerNude", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Dobakin/", L"PlayerNude.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_PlayerFull", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Dobakin/", L"Player2.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player

//#pragma endregion PLAYER
		
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_imperialSword", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Sword/", L"imperialsword.X")), E_FAIL);
	
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_GuardMan", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/", L"imperial.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_GuardMan2", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/", L"imperial2.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_BlackSmith", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/Female/", L"blacksmithFemale_idle.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_BarkeeperFemale", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/Female/", L"barkeeperFemale.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Market", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/Female/", L"marketFemale.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Graymane", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/", L"mt_smithing.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player

	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Yarl", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Yarl/", L"mt_sofrright.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player


	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Tulius", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/NPC/", L"mt_sprintforward.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player


	//Armor 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideArmor", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Armor/", L"HideArmor1.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideBoots", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Armor/", L"HideBoots.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideGauntlet", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Armor/", L"HideGauntlet.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideHelmet", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Armor/", L"IronHelmet.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player


	//대장간 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_BlacksmithBench", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Blacksmith/", L"BlackSmithBench.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_BlacksmithWheel", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Blacksmith/", L"BlackSmithWheel.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player




	//Armor Static 메쉬
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideArmorgnd", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Armor/", L"HideArmorgnd.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideBootsgnd", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Armor/", L"HideBootsgnd.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideGauntletgnd", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Armor/", L"HideGauntletgnd.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_HideHelmetgnd", CStaticMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/StaticMesh/Armor/", L"IronHelmetgnd.X")), E_FAIL); //Dobakin/   Male.x   PlayerXfile    Player





	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Mesh_Draugr", CDynamicMesh::Create(m_pGraphicDev, L"../Bin/Resource/Mesh/DynamicMesh/Giant/", L"Giant1.X")), E_FAIL)
	// 기타 등등
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Calculator", CCalculator::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Proto(L"Proto_Optimization", COptimization::Create(m_pGraphicDev, true, VTXCNTX, VTXCNTZ)), E_FAIL);



	//화이트런 맵 메쉬!
	if (FAILED(Ready_Proto(L"WhiterunMainload", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"WhiterunMainload.X"))))
return E_FAIL;//


	if (FAILED(Ready_Proto(L"wrmainroadplains", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadplains.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrarcadiabase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrarcadiabase01.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrbridgestone01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridgestone01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrbanneredmarebase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbanneredmarebase01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrblacksmithbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrblacksmithbase.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrbreezebase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbreezebase.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrcarlottaplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcarlottaplatform01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcommonhouseplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcommonhouseplatform01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlestairs01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestairs01.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrcompanionbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcompanionbase01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrdoormaingate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdoormaingate01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrdrunkenhuntsmanbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdrunkenhuntsmanbase.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrdrygoodsbase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrdrygoodsbase.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrgreathouseplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreathouseplatform01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrgreathouseplatform02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreathouseplatform02.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrgreymanebase", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrgreymanebase.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrguardhouse01arch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardhouse01arch01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrhallofdeadplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhallofdeadplatform01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrmainroadmarket", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadmarket.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrmainroadplains02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmainroadplains02.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondstairs01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondstairs01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondstairs02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondstairs02.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondwallcor01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallcor01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondwallstr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondwallstr02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr02.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrpondwallstr03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrpondwallstr03.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrshacksbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrshacksbase01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrshacksbase02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrshacksbase02.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrstairsplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrstairsplatform01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrulfberhplatform01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrulfberhplatform01.X"))))
return E_FAIL;


	//화이트런 wall
	if (FAILED(Ready_Proto(L"wrwallcap01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcap01.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrwallcap01nohighcol", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcap01nohighcol.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallcurve30up128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve30up128.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallcurve30up128a", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve30up128a.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallcurve45", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve45.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallcurve45up128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve45up128.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallcurve60", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallcurve60.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallmaingate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallmaingate01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallmaingate01ext", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallmaingate01ext.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstr01nohighcoll", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01nohighcoll.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrrubbled01free", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled01free.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrrubbled01short", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled01short.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrrubbled02free", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrrubbled02free.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrup128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrup128shift128", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128shift128.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstrup128shift128nohighcol", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstrup128shift128nohighcol.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltierdivide01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltierdivide01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltotem01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltotem01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltower01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltowercap01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowercap01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallwatergrate01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallwatergrate01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrstairswater01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrstairswater01.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrwalltowerfree01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerfree01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltower01collpiece", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltower01collpiece.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrguardhouse01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardhouse01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrbridge02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridge02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrbridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbridge01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtreecircle01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtreecircle01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrterwind01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrterwind01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtercloud01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud01.X"))))
return E_FAIL;
	if (FAILED(Ready_Proto(L"wrtercloud02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtercloud03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtercloud03.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"sovmeadhall01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/Sovengarde/", L"sovmeadhall01.X"))))
return E_FAIL;



	if (FAILED(Ready_Proto(L"wrcastlewoodtower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlewoodtower01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlestonetowertop01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetowertop01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlestonetower01porchint", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01porchint.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlestonetower01porch", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01porch.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlestonetower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlestonetower01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlemainbuilding01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlemainbuilding01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastleentrance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastleentrance01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrcastlebridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrcastlebridge01.X"))))
return E_FAIL;





	if (FAILED(Ready_Proto(L"wrskyforge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrskyforge01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrskyforge01door01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrskyforge01door01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstr01stockades01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades01.X"))))
return E_FAIL;



	if (FAILED(Ready_Proto(L"wrwallstr01stockades02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallstr01stockades03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallstr01stockades03.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrguardtowerentrance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrguardtowerentrance01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallruinsfree01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallruinsfree01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwallruinsfree01entance01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwallruinsfree01entance01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltowerchunk01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerchunk01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwalltowerrubbled01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwalltowerrubbled01.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrscafguardtower01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtower01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrscafguardtowerbase01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerbase01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrscafguardtowerlarge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerlarge01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrscafguardtowerspikes01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrscafguardtowerspikes01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrwoodbridge01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrwoodbridge01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"rockcliff04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"rockcliff04.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtempleofk01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtempleofk01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhallofthedead01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhallofthedead01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"rockcliff07", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"rockcliff07.X"))))
return E_FAIL;





	if (FAILED(Ready_Proto(L"wrbreezehome01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrbreezehome01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhouse02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouse02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousebanneredmare01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousebanneredmare01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhouseblacksmith01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseblacksmith01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousegreymane01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousegreymane01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousemeadery01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousemeadery01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousemeaderywarehouse01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousemeaderywarehouse01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhouseshack01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseshack01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhouseshack02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhouseshack02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousestables01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestables01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousestores01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestores01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousestores02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousestores02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousewind01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousewind02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousewind03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind03.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrhousewind04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrhousewind04.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrjorvaskr01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrjorvaskr01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrmarketstand01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrmarketstand02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand02.X"))))
return E_FAIL;


	if (FAILED(Ready_Proto(L"wrmarketstand03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrmarketstand03.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtrellisarch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrellisarch01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtrelliscolumn01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrelliscolumn01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrtrelliscomplete01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrtrelliscomplete01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountainpeak01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountainpeak01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountainpeak02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountainpeak02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountaincliff01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff01.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountaincliff02", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff02.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountaincliff03", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff03.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountaincliff04", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliff04.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"mountaincliffslope", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"mountaincliffslope.X"))))
return E_FAIL;

	if (FAILED(Ready_Proto(L"wrjorvaskr01backporch01", CStaticMesh::Create(m_pGraphicDev, L"../Resource/Mesh/StaticMesh/Whiterun/wrterrain/", L"wrjorvaskr01backporch01.X"))))
return E_FAIL;










	m_bFinish = true;

	lstrcpy(m_szLoading, L"Loading Complete");


	return 0;
}

Engine::_uint CLoading::LoadingForBoss(void)
{
	// 수업상 정의한 코드

	return 0;
}

unsigned int CALLBACK CLoading::Thread_Main(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	_uint	iFlag = 0;

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADING_STAGE:
		iFlag = pLoading->LoadingForStage();
		break;

	case LOADING_BOSS:
		iFlag = pLoading->LoadingForBoss();
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	// _endthreadex(0);		리턴값이 0인 경우엔 _endthreadex함수가 자동 호출되어 문제가 없지만 그렇지 않은 경우에는 예외처리로 이 함수를 호출할 수 있도록 설계해야 한다.
	
	return iFlag;
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{

	CLoading*	pInstance = new CLoading(pGraphicDev);

	if (FAILED(pInstance->Ready_Loading(eID)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading::Free(void)
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_Crt);

	Safe_Release(m_pGraphicDev);

}

