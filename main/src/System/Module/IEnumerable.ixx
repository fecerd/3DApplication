module;
#include<crtdbg.h>
#undef SIZE_MAX
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
export module IEnumerable;
import Exception;
import Function;
import Traits;
import Coroutine;
import Iterators;

//promise_type
export namespace System {
	template<class T, class G>
	struct promise_type {
		T* value;
		auto get_return_object() { return G(coroutine_handle<promise_type<T, G>>::from_promise(*this)); }
		auto initial_suspend() { return suspend_always(); }
		auto final_suspend() noexcept { return suspend_always(); }
		void unhandled_exception() { static_promise::terminate(); }
		void return_void() {}
		auto yield_value(T& value) {
			this->value = &value;
			return suspend_always();
		}
	};

	template<class T, class G>
	struct promise_type<T&, G> {
		T* value;
		auto get_return_object() { return G(coroutine_handle<promise_type<T&, G>>::from_promise(*this)); }
		auto initial_suspend() { return suspend_always(); }
		auto final_suspend() noexcept { return suspend_always(); }
		void unhandled_exception() { static_promise::terminate(); }
		void return_void() {}
		auto yield_value(T& value) {
			this->value = &value;
			return suspend_always();
		}
	};
}

//IEnumeratorBase
export namespace System {
	template<class T> struct EnumerableIterator;

	/// <summary>
	/// �eEnumerator�̊��^�B
	/// �������z�֐�begin, operator++, operator*���I�[�o�[���C�h����K�v������
	/// </summary>
	/// <typeparam name="T">�v�f�^</typeparam>
	template<class T>
	struct IEnumeratorBase {
	protected:
		bool m_done = false;	//coroutine_handle<promise_type<T, IEnumerator<T>>>::done()�̌��ʂ��L�^����
	public:
		constexpr IEnumeratorBase() noexcept = default;
		constexpr IEnumeratorBase(IEnumeratorBase&& arg) noexcept : m_done(arg.m_done) { arg.m_done = true; }
		constexpr IEnumeratorBase(bool done) noexcept : m_done(done) {}
		virtual constexpr ~IEnumeratorBase() noexcept { m_done = true; }
	public:
		constexpr IEnumeratorBase(const IEnumeratorBase&) noexcept = delete;
		constexpr IEnumeratorBase& operator=(const IEnumeratorBase&) noexcept = delete;
	public:
		/// <summary>
		/// Enumerator�̍ŏ��̗v�f���w���C�e���[�^���擾����
		/// </summary>
		/// <remarks>
		/// [�������̒���]
		/// �ێ����Ă���R���[�`�����I����Ԃ̂Ƃ��A�����j�����A�V���ɃR���[�`���𐶐�����B
		/// ���������ꂽ�R���[�`�����C���N�������g���āA�ŏ��̗v�f�Œ�~�����Ȃ���΂Ȃ�Ȃ�
		/// </remarks>
		virtual constexpr EnumerableIterator<T> begin() noexcept = 0;
		/// <summary>
		/// �R���[�`���̏I����Ԃ�\���C�e���[�^���擾����
		/// </summary>
		constexpr EnumerableIterator<T> end() const noexcept { return EnumerableIterator<T>(); }
		/// <summary>
		/// �R���[�`�����I�����Ă��邩�𒲂ׂ�
		/// </summary>
		constexpr bool Done() const noexcept { return m_done; }
	public:
		/// <summary>
		/// �R���[�`����i�߂�
		/// </summary>
		/// <remarks>
		/// [�������̒���]
		/// <remarks>
		virtual constexpr IEnumeratorBase<T>& operator++() noexcept = 0;
		/// <summary>
		/// ���ݎw���Ă���v�f��Ԃ�
		/// </summary>
		virtual constexpr T& operator*() noexcept = 0;
	public:
		/// <summary>
		/// �R���[�`���𕡐����A�V����IEnumerator�ւ̃|�C���^��Ԃ��B
		/// �m�ۂ��ꂽ��������IEnumerable�̃f�X�g���N�^�ŁACommonEnumerator::m_prev��
		/// �ċA�I�ɒH���ĉ�������
		/// </summary>
		virtual constexpr IEnumeratorBase<T>* Clone() const noexcept = 0;
		/// <summary>
		/// �R���[�`�����t���ɂ���
		/// </summary>
		virtual constexpr void Reverse() noexcept = 0;
	};
}

