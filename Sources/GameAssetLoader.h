#ifndef __GAMEASSETLOADER
#define __GAMEASSETLOADER

#include <map>

#include <Sources/Assets/TestAsset.h>
#include <Sources/Objects/IRenderable.h>

enum AssetID {
	//TetAsset�� �� �״�� �׽�Ʈ�̱� ������ �׽�Ʈ ������ ���̵� �߰��� ��
	ID_ASSET_TESTASSET = 50
};

using TargetInterface = IRenderable;

class CGameAssetLoader {
	//����� �� �����Ƿ� 
public:
	CGameAssetLoader();
	~CGameAssetLoader();
	
	static CGameAssetLoader& GetInstance();
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int* screenWidth, int* screenHeight);
	//void Release();

	//���� �ҷ��� �� Map�� ����
	TargetInterface* LoadAsset(int id, int bitmapWidth, int bitmapHeight);
	//TargetInterface* GetAsset(std::string id);

private:
	//void ClearMap();

private:
	//ICharacter�� �´°�...
	/*std::map<std::string, TargetInterface*> m_AssetMap;*/

	ID3D11Device * m_DeviceRef;
	ID3D11DeviceContext* m_DeviceContextRef;

	int* m_ScreenWidthRef;
	int* m_ScreenHeightRef;	
};

#endif