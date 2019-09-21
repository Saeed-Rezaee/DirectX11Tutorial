#pragma once

#include "../Helper/CObject2D.h"
#include "../Helper/CTexture.h"

class CEntity
{
	friend class CEntityPool;

public:
	CEntity(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext } {};
	~CEntity() {};

	void CreateRectangle(const XMFLOAT2& RectangleSize);

	void AddTexture(const string& TextureFileName);
	CTexture* GetTexture(size_t Index);

	void Draw();

public:
	XMFLOAT2				WorldPosition{};

private:
	ID3D11Device*			m_Device{};
	ID3D11DeviceContext*	m_DeviceContext{};

	vector<CObject2D>		m_vObject2Ds{};
	vector<CTexture>		m_vTextures{};

	XMMATRIX				m_MatrixWorld{ XMMatrixIdentity() };
};