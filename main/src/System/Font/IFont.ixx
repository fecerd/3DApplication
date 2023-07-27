export module IFont;
import CSTDINT;
import Objects;
import Point;
import IEnumerable;
import Drawing;

export namespace System {
	/// <summary>
	/// Font�t�@�C������ێ�����N���X���p������C���^�[�t�F�[�X
	/// </summary>
	class IFont {
	public:
		virtual ~IFont() noexcept = default;
	public:
		/// <summary>
		/// �x�[�X���C����0�Ƃ����Ƃ���(�X�P�[�����O����)�A�Z���_�[���C���̍������擾����
		/// </summary>
		/// <param name="scalePermill">�`��X�P�[��(�番��)</param>
		virtual int16_t GetAscenderLine(uint32_t scalePermill) const noexcept = 0;
		/// <summary>
		/// �w�肵���X�P�[���ŕ�������r�b�g�}�b�v�ɕ`�悵���Ƃ��̕����擾����
		/// </summary>
		/// <param name="str">������B���s�L��'\n'���܂܂��ꍇ�A��ԕ����傫���s�̕���Ԃ�</param>
		/// <param name="scalePermill">�`��X�P�[��(�番��)</param>
		/// <remarks>
		/// [������]
		/// ��������R�[�h�ʒu�ɕϊ����A�s���Ƃ�(�X�P�[�����O����)�O�i���̘a�����߁A�ő�l��Ԃ��B
		/// �X�P�[�����O�ɂ��؂�̂Ă������邽�߁A1���Z����ق����ǂ�
		/// </remarks>
		virtual uint32_t GetWidth(const String& str, uint32_t scalePermill) const noexcept = 0;
		/// <summary>
		/// �w�肵���X�P�[���Ŏw�肵���s���̕�������r�b�g�}�b�v�ɕ`�悵���Ƃ��̍������擾����
		/// </summary>
		/// <param name="scalePermill">�`��X�P�[��(�番��)</param>
		/// <param name="numLines">�`�悷��s��</param>
		/// <remarks>
		/// [������]
		/// (�X�P�[�����O����)�A�Z���_�[���C������f�B�Z���_�[���C���܂ł̍�������s�̍����Ƃ��A
		/// (�X�P�[�����O����)�e�s�̊�(�s��-1)�̍s�ԕ��𑫂����l��Ԃ��B
		/// �X�P�[�����O�ɂ��؂�̂Ă������邽�߁A1���Z����ق����ǂ�
		/// </remarks>
		virtual uint32_t GetHeight(uint32_t scalePermill, uint32_t numLines) const noexcept = 0;
		/// <summary>
		/// �R�[�h�|�C���g���w�肵�ăO���t�̉摜�f�[�^�ւ̎Q�Ƃ��擾����
		/// </summary>
		/// <param name="codePoint">�擾����O���t�̃R�[�h�|�C���g(Unicode)</param>
		/// <param name="scalePermill">�`��X�P�[��(�番��)</param>
		/// <remarks>
		/// [������]
		/// �t�H���g��񂩂�O���t��`�悵�A�C���X�^���X���ɃL���b�V������B
		/// ���̌�A�L���b�V���f�[�^�ւ̎Q�Ƃ�Ԃ�
		/// </remarks>
		/// <returns>�O���t�摜�f�[�^�ւ�const�Q��</returns>
		virtual const System::Drawing::Image& GetImage(uint32_t codePoint, uint32_t scalePermill) const noexcept = 0;
	public:
		/// <summary>
		/// �Ăяo�����тɕ`���ɕ����񂩂�ꕶ�����`�悷��R���[�`�����擾����
		/// </summary>
		/// <param name="dst">�`����Image�I�u�W�F�N�g�ւ̎Q��</param>
		/// <param name="str">�`�悷�镶����</param>
		/// <param name="origin">�`����n�߂錴�_�Bx���W�͍ŏ��̕����̍��[�Ay���W�̓x�[�X���C���ƈ�v����</param>
		/// <param name="scalePermill">�`��X�P�[��(�番��)</param>
		/// <remarks>
		/// [������]
		/// IEnumerator��Ԃ��R���[�`���֐����쐬���AIEnumerable�����������ĕԂ��B
		/// [�R���[�`���֐��������ӓ_]
		/// 1. const�Q�Ƃ͉E�Ӓl�̏ꍇ�R���[�`�����Ƀf�X�g���N�g�����\�������邽�߁A
		/// �����̓R�s�[�ɂ���ď���������(const String&�ł͂Ȃ�String�ɂ���)�B
		/// 2. �`���摜�͈̔͊O�ɂ͕`�悵�Ȃ�(�͈͊O�A�N�Z�X�ɋC������)
		/// </remarks>
		/// <returns>�`�悳��Ă��Ȃ��������c���Ă���ꍇ�Atrue�B���ׂĂ̕�����`�悵���ꍇ�Afalse</returns>
		virtual IEnumerable<bool> UpdateStringImage(System::Drawing::Image& dst, const String& str, const Point<int32_t>& origin, uint32_t scalePermill) const noexcept = 0;
	public:
		virtual void SetBaseLineColor(const System::Drawing::Color& color) noexcept = 0;
		virtual void SetContoursColor(const System::Drawing::Color& color) noexcept = 0;
	};
}