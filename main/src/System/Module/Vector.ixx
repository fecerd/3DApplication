export module Vector;
export import Objects;
import Traits;
export import VectorBase;
export import ICollection;

export namespace System::Internal {
	template<class T>
	concept HasRead = requires(T& x, void* data, size_t count) {
		x.Read(data, count);
	};
}

#if defined(__GNUC__) && !defined(__clang__)

export namespace System {
	template<class T>
	struct no_mangling {
		mutable Function<void(Boost::push_type<T&>&)> m_internal;
		mutable Function<void(Boost::push_type<T&>&)> m_internal_r;
	public:
		template<class F, class Fr>
		no_mangling(F&& f, Fr&& fr) noexcept : m_internal(System::move(f)), m_internal_r(System::move(fr)) {}
		no_mangling(const no_mangling&) noexcept = default;
		no_mangling(no_mangling&&) noexcept = default;
	public:
		IEnumerator<T> operator()(bool r) const {
			return IEnumerator<T>(r
				? m_internal_r
				: m_internal
			);
		}
	};
}

#else
#endif

export namespace System {
	template<class T>
	class Vector : public VectorBase<T>, public Object, public ICollection<T> {
	public:
		using VectorBase<T>::VectorBase;
		Vector(VectorBase<T>&& arg) noexcept : VectorBase<T>(System::move(arg)) {}
		virtual ~Vector() noexcept {}
	public:/* Object override */
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Vector<T>&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Vector<T>>(); }
		String ToString() const noexcept override {
			return String::Joint(GetType().ToString(), u" Object, Count = ", this->Count());
		}
		uint32_t GetTypeID() const noexcept override { return GetID<Vector<T>>(); }
	public:/* ICollection override */
#if defined(__GNUC__) && !defined(__clang__)
		IEnumerator<T> GetEnumerator(bool reverse = false) noexcept override {
			auto internal = [this](Boost::push_type<T&>& sink) {
				for (size_t i = 0, count = this->Count(); i < count; ++i) {
					T& ret = this->operator[](i);
					sink(ret);
				}
			};
			auto internal_r = [this](Boost::push_type<T&>& sink) {
				for (size_t i = this->Count(); i-- > 0;) {
					T& ret = this->operator[](i);
					sink(ret);
				}
			};
			return IEnumerator<T>(no_mangling<T>(internal, internal_r), reverse);
		}
		IEnumerator<T const> GetEnumerator(bool reverse = false) const noexcept override {
			auto internal = [this](Boost::push_type<T const&>& sink) {
				for (size_t i = 0, count = this->Count(); i < count; ++i) {
					T const& ret = this->operator[](i);
					sink(ret);
				}
			};
			auto internal_r = [this](Boost::push_type<T const&>& sink) {
				for (size_t i = this->Count(); i-- > 0;) {
					T const& ret = this->operator[](i);
					sink(ret);
				}
			};
			return IEnumerator<T const>(no_mangling<T const>(internal, internal_r), reverse);
		}
#else
	private:
		IEnumerator<T> Internal(bool reverse) noexcept {
			if (reverse) {
				for (size_t i = this->Count(); i-- > 0;) {
					co_yield this->operator[](i);
				}
			} else {
				for (size_t i = 0, count = this->Count(); i < count; ++i) {
					co_yield this->operator[](i);
				}
			}
		}
		IEnumerator<T const> Internal(bool reverse) const noexcept {
			if (reverse) {
				for (size_t i = this->Count(); i-- > 0;) {
					co_yield this->operator[](i);
				}
			} else {
				for (size_t i = 0, count = this->Count(); i < count; ++i) {
					co_yield this->operator[](i);
				}
			}
		}
	public:
		IEnumerator<T> GetEnumerator(bool reverse = false) noexcept override {
			return IEnumerator<T>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
		IEnumerator<T const> GetEnumerator(bool reverse = false) const noexcept override {
			return IEnumerator<T const>(
				[this](bool r) { return Internal(r); },
				reverse
			);
		}
#endif
	public:/* Stream用AddRange */
		using VectorBase<T>::AddRange;
		template<Internal::HasRead S>
		bool AddRange(S& file, size_t count, size_t typeSize = sizeof(T)) noexcept {
			if (this->Allocate(count)) {
				if constexpr (Traits::Concepts::CSame<T, uint8_t>){
					file.Read(this->m_data + this->m_count, count);
				}
				else {
					if (typeSize == sizeof(T)) {
						file.Read(this->m_data + this->m_count, typeSize * count);
					}
					else if (typeSize < sizeof(T)) {
						T* ptr = this->m_data + this->m_count;
						for (size_t i = 0; i < count; ++i) file.Read(ptr + i, typeSize);
					}
					else {
						//型Tより大きい単位で読み取るべきではない
						return false;
					}
				}
				this->m_count += count;
				return true;
			}
			else return false;
		}
		template<Internal::HasRead S, class Func>
		requires requires(Func& f, S& file, T* ptr) { f(file, ptr); }
		bool AddRange(S& file, size_t count, Func& f) noexcept {
			if (this->Allocate(count)) {
				Allocator<T> alloc;
				using traits = AllocatorTraits<Allocator<T>>;
				T* ptr = this->m_data + this->m_count;
				for (size_t i = 0; i < count; ++i) {
					traits::ConstructDefault(alloc, ptr);
					f(file, ptr);
					++ptr;
				}
				this->m_count += count;
				return true;
			}
			else return false;
		}
	};
}
