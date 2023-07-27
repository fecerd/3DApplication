export module Vector;
import Objects;
import IEnumerable;
import Traits;
import VectorBase;

export namespace System {
	template<class T>
	concept Streams = requires(T& x, void* data, size_t count) {
		x.Read(data, count);
	};

	template<class T>
	class Vector : public Object, public VectorBase<T>, public ICollection<T> {
	public:
		using VectorBase<T>::VectorBase;
		Vector(VectorBase<T>&& arg) noexcept : VectorBase<T>(static_cast<VectorBase<T>&&>(arg)) {}
	public:
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Vector<T>&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Vector<T>>(); }
		String ToString() const noexcept override {
			return String::Joint(GetType().ToString(), u" Object, Count = ", this->Count());
		}
		uint32_t GetTypeID() const noexcept override { return GetID<Vector<T>>(); }
	public:
		IEnumerator<T> GetEnumerator() noexcept {
			for (size_t i = 0, count = this->Count(); i < count; ++i) co_yield this->operator[](i);
		}
		IEnumerator<const T> GetEnumerator() const noexcept {
			for (size_t i = 0, count = this->Count(); i < count; ++i) co_yield this->operator[](i);
		}
		IEnumerator<T> GetReverseEnumerator() noexcept {
			for (size_t i = this->Count(); i-- > 0;) co_yield this->operator[](i);
		}
		IEnumerator<const T> GetReverseEnumerator() const noexcept {
			for (size_t i = this->Count(); i-- > 0;) co_yield this->operator[](i);
		}
	public:
		using VectorBase<T>::AddRange;
		template<Streams S>
		void AddRange(S& file, size_t count, size_t typeSize = sizeof(T)) noexcept {
			if (this->Allocate(count)) {
				if (typeSize == sizeof(T)) file.Read(this->m_data + this->m_count, typeSize * count);
				else {
					T* ptr = this->m_data + this->m_count;
					for (size_t i = 0; i < count; ++i) file.Read(ptr + i, typeSize);
				}
				this->m_count += count;
			}
		}
		template<Streams S, class Func>
		requires requires() {
			System::Traits::is_invocable_v<Func, S&, T*>;
		}
		void AddRange(S& file, size_t count, Func& f) noexcept {
			if (this->Allocate(count)) {
				T* ptr = this->m_data + this->m_count;
				for (size_t i = 0; i < count; ++i) {
					f(file, ptr);
					++ptr;
				}
				this->m_count += count;
			}
		}
	};
}

export namespace System {
	template<>
	class Vector<uint8_t> : public Object, public VectorBase<uint8_t>, ICollection<uint8_t> {
	public:
		using VectorBase<uint8_t>::AddRange;
		template<Streams S>
		void AddRange(S& file, size_t count) noexcept {
			if (this->Allocate(count)) {
				file.Read(this->m_data + this->m_count, count);
				this->m_count += count;
			}
		}
	public:
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Vector<uint8_t>&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Vector<uint8_t>>(); }
		String ToString() const noexcept override {
			return String::Joint(GetType().ToString(), String(u" Object, Count = "), String(GetCStringBlock<char16_t>(this->Count())));
		}
		uint32_t GetTypeID() const noexcept override { return GetID<Vector<uint8_t>>(); }
	public:
		IEnumerator<uint8_t> GetEnumerator() noexcept override {
			for (size_t i = 0, count = this->Count(); i < count; ++i) co_yield this->operator[](i);
		}
		IEnumerator<const uint8_t> GetEnumerator() const noexcept override {
			for (size_t i = 0, count = this->Count(); i < count; ++i) co_yield this->operator[](i);
		}
		IEnumerator<uint8_t> GetReverseEnumerator() noexcept override {
			for (size_t i = this->Count(); i-- > 0;) co_yield this->operator[](i);
		}
		IEnumerator<const uint8_t> GetReverseEnumerator() const noexcept override {
			for (size_t i = this->Count(); i-- > 0;) co_yield this->operator[](i);
		}
	};
}