export module Script1;
import System;
import Application;
import InputSystem;
import Engine;
using namespace System;
using namespace System::Application;
using namespace Engine;

export class ControllerScript : public Script {
	//スクリーンx軸方向の移動量[-1, 1]
	float horizontal = 0;
	//スクリーンz軸方向の移動量[-1, 1]
	float vertical = 0;
	//このカメラを基準に移動方向を決定する
	GameObject* camera = nullptr;
	//一度のUpdate関数で移動する最大距離の基準
	float delta = 20;
	//一時的に変化するdeltaの倍率(Bダッシュなど)
	float scale = 1;
public:
	using Script::Script;
public:
	Vector3 GetForward() const noexcept {
		if (!camera) return GetObject().GetTransform().Forward();
		else return Vector3::Scale(camera->GetTransform().Forward(), Vector3(1, 0, 1)).Normalized();
	}
	Vector3 GetRight() const noexcept {
		if (!camera) return GetObject().GetTransform().Right();
		else return camera->GetTransform().Right();
	}
public:
	InputActions ia;
	void Init() noexcept override {
		this->EnableUpdate = true;
		this->EnableFixedUpdate = false;
		this->EnableKeyDown = false;
		this->EnableKeyUp = false;
		this->EnableInput = true;

		Action end(u"End", InputValueType::Button);
		end.AddBinding().With(ControllerButtons::MMenu);
		end.Canceled += [this](const ActionContext& context) {
			for (IWindow* window : Application::GetWindows().Values()) {
				window->Close();
				break;
			}
		};
		ia.RegisterAction(static_cast<Action&&>(end));
		Action move(u"Move", InputValueType::Vector2);
		move.AddBinding().With(ControllerButtons::LStick);
		move.AddBinding(InputBindingType::Composite).With(InputSpecialBindingTarget::WASD);
		move.Performed += [this](const ActionContext& context) {
			this->horizontal = context.State.XY.x;
			this->vertical = context.State.XY.y;
		};
		move.Canceled += [this](const ActionContext& context) {
			this->horizontal = 0;
			this->vertical = 0;
		};
		ia.RegisterAction(static_cast<Action&&>(move));
		Action speed("Speed", InputValueType::Button);
		speed.AddBinding().With(ControllerButtons::RRight);
		speed.AddBinding().With(ControllerButtons::KeyP);
		speed.AddBinding().With(ControllerButtons::KeyL);
		speed.Performed += [this](const ActionContext&) {
			this->scale = 2.f;
		};
		speed.Canceled += [this](const ActionContext&) {
			this->scale = 1.f;
		};
		ia.RegisterAction(static_cast<Action&&>(speed));

		//Key Test
		Action test(u"Test", InputValueType::Vector2);
		test.AddBinding(InputBindingType::Composite).With(InputSpecialBindingTarget::WASD);
		test.AddBinding().With(ControllerButtons::LStick);
		test.AddBinding().With(ControllerButtons::LMouse);
		test.AddBinding().With(ControllerButtons::MMouse);
		test.AddBinding().With(ControllerButtons::RMouse);
		test.AddBinding().With(ControllerButtons::MouseXYZ);
		test.Started += [](const ActionContext& context) {
			System::Application::Log(System::Application::ToString(context.Binding.Button) +  u": Started");
		};
		test.Performed += [](const ActionContext& context) {
			System::Application::Log(System::Application::ToString(context.Binding.Button) + u": Performed");
			if (context.Binding.Button == ControllerButtons::LMouse) {
				System::Application::Log(context.GetCursorPosOnWindow().ToString());
			}
		};
		test.Canceled += [](const ActionContext& context) {
			System::Application::Log(System::Application::ToString(context.Binding.Button) + u": Canceled");
		};
		//ia.RegisterAction(static_cast<Action&&>(test));
	}
	void Update() noexcept override {
		const float timestep = Script::GetTimer().DeltaTime<milliseconds>() / 1000.f;
		const Vector3 velocity = (GetForward() * vertical + GetRight() * horizontal).Normalized() * delta * scale;
		Transform& playerTr = GetObject().GetTransform();
		if (!velocity.Equal(Vector3::Zero())) {
			playerTr.Position(playerTr.Position() + velocity * timestep);
			playerTr.Rotation(Quaternion::LookRotation(velocity));
		}
	}
public:
	ControllerScript& SetCamera(GameObject* camera) noexcept {
		this->camera = camera;
		return *this;
	}
	ControllerScript& SetDelta(float delta) noexcept {
		this->delta = delta;
		return *this;
	}
};

