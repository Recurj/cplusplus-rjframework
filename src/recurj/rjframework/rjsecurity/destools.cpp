#include "stdafx.h"
#include <recurj/rjframework/libs/security/destools.h>
#include <recurj/rjframework/libs/base/datatools/bytepacker.h>
#include <recurj/rjframework/libs/base/datetime/dtimebinary.h>
#include <recurj/rjframework/libs/base/datatools/datatools.h>
using namespace rjSecurity;
#define SET_BIT(A,B,Y,Z) (B[A/8]|=(Z[Y/8]&(0x80>>(Y%8)))?(0x80>>(A%8)):0)
 static size_t theKeyLengths[2]= {
	8, 16
};
const uint8_t bDesInit[]= {
	58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46, 38, 30,
	22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43,
	35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};
const uint8_t bDesFirst[]= {
	57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35, 27, 19,
	11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22, 14,
	6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};
const uint8_t bDesSecond[]= {
	14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13,
	2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42,
	50, 36, 29, 32
};
const uint8_t bDesPerm[]= {
	16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14, 32, 27,
	3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};
const uint8_t bDesExp[]= {
	32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27, 28, 29, 28, 29, 30,
	31, 32, 1
};
const uint8_t bDesSel[8][64]= {
	{
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7, 0, 15, 7, 4, 14, 2, 13, 1, 10,
		6, 12, 11, 9, 5, 3, 8, 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0, 15, 12,
		8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
	}
	, {
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10, 3, 13, 4, 7, 15, 2, 8, 14, 12,
		0, 1, 10, 6, 9, 11, 5, 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15, 13, 8,
		10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
	}
	, {
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8, 13, 7, 0, 9, 3, 4, 6, 10, 2,
		8, 5, 14, 12, 11, 15, 1, 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7, 1,
		10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
	}
	, {
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15, 13, 8, 11, 5, 6, 15, 0, 3, 4,
		7, 2, 12, 1, 10, 14, 9, 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4, 3, 15,
		0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
	}
	, {
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9, 14, 11, 2, 12, 4, 7, 13, 1,
		5, 0, 15, 10, 3, 9, 8, 6, 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14, 11,
		8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
	}
	, {
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11, 10, 15, 4, 2, 7, 12, 9, 5, 6,
		1, 13, 14, 0, 11, 3, 8, 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6, 4, 3,
		2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
	}
	, {
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1, 13, 0, 11, 7, 4, 9, 1, 10, 14,
		3, 5, 12, 2, 15, 8, 6, 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2, 6, 11,
		13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
	}
	, {
		13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7, 1, 15, 13, 8, 10, 3, 7, 4, 12,
		5, 6, 11, 0, 14, 9, 2, 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8, 2, 1,
		14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
	}
};
const uint8_t bDesLast[]= {
	40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22,
	62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11,
	51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};
const uint8_t bDesShift[]= {
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};
const int32_t bDiv8[32]= {
	0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,
	3, 3, 3, 3
};
#define _DES_ENGINE_START *((uint64_t *)temp) = *((uint64_t *)bSrc);*((uint64_t *)work) = 0;for (i = 0; i < 64; ++i) {pos = bDesInit[i] - 1;SET_BIT(i, work, pos, temp);} \
 (*(uint32_t *)c) = 0;(*(uint32_t *)d) = 0;for (i = 0; i < 28; ++i) {pos = bDesFirst[i] - 1;SET_BIT(i, c, pos, bKey);pos = bDesFirst[i + 28] - 1;SET_BIT(i, d, pos, bKey);}
#define _DES_ENGINE_DONE (*(uint64_t *)temp) = (*(uint64_t *)work);(*(uint32_t *)&work[4]) = (*(uint32_t *)temp);(*(uint32_t *)work) = (*(uint32_t *)&temp[4]);\
 (*(uint64_t *)temp) = (*(uint64_t *)work);(*(uint64_t *)work) = 0;\
 for (i = 0; i < 64; ++i) { pos = bDesLast[i] - 1; SET_BIT(i, work, pos, temp); }\
 (*(uint64_t *)bDst) = (*(uint64_t *)work);
