#include "pch.h"
#include "Scene1.h"
#include "Export_Function.h"
#include "DynamicCamera.h"

#include "CStatic_Object.h"
#include "Monster_draugr.h"
#include "Stone.h"


Scene1::Scene1(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)

{
}

Scene1::~Scene1(void)
{
}

HRESULT Scene1::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(CScene::Ready_Scene(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Environment_Layer(L"Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_GameLogic_Layer(L"GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_UI_Layer(L"UI"), E_FAIL);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	return S_OK;
}

_int Scene1::Update_Scene(const _float & fTimeDelta)
{
	return CScene::Update_Scene(fTimeDelta);





	if(m_vecStatic_Object.size()== m_vecObjectCount)
	{ }

	



}

void Scene1::Render_Scene(void)
{
}

_int Scene1::Late_Update_Scene(_int Objecttype, int ObjectID)
{
	CLayer*		pLayer = CLayer::Create();
	CGameObject*			pGameObject = nullptr;
	

	CStdioFile tip_file;

	CString Proto_name, str, Xname;

	int indexorder = 0;


	if (Objecttype == 0) //static
	{
		pLayer = this->Get_Layer(L"Environment");



		if (tip_file.Open(L"WhiterunIndex.txt", CFile::modeRead | CFile::typeText))
		{

			while (tip_file.ReadString(str))
			{
				Proto_name += str;

				if (indexorder == ObjectID)
				{
					pGameObject = CStatic_Object::Create(m_pGraphicDev, Proto_name);
					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
					Proto_name.Empty();
					str.Empty();
					break;
				}
				Proto_name.Empty();
				str.Empty();
				indexorder++;
			}

			tip_file.Close();
		}
	}

	//if (Objecttype == 0) //static
	//{
	//	pLayer = this->Get_Layer(L"Environment");
	//
	//	
	//
	//
	//	switch (ObjectID)
	//	{
	//	case Engine::BRIDGE:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stone");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::House0:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_House0");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::House1:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_House1");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::House2:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_House2");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock0:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock0");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock1:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock1");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock2:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock2");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock3:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock3");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock4:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock4");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Rock5:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Rock5");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Staris0:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stairs0");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Staris1:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stairs1");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Staris2:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stairs2");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Staris3:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stairs3");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Staris4:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Stairs4");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Tree0:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Tree0");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Tree1:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Tree1");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::Tree2:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_Tree2");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//
	//	case Engine::Whiterun:
	//		pGameObject = CStatic_Object::Create(m_pGraphicDev, L"Proto_Mesh_WhitrunMainload");
	//		NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//		FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	//		break;
	//	case Engine::STATIC_OBJECT_ID_END:
	//		break;
	//	default:
	//		break;
	//	}
	//
	//}
	else //dynamic
	{
		pLayer = this->Get_Layer(L"GameLogic");

		switch (ObjectID)
		{
			//case PLAYER:
			//	pGameObject = CPlayer::Create(m_pGraphicDev);
			//	NULL_CHECK_RETURN(pGameObject, E_FAIL);
			//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_PLAYER, pGameObject), E_FAIL);
			//	//m_mapLayer.emplace(L"GameLogic", pLayer);

			break;

		case OBJECT_DRAUGR:
			pGameObject = CMonster_draugr::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_DRAUGR, pGameObject), E_FAIL);

			break;
		}

	}




	


	return _int();
}

_int Scene1::Load_Update_Scene(ObjectInfo*  pInfo)
{

	CLayer*		pLayer = CLayer::Create();
	CGameObject*			pGameObject = nullptr;


	CStdioFile tip_file;

	CString Proto_name, str, Xname;

	

	int indexorder = 0;


	if (pInfo->Object_type == 0) //static
	{
		pLayer = this->Get_Layer(L"Environment");



		if (tip_file.Open(L"WhiterunIndex.txt", CFile::modeRead | CFile::typeText))
		{

			while (tip_file.ReadString(str))
			{
				Proto_name += str;

				if (indexorder == pInfo->Object_ID)
				{
					pGameObject = CStatic_Object::Create(m_pGraphicDev, Proto_name);
					dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Pos(pInfo->vPos.x, pInfo->vPos.y, pInfo->vPos.z);
					dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Set_Scale(pInfo->vScale.x, pInfo->vScale.y, pInfo->vScale.z);
					dynamic_cast<CStatic_Object*>(pGameObject)->Get_Transform()->Rotation(ROTATION::ROT_Y,pInfo->vRot.y);

					NULL_CHECK_RETURN(pGameObject, E_FAIL);
					FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
					Proto_name.Empty();
					str.Empty();
					break;
				}
				Proto_name.Empty();
				str.Empty();
				indexorder++;
			}

			tip_file.Close();
		}
	}
	count++;

	if (count == 99)
	{
		int a = 0;
	}
	if (count == 100)
	{
		int b = 0;
	}

	return _int();
}

HRESULT Scene1::Ready_Environment_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT Scene1::Ready_GameLogic_Layer(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*			pGameObject = nullptr;

	
	// DynamicCamera 
	pGameObject = CDynamicCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f),
		D3DXToRadian(60.f), (_float)WINCX / (_float)WINCY, 0.1f, 1000.f);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_CAMERA, pGameObject), E_FAIL);

	// Terrain
	pGameObject = MFC_Terrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_TERRAIN, pGameObject), E_FAIL);


	// Stone
	//pGameObject = CStone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(OBJECT_BRIDGE, pGameObject), E_FAIL);
	// Player
	//pGameObject = CPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	//pGameObject = CMonster_draugr::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"CMonster_draugr", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT Scene1::Ready_UI_Layer(const _tchar * pLayerTag)
{
	return S_OK;
}

Scene1 * Scene1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Scene1*	pInstance = new Scene1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void Scene1::Free(void)
{
	CScene::Free();

}
