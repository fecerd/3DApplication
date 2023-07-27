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
	//Init‚ğŒÄ‚Ño‚·•K—v‚ª‚ ‚é
	return ret;
}
