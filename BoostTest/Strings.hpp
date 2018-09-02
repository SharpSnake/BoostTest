#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <cstdlib>
#include <string>
#include <memory>
#include <locale>

using std::string;
using std::wstring;
using std::unique_ptr;
using std::size_t;


// 字符串转换环境参数自动设置与还原
// 当涉及到中文字符时，必须设置
struct StringsEnv
{
	StringsEnv( const std::locale &loc = std::locale( "" ) )
		: m_OldLoc( std::locale::global( loc ) ) {}
	~StringsEnv() { std::locale::global( m_OldLoc ); }
	std::locale m_OldLoc;
};


// 通用的宽、窄字符串互转
inline string WStr2Str( const wstring &wstr )
{
	const wchar_t *wcstr = wstr.c_str();
	size_t destLen = wcstombs( nullptr, wcstr, 0 ) + 1;
	unique_ptr< char[] > cstr( new char[ destLen ]() );
	wcstombs( cstr.get(), wcstr, destLen );
	return string( cstr.get() );
}

inline wstring Str2WStr( const string &str )
{
	const char *cstr = str.c_str();
	size_t destLen = mbstowcs( nullptr, cstr, 0 ) + 1;
	unique_ptr< wchar_t[] > wcstr( new wchar_t[ destLen ]() );
	mbstowcs( wcstr.get(), cstr, destLen );
	return wstring( wcstr.get() );
}

#endif // !STRINGS_HPP