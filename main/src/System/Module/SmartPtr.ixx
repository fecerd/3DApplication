export module SmartPtr;
import <memory>;

export namespace System
{
	template <class T>
	using UniquePtr = std::unique_ptr<T>;
	template <class T>
	UniquePtr<T> Unique() noexcept
	{
		return std::make_unique<T>();
	}
	template <class T, class V>
	UniquePtr<T> Unique(const V &val) noexcept
	{
		return std::make_unique<T>(val);
	}
	template <class T, class V>
	UniquePtr<T> Unique(V &&val) noexcept
	{
		return std::make_unique<T>(static_cast<V &&>(val));
	}
	template <class T>
	UniquePtr<T> UniqueArray(size_t n) { return std::make_unique<T>(n); }

	template <class T>
	using SharedPtr = std::shared_ptr<T>;
	template <class T>
	SharedPtr<T> Shared() noexcept
	{
		return std::make_shared<T>();
	}
	template<class T, class ...Args>
	SharedPtr<T> Shared(Args&& ...args) noexcept {
		return std::make_shared<T>(static_cast<Args&&>(args)...);
	}
	template <class T>
	SharedPtr<T> SharedArray(size_t n) noexcept
	{
		return std::make_shared<T>(n);
	}
}