export class CameraManager : public Script {
	//このスクリプトがアタッチされているオブジェクトの姿勢。
	//LookTargetに追従する
	Transform* m_cameraParent = nullptr;
	//
	Transform* m_cameraChild = nullptr;
	Transform* m_camera = nullptr;
public:
	//カメラが追いかけるオブジェクトの姿勢への参照(位置)
	Transform* LookTarget = nullptr;
	//追いかけるオブジェクトからカメラまでの距離
	float Distance = 10.f;
	Vector3 OffsetPosition = Vector3(0, 1.f, 1.f);
	//追従スピード。大きいほど素早く追従する
	float speed = 5.f;
private:
	//追いかけるオブジェクトを中心としたカメラの回転
	Quaternion LookRotation = Quaternion::Identity();
	float currentYaw = 0;
	float currentPitch = 20;
	static constexpr float PitchMin = -20;
	static constexpr float PitchMax = 30;
private:
	InputActions ia;
public:
	using Script::Script;
public:
	void Init() noexcept override {
		this->EnableLastUpdate = true;
		this->EnableInput = true;

		m_cameraParent = &GetObject().GetTransform();
		try {
			m_cameraChild = m_cameraParent->GetChildren()[0];
			if (m_cameraChild) m_camera = m_cameraChild->GetChildren()[0];
		}
		catch (Exception&) {
			m_cameraChild = nullptr;
			m_camera = nullptr;
		}

		Action rotate("CameraRotate", InputValueType::Vector2);
		rotate.AddBinding().With(ControllerButtons::RStick);
		rotate.AddBinding().With(InputSpecialBindingTarget::Arrow);
		rotate.Performed += [this](const ActionContext& context) {
			this->SetLookRotation(context.State.XY.y * this->speed, context.State.XY.x * this->speed, false);
		};
		ia.RegisterAction(static_cast<Action&&>(rotate));
		Action dollyin("DollyIn", InputValueType::Button);
		dollyin.AddBinding().With(ControllerButtons::LUp);
		dollyin.Started += [this](const ActionContext&) {
			Distance += -1;
			if (Distance < 0) Distance = 0;
		};
		ia.RegisterAction(static_cast<Action&&>(dollyin));
		Action dollyout("DollyOut", InputValueType::Button);
		dollyout.AddBinding().With(ControllerButtons::LDown);
		dollyout.Started += [this](const ActionContext&) {
			Distance += 1;
			if (Distance > 20) Distance = 20;
		};
		ia.RegisterAction(static_cast<Action&&>(dollyout));
	}
	void LastUpdate() noexcept override {
		if (!m_cameraParent || !m_cameraChild || !m_camera || !LookTarget) return;
		m_cameraParent->Position(Vector3::Lerp(m_cameraParent->Position(), LookTarget->Position(), speed * System::Application::GetTimer().DeltaTime<milliseconds>() / 1000.f));
		m_cameraChild->LocalPosition.z = -Distance;
		m_cameraParent->LocalRotation = LookRotation;
		m_camera->LocalPosition = OffsetPosition;
	}
public:
	void SetLookRotation(float pitch, float yaw, bool absolue = true) noexcept {
		currentYaw = absolue ? yaw : currentYaw + yaw;
		while (System::Math::Abs(currentYaw) > 360.f) {
			currentYaw += currentYaw < 0 ? 360.f : -360.f;
		}
		currentPitch = absolue ? pitch : currentPitch + pitch;
		while (System::Math::Abs(currentPitch) > 360.f) {
			currentPitch += currentPitch < 0 ? 360.f : -360.f;
		}
		currentPitch = currentPitch < PitchMin ? PitchMin : currentPitch > PitchMax ? PitchMax : currentPitch;
		LookRotation = Quaternion::AngleAxis(currentYaw, Vector3::Up()) * Quaternion::AngleAxis(currentPitch, Vector3::Right());
	}
};

enum class JumpState {
	Walk,
	Jump,
	Jumping,
	Falling,
	Endless,
	EndlessCanceled,
	Stopping
};

