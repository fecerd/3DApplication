
import <iostream>;
import Module;
using namespace System;

int main(int argc, char** argv) {

	List<int> list;
	list.PushBack(10);
	list.PushBack(20);

	for (auto ite = list.begin(),e = list.end(); ite != e;) {
		auto cur = ite;
		list.Remove(cur);
		++ite;
	}

	return 0;
}