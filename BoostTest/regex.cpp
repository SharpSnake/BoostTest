#include <iostream>
#include <string>
#include <regex>
#include <boost/regex.hpp>

#include "Macros.h"

using std::cout;
using std::endl;
using std::string;

// C++11标准库与boost的所有类名、函数名都相同，可以分别测试
//#define LIBboost

#if defined( LIBboost )
	using namespace boost;
#else
	using namespace std;
#endif


// 打印一个匹配结果
void Printsmatch( const smatch &sm )
{
	// 这个是完整匹配
	cout << "\t[" << sm.str() << "]" << endl;

	// 如果有子匹配项，下标从1开始，字匹配项可能是空的
	std::size_t subMat = sm.size();
	if( subMat > 1 )
	{
		cout << "\t\t" << ( subMat - 1 ) << " Sub Matches:" << endl;
		for( auto i = 1U; i < subMat; ++i )
			cout << "\t\t[" << sm[ i ].str() << "]" << endl;
	}
}


// 判断是否匹配，匹配则打印句子中第一个匹配项
void match( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	smatch sm;
	if( regex_match( str, sm, regex( regstr, regflag ) ) )
		cout << "\tMatch:\t" << "[" << sm.str() << "]" << endl;
	else
		cout << "\tNot Match!" << endl;
}

// regex_search只能匹配一个结果
void search_one( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	smatch sm;
	if( regex_search( str, sm, regex( regstr, regflag ) ) )
		Printsmatch( sm );
	else
		cout << "\tNo Match." << endl;	
}

// 使用迭代器匹配全部结果，sregex_iterator里面其实包了一个smatch，
// iterator的解引用运算符返回的就是一个smatch
void search_all( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	regex reg( regstr, regflag );
	auto matbeg = sregex_iterator( str.begin(), str.end(), reg );
	auto matend = sregex_iterator();

	for( auto i = matbeg; i != matend; ++i )
		Printsmatch( *i );
}

// 替换
void replace( const string &str, const string &regstr, const string &fmt, 
	regex::flag_type regflag = regex::ECMAScript )
{
	string result = regex_replace( str, regex( regstr, regflag ), fmt );
	cout << str << " --> " << endl << result << endl;
}


void RegexMain()
{
	cout << "匹配次数" << endl;
	// 【模式】找出所有等式，有一个或两个等号，变量名和等号间可能有空白字符
	// 【语法】次数限定符有：*, +, ?, {n}, {n,}, {n,m}
	search_all( "a ==b c=== d ef=hij l== mn", "\\w+\\s*={1,2}\\s*\\w+" );


	coute << "匹配次数——非贪婪" << endl;
	// 【模式】匹配<>中的标签，标签可以是换行符外的任意字符，使用非贪婪，否则整句都匹配
	// 【语法】*和+后加?，表示非贪婪，否则会尽可能多的匹配它前面的字符
	search_all( "<H1>第一部 沙丘之子</H1><p>dune</p>", "<.+?>" );


	coute << "整句定位匹配" << endl;
	// 【模式】是否是一个完整的html标签，后面更加完善的匹配：<xx>......</xx>
	// 【语法】^和$表示字符串的开头和结尾
	match( "<H1>第一部 沙丘之子</H1>", "^<.*>$" );
	match( "<H1>第二部 沙丘神帝<", "^<.*>$" );


	coute << "单词定位匹配" << endl;
	// 【模式】找出opengl浮点版本的坐标函数名
	// 【语法】\b表示单词边界，边界可能是空格，或者句子的开头、结尾
	search_all( "glxxx4fb	glTexCoord2f fseek glVertex3f", "\\bgl\\w*\\df\\b" );


	coute << "子匹配项" << endl;
	// 【模式】解析opengl坐标函数，什么坐标（纹理、颜色）？坐标维度？
	// 坐标数据类型？坐标是否是数组形式（gl*v）？
	// 【语法】圆括号表示子项，?:表示匹配但不捕获，数据类型采用非贪婪匹配字母
	// ?:仅仅是子项不捕获，但完整匹配里面仍然会包含这个子项
	string regstr1 = "\\b(?:gl)(\\w+)(\\d)(\\w+?)(v?)\\b";
	search_one( "glColor3ubv", regstr1 );
	search_one( "glTexCoord2i", regstr1 );


	coute << "子匹配项序号" << endl;
	// 【模式】将标签分割，文字夹在中间
	// 【语法】每个需要捕获的子项都有一个编号，后面可以直接利用，
	// 非捕获不占编号，从外向里、从左向右依次是\1、\2、\3...
	string regstr2 = "(<.+?>)(.+)(?:\\1)";
	search_one( "<H1>第一部 沙丘之子<H1>", regstr2 );
	search_one( "<span>“我叫斯第尔格，弗雷曼人”<span>", regstr2 );

	cout << "找重复的单词，忽略大小写" << endl;
	search_all( "just balaBala, Haha and bebe", "\\b(\\w+?)\\1\\b", regex::icase );


	coute << "非捕获——正向肯定预查（即后面必须跟着xxx）" << endl;
	// 【模式】找出链接库的名称
	// 【语法】?=：表示后面跟着的必须是某某，但后续匹配不会跳过某某，
	// 而是接着从某某开始下一个匹配，也就是“零宽度”的意思
	search_all( "hello.exe opengl.dll glfw32.dll.lib", "\\w+(?=\\.[dll|lib])" );

	coute << "非捕获——正向否定预查（即后面不能跟着xxx）" << endl;
	// 【模式】排除链接库，找出可执行程序的名称
	// 【语法】?!
	search_all( "hello.exe opengl.dll, edit.com", "(\\w+)\\.(?![dll|lib])" );


	// C++11采用ECMAScript语法，不支持反向预查，编译失败！但boost默认支持
#if defined( LIBboost )
	coute << "非捕获——反向肯定预查（即前面必须是xxx）" << endl;
	// 【模式】找出Tom说的话
	// 【语法】?<=
	search_all( "Tom:Thanks. Lili:Don't. Tom:I mean it.", "(?<=Tom:).+?\\." );

	coute << "非捕获——反向否定预查（即前面不能是xxx）" << endl;
	// 【模式】找出除了Tom说的话
	// 【语法】?<!
	search_all( "Tom:Thanks. Lili:Don't. Jack:Thank me.", "(?<!Tom):(.+?\\.)" );
#else
	coute << "std::regex 不支持反向预查" << endl;
#endif


	coute << "简单的单词替换" << endl;
	// 【模式】将opengl api的开头替换成单词"glew"
	replace( "glTexCoord2f gl fseek glVertex3f", "\\bgl(?=\\w+)", "glew" );


	coute << "匹配项不变，前后添加一些东西" << endl;
	// 【模式】将等式变为不等式
	// 【语法】$&即$0，在替换条件中表示整个匹配项
	replace( "a =b c= d ef=hij", "=", "!$&" );

	// 【模式】在文件的后缀前面加一个old，表示这个文件是旧的，将被淘汰
	replace( "hello.exe opengl.dll glfw32.lib", "\\.\\w+", ".old$&" );


	coute << "替换中使用组序号" << endl;
	// 【模式】电子书中的注释上下文一般都关联一个序号，将星花（也可能是别的字符）换成这个序号
	// 【语法】分别将序号和星花分组，替换条件中序号不动，星花采用序号的组号
	// 在替换条件中，组号用$表示，似乎最大序号只能到9，没有验证
	replace( "id=\"fnref19\"><sup>※</sup>", "fnref(\\d+)\"><sup>(.*?)</sup>", 
		"fnref$1\"><sup>[$1]</sup>" );
}