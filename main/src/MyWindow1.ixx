export module MyWindow1;
import DefaultWindow;

import SplashScreenScene;

using namespace System;
using namespace System::Application;
using namespace Engine;

export class MyWindow1 : public DefaultWindow {
public:
	MyWindow1() noexcept : DefaultWindow(u"MyWindow1", StartScene<SplashScreenScene>()){}
};
