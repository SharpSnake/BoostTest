#include <iostream>
#include <string>
#include <regex>
#include <boost/regex.hpp>

#include "Macros.h"

using std::cout;
using std::endl;
using std::string;

// C++11��׼����boost����������������������ͬ�����Էֱ����
//#define LIBboost

#if defined( LIBboost )
	using namespace boost;
#else
	using namespace std;
#endif


// ��ӡһ��ƥ����
void Printsmatch( const smatch &sm )
{
	// ���������ƥ��
	cout << "\t[" << sm.str() << "]" << endl;

	// �������ƥ����±��1��ʼ����ƥ��������ǿյ�
	std::size_t subMat = sm.size();
	if( subMat > 1 )
	{
		cout << "\t\t" << ( subMat - 1 ) << " Sub Matches:" << endl;
		for( auto i = 1U; i < subMat; ++i )
			cout << "\t\t[" << sm[ i ].str() << "]" << endl;
	}
}


// �ж��Ƿ�ƥ�䣬ƥ�����ӡ�����е�һ��ƥ����
void match( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	smatch sm;
	if( regex_match( str, sm, regex( regstr, regflag ) ) )
		cout << "\tMatch:\t" << "[" << sm.str() << "]" << endl;
	else
		cout << "\tNot Match!" << endl;
}

// regex_searchֻ��ƥ��һ�����
void search_one( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	smatch sm;
	if( regex_search( str, sm, regex( regstr, regflag ) ) )
		Printsmatch( sm );
	else
		cout << "\tNo Match." << endl;	
}

// ʹ�õ�����ƥ��ȫ�������sregex_iterator������ʵ����һ��smatch��
// iterator�Ľ�������������صľ���һ��smatch
void search_all( const string &str, const string &regstr, 
	regex::flag_type regflag = regex::ECMAScript )
{
	regex reg( regstr, regflag );
	auto matbeg = sregex_iterator( str.begin(), str.end(), reg );
	auto matend = sregex_iterator();

	for( auto i = matbeg; i != matend; ++i )
		Printsmatch( *i );
}

// �滻
void replace( const string &str, const string &regstr, const string &fmt, 
	regex::flag_type regflag = regex::ECMAScript )
{
	string result = regex_replace( str, regex( regstr, regflag ), fmt );
	cout << str << " --> " << endl << result << endl;
}


