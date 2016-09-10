#include <cstdio>
#include <cstdint>
#include <string>
#include "CNS_Phon.h"
#include "futf8.h"
#include "CNS_Uni.h"

CnsPhonData::CnsPhonData():uni(0){}
CnsPhonData::CnsPhonData(uint32_t phonetic,uint32_t unicode,uint32_t CNS):phon(phonetic),uni(unicode),cns(CNS){}
CnsPhonData::CnsPhonData(const CnsPhonData& CPD):phon(CPD.phon),uni(CPD.uni),cns(CPD.cns){}

int GetCinFile(std::vector<CnsPhonData>& CPT,CnsUniTable& CUT,std::vector<std::string>& src_fname,const char* log_fname,uint32_t newLine)
{
	FILE *tmp,*dest,*log;
	std::vector<FILE*> src;
	for(int i=0,n=src_fname.size();i<n;++i)
	{
		if((tmp=fopen(src_fname[i].c_str(),"r"))==NULL)
		{
			for(int j=0,m=src.size();j<m;++j)
				fclose(src[j]);
			return i;
		}
		src.push_back(tmp);
	}
	if((log=fopen(log_fname,"wb"))==NULL)
	{
		for(int j=0,m=src.size();j<m;++j)
			fclose(src[j]);
		return -1;
	}

	int i,k,f,shift;
	uint32_t uc,cns,uni;
	uint32_t phon;
	std::vector<uint32_t> str;

	k=0;
	for(int j=0,m=src.size();j<m;++j)
	{
		k+=GetLines(src[j]);
		fseek(src[j],0,SEEK_SET);
	}
	CPT.reserve(k);

	//printf("{%d}",src.size);
	for(int j=0,m=src.size();j<m;++j)
	{
		while(1)
		{
			str.clear();
			cns=0;
			for(uc=fgetUTF8c(src[j]);is_hex_ch(uc);uc=fgetUTF8c(src[j]))
			{
				cns=cns<<4|('0'<=uc && uc<='9'?uc-'0':uc-'A'+10);
				str.push_back(uc);
			}
			k=str.size();
			if(k<1 || k>2 || uc!='\t')
			{
				for(;uc!=UNIEOF && uc!='\n';uc=fgetUTF8c(src[j]))
					str.push_back(uc);
				str.push_back('\n');
				fputUTF8s(log,str);
				if(uc==UNIEOF)
					break;
				else
					continue;
			}
			str.push_back(uc);
			for(uc=fgetUTF8c(src[j]);is_hex_ch(uc);uc=fgetUTF8c(src[j]))
			{
				cns=cns<<4|('0'<=uc && uc<='9'?uc-'0':uc-'A'+10);
				str.push_back(uc);
			}
			if(str.size()-k!=5 || uc!='\t' || (uni=CUT.GetUnicode(cns))==0)
			{
				for(;uc!=UNIEOF && uc!='\n';uc=fgetUTF8c(src[j]))
					str.push_back(uc);
				str.push_back('\n');
				fputUTF8s(log,str);
				continue;
			}
			str.push_back(uc);
			k=f=0;
			phon=0;
			shift=24;
			for(uc=fgetUTF8c(src[j]);uc!=UNIEOF && uc!='\n';uc=fgetUTF8c(src[j]))
			{
				str.push_back(uc);
				if(uc==0x02d9 && k==0) //輕聲
				{
					k=1;
					f=1;
				}
				else if(0x3105<=uc && uc<=0x3119 && k<2) //ㄅ到ㄙ
				{
					k=2;
					phon|=(uc-0x3105+5)<<(shift-=6);
				}
				else if(0x3127<=uc && uc<=0x3129 && k<3) //ㄧ到ㄩ
				{
					k=3;
					phon|=(uc-0x3127+39)<<(shift-=6);
				}
				else if(0x311A<=uc && uc<=0x3126 && k<4) //ㄚ到ㄦ
				{
					k=4;
					phon|=(uc-0x311A+26)<<(shift-=6);
				}
				else if((uc==0x02d9 || uc==0x02ca || uc==0x02c7 || uc==0x02cb) && k<5 && f==0) //二到四聲
				{
					k=5;
					phon|=(uc==0x02ca?3:uc==0x02c7?2:uc==0x02d9?1:4)<<(shift-=6);
				}
				else
				{
					for(uc=fgetUTF8c(src[j]);uc!=UNIEOF && uc!='\n';uc=fgetUTF8c(src[j]))
						str.push_back(uc);
						str.push_back(0x0D);
						str.push_back(0x0A);
					fputUTF8s(log,str);
					k=-1;
					break;
				}
			}
			if(k>=0 && uni>0)
			{
				if(f==1)
					phon|=1<<(shift-=6);
				CPT.push_back(CnsPhonData(phon,uni,cns));
			}
		}
	}
	for(int j=0,m=src.size();j<m;++j)
		fclose(src[j]);
	fclose(log);
	return 0;
}
char* CnsPhonData::phon_str(char* buf)
{
	const char *P="73641qaz2wsxedcrfv5tgbyhn8ik,9ol.0p;/-ujm";
	int n=0,shift=18;
	for(;shift>=0 && (phon>>shift&0x3F);shift-=6)
		buf[n++]=P[(phon>>shift&0x3F)-1];
	buf[n]='\0';
	return buf;
}
bool CnsPhonCmp_PC(CnsPhonData a,CnsPhonData b)
{
	return a.phon==b.phon?a.cns<b.cns:a.phon<b.phon;
}
bool CnsPhonCmp_PU(CnsPhonData a,CnsPhonData b)
{
	return a.phon==b.phon?a.uni<b.uni:a.phon<b.phon;
}
