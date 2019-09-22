#include "CEntity.h"

void CEntity::CreateRectangle(const XMFLOAT2& RectangleSize)
{
	if (m_vObject2Ds.size()) return;

	m_vObject2Ds.emplace_back(m_Device, m_DeviceContext);

	float half_width{ RectangleSize.x / 2 };
	float half_height{ RectangleSize.y / 2 };

	vector<SVertex2D> vertices
	{
		SVertex2D(XMFLOAT2(-half_width, +half_height), XMFLOAT2(0.0f, 0.0f)),
		SVertex2D(XMFLOAT2(+half_width, +half_height), XMFLOAT2(1.0f, 0.0f)),
		SVertex2D(XMFLOAT2(-half_width, -half_height), XMFLOAT2(0.0f, 1.0f)),
		SVertex2D(XMFLOAT2(+half_width, -half_height), XMFLOAT2(1.0f, 1.0f)),
	};

	vector<SIndex> indices
	{
		SIndex(0, 1, 2),
		SIndex(1, 3, 2),
	};

	m_vObject2Ds.back().Create(vertices, indices);
}

void CEntity::AddTexture(const string& TextureFileName)
{
	m_vTextures.emplace_back(m_Device, m_DeviceContext);

	m_vTextures.back().CreateTexture(TextureFileName);
}

CTexture* CEntity::GetTexture(size_t Index)
{
	return &m_vTextures[Index];
}

void CEntity::Draw()
{
	if (m_vTextures.size())
	{
		m_vTextures.back().Use();
	}
	
	if (m_vObject2Ds.size())
	{
		m_vObject2Ds.back().Draw();
	}
}