//IEnumerator
export namespace System {
	template<class T> class ICollection;

	/// <summary>
	/// �R���[�`���֐��̖߂�l�^
	/// </summary>
	/// <typeparam name="T">�R���[�`�����Ԃ��v�f�^</typeparam>
	template<class T>
	struct IEnumerator : public IEnumeratorBase<T> {
		using promise_type = System::promise_type<T, IEnumerator<T>>;	//�R���[�`���֐��̈����^�ɕK�{�ȃG�C���A�X
	protected:
		//ICollection�̗v�f�^
		using object_type = System::Traits::remove_cv_t<T>;
		//����IEnumerator�𐶐�����ICollection�^
		using collection_type = System::Traits::conditional_t<System::Traits::is_const_v<T>, const ICollection<object_type>, ICollection<object_type>>;
		//GetEnumerator()�֐�����������R���[�`���n���h���^
		using handle = coroutine_handle<System::promise_type<T, IEnumerator<T>>>;
	protected:
		//�R���[�`���n���h��
		handle m_handle = handle();
		//�t�������w�肷��bool�l�������ɂƂ�AIEnumerator�𐶐�����֐�
		//[this(�R���[�`���𐶐�����C���X�^���X)](bool reverse) { return reverse ? GetReverseEnumerator() : GetEnumerator(); }
		//�̂悤�ȃ����_����ێ����A�Đ����Ɣ��]���\�ɂ���
		Function<IEnumerator<T>(bool)> m_enumFunc;
		//���݂̃R���[�`�����t�����̂Ƃ��Atrue
		bool m_reverse = false;
	public:
		//promise_type���Ŏg�p����邽�ߕK�{
		constexpr IEnumerator(handle h) noexcept : m_handle(h) {}
		constexpr IEnumerator(const Function<IEnumerator<T>(bool)>& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(getEnumFunc)
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(Function<IEnumerator<T>(bool)>&& getEnumFunc, bool reverse = false) noexcept
			: m_reverse(reverse), m_enumFunc(static_cast<Function<IEnumerator<T>(bool)>&&>(getEnumFunc))
		{
			IEnumerator<T>&& tmp = m_enumFunc(m_reverse);
			m_handle = tmp.m_handle;
			tmp.m_handle = handle();
			tmp.m_done = true;
		}
		constexpr IEnumerator(IEnumerator<T>&& arg) noexcept
			: m_handle(arg.m_handle), m_reverse(arg.m_reverse), m_enumFunc(static_cast<Function<IEnumerator<T>(bool)>&&>(arg.m_enumFunc))
		{
			arg.m_handle = handle();
			arg.m_done = true;
		}
		constexpr ~IEnumerator() noexcept {
			if (m_handle) m_handle.destroy();
			m_handle = handle();
		}
	public:
		/// <summary>
		/// �ŏ��̗v�f���w���C�e���[�^���擾����
		/// </summary>
		constexpr EnumerableIterator<T> begin() noexcept override {
			//���̃R���[�`�������łɎ��s����Ă���(�I�����܂�)�Ƃ��A�V���ɐ�������
			if (m_handle) *this = IEnumerator<T>(m_enumFunc, m_reverse);
			operator++();	//m_handle.promise().value�ɍŏ��̗v�f������
			return EnumerableIterator<T>(this);
		}
	public:
		constexpr IEnumerator<T>& operator++() noexcept override {
			m_handle.resume();
			this->m_done = m_handle.done();
			return *this;
		}
		constexpr T& operator*() noexcept override { return *m_handle.promise().value; }
	public:
		constexpr IEnumerator<T>* Clone() const noexcept override {
			return new IEnumerator<T>(m_enumFunc, m_reverse);
		}
		constexpr void Reverse() noexcept override {
			*this = IEnumerator<T>(m_enumFunc, !m_reverse);
		}
	private:
		//begin(), Reverse()���ł̂ݎg�p���邽��private
		constexpr IEnumerator<T>& operator=(IEnumerator<T>&& rhs) noexcept {
			if (this == &rhs) return *this;
			if (m_handle) m_handle.destroy();
			this->m_done = rhs.m_done;
			m_handle = rhs.m_handle;
			m_enumFunc = static_cast<System::Function<IEnumerator<T>(bool)>&&>(rhs.m_enumFunc);
			m_reverse = rhs.m_reverse;
			rhs.m_done = true;
			rhs.m_handle = handle();
			return *this;
		}
	};
	template<class T>
	IEnumerator(IEnumerator<T>&&, ICollection<T>*)->IEnumerator<T>;
}

//CommonEnumerator
export namespace System {
	template<class Dst, class Src>
	struct CommonEnumerator : public IEnumeratorBase<Dst> {
		IEnumeratorBase<Src>* m_prev = nullptr;	//���O��Enumerator�ւ̃|�C���^
	public:
		constexpr CommonEnumerator(IEnumeratorBase<Src>* prev) noexcept : m_prev(prev) {}
		virtual constexpr ~CommonEnumerator() noexcept {
			delete m_prev;
			m_prev = nullptr;
		}
	public:
		CommonEnumerator(const CommonEnumerator&) noexcept = delete;
		CommonEnumerator(CommonEnumerator<Dst, Src>&& arg) noexcept = delete;
		CommonEnumerator& operator=(const CommonEnumerator&) noexcept = delete;
		CommonEnumerator& operator=(CommonEnumerator&&) noexcept = delete;
	public:
		constexpr void Reverse() noexcept override { m_prev->Reverse(); }
	};
}

//Enumerators
export namespace System {
	/// <summary>
	/// Select�֐�����������Enumerator
	/// </summary>
	/// <typeparam name="Dst">�ϊ���̗v�f�^</typeparam>
	/// <typeparam name="Src">�ϊ��O�̗v�f�^</typeparam>
	template<class Dst, class Src>
	struct SelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//this->GetEnumerator()����������R���[�`��������Enumerator�ւ̃|�C���^
		System::Function<Dst(Src&)> m_func;	//�ϊ��p�֐�
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr SelectEnumerator(IEnumeratorBase<Src>* prev, const System::Function<Dst(Src&)>& func) noexcept : CommonEnumerator<Dst, Src>(prev), m_func(func) {
			m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
		}
		constexpr ~SelectEnumerator() noexcept {
			delete m_converter;
			m_converter = nullptr;
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			//�ϊ��Ɏg�p����R���[�`�����I�����Ă���ꍇ�A�V���ɐ�������
			if (m_converter->Done()) {
				delete m_converter;
				m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
			}
			m_converter->begin();	//m_conveter����m_prev���g�p����邽�߁Am_prev->begin()�̌�ɌĂяo��
			return EnumerableIterator<Dst>(m_converter);
		}
	public:
		constexpr SelectEnumerator<Dst, Src>& operator++() noexcept override {
			m_converter->operator++();
			this->m_done = m_converter->Done();
			return *this;
		}
		constexpr Dst& operator*() noexcept override { return m_converter->operator*(); }
	public:
		constexpr SelectEnumerator<Dst, Src>* Clone() const noexcept override { return new SelectEnumerator<Dst, Src>(m_prev->Clone(), m_func); }
	private:
		/// <summary>
		/// ���O��Enumerator�̒l��ϊ��p�֐��ŕϊ����A�񋓂���
		/// </summary>
		IEnumerator<Dst> GetEnumerator() noexcept {
			while (m_prev && !m_prev->Done()) {
				Dst tmp = m_func(m_prev->operator*());
				co_yield tmp;
				m_prev->operator++();
			}
		}
	};

