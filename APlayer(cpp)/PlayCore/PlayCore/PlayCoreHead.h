#ifndef APLAYER_PLAYERCOREHEAD
#define APLAYER_PLAYERCOREHEAD

#include "libzplay.h"
using namespace libZPlay;

#include <iostream>
using std::wstring;

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
