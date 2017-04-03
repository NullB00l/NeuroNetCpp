// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL3_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL3_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL3_EXPORTS
#define DLL3_API __declspec(dllexport)
#else
#define DLL3_API __declspec(dllimport)
#endif

// This class is exported from the DLL3.dll
class DLL3_API CDLL3 {
public:
	CDLL3(void);
	// TODO: add your methods here.
};

extern DLL3_API int nDLL3;

DLL3_API int fnDLL3(void);