	/// <summary>
	/// MemberSelect�֐�����������Enumerator
	/// </summary>
	/// <typeparam name="Dst">�����o�ϐ��̌^�B�C�e���[�^��Dst&��Ԃ�</typeparam>
	/// <typeparam name="Src">���O��Enumerator�̌^�B�����o�ϐ�������</typeparam>
	template<class Dst, class Src>
	struct MemberSelectEnumerator : public CommonEnumerator<Dst, Src> {
		IEnumerator<Dst>* m_converter = nullptr;	//this->GetEnumerator()����������R���[�`��������Enumerator�ւ̃|�C���^
		Dst Src::* m_member = nullptr;	//�����o�ϐ��|�C���^
		using CommonEnumerator<Dst, Src>::m_prev;
	public:
		constexpr MemberSelectEnumerator(IEnumeratorBase<Src>* prev, Dst Src::* member) noexcept : CommonEnumerator<Dst, Src>(prev), m_member(member) {
			m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
		}
		constexpr ~MemberSelectEnumerator() noexcept {
			delete m_converter;
			m_converter = nullptr;
		}
	public:
		constexpr EnumerableIterator<Dst> begin() noexcept override {
			if (!m_prev || !m_converter) return EnumerableIterator<Dst>();
			m_prev->begin();
			//�ϊ��Ɏg�p����R���[�`�����I�����Ă���ꍇ�A�V���ɐ�������
			if (m_converter->Done()) {
				delete m_converter;
				m_converter = new IEnumerator<Dst>([this](bool) { return this->GetEnumerator(); }, false);
			}
			m_converter->begin();	//m_conveter����m_prev���g�p����邽�߁Am_prev->begin()�̌�ɌĂяo��
			return EnumerableIterator<Dst>(m_converter);
		}
	public:
		constexpr MemberSelectEnumerator<Dst, Src>& operator++() noexcept override {
			m_converter->operator++();
			this->m_done = m_converter->Done();
			return *this;
		}
		constexpr Dst& operator*() noexcept override { return m_converter->operator*(); }
	public:
		constexpr MemberSelectEnumerator<Dst, Src>* Clone() const noexcept override { return new MemberSelectEnumerator<Dst, Src>(m_prev->Clone(), m_member); }
	public:
		/// <summary>
		/// ���O��Enumerator�̒l�̎������o�ϐ�(�Q��)��񋓂���
		/// </summary>
		IEnumerator<Dst> GetEnumerator() noexcept {
			while (m_prev && !m_prev->Done()) {
				co_yield m_prev->operator*().*m_member;
				m_prev->operator++();
			}
		}
	};

