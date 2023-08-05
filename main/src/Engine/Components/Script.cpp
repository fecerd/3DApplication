module Components:Script;
import SceneManager;
using namespace System;
using namespace Engine;

Timer& Script::GetTimer() noexcept {
	return SceneManager::GetManager().GetTimer();
}

Script* Script::Clone(GameObject* object) noexcept {
	Script* ret = m_clone(object);
	ret->m_clone = m_clone;
	ret->m_typeID = m_typeID;
	//Initを呼び出す必要がある
	return ret;
}
