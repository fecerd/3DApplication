module Objects_Internal;
import TypeID;

namespace System {
	Type Object::GetType() const noexcept { return Type::CreateType<Object>(); }
	String Object::ToString() const noexcept { return String("Undefined Object"); }
	uint32_t Object::GetTypeID() const noexcept { return GetID<Object>(); }
}

namespace System {
	Type String::GetType() const noexcept { return Type::CreateType<String>(); }
	uint32_t String::GetTypeID() const noexcept { return GetID<String>(); }
}

namespace System {
	uint32_t Type::GetTypeID() const noexcept {
		return GetID<Type>();
	}
}