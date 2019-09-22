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

	m_vObject2Ds.back().CreateDynamic(vertices, indices);
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


void CEntity::SetRectangleUVRange(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip)
{
	auto texture_size{ m_vTextures.front().GetTextureSize() };
	XMFLOAT2 OffsetUV{}, SizeUV{};
	OffsetUV.x = OffsetInTexture.x / texture_size.x;
	OffsetUV.y = OffsetInTexture.y / texture_size.y;
	SizeUV.x = SizeInTexture.x / texture_size.x;
	SizeUV.y = SizeInTexture.y / texture_size.y;

	m_vObject2Ds.front().m_Vertices[3].TexCoord = m_vObject2Ds.front().m_Vertices[2].TexCoord =
		m_vObject2Ds.front().m_Vertices[1].TexCoord = m_vObject2Ds.front().m_Vertices[0].TexCoord = OffsetUV;

	m_vObject2Ds.front().m_Vertices[1].TexCoord.x += SizeUV.x;
	m_vObject2Ds.front().m_Vertices[2].TexCoord.y += SizeUV.y;
	m_vObject2Ds.front().m_Vertices[3].TexCoord.x += SizeUV.x;
	m_vObject2Ds.front().m_Vertices[3].TexCoord.y += SizeUV.y;

	if (Flip == ERenderFlipOption::Horizontal || Flip == ERenderFlipOption::HorizontalAndVertical)
	{
		std::swap(m_vObject2Ds.front().m_Vertices[0].TexCoord.x, m_vObject2Ds.front().m_Vertices[1].TexCoord.x);
		std::swap(m_vObject2Ds.front().m_Vertices[2].TexCoord.x, m_vObject2Ds.front().m_Vertices[3].TexCoord.x);
	}

	if (Flip == ERenderFlipOption::Vertical || Flip == ERenderFlipOption::HorizontalAndVertical)
	{
		std::swap(m_vObject2Ds.front().m_Vertices[0].TexCoord.y, m_vObject2Ds.front().m_Vertices[2].TexCoord.y);
		std::swap(m_vObject2Ds.front().m_Vertices[1].TexCoord.y, m_vObject2Ds.front().m_Vertices[3].TexCoord.y);
	}

	m_vObject2Ds.front().UpdateVertexBuffer();
}

void CEntity::SetRectangleUVRangeAndSize(const XMFLOAT2& OffsetInTexture, const XMFLOAT2& SizeInTexture, ERenderFlipOption Flip)
{
	m_vObject2Ds.front().m_Vertices[0].Position.x = -SizeInTexture.x / 2;
	m_vObject2Ds.front().m_Vertices[0].Position.y = +SizeInTexture.y / 2;
	m_vObject2Ds.front().m_Vertices[1].Position.x = +SizeInTexture.x / 2;
	m_vObject2Ds.front().m_Vertices[1].Position.y = +SizeInTexture.y / 2;
	m_vObject2Ds.front().m_Vertices[2].Position.x = -SizeInTexture.x / 2;
	m_vObject2Ds.front().m_Vertices[2].Position.y = -SizeInTexture.y / 2;
	m_vObject2Ds.front().m_Vertices[3].Position.x = +SizeInTexture.x / 2;
	m_vObject2Ds.front().m_Vertices[3].Position.y = -SizeInTexture.y / 2;

	SetRectangleUVRange(OffsetInTexture, SizeInTexture, Flip);
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