	/// <summary>
	/// Where�֐�����������Enumerator
	/// </summary>
	/// <typeparam name="T">�v�f�^</typeparam>
	template<class T>
	struct WhereEnumerator : public CommonEnumerator<T, T> {
		System::Function<bool(T const&)> m_func;	//������
		using CommonEnumerator<T, T>::m_prev;
	public:
		constexpr WhereEnumerator(IEnumeratorBase<T>* prev, const System::Function<bool(T const&)>& func) noexcept : CommonEnumerator<T, T>(prev), m_func(func) {}
	public:
		constexpr EnumerableIterator<T> begin() noexcept override {
			if (!this->m_prev) return EnumerableIterator<T>();
			m_prev->begin();
			this->m_done = m_prev->Done();
			while (!this->m_done && !m_func(m_prev->operator*())) {
				m_prev->operator++();
				this->m_done = m_prev->Done();
			}
			return EnumerableIterator<T>(this);
		}
	public:
		constexpr WhereEnumerator<T>& operator++() noexcept override {
			do {
				m_prev->operator++();
				this->m_done = m_prev->Done();
			} while (!this->m_done && !m_func(m_prev->operator*()));
			return *this;
		}
		constexpr T& operator*() noexcept override { return m_prev->operator*(); }
	public:
		constexpr WhereEnumerator<T>* Clone() const noexcept override { return new WhereEnumerator<T>(m_prev->Clone(), m_func); }
	};
}

