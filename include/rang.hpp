#ifndef RANG_H
#define RANG_H

#include <iostream>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <type_traits>
extern "C" {
#include <unistd.h>
}

std::streambuf const *RANG_coutbuf = std::cout.rdbuf();
std::streambuf const *RANG_cerrbuf = std::cerr.rdbuf();
std::streambuf const *RANG_clogbuf = std::clog.rdbuf();

namespace rang {

	enum class style {
		reset     = 0,
		bold      = 1,
		dim       = 2,
		italic    = 3,
		underline = 4,
		blink     = 5,
		reversed  = 6,
		conceal   = 7,
		crossed   = 8
	};
	enum class fg {
		black   = 30,
		red     = 31,
		green   = 32,
		yellow  = 33,
		blue    = 34,
		magenta = 35,
		cyan    = 36,
		gray    = 37
	};
	enum class bg {
		black   = 40,
		red     = 41,
		green   = 42,
		yellow  = 43,
		blue    = 44,
		magenta = 45,
		cyan    = 46,
		gray    = 47
	};
	enum class fgB {
		black   = 90,
		red     = 91,
		green   = 92,
		yellow  = 93,
		blue    = 94,
		magenta = 95,
		cyan    = 96,
		gray    = 97
	};
	enum class bgB {
		black   = 100,
		red     = 101,
		green   = 102,
		yellow  = 103,
		blue    = 104,
		magenta = 105,
		cyan    = 106,
		gray    = 107
	};
}


inline bool supportsColor()
{
	const std::string Terms[] = { "ansi",  "color",   "console", "cygwin",
		                          "gnome", "konsole", "kterm",   "linux",
		                          "msys",  "putty",   "rxvt",    "screen",
		                          "vt100", "xterm" };

	static const bool result =
	    std::any_of(std::begin(Terms), std::end(Terms), [&](std::string term) {

		    if(const char *env_p = std::getenv("TERM")) {
			    return std::string(env_p).find(term) != std::string::npos;
		    }
		    else
			    return false;
		});

	return result;
}

inline bool isTerminal(const std::streambuf *osbuf)
{
	if(osbuf == RANG_coutbuf) {
		return isatty(fileno(stdout));
	}
	if(osbuf == RANG_cerrbuf || osbuf == RANG_clogbuf) {
		return isatty(fileno(stderr));
	}
	return false;
}


template <typename T>
using enable = typename std::enable_if
			<
				std::is_same<T, rang::style>::value ||
				std::is_same<T, rang::fg>::value ||
				std::is_same<T, rang::bg>::value,
				std::ostream &
			>::type;

template <typename T>
inline enable<T> operator<<(std::ostream &os, T const value)
{
	std::streambuf const *osbuf = os.rdbuf();
	return ((supportsColor()) && (isTerminal(osbuf)))
	           ? os << "\033[" << static_cast<int>(value) << "m"
	           : os;
}

#endif /* ifndef RANG_H */