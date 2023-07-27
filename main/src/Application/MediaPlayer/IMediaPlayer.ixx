export module IMediaPlayer;
export import System;
export import Drawing;
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
		IMediaPlayer() noexcept = default;
		IMediaPlayer(const IMediaPlayer&) noexcept = delete;
		IMediaPlayer(IMediaPlayer&&) noexcept = delete;
		virtual ~IMediaPlayer() noexcept = default;
	public:
		/// <summary>
		/// ���f�B�A�t�@�C����ǂݍ��݁A�L���b�V������
		/// </summary>
		/// <param name="name">���f�B�A�t�@�C���ɑΉ�����ŗL��</param>
		/// <param name="type">
		/// �L���b�V���̎g�p�͈́B
		/// Global�̏ꍇ�A���ׂĂ�IMediaPlayer���猟���\�ł���A
		/// Local�̏ꍇ�ALoad�֐����Ăяo����IMediaPlayer����̂݌����\�ƂȂ�
		/// </param>
		/// <param name="filePath">���f�B�A�t�@�C���̐�΃p�X�A�������͎��s�t�@�C������̑��΃p�X</param>
		/// <param name="setCurrent">
		/// true�̂Ƃ��A���ݍĐ����̃��f�B�A�Ƃ��Đݒ肷��
		/// </param>
		/// <returns>
		/// ���f�B�A�t�@�C���̓ǂݍ��݂Ɏ��s�����A
		/// �������͌ŗL���������L���b�V���͈͂Ɋ��ɓo�^����Ă���ꍇ�Afalse
		/// </returns>
		virtual bool Load(const String& name, MediaPlayerSourceType type, const String& filePath, bool setCurrent) noexcept = 0;
		/// <summary>
		/// ���ݐݒ蒆�̃��f�B�A���Đ�����
		/// </summary>
		/// <param name="pos">�Đ��J�n�ʒu�̊�_</param>
		/// <param name="nsOffset">��_����̃I�t�Z�b�g(ns)</param>
		virtual bool Play(MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// <summary>
		/// ���f�B�A���Đ�����
		/// </summary>
		/// <param name="name">���f�B�A�t�@�C���ɑΉ�����ŗL��</param>
		/// <param name="type">�L���b�V���͈�</param>
		/// <param name="pos">�Đ��J�n�ʒu�̊�_</param>
		/// <param name="nsOffset">��_����̃I�t�Z�b�g(ns)</param>
		virtual bool Play(const String& name, MediaPlayerSourceType type, MediaPlayerSeekPos pos, int64_t nsOffset) noexcept = 0;
		/// <summary>
		/// ���f�B�A���ꎞ��~����
		/// </summary>
		virtual bool Pause() noexcept = 0;
		/// <summary>
		/// ���f�B�A���~����
		/// </summary>
		virtual bool Stop() noexcept = 0;
		/// <summary>
		/// ���f�B�A�v���C���[���Đ��������ׂ�
		/// </summary>
		virtual bool IsPlaying() noexcept = 0;
		/// <summary>
		/// ���f�B�A�v���C���[���ꎞ��~�������ׂ�
		/// </summary>
		virtual bool IsPaused() noexcept = 0;
		/// <summary>
		/// ���f�B�A�v���C���[�̍Đ����I�����Ă��邩���ׂ�
		/// </summary>
		virtual bool IsEnded() noexcept = 0;
		/// <summary>
		/// ���C�����[�v�ŌĂяo���čĐ��������s��
		/// </summary>
		virtual void Update() noexcept = 0;
		/// <summary>
		/// ���f�B�A�v���C���[�̃��[�v�Đ��@�\��ݒ肷��
		/// </summary>
		/// <param name="loop"></param>
		virtual void SetLoopMode(bool loop) noexcept = 0;
		/// <summary>
		/// �`���̃E�B���h�E���w�肷��B
		/// �w�肳��Ă��Ȃ��Ă������E����̍Đ��͉\�łȂ��Ă͂Ȃ�Ȃ�
		/// </summary>
		/// <param name="window">�`���̃E�B���h�E�Bnullptr���w��ł���</param>
		virtual void SetWindow(IWindow* window) noexcept = 0;
		/// <summary>
		/// ���ݍĐ����̃t���[���摜���擾����B
		/// ����Đ����ȊO�͖����ȉ摜��Ԃ�
		/// </summary>
		virtual Image GetCurrentFrame() noexcept = 0;
	public:
		IMediaPlayer& operator=(const IMediaPlayer&) noexcept = delete;
		IMediaPlayer& operator=(IMediaPlayer&&) noexcept = delete;
	};
}