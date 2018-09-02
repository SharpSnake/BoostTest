#include <iostream>
#include <string>
#include <vector>
#include <boost/tokenizer.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "Macros.h"

using namespace std;

template< typename Container >
void PrintStrings( const Container &strs )
{
	for( const auto &i : strs )
		cout << i << endl;
	cout << endl;
}

// 这个版本中，空的分割结果被舍弃
inline void split_regex_ex( vector< string > &Result, const std::string &Input, 
	const boost::regex &Rx )
{
	using SplitIterator = boost::algorithm::split_iterator< std::string::const_iterator >;

	for( SplitIterator i = make_split_iterator( Input, regex_finder( Rx ) ); 
		i !=  SplitIterator(); ++i )
	{
		if( i->size() )
			Result.push_back( string( i->begin(), i->end() ) );
	}
}


void TokenizerMain()
{
	//===========================================================================
	// 1、单个字符语义的分割
	using CharSeparator = boost::char_separator< char >;
	using CharTokenizer = boost::tokenizer< CharSeparator >;


	cout << "======= 构造及结果保存 =======" << endl;
	// 要分割的字符串不能是一个即将销毁的右值，字符串的生命周期要贯穿整个分割过程
	string str1 = "Hello, Boost C++ Libraries!";

	// 默认按空格分割，标点符号会单独分割出来
	CharTokenizer tokDefault( str1 );

	// 在使用分割结果前，str不能有变动，否则将获取到意想不到的结果
	// 为此，可以将结果保存到一个稳定的容器里，或抓紧时间使用分割结果
	vector< string > tok2vec( tokDefault.begin(), tokDefault.end() );
	PrintStrings( tok2vec );


	cout << "======= 设置分隔符 =======" << endl;
	string str2 = "if lilei = tom; then hanmei = lili;";

	// 参数1：所有用于分割的字符，注意是单个字符，不是按字符串分割
	// 参数2：保留的字符，指定的字符被单独分割
	// 参数3：默认是drop_empty_tokens，否则保留空的分割结果
	CharSeparator cSep{ " =", ";", boost::drop_empty_tokens };
	CharTokenizer tok2( str2, cSep );
	PrintStrings( tok2 );


	//===========================================================================
	// 2、将一个小字符串作为分隔符，要用到regex
	cout << "======= C# String.Split like =======" << endl;
	string str3 = "if lileiequalto tom; then hanmei ==lili;";

	// 分隔符用boost的regex表示，多个字符串分隔符用|间隔开
	boost::regex strSep( " |equalto|==|;" );

	// 分割结果存在一个容器里，boost默认空字符串的结果也会返回，
	vector< string > splitresult;
	boost::algorithm::split_regex( splitresult, str3, strSep );
	PrintStrings( splitresult );

	cout << "split result without empty tokens:" << endl;
	vector< string > splitresult_without_empty;
	split_regex_ex( splitresult_without_empty, str3, strSep );
	PrintStrings( splitresult_without_empty );
}