//EnumerableIterator
export namespace System {
	/// <summary>
	/// IEnumerable���Ԃ��C�e���[�^�^
	/// </summary>
	/// <typeparam name="T">�v�f�^�Boperator*�ŎQ�Ƃ��擾�\</typeparam>
	template<class T>
	struct EnumerableIterator {
		using value_type = T;
		using iterator_concept = System::Traits::forward_iterator_tag;
	protected:
		IEnumeratorBase<T>* m_enumerator = nullptr;	//T�^�v�f��񋓂���Enumerator�ւ̃|�C���^
	public:
		EnumerableIterator() noexcept = default;
		EnumerableIterator(IEnumeratorBase<T>* enumerator) noexcept : m_enumerator(enumerator) {}
		~EnumerableIterator() noexcept { m_enumerator = nullptr; }
	protected:
		/// <summary>
		/// �C�e���[�^���Ō�̗v�f���z���Ă��邩���ׂ�
		/// </summary>
		constexpr bool Done() const noexcept { return m_enumerator ? m_enumerator->Done() : true; }
	public:
		constexpr EnumerableIterator<T>& operator++() noexcept {
			if (m_enumerator) m_enumerator->operator++();
			return *this;
		}
		constexpr T& operator*() const {
			if (!m_enumerator) throw System::InvalidOperationException(__FUNCSIG__, "�C�e���[�^��IEnumerator�ւ̃|�C���^�������Ă��܂���B", __FILE__, __LINE__);
			return m_enumerator->operator*();
		}
		constexpr bool operator==(const EnumerableIterator<T>& rhs) const noexcept { return this->Done() == rhs.Done(); }
		constexpr EnumerableIterator<T>& operator=(const EnumerableIterator&) noexcept = default;
	};
}

