﻿export module NamespaceHelper;
//g++で入れ子になった名前空間名の解決に失敗するエラーを防ぐためにインポートする。
//定義ユニット(.cpp)で必要になりやすい

export namespace System {}
export namespace System::Traits {}
export namespace System::Traits::Concepts {}
export namespace System::Boost {}
export namespace System::Application {}
export namespace System::Application::Windows {}
export namespace System::Application::Windows::Forms {}
export namespace System::Application::Windows::DirectX {}

export namespace System::Application::SDL {}
export namespace System::Application::Common3D {}
export namespace WinAPI {}

export namespace Engine {}
export namespace Engine::Internal {}

export namespace ResourcePaths {}
export namespace ResourcePaths::Font {}
export namespace ResourcePaths::Model {}
export namespace ResourcePaths::Motion {}
export namespace ResourcePaths::Audio {}
export namespace ResourcePaths::Video {}
