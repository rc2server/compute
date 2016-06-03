//base struct that initializes all members to zero/null
struct ZeroInitializedStruct {
	static void *alloc_zero(size_t size) {
		void *mem = ::operator new(size);
		std::memset(mem, 0, size);
		return mem;
	}
	void *operator new(size_t size) { return alloc_zero(size); }
	void *operator new[](size_t size) { return alloc_zero(size); }
};

class ZeroInitializedClass {
public:
	static void *alloc_zero(size_t size) {
		void *mem = ::operator new(size);
		std::memset(mem, 0, size);
		return mem;
	}
	void *operator new(size_t size) { return alloc_zero(size); }
	void *operator new[](size_t size) { return alloc_zero(size); }
};