//IEnumerable
export namespace System {
	/// <summary>
	/// ICollection�̎��e���\�b�h�̖߂�l�^�B
	/// ���\�b�h�`�F�[���ɂ��ALinq���C�N�̋@�\��񋟂���B
	/// begin, end�֐����g�p���āA�C�e���[�^���擾�ł���
	/// </summary>
	/// <typeparam name="T">�v�f�^</typeparam>
	template<class T>
	class IEnumerable {
		IEnumeratorBase<T>* m_last = nullptr;	//���\�b�h�`�F�[���̍Ō��Enumerator�ւ̃|�C���^
	public:
		IEnumerable(IEnumerable<T>&& arg) noexcept : m_last(arg.m_last) { arg.m_last = nullptr; }
		IEnumerable(IEnumeratorBase<T>* enumerator) noexcept : m_last(enumerator) {}
		~IEnumerable() noexcept { delete m_last; m_last = nullptr; }
	public:
		/// <summary>
		/// �ŏ��̗v�f���w���C�e���[�^���擾����
		/// </summary>
		EnumerableIterator<T> begin() const noexcept { return m_last ? m_last->begin() : EnumerableIterator<T>(); }
		/// <summary>
		/// �I����\���C�e���[�^���擾����
		/// </summary>
		EnumerableIterator<T> end() const noexcept { return m_last ? m_last->end() : EnumerableIterator<T>(); }
	private:
		template<class Dst>
		IEnumerable<Dst> To(IEnumeratorBase<Dst>* ptr) noexcept {
			m_last = nullptr;
			return IEnumerable<Dst>(ptr);
		}
	public:
		/// <summary>
		/// �ŏ��̗v�f���擾����
		/// </summary>
		/// <exception>
		/// �v�f�����݂��Ȃ��ꍇ�ASystem::InvalidOperationException�𑗏o����
		/// </exception>
		T& First() const {
			EnumerableIterator<T> ite = begin(), e = end();
			if (!(ite != e)) throw System::InvalidOperationException("IEnumerable::First(): �v�f�����݂��܂���B");
			T& ret = *ite;
			for (; ite != e; ++ite);
			return ret;
		}
		/// <summary>
		/// �ŏ��̗v�f�ւ̃|�C���^���擾����
		/// </summary>
		/// <returns>
		/// �v�f�����݂��Ȃ��ꍇ�Anullptr��Ԃ�
		/// <returns>
		System::Traits::remove_cvref_t<T>* TryFirst() const noexcept {
			EnumerableIterator<T> ite = begin(), e = end();
			if (!(ite != e)) return nullptr;
			T& ret = *ite;
			for (; ite != e; ++ite);
			return &ret;
		}
		System::Traits::remove_cvref_t<T>* TryIndex(size_t index) const noexcept {
			EnumerableIterator<T> ite = begin(), e = end();
			for (size_t i = 0; i < index && ite != e; ++i, ++ite);
			if (!(ite != e)) return nullptr;
			T& ret = *ite;
			for (; ite != e; ++ite);
			return &ret;
		}
		/// <summary>
		/// �v�f�����擾����
		/// </summary>
		size_t EnumeratorCount() const noexcept {
			size_t ret = 0;
			for (auto b = begin(), e = end(); b != e; ++b, ++ret);
			return ret;
		}
	public:
		IEnumerable<T> Clone() const noexcept { return IEnumerable<T>(m_last->Clone()); }
	public:
		/// <summary>
		/// ���݂Ɠ���IEnumerable���擾����
		/// </summary>
		IEnumerable<T> Chain() && noexcept { return To(m_last); }
		IEnumerable<T> Chain() & noexcept { return IEnumerable<T>(m_last->Clone()); }
		/// <summary>
		/// �w�肵���֐����g�p���āA�ʂ̗v�f�^������IEnumerable���擾����
		/// </summary>
		/// <typeparam name="Dst">�ϊ���̌^</typeparam>
		/// <param name="func">����IEnumerable�����v�f�^�ւ̎Q�Ƃ������Ƃ��A�ϊ���̌^��Ԃ��֐��I�u�W�F�N�g</param>
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) && noexcept {
			return To(new SelectEnumerator<Dst, T>(m_last, func));
		}
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) & noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T>(m_last->Clone(), func));
		}
		/// <summary>
		/// �������𖞂����v�f������IEnumerable���擾����
		/// </summary>
		/// <param name="func">����IEnumerable�̗v�f�^�ւ̎Q�Ƃ������Ƃ��Abool�^��Ԃ��֐��I�u�W�F�N�g</param>
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) && noexcept {
			return To(new WhereEnumerator<T>(m_last, func));
		}
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) & noexcept {
			return IEnumerable<T>(new WhereEnumerator<T>(m_last->Clone(), func));
		}
		/// <summary>
		/// �w�肵�������o�ϐ��|�C���^���g�p���āA�����o�ϐ��ւ̎Q�ƌ^������IEnumerable���擾����
		/// </summary>
		/// <typeparam name="Dst">�����o�ϐ��^</typeparam>
		/// <typeparam name="M">�����o�ϐ��|�C���^�^</typeparam>
		/// <param name="member">�����o�ϐ��|�C���^</param>
		template<class Dst, class R>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<R, T> member) && noexcept {
			return To(new MemberSelectEnumerator<Dst, T>(m_last, member));
		}
		template<class Dst, class R>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<R, T> member) & noexcept {
			return IEnumerable<Dst>(new MemberSelectEnumerator<Dst, T>(m_last->Clone(), member));
		}
		/// <summary>
		/// �v�f�̏����𔽓]����IEnumerable���擾����
		/// </summary>
		IEnumerable<T> Reverse() && noexcept {
			m_last->Reverse();
			return To(m_last);
		}
		IEnumerable<T> Reverse() & noexcept {
			IEnumeratorBase<T>* tmp = m_last->Clone();
			tmp->Reverse();
			return IEnumerable<T>(tmp);
		}
	};
}

