#pragma once
#define DECLARE_SINGLETON(ClassName)							\
public:															\
	static ClassName* Get_Instance()							\
	{															\
		if (nullptr == m_pInstance)								\
			m_pInstance = new ClassName;						\
			return m_pInstance;									\
	}															\
	static void Destroy_Instance()								\
	{															\
		if (m_pInstance)										\
		{														\
			delete m_pInstance;									\
			m_pInstance = nullptr;								\
		}														\
	}															\
private:														\
		static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)							\
ClassName* ClassName::m_pInstance = nullptr; 


#define WINCX 800
#define WINCY 600

extern  HWND	g_hWnd;


enum e_TerrainBtn {
	TBtn_Vertex,
	TBtn_Transform,
	TBtn_Texture,
	TBtn_End
};
