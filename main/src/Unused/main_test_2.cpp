import Module;
import <iostream>;
import System;
using namespace System;

template<class T>
void OutputName() noexcept {
	String fullName = TypeInfo<T>::GetFullName();
	String typeName = TypeInfo<T>::GetTypeName();
	String namespaceName = TypeInfo<T>::GetNamespace();

	std::cout << "FullName: " << fullName << std::endl;
	std::cout << "TypeName: " << typeName << std::endl;
	std::cout << "NamespaceName: " << namespaceName << std::endl;
}

template<Traits::Concepts::CEnum E, E val>
void OutputEnumName() noexcept {
	String enumName = EnumInfo<E, val>::GetEnumName();
	String valueName = EnumInfo<E, val>::GetValueName();

	std::cout << "EnumName: " << enumName << std::endl;
	std::cout << "ValueName: " << valueName << std::endl;
}

int main(int argc, char** argv) {
	std::cout << cout_green << "main start." << cout_reset << std::endl;

	OutputName<String const>();
	OutputName<Atomic<int> const>();
	OutputName<HashMap<CStringBlock<char32_t, 54> const, TaskPromise<bool, void>>>();

	OutputEnumName<TaskState, TaskState::Suspended>();

	std::cout << cout_green << "main end." << cout_reset << std::endl;
	return 0;
}
