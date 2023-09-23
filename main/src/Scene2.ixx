export module Scene2;
import Engine;

import MediaPlayer;
import Resource1;
import Scene1;

using namespace System;
using namespace System::Application;
using namespace Engine;

export class Scene2 : public Scene {
	using Scene::Scene;
public:
	Timer timer;
	MediaPlayer m_player;
	void Init() noexcept override {
		m_player.Load(u"Waterfall", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Video::Waterfall), false);
	}
	void OnActivate() noexcept override {
		timer.Start();
		m_player.Play();
		this->EndActivate();
	}
	void OnUpdate() noexcept override {
		m_player.Update();
		timer.Update();
	}
	void OnLastUpdate() noexcept override {
		if (timer.RealTimeSinceStartup<seconds>() > 5 || m_player.IsEnded()) {
			SceneManager::GetManager().LoadScene<Scene1>(u"Scene1");
			SceneManager::GetManager().ActivateScene(u"Scene1", 0);
			SceneManager::GetManager().DeactivateScene(this->GetName());
		}
	}
	void OnDeactivate() noexcept override {
		if (SceneManager::GetManager().IsActive(u"Scene1")) {
			m_player.Stop();
			this->EndDeactivate();
		}
	}
};