#define _DES_ENGINE_DONE_COMMON_SEL_GET(_ind) row = (((int32_t)s & 0x20) >> 4) | ((int32_t)s & 1); col = ((int32_t)s & 0x1E) >> 1; sel = bDesSel[_ind][16 * row + col]
#define _DES_ENGINE_DONE_COMMON_SEL_ODD(_ind,_ind2) _DES_ENGINE_DONE_COMMON_SEL_GET(_ind);to_perm[_ind2] |= sel
#define _DES_ENGINE_DONE_COMMON_SEL_EVEN(_ind,_ind2)  _DES_ENGINE_DONE_COMMON_SEL_GET(_ind);to_perm[_ind2] = (uint8_t)(sel << 4)
#define _DES_ENGINE_DONE_COMMON *((uint64_t*)kn) = 0x00;\
 for (i = 0; i < 48; ++i) {pos = bDesSecond[i] - 1;if (pos < 28) {SET_BIT(i, kn, pos, c);}else { pos -= 28;SET_BIT(i, kn, pos, d);}}\
 (*(uint64_t *)temp) = (*(uint64_t *)work); (*(uint32_t *)&work[4]) = (*(uint32_t *)temp); (*(uint32_t *)work) = (*(uint32_t *)&temp[4]); \
 for (i = 0; i < 6; ++i) temp[i] = 0;\
 for (i = 0; i < 48; ++i) { pos = bDesExp[i] - 1;SET_BIT(i, temp, pos, work);}\
 for (i = 0; i < 6; ++i)   temp[i] ^= kn[i];\
 s = (uint8_t)(temp[0] >> 2);_DES_ENGINE_DONE_COMMON_SEL_EVEN(0, 0);\
 s = (uint8_t)((temp[0] & 0x3) << 4 | temp[1] >> 4);_DES_ENGINE_DONE_COMMON_SEL_ODD(1, 0);\
 s = (uint8_t)((temp[1] & 0xF) << 2 | temp[2] >> 6);_DES_ENGINE_DONE_COMMON_SEL_EVEN(2, 1);\
 s = (uint8_t)((temp[2] & 0x3F));_DES_ENGINE_DONE_COMMON_SEL_ODD(3, 1);\
 s = (uint8_t)(temp[3] >> 2);_DES_ENGINE_DONE_COMMON_SEL_EVEN(4, 2);\
 s = (uint8_t)((temp[3] & 0x3) << 4 | temp[4] >> 4);_DES_ENGINE_DONE_COMMON_SEL_ODD(5, 2);\
 s = (uint8_t)((temp[4] & 0xF) << 2 | temp[5] >> 6);_DES_ENGINE_DONE_COMMON_SEL_EVEN(6, 3);\
 s = (uint8_t)((temp[5] & 0x3F));_DES_ENGINE_DONE_COMMON_SEL_ODD(7, 3);\
 for (i = 0; i < 32; ++i) {pos = bDesPerm[i] - 1;if (!(i&7)) temp[bDiv8[i]] = 0;SET_BIT(i, temp, pos, to_perm);}\
 for (i = 0; i < 4; ++i)  work[4 + i] ^= temp[i];
#define _DES_ENGINE_ENCRYPT s = c[0];ltemp = (uint32_t)(c[3] + 0x100L * c[2] + 0x10000L * c[1] + 0x1000000L * c[0]);bits = bDesShift[iter];ltemp <<= bits;s >>= 8 - bits;\
 for (i = 0; i < 4; ++i) {c[3 - i] = (uint8_t)(ltemp & 0xFF);ltemp >>= 8;}\
 c[3] |= s << 4; c[3] &= 0xF0;s = d[0];  ltemp = (uint32_t)(d[3] + 0x100L * d[2] + 0x10000L * d[1] + 0x1000000L * d[0]);bits = bDesShift[iter];ltemp <<= bits;s >>= 8 - bits;\
 for (i = 0; i < 4; ++i) {d[3 - i] = (uint8_t)(ltemp & 0xFF);ltemp >>= 8;}\
 d[3] |= s << 4;d[3] &= 0xF0;
