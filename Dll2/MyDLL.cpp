#include "MyDLL.h"

extern "C" __declspec(dllexport) int MyFunc(const int it){

	for(int i = it; i < it; i++){
		int z = 1 + 1;
	}

	return 0;
}


