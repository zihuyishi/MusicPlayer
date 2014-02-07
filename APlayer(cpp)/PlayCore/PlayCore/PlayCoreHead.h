#ifndef APLAYER_PLAYERCOREHEAD
#define APLAYER_PLAYERCOREHEAD

#include "libzplay.h"
using namespace libZPlay;

#include <iostream>
using std::wstring;

typedef int(__stdcall *MCallbackFunc)(void* instance, void* lpParam);

class UnCopyable {
public:
	UnCopyable() {}
	~UnCopyable() {}
private:
	UnCopyable(const UnCopyable &) {}
	UnCopyable& operator=(const UnCopyable &) {}
};

#define PLAYCORE_BEGIN namespace PlayCore {
#define PLAYCORE_END }

#endif
