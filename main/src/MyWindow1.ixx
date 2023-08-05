export module MyWindow1;
import DefaultWindow;
using namespace System;
using namespace System::Application;
using namespace Engine;

import SplashScreenScene;

export class MyWindow1 : public DefaultWindow {
public:
	MyWindow1() noexcept : DefaultWindow(u"MyWindow1", StartScene<SplashScreenScene>()){}
};