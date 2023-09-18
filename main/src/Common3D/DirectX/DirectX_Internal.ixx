export module DirectX_Internal;
import System;
import Application;	//IWindow
import Common3DInterface;
import DirectXHelper;
import DirectXResource;
import ManagedObject;
import DXHeader;
using namespace System::Drawing;
using namespace System::Application::Common3D;
using namespace DX;

#undef IID_PPV_ARGS
#define IID_PPV_ARGS(ppType) DX::GetIID(ppType), DX::IID_PPV_ARGS_Helper(ppType)
