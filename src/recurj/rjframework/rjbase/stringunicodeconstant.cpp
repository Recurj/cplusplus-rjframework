#include "stdafx.h"
#include <local/rj/libs/base/strings/unicode/constant.h>
using namespace rjBase;
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_spaceGroup(L"\u0020\u00A0\u0009\u1680\u2000\u2001\u2002\u2003\u2004\u2005\u2006\u2007\u2008\u2009\u200A\u202F\u205F\u3000");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_lineBreakGroup(L"\u000A\u0085\u2028\u2029");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_controlGroup(L"\u0000\u0001\u0002\u0003\u0004\u0005\u0006\u0007"
L"\u0008\u0009\u000A\u000B\u000C\u000D\u000E\u000F"
L"\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017"
L"\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F"
L"\u007F\u0080\u0081\u0082\u0083\u0084\u0085\u0086"
L"\u0087\u0088\u0089\u008A\u008B\u008C\u008D\u008E"
L"\u008F\u0090\u0091\u0092\u0093\u0094\u0095\u0096"
L"\u0097\u0098\u0099\u009A\u009B\u009C\u009D\u009E"
L"\u009F");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_forwardSlash(L"\u002F\u01C0\u0338\u2044\u2215");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_backSlash(L"\u005C\u20E5\u2216");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_asterisk(L"\u002A\u066D\u204E\u2217\u26B9\u2731");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_apostrophe(L"\u0027\u2019\u02B9\u02BC\u02C8\u0301\u05F3\u2032\uA78C");
CUStringUnicodeConstantC CUStringUnicodeConstantC::s_questionMark(L"\u0022\u02BA\u030B\u030E\u05F4\u2033\u3003");
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordYES(L"YES", 3);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordTRUE(L"TRUE", 4);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordNO(L"NO", 2);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordFALSE(L"FALSE", 5);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_Word1(L"1", 1);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_Word0(L"0", 1);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordN(L"N", 1);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_WordY(L"Y", 1);
#ifdef _rjOS_FAMILY_UNIX
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_LineNext(L"\x0A", 1);
#endif 
#ifdef _rjOS_WIN
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_LineNext(L"\x0A", 1);
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_LineFeed(L"\x0D\x0A", 2);
#endif 
CRJStringUnicodeConstant CRJStringUnicodeConstant::s_CRLF(L"\x0D\x0A", 2);
CRJStringUnicodeConstant::CRJStringUnicodeConstant(const wchar_t* p, size_t lLen):
	CRJStringUnicode(p, lLen) {
	m_lLength=lLen;
}
CRJStringUnicodeConstant::CRJStringUnicodeConstant(const CRJStringUnicode& str):
	CRJStringUnicode(str.GetHeadForRead(), str.GetSize()) {
	m_lLength=str.GetLength();
}
CUStringUnicodeConstantC::CUStringUnicodeConstantC(const wchar_t* p, size_t l):
	m_clOutside(p, l) {}

