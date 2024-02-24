#include "RenderingLayer.h"

namespace Drizzle3D {
	void RenderingLayer::OnAttach() {
        HRESULT hr = S_OK;
        hr = D3DCompile(vertexShaderSource, strlen(vertexShaderSource), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &vertexShaderBlob, NULL);
        if (FAILED(hr)) {
            fprintf(stderr, "Failed to compile vertex shader\n");
            // Handle error
        }

        hr = D3DCompile(pixelShaderSource, strlen(pixelShaderSource), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &pixelShaderBlob, NULL);
        if (FAILED(hr)) {
            fprintf(stderr, "Failed to compile pixel shader\n");
            // Handle error
        }


        // Create shaders
        device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
        device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);

        // Create input layout
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);
        device->CreateInputLayout(layout, numElements, vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);

        // Set input layout and shaders
        deviceContext->IASetInputLayout(inputLayout);
        deviceContext->VSSetShader(vertexShader, nullptr, 0);
        deviceContext->PSSetShader(pixelShader, nullptr, 0);


        // Define vertices for the triangle
        struct Vertex {
            float position[3];
        };
        
        Vertex vertices[] = {
            { { 0.0f, 0.5f, 0.0f } },   // Top
            { { 0.5f, -0.5f, 0.0f } },  // Right
            { { -0.5f, -0.5f, 0.0f } }  // Left
        };

        // Create vertex buffer
        D3D11_BUFFER_DESC vertexBufferDesc = { 0 };
        vertexBufferDesc.ByteWidth = sizeof(vertices);
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
        vertexBufferData.pSysMem = vertices;
        ID3D11Buffer* vertexBuffer;
        device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &vertexBuffer);

        // Set vertex buffer
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void RenderingLayer::Render() {

        float clearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA format, so this is blue with full alpha
        deviceContext->ClearRenderTargetView(swapChainRenderTargetView, clearColor);

        // Set viewport
        D3D11_VIEWPORT viewport = { 0 };
        viewport.Width = 800;
        viewport.Height = 600;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        deviceContext->RSSetViewports(1, &viewport);

        // Set primitive topology
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


        deviceContext->Draw(3, 0);

        swapChain->Present(1, 0);
	}
}