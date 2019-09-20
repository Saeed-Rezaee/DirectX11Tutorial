#pragma once

#include "CommonHeader.h"

struct SVertex2D
{
	SVertex2D() {};
	SVertex2D(const XMFLOAT2& _Position) : Position{ _Position } {};

	XMFLOAT2 Position{};
};

class CObject2D
{
public:
	CObject2D(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext) : m_Device{ Device }, m_DeviceContext{ DeviceContext }  {};
	~CObject2D() {};

	void Create(const vector<SVertex2D>& vVertices);

	void Draw();

private:
	ID3D11Device*			m_Device{};
	ID3D11DeviceContext*	m_DeviceContext{};

	ComPtr<ID3D11Buffer>	m_VertexBuffer{};

	UINT					m_VertexBufferStride{ sizeof(SVertex2D) };
	UINT					m_VertexBufferOffset{};

	vector<SVertex2D>		m_Vertices{};
};
