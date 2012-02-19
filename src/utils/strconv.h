#ifndef _STR_CNV
#define _STR_CNV

#include <atlconv.h>
static std::wstring strconv(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string strconv(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};

#endif