#define _DES_ENGINE_DECRYPT bits = bDesShift[15 - iter];  s = c[3];ltemp = (uint32_t)(c[3] + (0x100L * c[2]) + (0x10000L * c[1]) + (0x1000000L * c[0]));ltemp >>= bits; s <<= 4 - bits;\
 for (i = 0; i < 4; ++i) {c[3 - i] = (uint8_t)(ltemp & 0xFF);ltemp >>= 8;}\
 c[0] |= s;c[3] &= 0xF0; s = d[3];ltemp = (uint32_t)(d[3] + (0x100L * d[2]) + (0x10000L * d[1]) + (0x1000000L * d[0]));ltemp >>= bits; s <<= 4 - bits;\
 for (i = 0; i < 4; ++i) {d[3 - i] = (uint8_t)(ltemp & 0xFF);ltemp >>= 8;}\
 d[0] |= s;d[3] &= 0xF0;
 void CRJSecurityDesTools::DoEncryptDes(uint8_t* bDst,const uint8_t* bSrc,const uint8_t* bKey) {
	uint32_t ltemp;
	int32_t iter, pos, bits, row, col, sel, i;
	uint8_t c[4], d[4], to_perm[4], work[8], kn[8], temp[8];
	uint8_t s=0;
#ifdef _DEBUG
	if (!bSrc) CRJDataTools::DebugBreak();
#endif 
	_DES_ENGINE_START
	for (iter=0; iter<16;++iter) {
		_DES_ENGINE_ENCRYPT _DES_ENGINE_DONE_COMMON
	}
	_DES_ENGINE_DONE
}
void CRJSecurityDesTools::DoDecryptDes(uint8_t* bDst,const uint8_t* bSrc,const uint8_t* bKey) {
	uint32_t ltemp;
	int32_t iter, pos, bits, row, col, sel, i;
	uint8_t c[4], d[4], to_perm[4], work[8], kn[8], temp[8];
	uint8_t s=0;
	_DES_ENGINE_START
	for (iter=0; iter<16;++iter) {
		_DES_ENGINE_DONE_COMMON _DES_ENGINE_DECRYPT
	}
	_DES_ENGINE_DONE
}
void CRJSecurityDesTools::DoEncryptDesBlock(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	s&=0xFFFFFFFFFFFFFFF8;
	while (s>0) {
		CRJSecurityDesTools::DoEncryptDes(bDst, bDst, bKey);
		bDst+=8;
		s-=8;
	}
}
void CRJSecurityDesTools::DoDecryptDesBlock(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	s&=0xFFFFFFFFFFFFFFF8;
	while (s>0) {
		CRJSecurityDesTools::DoDecryptDes((uint8_t*) bDst,(uint8_t*) bDst, bKey);
		bDst+=8;
		s-=8;
	}
}
void CRJSecurityDesTools::DoEncryptTripleDes(uint8_t* bDst,const uint8_t* bSrc,const uint8_t* bKey) {
	uint8_t t1[cSizeSecurityKey], t2[cSizeSecurityKey];
	CRJSecurityDesTools::DoEncryptDes(t1, bSrc, bKey);
	CRJSecurityDesTools::DoDecryptDes(t2, t1,& bKey[8]);
	CRJSecurityDesTools::DoEncryptDes(bDst, t2, bKey);
}
void CRJSecurityDesTools::DoDecryptTripleDes(uint8_t* bDst,const uint8_t* bSrc,const uint8_t* bKey) {
	uint8_t t1[cSizeSecurityKey], t2[cSizeSecurityKey];
	CRJSecurityDesTools::DoDecryptDes(t1, bSrc, bKey);
	CRJSecurityDesTools::DoEncryptDes(t2, t1,& bKey[8]);
	CRJSecurityDesTools::DoDecryptDes(bDst, t2, bKey);
}
void CRJSecurityDesTools::DoDecryptTripleDesBlock(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	uint64_t bTmp;
	uint64_t bIni=0;
	s&=0xFFFFFFFFFFFFFFF8;
	while (s>0) {
		CRJSecurityDesTools::DoDecryptTripleDes((uint8_t*)& bTmp, bDst, bKey);
		bTmp^=bIni;
		bIni=(*(uint64_t*) bDst);
		(*(uint64_t*) bDst)=bTmp;
		bDst+=8;
		s-=8;
	}
}
void CRJSecurityDesTools::DoEncryptTripleDesBlock(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	try {
		uint64_t bIni=0;
		s&=0xFFFFFFFFFFFFFFF8;
		while (s>0) {
			bIni^=(*(uint64_t*) bDst);
			CRJSecurityDesTools::DoEncryptTripleDes((uint8_t*)& bIni,(uint8_t*)& bIni, bKey);
			(*(uint64_t*) bDst)=bIni;
			bDst+=8;
			s-=8;
		}
	}
	catch(...) {}
}
void CRJSecurityDesTools::DoDecryptTripleDesBlockSimple(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	CRJSecurityDesTools::DoDecryptDesBlock(bDst, s, bKey);
	CRJSecurityDesTools::DoEncryptDesBlock(bDst, s,& bKey[8]);
	CRJSecurityDesTools::DoDecryptDesBlock(bDst, s, bKey);
}
void CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(uint8_t* bDst, size_t s,const uint8_t* bKey) {
	CRJSecurityDesTools::DoEncryptDesBlock(bDst, s, bKey);
	CRJSecurityDesTools::DoDecryptDesBlock(bDst, s,& bKey[8]);
	CRJSecurityDesTools::DoEncryptDesBlock(bDst, s, bKey);
}
bool CRJSecurityDesTools::CreateKey(SRJSecurity24KeyInfo& stInfo,const uint8_t* lmk,bool bShort) {
	try {
		int32_t i;
		uint32_t val[4], v1, v2;
		::memset(& stInfo, 0x00,sizeof(stInfo));
		uint32_t v=(uint32_t)(CRJDTimeTools::GetDTime());
		srand(v);
		for (i=0; i<4; i++) {
			v1=(uint32_t) rand();
			v2=(uint32_t) rand();
			val[i]=(uint32_t)((v1<<16)||
				v2);
		}
		memcpy(& stInfo.m_uchComp1, val, 16);
		for (i=0; i<4; i++) {
			v1=(uint32_t) rand();
			v2=(uint32_t) rand();
			val[i]=(uint32_t)((v1<<16)||
				v2);
		}
		memcpy(& stInfo.m_uchComp2, val, 16);
		if (bShort) {
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchKVV1, 8, stInfo.m_uchComp1);
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchKVV2, 8, stInfo.m_uchComp2);
			for (i=0; i<8; i++) {
				stInfo.m_uchPlain[i]=(uint8_t)(stInfo.m_uchComp1[i]^ stInfo.m_uchComp2[i]);
			}
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchKVVItself, 8, stInfo.m_uchPlain);
			*((uint64_t*) stInfo.m_uchItself)=*((uint64_t*) stInfo.m_uchMaster);
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchItself, 8, stInfo.m_uchPlain);
			*((uint64_t*) stInfo.m_uchMaster)=*((uint64_t*) stInfo.m_uchPlain);
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchMaster, 8, lmk);
		}
		else {
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchKVV1, 16, stInfo.m_uchComp1);
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchKVV2, 16, stInfo.m_uchComp2);
			for (i=0; i<16; i++) {
				stInfo.m_uchPlain[i]=(uint8_t)(stInfo.m_uchComp1[i]^ stInfo.m_uchComp2[i]);
			}
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchKVVItself, 16, stInfo.m_uchPlain);
			memcpy(stInfo.m_uchItself, stInfo.m_uchMaster, 16);
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchItself, 16, stInfo.m_uchPlain);
			memcpy(stInfo.m_uchMaster, stInfo.m_uchPlain, 16);
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchMaster, 16, lmk);
		}
		return true;
	}
	catch(...) {}
	return false;
}
bool CRJSecurityDesTools::CalcKVV(SRJSecurity24KeyInfo& stInfo,const uint8_t* lmk,bool bShort) {
	try {
		::memset(stInfo.m_uchKVVItself, 0x00,sizeof(stInfo.m_uchKVVItself));
		if (bShort) {
			CRJSecurityDesTools::DoDecryptDesBlock(stInfo.m_uchMaster, 8, lmk);
			CRJSecurityDesTools::DoEncryptDesBlock(stInfo.m_uchKVVItself, 8, stInfo.m_uchMaster);
		}
		else {
			CRJSecurityDesTools::DoDecryptTripleDesBlockSimple(stInfo.m_uchMaster, 16, lmk);
			CRJSecurityDesTools::DoEncryptTripleDesBlockSimple(stInfo.m_uchKVVItself, 16, stInfo.m_uchMaster);
		}
		return true;
	}
	catch(...) {}
	return false;
}
void CRJSecurityDesTools::BuildDesMac(uint8_t* dst,const uint8_t* key,const uint8_t* q, size_t len) {
	try {
		uint64_t s0, s1=0;
		while (len>=8) {
			s0=(*(uint64_t*) q);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptDes((uint8_t*)& s1,(uint8_t*)& s0, key);
			len-=8;
			q+=8;
		}
		if (len>0) {
			s0=0;
			_MEMMOVE_BYTE_POINTER(& s0, q, len);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptDes((uint8_t*)& s1,(uint8_t*)& s0, key);
		}
		CRJBytePacker::UnpackMachine((uint8_t*) dst,(uint8_t*)& s1, 8);
		dst[16]=0x00;
	}
	catch(...) {}
}
void CRJSecurityDesTools::BuildTripleDesMac(uint8_t* dst,const uint8_t* key,const uint8_t* q, size_t len) {
	try {
		uint64_t s0, s1=0;
		while (len>=8) {
			s0=(*(uint64_t*) q);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptDes((uint8_t*)& s1,(uint8_t*)& s0, key);
			len-=8;
			q+=8;
		}
		if (len>0) {
			s0=0;
			_MEMMOVE_BYTE_POINTER(& s0, q, len);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptDes((uint8_t*)& s1,(uint8_t*)& s0, key);
		}
		CRJSecurityDesTools::DoDecryptDes((uint8_t*)& s0,(uint8_t*)& s1,& key[8]);
		CRJSecurityDesTools::DoEncryptDes((uint8_t*)& s1,(uint8_t*)& s0, key);
		CRJBytePacker::UnpackMachine((uint8_t*) dst,(uint8_t*)& s1, 8);
		dst[16]=0x00;
	}
	catch(...) {}
}
void CRJSecurityDesTools::BuildTripleDesMacVariant(uint8_t* dst,const uint8_t* key,const uint8_t* q, size_t len) {
	try {
		uint64_t s0, s1=0;
		while (len>=8) {
			s0=(*(uint64_t*) q);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptTripleDes((uint8_t*)& s1,(uint8_t*)& s0, key);
			len-=8;
			q+=8;
		}
		if (len>0) {
			s0=0;
			_MEMMOVE_BYTE_POINTER(& s0, q, len);
			s0^=s1;
			CRJSecurityDesTools::DoEncryptTripleDes((uint8_t*)& s1,(uint8_t*)& s0, key);
		}
		CRJBytePacker::UnpackMachine((uint8_t*) dst,(uint8_t*)& s1, 8);
		dst[16]=0x00;
	}
	catch(...) {}
}
size_t CRJSecurityDesTools::GetKeyLength(size_t lOpts) {
	return theKeyLengths[lOpts& 1];
}