void RegexMain()
{
	cout << "ƥ�����" << endl;
	// ��ģʽ���ҳ����е�ʽ����һ���������Ⱥţ��������͵Ⱥż�����пհ��ַ�
	// ���﷨�������޶����У�*, +, ?, {n}, {n,}, {n,m}
	search_all( "a ==b c=== d ef=hij l== mn", "\\w+\\s*={1,2}\\s*\\w+" );


	coute << "ƥ�����������̰��" << endl;
	// ��ģʽ��ƥ��<>�еı�ǩ����ǩ�����ǻ��з���������ַ���ʹ�÷�̰�����������䶼ƥ��
	// ���﷨��*��+���?����ʾ��̰��������ᾡ���ܶ��ƥ����ǰ����ַ�
	search_all( "<H1>��һ�� ɳ��֮��</H1><p>dune</p>", "<.+?>" );


	coute << "���䶨λƥ��" << endl;
	// ��ģʽ���Ƿ���һ��������html��ǩ������������Ƶ�ƥ�䣺<xx>......</xx>
	// ���﷨��^��$��ʾ�ַ����Ŀ�ͷ�ͽ�β
	match( "<H1>��һ�� ɳ��֮��</H1>", "^<.*>$" );
	match( "<H1>�ڶ��� ɳ�����<", "^<.*>$" );


	coute << "���ʶ�λƥ��" << endl;
	// ��ģʽ���ҳ�opengl����汾�����꺯����
	// ���﷨��\b��ʾ���ʱ߽磬�߽�����ǿո񣬻��߾��ӵĿ�ͷ����β
	search_all( "glxxx4fb	glTexCoord2f fseek glVertex3f", "\\bgl\\w*\\df\\b" );


	coute << "��ƥ����" << endl;
	// ��ģʽ������opengl���꺯����ʲô���꣨������ɫ��������ά�ȣ�
	// �����������ͣ������Ƿ���������ʽ��gl*v����
	// ���﷨��Բ���ű�ʾ���?:��ʾƥ�䵫�������������Ͳ��÷�̰��ƥ����ĸ
	// ?:������������񣬵�����ƥ��������Ȼ������������
	string regstr1 = "\\b(?:gl)(\\w+)(\\d)(\\w+?)(v?)\\b";
	search_one( "glColor3ubv", regstr1 );
	search_one( "glTexCoord2i", regstr1 );


	coute << "��ƥ�������" << endl;
	// ��ģʽ������ǩ�ָ���ּ����м�
	// ���﷨��ÿ����Ҫ����������һ����ţ��������ֱ�����ã�
	// �ǲ���ռ��ţ����������������������\1��\2��\3...
	string regstr2 = "(<.+?>)(.+)(?:\\1)";
	search_one( "<H1>��һ�� ɳ��֮��<H1>", regstr2 );
	search_one( "<span>���ҽ�˹�ڶ��񣬸������ˡ�<span>", regstr2 );

	cout << "���ظ��ĵ��ʣ����Դ�Сд" << endl;
	search_all( "just balaBala, Haha and bebe", "\\b(\\w+?)\\1\\b", regex::icase );


	coute << "�ǲ��񡪡�����϶�Ԥ�飨������������xxx��" << endl;
	// ��ģʽ���ҳ����ӿ������
	// ���﷨��?=����ʾ������ŵı�����ĳĳ��������ƥ�䲻������ĳĳ��
	// ���ǽ��Ŵ�ĳĳ��ʼ��һ��ƥ�䣬Ҳ���ǡ����ȡ�����˼
	search_all( "hello.exe opengl.dll glfw32.dll.lib", "\\w+(?=\\.[dll|lib])" );

	coute << "�ǲ��񡪡������Ԥ�飨�����治�ܸ���xxx��" << endl;
	// ��ģʽ���ų����ӿ⣬�ҳ���ִ�г��������
	// ���﷨��?!
	search_all( "hello.exe opengl.dll, edit.com", "(\\w+)\\.(?![dll|lib])" );


	// C++11����ECMAScript�﷨����֧�ַ���Ԥ�飬����ʧ�ܣ���boostĬ��֧��
#if defined( LIBboost )
	coute << "�ǲ��񡪡�����϶�Ԥ�飨��ǰ�������xxx��" << endl;
	// ��ģʽ���ҳ�Tom˵�Ļ�
	// ���﷨��?<=
	search_all( "Tom:Thanks. Lili:Don't. Tom:I mean it.", "(?<=Tom:).+?\\." );

	coute << "�ǲ��񡪡������Ԥ�飨��ǰ�治����xxx��" << endl;
	// ��ģʽ���ҳ�����Tom˵�Ļ�
	// ���﷨��?<!
	search_all( "Tom:Thanks. Lili:Don't. Jack:Thank me.", "(?<!Tom):(.+?\\.)" );
#else
	coute << "std::regex ��֧�ַ���Ԥ��" << endl;
#endif


	coute << "�򵥵ĵ����滻" << endl;
	// ��ģʽ����opengl api�Ŀ�ͷ�滻�ɵ���"glew"
	replace( "glTexCoord2f gl fseek glVertex3f", "\\bgl(?=\\w+)", "glew" );


	coute << "ƥ����䣬ǰ�����һЩ����" << endl;
	// ��ģʽ������ʽ��Ϊ����ʽ
	// ���﷨��$&��$0�����滻�����б�ʾ����ƥ����
	replace( "a =b c= d ef=hij", "=", "!$&" );

	// ��ģʽ�����ļ��ĺ�׺ǰ���һ��old����ʾ����ļ��Ǿɵģ�������̭
	replace( "hello.exe opengl.dll glfw32.lib", "\\.\\w+", ".old$&" );


	coute << "�滻��ʹ�������" << endl;
	// ��ģʽ���������е�ע��������һ�㶼����һ����ţ����ǻ���Ҳ�����Ǳ���ַ�������������
	// ���﷨���ֱ���ź��ǻ����飬�滻��������Ų������ǻ�������ŵ����
	// ���滻�����У������$��ʾ���ƺ�������ֻ�ܵ�9��û����֤
	replace( "id=\"fnref19\"><sup>��</sup>", "fnref(\\d+)\"><sup>(.*?)</sup>", 
		"fnref$1\"><sup>[$1]</sup>" );
}