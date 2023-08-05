export module Vector;
import Objects;
import Traits;
import VectorBase;
export import IEnumerable;

namespace System::Internal {
	template<class T>
	concept Streams = requires(T& x, void* data, size_t count) {
		x.Read(data, count);
	};
}
using namespace System::Internal;

export namespace System {
	template<class T>
	class Vector : public Object, public VectorBase<T>, public ICollection<T> {
	public:
		using VectorBase<T>::VectorBase;
		Vector(VectorBase<T>&& arg) noexcept
		    : VectorBase<T>(System::move(arg)) {}
	public:/* Object override */
		bool Equals(const Object& obj) const noexcept override { return obj.GetTypeID() == GetTypeID() ? *this == static_cast<const Vector<T>&>(obj) : false; }
		Type GetType() const noexcept override { return Type::CreateType<Vector<T>>(); }
		String ToString() const noexcept override {
			return String::Joint(GetType().ToString(), u" Object, Count = ", this->Count());
		}
		uint32_t GetTypeID() const noexcept override { return GetID<Vector<T>>(); }
	public:/* ICollection override */
		IEnumerator<T> GetEnumerator() noexcept override {
			for (size_t i = 0, count = this->Count(); i < count; ++i) co_yield this->operator[](i);
		}
		IEnumerator<const T> GetEnumerator() const noexcept override {
			for (size_t i = 0, count = this->Count(); i < count; ++i)
				co_yield this->operator[](i);
		}
		IEnumerator<T> GetReverseEnumerator() noexcept override {
			for (size_t i = this->Count(); i-- > 0;)
				co_yield this->operator[](i);
		}
		IEnumerator<const T> GetReverseEnumerator() const noexcept override {
			for (size_t i = this->Count(); i-- > 0;)
				co_yield this->operator[](i);
		}
	public:/* Stream用AddRange */
		using VectorBase<T>::AddRange;
		template<Streams S>
		void AddRange(S& file, size_t count, size_t typeSize = sizeof(T)) noexcept {
			if (this->Allocate(count)) {
				if constexpr (Traits::Concepts::CSame<T, uint8_t>){
					file.Read(this->m_data + this->m_count, count);
				}
				else{
					if (typeSize == sizeof(T))
						file.Read(this->m_data + this->m_count, typeSize * count);
					else {
						T* ptr = this->m_data + this->m_count;
						for (size_t i = 0; i < count; ++i)
							file.Read(ptr + i, typeSize);
					}
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
