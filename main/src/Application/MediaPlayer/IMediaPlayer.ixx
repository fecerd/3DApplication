export module IMediaPlayer;
export import System;
export import IWindow;
using System::Drawing::Image;

export namespace System::Application {
	enum class MediaPlayerSeekPos : uint8_t {
		Begin,
		Current,
		End
	};

	enum class MediaPlayerSourceType {
		Local,
		Global
	};

	class IMediaPlayer {
	public:
		IMediaPlayer() noexcept {}
		IMediaPlayer(const IMediaPlayer&) noexcept = delete;
		IMediaPlayer(IMediaPlayer&&) noexcept = delete;
		virtual ~IMediaPlayer() noexcept {}
	public:
		/// @brief メディアファイルを読み込み、キャッシュする。
		/// @param name メディアファイルに対応する固有名
		/// @param type キャッシュの使用範囲。Globalの場合、すべてのIMediaPlayerから検索可能であり、Localの場合、Load関数を呼び出したIMediaPlayerからのみ検索可能となる。
		/// @param filePath メディアファイルの絶対パス、もしくは実行ファイルからの相対パス。
		/// @param setCurrent trueのとき、現在再生中のメディアとして設定する。
		/// @retval 読み込みに成功したとき、true。
		/// @retval 読み込みに失敗した、もしくは 固有名が同じキャッシュ範囲に既に登録されているとき、false。
		virtual bool Load(const String& name, MediaPlayerSourceType type, const String& filePath, bool setCurrent) noexcept = 0;	
		/// @brief 現在設定中のメディアを再生する。
		/// @param pos 再生開始位置の基準点
		/// @param nsOffset 基準点からのオフセット(ns)
		/// @return 再生を開始したとき、true
		virtual bool Play(MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// @brief メディアを再生する。
		/// @param name 再生するメディアファイルに対応する固有名
		/// @param type キャッシュ範囲
		/// @param pos 再生開始位置の基準点
		/// @param nsOffset 基準点からのオフセット(ns)
		/// @return 再生を開始したとき、true
		virtual bool Play(const String& name, MediaPlayerSourceType type, MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// @brief メディアを一時停止する。
		/// @return 一時停止したとき、true
		virtual bool Pause() noexcept = 0;
		/// @brief メディアを停止する。
		/// @return 停止したとき、true。
		virtual bool Stop() noexcept = 0;
		/// @brief メディアプレイヤーが再生中か調べる。
		virtual bool IsPlaying() noexcept = 0;
		/// @brief メディアプレイヤーが一時停止中か調べる。
		virtual bool IsPaused() noexcept = 0;
		/// @brief メディアプレイヤーの再生が終了しているか調べる。
		virtual bool IsEnded() noexcept = 0;
		/// @brief メインループで呼び出して再生処理を行う。
		virtual void Update() noexcept = 0;
		/// @brief メディアプレイヤーのループ再生機能を設定する
		/// @param loop ループするとき、true
		/// @details メディアではなく、メディアプレイヤー側にループが設定される。
		virtual void SetLoopMode(bool loop) noexcept = 0;
		/// @brief 映像の描画先ウィンドウを指定する。
		/// @param window 描画先のウィンドウ。nullptrを指定できる。
		/// @details 指定されていなくても音声・動画の再生は可能でなくてはならない。
		virtual void SetWindow(IWindow* window) noexcept = 0;
		/// @brief 現在再生中の映像のフレーム画像を取得する。
		/// @return 動画再生中以外は無効な画像を返す。
		virtual Image GetCurrentFrame() noexcept = 0;
	public:
		/// @brief 絶対値で音量を設定する。
		/// @param absoluteVolume 音量の絶対値。[0, 100]に丸められる。
		/// @return 設定できなかったとき、false。
		virtual bool SetVolume(int32_t absoluteVolume) noexcept = 0;
		/// @brief 現在の音量からの相対値で音量を設定する。
		/// @param relativeVolume 現在の音量からの相対値。
		/// @details 設定後の音量の絶対値が[0, 100]を超えることはない。
		/// @return 設定できなかったとき、false。
		virtual bool SetRelativeVolume(int32_t relativeVolume) noexcept = 0;
		/// @brief 現在の音量を取得する。
		/// @return [0, 100]の値を返す。失敗したとき、負の値を返す。
		virtual int32_t GetVolume() const noexcept = 0;
		/// @brief ミュート状態にする、もしくは解除する
		/// @param mute trueのとき、ミュート状態にする。falseのとき、解除する。
		/// @return 設定に成功した、もしくは既に指定した状態のとき、true。
		virtual bool SetMute(bool mute) noexcept = 0;
		/// @brief ミュート状態を切り替える
		/// @return 成功したとき、true。
		virtual bool ToggleMute() noexcept = 0;
		/// @brief 現在、ミュート状態か調べる。
		virtual bool IsMuted() const noexcept = 0;
	public:
		/// @brief 絶対値でマスター音量を設定する。
		/// @param absoluteVolume 音量の絶対値。[0, 100]に丸められる。
		/// @return 設定できなかったとき、false。
		virtual bool SetMasterVolume(int32_t absoluteVolume) noexcept = 0;
		/// @brief 現在のマスター音量からの相対値でマスター音量を設定する。
		/// @param relativeVolume 現在の音量からの相対値。
		/// @details 設定後の音量の絶対値が[0, 100]を超えることはない。
		/// @return 設定できなかったとき、false。
		virtual bool SetRelativeMasterVolume(int32_t relativeVolume) noexcept = 0;
		/// @brief 現在のマスター音量を取得する。
		/// @return [0, 100]の値を返す。失敗したとき、負の値を返す。
		virtual int32_t GetMasterVolume() const noexcept = 0;
		/// @brief マスター音量をミュート状態にする、もしくは解除する
		/// @param mute trueのとき、ミュート状態にする。falseのとき、解除する。
		/// @details この関数によってそれぞれのメディアプレイヤーのミュート状態が変わることはない。
		/// @return 設定に成功した、もしくは既に指定した状態のとき、true。
		virtual bool SetMuteMaster(bool mute) noexcept = 0;
		/// @brief マスター音量のミュート状態を切り替える
		/// @return 成功したとき、true。
		virtual bool ToggleMuteMaster() noexcept = 0;
		/// @brief 現在、マスター音量がミュート状態か調べる。
		virtual bool IsMutedMaster() const noexcept = 0;
	public:
		/// @brief 再生位置を移動する。
		/// @param nsOffset 現在の位置からのオフセット(ns)
		/// @return 成功したとき、true。
		virtual bool SeekRelative(int64_t nsOffset) noexcept = 0;
		/// @brief 再生速度を変更する。
		/// @param speed 再生速度の倍率。1.0fが通常の速度。
		/// @return 設定できなかったとき、false。
		virtual bool SetSpeed(float speed) noexcept = 0;
		/// @brief 現在の再生速度を取得する。
		/// @return 失敗したとき、負の値を返す。
		virtual float GetSpeed() const noexcept = 0;
	public:
		IMediaPlayer& operator=(const IMediaPlayer&) noexcept = delete;
		IMediaPlayer& operator=(IMediaPlayer&&) noexcept = delete;
	};
}