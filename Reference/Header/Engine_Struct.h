#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagObjectInfo
	{
		_vec3		vPos;
		_vec3		vScale;
		_vec3		vRot;
		_ulong		Object_ID;
		_ulong		Object_type;

		tagObjectInfo(){}

		tagObjectInfo(_vec3 Pos, _vec3 Scale, _vec3 Rot, _ulong	pObject_type, _ulong		pObject_ID)
		{
			vPos = Pos;
			vScale = Scale;
			vRot = Rot;
			Object_ID = pObject_ID;
			Object_type = pObject_type;
		}

	}ObjectInfo;


	typedef struct _tagCellInfo
	{
		_vec3 vPoint[3];
		int CellIndex = 0;

		_tagCellInfo() {};

		_tagCellInfo(_vec3 A, _vec3 B, _vec3 C, int index)
		{
			vPoint[0] = A;
			vPoint[1] = B;
			vPoint[2] = C;
			CellIndex = index;

		};

	}CellInfo;



	typedef struct _tagItemInfo
	{
		char	Item_Name[50];

		_ulong Item_Code = 0;
		_ulong	Item_Type = 0;
		_ulong	Item_Weight = 0;
		_ulong	Item_Value = 0;
		_ulong	Item_Stat = 0;



		_tagItemInfo() {};

		_tagItemInfo(int iItem_Code, int iItem_Type, int iItem_Weight, int iItem_Value, int iItem_Stat)
		{

			//strcpy_s(Item_Name, name);
			Item_Code = iItem_Code;
			Item_Type = iItem_Type;
			Item_Weight = iItem_Weight;
			Item_Value = iItem_Value;
			Item_Stat = iItem_Stat;

		};

	}ItemInfo;



	typedef struct _tagQuestInfo
	{
		char NPC_Type[50];

		int	NPC_Code = 0;
		int Quest_ID = 0;
		int Quest_State = 0;
		char Quest_Name[100];


		_tagQuestInfo() {};

		_tagQuestInfo(char A[50], int code, int ID, int state, char Name[50])
		{
			strcpy_s(NPC_Type, A);
			NPC_Code = code;
			Quest_ID = ID;
			Quest_State = state;
			strcpy_s(Quest_Name, Name);

		};

	}QuestInfo;


	struct Attribute
	{
		_vec3 vPos;
		_vec3	velocity;
		_vec3	acc;
		float	lifetime;
		float	age;
		D3DXCOLOR color;
		D3DXCOLOR colorFade;

		bool	isAlive;

	};



	typedef struct tagParticle
	{

		_vec3	vPos;
		D3DCOLOR	color;
		float		size;
		


	}Particle;

	// DWORD FVF_PTC = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_PSIZE;


	 struct BoundingBox
	 {
	 public:
		 BoundingBox();
	 public:
		 bool isPointInside(D3DXVECTOR3& p);

		 D3DXVECTOR3 _min;
		 D3DXVECTOR3 _max;
	 };

	 struct BoundingSphere
	 {
		 BoundingSphere();

		 D3DXVECTOR3 _center;
		 float       _radius;
	 };










	typedef	struct tagVertexColor
	{
		_vec3		vPos;			
		_ulong		dwColor;		

	}VTXCOL;

	const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef	struct tagVertexTexture
	{
		_vec3		vPos;
		_vec3		vNormal;
		_vec2		vTexUV;	

	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		_vec3			vPos;
		_vec3			vTexUV;

	}VTXCUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // FLOAT형 3개의 크기만큼의 텍스쳐 UV를 의미하는 FVF


	typedef struct tagIndex16
	{
		_ushort		_0;
		_ushort		_1;
		_ushort		_2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong		_0;
		_ulong		_1;
		_ulong		_2;

	}INDEX32;


	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix			CombinedTransformMatrix;
			
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture;
		
		LPD3DXMESH				pOriMesh;			// 최초 x파일을 로드했을 때 지닌 변하지 않는 원본 정점 정보를 보관하기 위한 컴객체

		_ulong					dwNumBones;			// 뼈의 개수(밑의 행렬들을 연산하여 최종적으로 출력하기 위한 상태 행렬을 만들기 위해 필요)

		_matrix*				pFrameOffsetMatrix;	// 뼈가 갖고 있는 애니메이션 정보를 사용하기 위한 행렬
		_matrix**				ppFrameCombinedTransformMatrix; // CombinedTransformMatrix들의 주소값을 보관(순회하며 위의 행렬과 곱하기를 하기 위함)하기 위한 행렬 포인터
		
		_matrix*				pRenderingMatrix;		// 최종적으로 모든 연산이 종료된 상태행렬
														// pRenderingMatrix = pFrameOffsetMatrix * (*ppFrameCombinedTransformMatrix)

	}D3DXMESHCONTAINER_DERIVED;

}



#endif // Engine_Struct_h__
