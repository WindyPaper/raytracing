#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#include <sstream>
#include <windows.h>

inline void debug_print_val(float val)
{
	std::ostringstream istream;
	istream << psi << std::endl;
	OutputDebugString(istream.str().c_str());
}

#endif