//ICollection
export namespace System {
	/// <summary>
	/// Linq���C�N�̋@�\��񋟂������R���e�i�N���X�ɉ��z�p��������N���X�B
	/// GetEnumerator�֐���GetReverseEnumerator�֐����I�[�o�[���C�h�K�{�ł���(const/��const����)
	/// </summary>
	/// <typeparam name="T">�v�f�^</typeparam>
	template<class T>
	class ICollection {
	public:
		constexpr ICollection() noexcept = default;
	public:
		/// <summary>
		/// ���̃R���e�i�����v�f��񋓂���
		/// </summary>
		virtual IEnumerator<T> GetEnumerator() noexcept = 0;
		/// <summary>
		/// ���̃R���e�i�����v�f��񋓂���(const��)
		/// </summary>
		virtual IEnumerator<T const> GetEnumerator() const noexcept = 0;
		/// <summary>
		/// ���̃R���e�i�����v�f���t���ŗ񋓂���
		/// </summary>
		virtual IEnumerator<T> GetReverseEnumerator() noexcept = 0;
		/// <summary>
		/// ���̃R���e�i�����v�f���t���ŗ񋓂���(const��)
		/// </summary>
		virtual IEnumerator<T const> GetReverseEnumerator() const noexcept = 0;
	private:
		IEnumerator<T>* GetPtr(bool reverse = false) noexcept {
			return new IEnumerator<T>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, reverse);
		}
		IEnumerator<T const>* GetPtr(bool reverse = false) const noexcept {
			return new IEnumerator<T const>([this](bool r) { return r ? this->GetReverseEnumerator() : this->GetEnumerator(); }, reverse);
		}
	public:
		/// <summary>
		/// ���̃R���e�i�����v�f�����H�����񋓂���IEnumerable���擾����
		/// </summary>
		IEnumerable<T> Chain() noexcept { return IEnumerable<T>(GetPtr()); }
		IEnumerable<T const> Chain() const noexcept { return IEnumerable<T const>(GetPtr()); }
		/// <summary>
		/// �w�肵���֐��̈����ɂ��̃R���e�i�����v�f��K�p���A�߂�l��񋓂���IEnumerable���擾����B
		/// �v�f�^�̎������o�ϐ��ւ̎Q�Ƃ�񋓂���ꍇ�́A���̊֐��̑����MemberSelect()�֐����g�p����
		/// </summary>
		/// <typeparam name="Dst">�߂�l�̌^</typeparam>
		/// <param name="func">�v�f�^�̎Q�Ƃ������ɂƂ�A�C�ӂ̌^��Ԃ��ϊ��֐�</param>
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T&)>& func) noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T>(GetPtr(), func));
		}
		template<class Dst>
		IEnumerable<Dst> Select(const System::Function<Dst(T const&)>& func) const noexcept {
			return IEnumerable<Dst>(new SelectEnumerator<Dst, T const>(GetPtr(), func));
		}
		/// <summary>
		/// ���̃R���e�i�����v�f�̒�����w�肵���֐���true��Ԃ����̂�񋓂���IEnumerable���擾����
		/// </summary>
		/// <param name="func">�v�f�^�ւ�const�Q�Ƃ������ɂƂ�Abool�^��Ԃ������֐�</param>
		IEnumerable<T> Where(const System::Function<bool(T const&)>& func) noexcept { return IEnumerable<T>(new WhereEnumerator<T>(GetPtr(), func)); }
		IEnumerable<T const> Where(const System::Function<bool(T const&)>& func) const noexcept { return IEnumerable<T const>(new WhereEnumerator<T const>(GetPtr(), func)); }
		/// <summary>
		/// ���̃R���e�i�����v�f�̎w�肵�������o�ϐ���񋓂���IEnumerable���擾����B
		/// ���̊֐��ɂ���Ď擾����IEnumerable�̃C�e���[�^�͎w�肵�������o�ϐ��ւ̎Q�Ƃ�\��
		/// </summary>
		/// <typeparam name="Dst">�����o�ϐ��^</typeparam>
		/// <param name="member">�v�f�^���������o�ϐ��|�C���^</param>
		template<class Dst>
		IEnumerable<Dst> MemberSelect(System::Traits::MemberPtr<Dst, T> member) noexcept requires(System::Traits::is_class_v<T> || System::Traits::is_union_v<T>) {
			return IEnumerable<Dst>(new MemberSelectEnumerator<Dst, T>(GetPtr(), member));
		}
		template<class Dst>
		IEnumerable<const Dst> MemberSelect(System::Traits::MemberPtr<Dst, T> member) const noexcept requires(System::Traits::is_class_v<T> || System::Traits::is_union_v<T>) {
			return IEnumerable<const Dst>(new SelectEnumerator<const Dst, T const>(GetPtr(), member));
		}
		/// <summary>
		/// ���̃R���e�i�����v�f���t���ɗ񋓂���IEnumerable���擾����
		/// </summary>
		IEnumerable<T> Reverse() noexcept { return IEnumerable<T>(GetPtr(true)); }
		IEnumerable<T const> Reverse() const noexcept { return IEnumerable<T const>(GetPtr(true)); }
	};
}