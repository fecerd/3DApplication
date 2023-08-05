export module Tuple;
import CSTDINT;
import Traits;
using namespace System::Traits;

//StaticTupleLeaf
export namespace System {
	template<size_t i, class T>
	class StaticTupleLeaf {
		T value;
	public:
		StaticTupleLeaf() noexcept = delete;
		StaticTupleLeaf(T& v) noexcept : value(v) {}
		StaticTupleLeaf(T&& v) noexcept : value(v) {}
		~StaticTupleLeaf() noexcept = default;
	public:
		T& Value() noexcept { return value; }
		T Value() const noexcept { return value; }
	};
	template<size_t i, class T>
	class StaticTupleLeaf<i, T&> {
		T& value;
	public:
		StaticTupleLeaf() noexcept = delete;
		StaticTupleLeaf(T& v) noexcept : value(v) {}
		~StaticTupleLeaf() noexcept = default;
	public:
		T& Value() const noexcept {
			return value;
		}
	};
}

//StaticTupleCount
namespace System {
	template<size_t i>
	class StaticTupleCount {
	public:
		static constexpr size_t count = i + 1;
	};
}

//StaticTuple
export namespace System {
	template<size_t i, class Head, class ...Tail>
	class StaticTuple : public StaticTupleLeaf<i, Head>, public StaticTuple<i + 1, Tail...> {
	public:
		StaticTuple() noexcept = delete;
		StaticTuple(Head&& head, Tail&& ...tail) noexcept : StaticTupleLeaf<i, Head>(head), StaticTuple<i + 1, Tail...>(tail...) {}
		~StaticTuple() noexcept = default;
	public:
		template<size_t i> auto& GetValue() noexcept { return StaticTupleLeaf<i, one_of_t<i, Head, Tail...>>::Value(); }
		template<size_t i, class T>
		void SetValue(T&& v) {
			auto& val = StaticTupleLeaf<i, one_of_t<i, Head, Tail...>>::Value();
			val = v;
		}
	public:
		constexpr size_t Count() const noexcept { return this->count; }
	};
	template<size_t i, class T>
	class StaticTuple<i, T> : public StaticTupleLeaf<i, T>, public StaticTupleCount<i> {
	public:
		StaticTuple() noexcept = delete;
		StaticTuple(T& v) noexcept : StaticTupleLeaf<i, T>(v) {}
		~StaticTuple() noexcept = default;
	public:
		template<size_t i> auto& GetValue() noexcept { return static_cast<StaticTupleLeaf<i, T>&>(*this).Value(); }
	};

	template<class Head, class ...Tail> StaticTuple(Head&& head, Tail&&...)->StaticTuple<0, Head, Tail...>;
}

//ToStaticTuple
export namespace System {
	template<class Head, class ...Args> auto ToStaticTuple(Head&& head, Args&& ...args) noexcept { return StaticTuple(head, args...); }
}

//Tuple
export namespace System {
	class LeafBase {
	protected:
		void* value = nullptr;
	public:
		virtual ~LeafBase() = default;
	public:
		void* GetValue() noexcept { return value; }
	};

	template<class T>
	class Leaf : public LeafBase {
	public:
		Leaf(const T& v) noexcept {
			if constexpr (is_array_v<T>) {
				value = (void*)&v;
			}
			else
			{
				T* ptr = new T(v);
				value = ptr;
			}
		}
		Leaf(T&& v) noexcept {
			T* ptr = new T(v);
			value = ptr;
		}
		~Leaf() {
			if constexpr (!is_array_v<T>) delete static_cast<T*>(value);
			value = nullptr;
		}
	};
	template<class T>
	class Leaf<T&> : public LeafBase {
	public:
		Leaf(T&& v) noexcept {
			value = &v;
		}
		~Leaf() noexcept {
			value = nullptr;
		}
	};

	template<class T>
	struct array_to_ptr {
		using type = T;
		static constexpr size_t Length = 0;
	};
	template<class T>
	struct array_to_ptr<T[]> {
		using type = T*;
		static constexpr size_t Length = 0;
	};
	template<class T, size_t N>
	struct array_to_ptr<T[N]> {
		using type = T*;
		static constexpr size_t Length = N;
	};

	template<class Head, class ...Tail>
	class Tuple {
		LeafBase** values = nullptr;
	public:
		static constexpr size_t Count = sizeof...(Tail) + 1;
	public:
		using type1 = Head;
		using type2 = System::Traits::one_of_t<0, Tail...>;
		using type3 = System::Traits::one_of_t<1, Tail...>;
		using type4 = System::Traits::one_of_t<2, Tail...>;
		using type5 = System::Traits::one_of_t<3, Tail...>;
		using type6 = System::Traits::one_of_t<4, Tail...>;
		using type7 = System::Traits::one_of_t<5, Tail...>;
		using type8 = System::Traits::one_of_t<6, Tail...>;
		using type9 = System::Traits::one_of_t<7, Tail...>;
	private:
		template<class H, class ...T>
		void Set(size_t i, H&& head, T&&... t) noexcept {
			values[i] = new Leaf<H>(static_cast<H&&>(head));
			if constexpr (sizeof...(T) == 0) return;
			else Set(++i, static_cast<T&&...>(t)...);
		}
	public:
		Tuple(Head&& head, Tail&&... tail) noexcept {
			values = new LeafBase * [sizeof...(Tail) + 1];
			values[0] = new Leaf<Head>(static_cast<Head&&>(head));
			Set(1, static_cast<Tail&&...>(tail)...);
		}
		~Tuple() noexcept {
			for (size_t i = 0; i < Count; ++i) delete values[i];
			delete[] values;
			values = nullptr;
		}
	public:
		void* GetValue(size_t i) noexcept {
			return values[i]->GetValue();
		}
	};
}
