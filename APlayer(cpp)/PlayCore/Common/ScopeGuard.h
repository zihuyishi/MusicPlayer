/**
*用来管理资源，需要支持c++11
*
*
*
*
*/


#include <functional>
#ifndef SCOPEGUARD_HEADER
#define SCOPEGUARD_HEADER

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

//释放资源宏
#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

class ScopeGuard
{
public:
	explicit ScopeGuard(std::function<void()> onExitScope)
		: _onExitScope(onExitScope), _dismissed(false)
	{}
	~ScopeGuard()
	{
		if (!_dismissed) {
			_onExitScope();
		}
	}

	void Dismiss()
	{
		_dismissed = true;
	}
private:
	ScopeGuard(ScopeGuard const&);
	ScopeGuard& operator=(ScopeGuard const&);
private:
	std::function<void()> _onExitScope;
	bool _dismissed;
};

#endif