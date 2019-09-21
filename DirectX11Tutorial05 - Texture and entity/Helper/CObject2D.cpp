#include "CObject2D.h"

void CObject2D::Create(const vector<SVertex2D>& vVertices, const vector<SIndex>& vIndices)
{
	assert(!m_VertexBuffer);
	assert(!m_IndexBuffer);

	m_Vertices = vVertices;
	m_Indices = vIndices;

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(SVertex2D) * m_Vertices.size());
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Vertices[0];

		assert(SUCCEEDED(m_Device->CreateBuffer(&buffer_desc, &subresource_data, &m_VertexBuffer)));
	}

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(SIndex) * m_Indices.size());
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = &m_Indices[0];

		assert(SUCCEEDED(m_Device->CreateBuffer(&buffer_desc, &subresource_data, &m_IndexBuffer)));
	}
}

void CObject2D::Draw()
{
	m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &m_VertexBufferStride, &m_VertexBufferOffset);
	m_DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_DeviceContext->DrawIndexed(static_cast<UINT>(m_Indices.size() * 3), 0, 0);
}