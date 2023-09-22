
FAddObjectLibrary(Forms_0
	SOURCES
		Brush.ixx
	DEPENDS
		Drawing
		WinAPI
)

FAddObjectLibrary(Forms_1
	SOURCES
		Control.ixx
	DEPENDS
		Forms_0
		System
		Windows_2	#WindowClass, DeviceContext, WindowsEventArgs
)

FAddObjectLibrary(Forms_2
	SOURCES
		Button.ixx
		CheckBox.ixx
		ComboBox.ixx
		Form.ixx
		GroupBox.ixx
		Label.ixx
		ListBox.ixx
		RadioButton.ixx
		ScrollBar.ixx
		TextBox.ixx
	DEPENDS
		Forms_1
		Application_0	#Log
)

FAddObjectLibrary(Forms_3
	SOURCES
		Forms.ixx
	DEPENDS
		Forms_2
)

FAddObjectLibrary(Forms_4
	SOURCES
		Control.cpp
	DEPENDS
		Forms_3
		Windows_3
)

FCombineObjectLibrary(Forms
	PRIVATE
		Forms_0
		Forms_1
		Forms_2
		Forms_3
		Forms_4
)

target_link_libraries(Forms PUBLIC Windows)
