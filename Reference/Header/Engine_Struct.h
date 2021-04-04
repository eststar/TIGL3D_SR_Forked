#ifndef Engine_Struct_h__
#define Engine_Struct_h__

BEGIN(Engine)

typedef struct tagVertexColor
{
	_vec3		vPosition;
	_ulong		dwColor;

}VTXCOL;

const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTexture
{
	_vec3		vPosition;
	_vec2		vTexUV;

}VTXTEX;

const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

typedef struct tagVertexCube
{
	_vec3		vPosition;
	_vec3		vTexUV;

}VTXCUBE;

const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

typedef struct tagVertexTetra
{
	_vec3		vPosition;
	_ulong		dwColor;
	//_vec3		vTexUV;

}VTXTETRA;

const _ulong		FVF_TETRA = D3DFVF_XYZ | D3DFVF_DIFFUSE;





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

//저장용 구조체. 임시. 추후변경 가능
typedef struct tagPathInformation
{
	const _tchar*		pPath = L"";
	const _tchar*		pObjectKey = L"";
	const _tchar*		pStateKey = L"";
	_int			iCount;
}PATHINFO;

typedef struct tagTerrainInformation
{
	_vec3	vPos;					//설치한 지형 전체 타일의 중심좌표
	_vec3	vSize;				//지형의 한사각형을 이루는 꼭지점간의 간격배율
	_vec3	vRotation;			//각각의 축을 기준으로 회전 Degree기준
	_int		iBufferType;		//버퍼타입 큐브인지 평면인지?
	_int		iDrawID;			//텍스처 이름 구별용. ex)지형0, 지형1, 지형iDrawID
	DWORD dwVtxCNX;		//가로 정점갯수
	DWORD dwVtxCNZ;		//세로 정점갯수
	DWORD dwVtxItv;			//정점간간격
	DWORD dwIndex;			//몇번째 지형인지
	DWORD dwParentIndex;	//Astar용도?
	BYTE	byOption;			//옵션저장용

}TERRAININFO;


END
#endif // Engine_Struct_h__
