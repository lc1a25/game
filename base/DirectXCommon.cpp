#include "DirectXCommon.h"

void DirectXCommon::Init(Win* win)
{
	timeBeginPeriod(1);
	InitFixFPS();
	ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(true);
	}
	this->win = win;
	InitDevice();
	ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(dev->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		D3D12_MESSAGE_ID denyIds[] = {

			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		infoQueue->PushStorageFilter(&filter);
	}
	

	viewPort = {
	  win->window_width / 2,0.0f				   ,0.0f,0.0f,
	  0.0f				   ,-win->window_height / 2,0.0f,0.0f,
	  0.0f				   ,0.0f				   ,1.0f,0.0f,
	  win->window_width / 2,win->window_height / 2 ,0.0f,1.0f
	};

	InitCommand();
	InitSwapChain();
	InitRenderTargetView();
	InitDepthBuffer();

	//// DXGIファクトリーの生成
	//result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	////// アダプターの列挙用
	////std::vector<ComPtr<IDXGIAdapter1>> adapters;//std::vector<IDXGIAdapter1*> adapters;
	////// ここに特定の名前を持つアダプターオブジェクトが入る
	////ComPtr<IDXGIAdapter1> tmpAdapter; // IDXGIAdapter1* tmpAdapter = nullptr;
	//for (int i = 0;
	//	dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
	//	i++)
	//{
	//	adapters.push_back(tmpAdapter); // 動的配列に追加する
	//}
	////追加されたアダプターオブジェクトを仕分ける
	//for (int i = 0; i < adapters.size(); i++)
	//{
	//	/*DXGI_ADAPTER_DESC1 adesc;*/
	//	adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

	//	// ソフトウェアデバイスを回避
	//	if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
	//		continue;
	//	}

	//	std::wstring strDesc = adesc.Description;   // アダプター名
	//	// Intel UHD Graphics（オンボードグラフィック）を回避
	//	if (strDesc.find(L"Intel") == std::wstring::npos)
	//	{
	//		tmpAdapter = adapters[i];   // 採用
	//		break;
	//	}
	//}

	//// 対応レベルの配列
	//D3D_FEATURE_LEVEL levels[] =
	//{
	//	D3D_FEATURE_LEVEL_12_1,
	//	D3D_FEATURE_LEVEL_12_0,
	//	D3D_FEATURE_LEVEL_11_1,
	//	D3D_FEATURE_LEVEL_11_0,
	//};

	////D3D_FEATURE_LEVEL featureLevel;

	//for (int i = 0; i < _countof(levels); i++)
	//{
	//	// 採用したアダプターでデバイスを生成
	//	result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
	//	if (result == S_OK)
	//	{
	//		// デバイスを生成できた時点でループを抜ける
	//		featureLevel = levels[i];
	//		break;
	//	}
	//}

	//// コマンドアロケータを生成
	//result = dev->CreateCommandAllocator(
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	IID_PPV_ARGS(&cmdAllocator));

	//// コマンドリストを生成
	//result = dev->CreateCommandList(0,
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	cmdAllocator.Get(), nullptr,
	//	IID_PPV_ARGS(&cmdList));

	////// 標準設定でコマンドキューを生成
	////D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	//dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	////// 各種設定をしてスワップチェーンを生成
	////DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	//swapchainDesc.Width = 1280;
	//swapchainDesc.Height = 720;
	//swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	//swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	//swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	//swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	//swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	//swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	////ComPtr<IDXGISwapChain1> swapchain1;

	//dxgiFactory->CreateSwapChainForHwnd(
	//	cmdQueue.Get(),
	//	win->GetHwnd(),
	//	&swapchainDesc,
	//	nullptr,
	//	nullptr,
	//	&swapchain1);// (IDXGISwapChain1**)
	//swapchain1.As(&swapchain);

	////// 各種設定をしてデスクリプタヒープを生成
	////D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	//heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	//heapDesc.NumDescriptors = 2;    // 裏表の２つ
	//dev->CreateDescriptorHeap(&heapDesc,
	//	IID_PPV_ARGS(&rtvHeaps));

	////// 裏表の２つ分について
	////std::vector<ComPtr<ID3D12Resource>> backBuffers(2);//std::vector<ID3D12Resource*> backBuffers(2);
	//backBuffers.resize(2);
	//for (int i = 0; i < 2; i++)
	//{
	//	// スワップチェーンからバッファを取得
	//	result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

	//	// デスクリプタヒープのハンドルを取得
	//	//CD3DX12_CPU_DESCRIPTOR_HANDLE (rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	//	//	i,
	//	//	dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
	//	//// 裏か表かでアドレスがずれる
	//	////handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);

	//	// レンダーターゲットビューの生成
	//	dev->CreateRenderTargetView(
	//		backBuffers[i].Get(),
	//		nullptr,
	//		CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	//			i,
	//			dev->GetDescriptorHandleIncrementSize(heapDesc.Type))
	//	);
	//}

	//// フェンスの生成
	//ComPtr<ID3D12Fence> fence;// ID3D12Fence* fence = nullptr;
	//UINT64 fenceVal = 0;

	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
}

