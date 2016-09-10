#ifndef CNSPHON_H
#define CNSPHON_H

#include <cstdio>
#include <cstdint>
#include <string>
#include "CNS_Uni.h"

#define is_hex_ch(ch) (('0'<=(ch) && (ch)<='9')||('A'<=(ch) && (ch)<='F'))

class CnsPhonData
{
public:
	uint32_t phon;
	uint32_t uni;
	uint32_t cns;
	
	CnsPhonData();
	CnsPhonData(uint32_t phonetic,uint32_t unicode,uint32_t CNS);
	CnsPhonData(const CnsPhonData& CPD);
	char* phon_str(char* buf); //buf>=5 elements
};

bool CnsPhonCmp_PC(CnsPhonData a,CnsPhonData b);
bool CnsPhonCmp_PU(CnsPhonData a,CnsPhonData b);


int GetCinFile(std::vector<CnsPhonData>& CPT,CnsUniTable& CUT,std::vector<std::string>& src_fname,const char* log_fname,uint32_t newLine);

#endif
