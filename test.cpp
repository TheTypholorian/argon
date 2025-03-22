#include "array.hpp"
#include <iostream>

using namespace std;
using namespace H;

int main() {
	Array<huint> array = Array<huint>(3);

	array.data[0] = 1;
	array.data[1] = 5;
	array.data[2] = 3;

	array.add(5);
	array.add(4);

	for (huint t : array) {
		cout << t << " ";
	}

	return 0;
}