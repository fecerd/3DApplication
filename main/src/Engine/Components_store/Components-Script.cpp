#if defined(__GNUC__) //&& !defined(__clang__)
module Components;
import :GameObject;
#else
module Components:Script;
import Components;
#endif
#if !defined(__GNUC__) || defined(__clang__)
import SceneManager;
#endif
import System;
using namespace System;
using namespace Engine;

Script* Script::Clone(GameObject* object) noexcept {
	Script* ret = m_clone(object);
	ret->m_clone = m_clone;
	ret->m_typeID = m_typeID;
	//Initを呼び出す必要がある
	return ret;
}

#if !defined(__GNUC__) || defined(__clang__)
Timer& Script::GetTimer() noexcept {
	return SceneManager::GetManager().GetTimer();
}
#endif
