module Objects_Internal;
import CSTDINT;
import TypeID;
import StringBlock;

namespace System
{
	Type Object::GetType() const noexcept { return Type::CreateType<Object>(); }
	String Object::ToString() const noexcept { return String("Undefined Object"); }
	uint32_t Object::GetTypeID() const noexcept { return GetID<Object>(); }
}

namespace System
{
	Type String::GetType() const noexcept { return Type::CreateType<String>(); }
	uint32_t String::GetTypeID() const noexcept { return GetID<String>(); }
}

namespace System
{
	Type::Type(const Type &arg) noexcept : m_templateCount(arg.m_templateCount)
	{
		if (arg.m_typeName)
			m_typeName = new String(*arg.m_typeName);
		if (arg.m_templateArgs)
		{
			m_templateArgs = new String[m_templateCount];
			for (size_t i = 0; i < m_templateCount; ++i)
				m_templateArgs[i] = arg.m_templateArgs[i];
		}
	}
	Type::Type(const char16_t *name) noexcept
	{
	}
	Type::~Type() noexcept
	{
		delete m_typeName;
		m_typeName = nullptr;
		delete m_templateArgs;
		m_templateArgs = nullptr;
		m_templateCount = 0;
	}
	String Type::ToString() const noexcept
	{
		String ret;

		return ret;
	}
	uint32_t Type::GetTypeID() const noexcept { return GetID<Type>(); }
	bool Type::operator==(const Type &rhs) const noexcept
	{
		if (!m_typeName || !rhs.m_typeName)
			return m_typeName == rhs.m_typeName;
		if (*m_typeName != *rhs.m_typeName)
			return false;
		if (m_templateCount != rhs.m_templateCount)
			return false;
		for (size_t i = 0; i < m_templateCount; ++i)
		{
			if (m_templateArgs[i] != rhs.m_templateArgs[i])
				return false;
		}
		return true;
	}
}
