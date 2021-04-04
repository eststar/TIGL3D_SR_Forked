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

//����� ����ü. �ӽ�. ���ĺ��� ����
typedef struct tagPathInformation
{
	const _tchar*		pPath = L"";
	const _tchar*		pObjectKey = L"";
	const _tchar*		pStateKey = L"";
	_int			iCount;
}PATHINFO;

typedef struct tagTerrainInformation
{
	_vec3	vPos;					//��ġ�� ���� ��ü Ÿ���� �߽���ǥ
	_vec3	vSize;				//������ �ѻ簢���� �̷�� ���������� ���ݹ���
	_vec3	vRotation;			//������ ���� �������� ȸ�� Degree����
	_int		iBufferType;		//����Ÿ�� ť������ �������?
	_int		iDrawID;			//�ؽ�ó �̸� ������. ex)����0, ����1, ����iDrawID
	DWORD dwVtxCNX;		//���� ��������
	DWORD dwVtxCNZ;		//���� ��������
	DWORD dwVtxItv;			//����������
	DWORD dwIndex;			//���° ��������
	DWORD dwParentIndex;	//Astar�뵵?
	BYTE	byOption;			//�ɼ������

}TERRAININFO;


END
#endif // Engine_Struct_h__