void DirectXCommon::InitDevice()
{
	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;//std::vector<IDXGIAdapter1*> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter; // IDXGIAdapter1* tmpAdapter = nullptr;
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // 動的配列に追加する
	}
	//追加されたアダプターオブジェクトを仕分ける
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);  // アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
			continue;
		}

		std::wstring strDesc = adesc.Description;   // アダプター名
		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];   // 採用
			break;
		}
	}

	// 対応レベルの配列
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		// 採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
}

void DirectXCommon::InitCommand()
{
	// コマンドアロケータを生成
	result = dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));

	// コマンドリストを生成
	result = dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&cmdList));

	//// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};

	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));
}

void DirectXCommon::InitSwapChain()
{

	//// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 色情報の書式
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapchainDesc.BufferCount = 2;  // バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapchain1;

	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		win->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);// (IDXGISwapChain1**)
	swapchain1.As(&swapchain);

	
}

void DirectXCommon::InitRenderTargetView()
{
	//// 各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;    // 裏表の２つ
	dev->CreateDescriptorHeap(&heapDesc,
		IID_PPV_ARGS(&rtvHeaps));

	//// 裏表の２つ分について
	//std::vector<ComPtr<ID3D12Resource>> backBuffers(2);//std::vector<ID3D12Resource*> backBuffers(2);
	backBuffers.resize(2);
	for (int i = 0; i < 2; i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));

		// デスクリプタヒープのハンドルを取得
		//CD3DX12_CPU_DESCRIPTOR_HANDLE (rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		//	i,
		//	dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		//// 裏か表かでアドレスがずれる
		////handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);

		// レンダーターゲットビューの生成
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
				i,
				dev->GetDescriptorHandleIncrementSize(heapDesc.Type))
		);
	}

}

void DirectXCommon::InitDepthBuffer()
{

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(DXGI_FORMAT_D32_FLOAT,
		win->window_width,
		win->window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに必要
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);

	//深度ビュー用のデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは一つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	//ComPtr<ID3D12DescriptorHeap> dsvHeap;// ID3D12DescriptorHeap* dsvHeap = nullptr;
	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	//深度ビューを設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void DirectXCommon::BeginDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// １．リソースバリアで書き込み可能に変更
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// ２．描画先指定

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
		bbIndex, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


	// ３．画面クリア       R     G     B    A
	float clearColor[] = { 0.0f,0.00f, 0.8f,1.0f }; // 青っぽい色
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, win->window_width, win->window_height));

	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, win->window_width, win->window_height));
}

void DirectXCommon::EndDraw()
{
	// バックバッファの番号を取得（2つなので0番か1番）
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 命令のクローズ
	cmdList->Close();

	// コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { cmdList.Get() }; // コマンドリストの配列
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	// バッファをフリップ（裏表の入替え）
	swapchain->Present(1, 0);

	// コマンドリストの実行完了を待つ
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	UpdateFixFPS();

	cmdAllocator->Reset(); // キューをクリア
	cmdList->Reset(cmdAllocator.Get(), nullptr);  // 再びコマンドリストを貯める準備

	
}

void DirectXCommon::InitFixFPS()
{
	//
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1 / 60 1000 / 1 1000/ 1
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));

	//1 / 60Z
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	//L
	std::chrono::steady_clock::time_point now =
		std::chrono::steady_clock::now();

	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	//1/60
	if (elapsed < kMinTime)
	{
		//1/60
		while (std::chrono::steady_clock::now() - reference_ < kMinTime)
		{
			//
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	//
	reference_ = std::chrono::steady_clock::now();



}