export class ModelScript : public Script {
	bool m_walk = false;
	Vector3 m_position = Vector3();
	bool m_dash = false;
	JumpState m_jump = JumpState::Walk;
public:
	using Script::Script;
public:
	InputActions ia;
	void Init() noexcept override {
		this->EnableLastUpdate = true;
		this->EnableUpdate = true;

		Action dash(u"Dash", InputValueType::Button);
		dash.AddBinding().With(ControllerButtons::KeyP);
		dash.AddBinding().With(ControllerButtons::RRight);
		dash.Performed += [this](const ActionContext&) {
			m_dash = true;
		};
		dash.Canceled += [this](const ActionContext&) {
			m_dash = false;
		};
		ia.RegisterAction(static_cast<Action&&>(dash));
		Action jump(u"Jump", InputValueType::Button);
		jump.AddBinding().With(ControllerButtons::KeyJ);
		jump.AddBinding().With(ControllerButtons::RDown);
		jump.Started += [this](const ActionContext&) {
			if (m_jump == JumpState::Walk) m_jump = JumpState::Jump;
		};
		ia.RegisterAction(static_cast<Action&&>(jump));
		Action fall(u"Fall", InputValueType::Button);
		fall.AddBinding().With(ControllerButtons::KeyF);
		fall.AddBinding().With(ControllerButtons::RShoulder);
		fall.Started += [this](const ActionContext&) {
			m_jump = JumpState::Endless;
		};
		fall.Canceled += [this](const ActionContext&) {
			m_jump = JumpState::EndlessCanceled;
		};
		ia.RegisterAction(static_cast<Action&&>(fall));

		GameObject& gObj = GetObject();
		if (!gObj.HasComponent<Animator>()) return;
		AnimationController& controller = gObj.GetComponent<Animator>().GetAnimationController();
		controller.AddTransition(u"Empty", u"Stopping", [](GameObject&) { return true; });
		controller.AddTransition(u"Stopping", u"StopToWalk", [this](GameObject&) { return m_walk && !m_dash; });
		controller.AddTransition(u"Stopping", u"StopToDash", [this](GameObject&) { return m_walk && m_dash; });
		controller.AddTransition(u"StopToWalk", u"Walking", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"StopToDash", u"Dash", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Walking", u"WalkToDash", [this](GameObject&) { return m_walk && m_dash; });
		controller.AddTransition(u"Walking", u"WalkToStop", [this](GameObject&) { return !m_walk; });
		controller.AddTransition(u"WalkToDash", u"Dash", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"WalkToStop", u"Stopping", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Dash", u"DashToStop", [this](GameObject&) { return !m_walk; });
		controller.AddTransition(u"Dash", u"DashToWalk", [this](GameObject&) { return m_walk && !m_dash; });
		controller.AddTransition(u"DashToStop", u"Stopping", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"DashToWalk", u"Walking", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Walking", u"WalkToJump", [this](GameObject&) { return m_jump == JumpState::Jump || m_jump == JumpState::Jumping; });
		controller.AddTransition(u"WalkToJump", u"Jumping", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Jumping", u"JumpToFall", [this](GameObject&) { return m_jump != JumpState::Jumping; });
		controller.AddTransition(u"JumpToFall", u"Falling", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Falling", u"FallToWalk", [this](GameObject&) { return m_jump == JumpState::Walk; });
		controller.AddTransition(u"FallToWalk", u"Walking", [this](GameObject& gObj) { return gObj.GetComponent<Animator>().IsEnd(); });
		controller.AddTransition(u"Stopping", u"Falling", [this](GameObject&) { return m_jump == JumpState::Endless; });
		controller.AddTransition(u"Falling", u"Stopping", [this](GameObject&) { return m_jump == JumpState::Stopping; });


		//controller.AddTransition(u"Empty", u"Walk", [&walk](GameObject&) { return walk; });
		//controller.AddTransition(u"Walk", u"Dance", [&walk](GameObject&) { return !walk; });
		//controller.AddTransition(u"Dance", u"Walk", [&walk](GameObject&) { return walk; });
		m_position = gObj.GetTransform().Position();
	}
	GameObject* container = nullptr;
	Vector3 speed = Vector3(0, 20.f, 0);
	uint64_t jumpMs = 2000;
	uint64_t fallMs = 1500;
	void Update() noexcept override {
		if (!container) {
			Scene* scene = this->GetObject().GetScene();
			if (!scene) return;
			container = scene->GetObject(u"PMDContainer");
			if (!container) return;
		}
		if (m_jump == JumpState::Walk || m_jump == JumpState::Stopping) return;
		if (m_jump == JumpState::Endless) {
			container->GetTransform().LocalPosition.y -= 20.f * System::Application::GetTimer().DeltaTime<milliseconds>() / 1000.f;
			return;
		}
		else if (m_jump == JumpState::EndlessCanceled) {
			container->GetTransform().LocalPosition.y = 0;
			m_jump = JumpState::Stopping;
			return;
		}
		static uint64_t sumJumpMs = 0;
		static uint64_t sumFallMs = 0;
		if (m_jump == JumpState::Jump) {
			if (!container->HasScript<ControllerScript>()) return;
			m_jump = JumpState::Jumping;
		}
		else if (m_jump == JumpState::Jumping) {
			uint64_t deltaMs = System::Application::GetTimer().DeltaTime<milliseconds>();
			sumJumpMs += deltaMs;
			container->GetTransform().LocalPosition.y = Vector3::Lerp(Vector3::Zero(), speed, sumJumpMs / static_cast<float>(jumpMs)).y;
			if (sumJumpMs >= jumpMs) {
				sumJumpMs = 0;
				m_jump = JumpState::Falling;
			}
		}
		else if (m_jump == JumpState::Falling) {
			uint64_t deltaMs = System::Application::GetTimer().DeltaTime<milliseconds>();
			sumFallMs += deltaMs;
			container->GetTransform().LocalPosition.y = Vector3::Lerp(speed, Vector3::Zero(), sumFallMs / static_cast<float>(fallMs)).y;
			if (sumFallMs >= fallMs) {
				sumFallMs = 0;
				m_jump = JumpState::Walk;
			}
		}
	}
	void LastUpdate() noexcept override {
		static int tmp = 0;
		if (++tmp < 10) return;
		else tmp = 0;
		Vector3 currentPos = GetObject().GetTransform().Position();
		if (m_position != currentPos) m_walk = true;
		else m_walk = false;
		m_position = currentPos;
	}
};

import MediaPlayer;
import Resource1;

export class AudioScript : public Script {
public:
	using Script::Script;
public:
	System::Application::MediaPlayer m_player;
	System::Application::MediaPlayer m_se;
	InputActions ia;
	void Init() noexcept override {
		this->EnableSceneActivate = true;
		this->EnableUpdate = true;
		
		m_player.Load(u"BGM", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Audio::Lesson), true);
		m_player.SetLoopMode(true);

		Action pause(u"Pause", InputValueType::Button);
		pause.AddBinding().With(ControllerButtons::KeySpace);
		pause.Started += [this](const ActionContext& context) {
			if (!m_player.Pause()) m_player.Play(MediaPlayerSeekPos::End, seconds(-10));
		};
		ia.RegisterAction(static_cast<Action&&>(pause));

		m_se.Load(u"SE1", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Audio::Corrent1));
		m_se.Load(u"SE2", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Audio::Corrent2));
		m_se.Load(u"SE3", MediaPlayerSourceType::Local, ResourcePaths::GetPathName(ResourcePaths::Audio::Corrent3));

		Action bell(u"Bell", InputValueType::Button);
		bell.AddBinding().With(ControllerButtons::Key1);
		bell.AddBinding().With(ControllerButtons::Key2);
		bell.AddBinding().With(ControllerButtons::Key3);
		bell.Started += [this](const ActionContext& context) {
			switch (context.Binding.Button) {
			case ControllerButtons::Key1:
				m_se.Play(u"SE1", MediaPlayerSourceType::Local);
				break;
			case ControllerButtons::Key2:
				m_se.Play(u"SE2", MediaPlayerSourceType::Local);
				break;
			case ControllerButtons::Key3:
				m_se.Play(u"SE3", MediaPlayerSourceType::Local);
				break;
			default:
				break;
			}
		};
		ia.RegisterAction(static_cast<Action&&>(bell));
	}
	void SceneActivate() noexcept override {
		m_player.Play(MediaPlayerSeekPos::Begin);
	}
	void Update() noexcept override {
		m_player.Update();
		m_se.Update();
	}
};

export class MirrorScript : public Script {
public:
	using Script::Script;
public:
	Transform* tr = nullptr;
	float base = 1.f;
	float limit = 1.f;
	bool up = true;
	void Init() noexcept override {
		this->EnableUpdate = true;
		tr = this->GetObject().GetComponentPtr<Transform>();
		base = tr->LocalScale.x;
		limit = base * 5.f;
	}
	void Update() noexcept override {
		if (tr->LocalScale.x - limit >= -0.05f) up = false;
		else if (tr->LocalScale.x - base <= 0.05f) up = true;
		tr->LocalScale.x = tr->LocalScale.x * (1 - 0.01f) + (up ? limit : base) * 0.01f;
	}
};
