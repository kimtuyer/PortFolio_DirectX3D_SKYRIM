// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"
#include "Base.h"
//#include "stdafx.h"
//#include "../Client/Code/Define.h"
#include "define.h"
//#include "../System\Code/GraphicDev.h"
//#include "CGraphic_Device.h"
//#include "GraphicDev.h"
#include "TerrainTex.h"
#include "../Client/Code/Terrain.h"





#endif //PCH_H

#ifdef _AFX
#define ERR_MSG(msg) AfxMessageBox(msg)
#else 
#define ERR_MSG(msg) MessageBox(nullptr, msg, L"System Error", MB_OK)
#endif // _AFX
#include <afxext.h>
#include <afxext.h>
#include <afxext.h>
#include <afxwin.h>
#include <afxext.h>
#include <afxext.h>
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxext.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
