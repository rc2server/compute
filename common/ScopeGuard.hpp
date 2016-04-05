#include <memory>

///Pass a lambda to be executed when this object goes out of scope
template<class Fun>
class ScopeGuard {
	Fun _f;
	bool _active;
public:
	ScopeGuard(Fun f)
		:_f(std::move(f)), _active(true) {
			
		}
	~ScopeGuard() { if (_active) _f(); }
	void dismiss() { _active = false; }
	
	ScopeGuard() = delete;
	ScopeGuard(const ScopeGuard&) = delete;
	ScopeGuard& operator=(const ScopeGuard&) = delete;
	ScopeGuard(ScopeGuard &rhs) 
		:_f(std::move(rhs._f)), _active(rhs._active) {
			rhs.dismiss();
		}
};
