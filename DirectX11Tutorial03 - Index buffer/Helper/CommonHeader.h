#pragma once

#include <windows.h>
#include <cassert>
#include <vector>
#include <memory>
#include <d3d11.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include "../../DirectXTK/pch.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using std::vector;
using std::unique_ptr;
using std::make_unique;