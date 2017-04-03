// DLL3.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "DLL3.h"


// This is an example of an exported variable
DLL3_API int nDLL3=0;

// This is an example of an exported function.
DLL3_API int fnDLL3(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see DLL3.h for the class definition
CDLL3::CDLL3()
{
    return;
}

extern "C" {

	__declspec(dllexport) int MyFuncC(){
		return 100500;
	}

	__declspec(dllexport) int MyFunc(){
		return 500100;
	}

}
