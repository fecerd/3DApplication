#if defined(__GNUC__) && !defined(__clang__)
import Scene;
import SceneManager;
#else
module Scene;
import SceneManager;
#endif
using namespace Engine;

ISceneManager& ISceneManager::GetSceneManager() noexcept {
	return SceneManager::GetManager();
}
