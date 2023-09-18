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
		/// <summary>
		/// メディアファイルを読み込み、キャッシュする
		/// </summary>
		/// <param name="name">メディアファイルに対応する固有名</param>
		/// <param name="type">
		/// キャッシュの使用範囲。
		/// Globalの場合、すべてのIMediaPlayerから検索可能であり、
		/// Localの場合、Load関数を呼び出したIMediaPlayerからのみ検索可能となる
		/// </param>
		/// <param name="filePath">メディアファイルの絶対パス、もしくは実行ファイルからの相対パス</param>
		/// <param name="setCurrent">
		/// trueのとき、現在再生中のメディアとして設定する
		/// </param>
		/// <returns>
		/// メディアファイルの読み込みに失敗した、
		/// もしくは固有名が同じキャッシュ範囲に既に登録されている場合、false
		/// </returns>
		virtual bool Load(const String& name, MediaPlayerSourceType type, const String& filePath, bool setCurrent) noexcept = 0;
		/// <summary>
		/// 現在設定中のメディアを再生する
		/// </summary>
		/// <param name="pos">再生開始位置の基準点</param>
		/// <param name="nsOffset">基準点からのオフセット(ns)</param>
		virtual bool Play(MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// <summary>
		/// メディアを再生する
		/// </summary>
		/// <param name="name">メディアファイルに対応する固有名</param>
		/// <param name="type">キャッシュ範囲</param>
		/// <param name="pos">再生開始位置の基準点</param>
		/// <param name="nsOffset">基準点からのオフセット(ns)</param>
		virtual bool Play(const String& name, MediaPlayerSourceType type, MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// <summary>
		/// メディアを一時停止する
		/// </summary>
		virtual bool Pause() noexcept = 0;
		/// <summary>
		/// メディアを停止する
		/// </summary>
		virtual bool Stop() noexcept = 0;
		/// <summary>
		/// メディアプレイヤーが再生中か調べる
		/// </summary>
		virtual bool IsPlaying() noexcept = 0;
		/// <summary>
		/// メディアプレイヤーが一時停止中か調べる
		/// </summary>
		virtual bool IsPaused() noexcept = 0;
		/// <summary>
		/// メディアプレイヤーの再生が終了しているか調べる
		/// </summary>
		virtual bool IsEnded() noexcept = 0;
		/// <summary>
		/// メインループで呼び出して再生処理を行う
		/// </summary>
		virtual void Update() noexcept = 0;
		/// <summary>
		/// メディアプレイヤーのループ再生機能を設定する
		/// </summary>
		/// <param name="loop"></param>
		virtual void SetLoopMode(bool loop) noexcept = 0;
		/// <summary>
		/// 描画先のウィンドウを指定する。
		/// 指定されていなくても音声・動画の再生は可能でなくてはならない
		/// </summary>
		/// <param name="window">描画先のウィンドウ。nullptrを指定できる</param>
		virtual void SetWindow(IWindow* window) noexcept = 0;
		/// <summary>
		/// 現在再生中のフレーム画像を取得する。
		/// 動画再生中以外は無効な画像を返す
		/// </summary>
		virtual Image GetCurrentFrame() noexcept = 0;
	public:
		IMediaPlayer& operator=(const IMediaPlayer&) noexcept = delete;
		IMediaPlayer& operator=(IMediaPlayer&&) noexcept = delete;
	};
}