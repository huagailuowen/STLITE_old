#ifndef SJTU_EXCEPTIONS_HPP
#define SJTU_EXCEPTIONS_HPP

#include <cstddef>
#include <cstring>
#include <string>

namespace sjtu {

class exception {
protected:
	const std::string variant = "";
	std::string detail = "";
public:
	exception() {}
	exception(std::string str) : detail(str) {}
	exception(const exception &ec) : variant(ec.variant), detail(ec.detail) {}
	virtual std::string what() {
		return variant + " " + detail;
	}
};

class index_out_of_bound : public exception {
	/* __________________________ */
	public:
	index_out_of_bound(std::string str) : exception(str) {}
};

class runtime_error : public exception {
	/* __________________________ */
	public:
	runtime_error(std::string str) : exception(str) {}
};

class invalid_iterator : public exception {
	/* __________________________ */
	public:
	invalid_iterator(std::string str) : exception(str) {}
};

class container_is_empty : public exception {
	/* __________________________ */
	public:
	container_is_empty(std::string str) : exception(str) {}
};
}

#endif
