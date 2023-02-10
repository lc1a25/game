#pragma once
#include <Windows.h>
#include <wrl.h> // 7-1
#include <d3d12.h>//7-3
#include <d3dx12.h>//7-3
#include <dxgi1_6.h>
#include "Win.h"
#include <DirectXMath.h>
#include <cassert>
#include <chrono>
#include<thread>


#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;
using namespace DirectX;
class DirectXCommon
{
public:

	HRESULT result;
	ComPtr<ID3D12Device> dev;// ID3D12Device* dev = nullptr;
	ComPtr<IDXGIFactory6> dxgiFactory;//IDXGIFactory6* dxgiFactory = nullptr;
	ComPtr<IDXGISwapChain4> swapchain;//IDXGISwapChain4* swapchain = nullptr;
	ComPtr<ID3D12CommandAllocator> cmdAllocator; //ID3D12CommandAllocator* cmdAllocator = nullptr;
	ComPtr<ID3D12GraphicsCommandList> cmdList; //ID3D12GraphicsCommandList* cmdList = nullptr;
	ComPtr<ID3D12CommandQueue> cmdQueue; // ID3D12CommandQueue* cmdQueue = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;// ID3D12DescriptorHeap* rtvHeaps = nullptr;

	// ���\�̂Q���ɂ���
	std::vector<ComPtr<ID3D12Resource>> backBuffers;//std::vector<ID3D12Resource*> backBuffers(2);

		// �t�F���X�̐���
	ComPtr<ID3D12Fence> fence;// ID3D12Fence* fence = nullptr;
	UINT64 fenceVal = 0;

	ComPtr<ID3D12Resource> depthBuffer;
	
	ComPtr<ID3D12DescriptorHeap> dsvHeap;



	void Init(Win* win);
	void InitDevice();
	void InitCommand();
	void InitSwapChain();
	void InitRenderTargetView();
	void InitDepthBuffer();

	void BeginDraw();
	void EndDraw();

	ID3D12Device* GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdlist() { return cmdList.Get(); }
	XMMATRIX GetViewPort() { return viewPort; }

	

private:
	XMMATRIX viewPort;
	Win* win = nullptr;


	//fps�Œ�
	void InitFixFPS();
	void UpdateFixFPS();

	//
	std::chrono::steady_clock::time_point reference_;
};
