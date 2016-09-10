//讀取檔案，確認正確性，建立對映表，提供查詢函數(CNS->UNICODE)

#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstdint>
#include "futf8.h"
#include "CNS_Uni.h"


CnsUniData::CnsUniData():Cns(0),Uni(0){}
CnsUniData::CnsUniData(const CnsUniData& CUD):Cns(CUD.Cns),Uni(CUD.Uni){}
CnsUniData::CnsUniData(uint32_t cns,uint32_t uni):Cns(cns),Uni(uni){}

CnsUniTable::CnsUniTable()
{
}
int CnsUniTable::check(FILE *fp,const char* filename)
{
	char ch;
	int st=0,count=0;
	int err=0,L=1;
	int nData=0;
	while((ch=fgetc(fp))!=EOF)
	{
		switch(st)
		{
			case 0:
				//printf("Line:%d\tst:%d\tcount:%d\n",L,st,count);
				if(is_hex_ch(ch) && count<2)
					++count;
				else if(ch=='\t' && count>=1 && count<=2)
				{
					count=0;
					++st;
				}
				else
					on_check_error;
				break;
			case 1:
				//printf("Line:%d\tst:%d\tcount:%d\n",L,st,count);
				if(is_hex_ch(ch) && count<4)
					++count;
				else if(ch=='\t' && count==4)
				{
					count=0;
					++st;
				}
				else
					on_check_error;
				break;
			case 2:
				//printf("Line:%d\tst:%d\tcount:%d\n",L,st,count);
				if(is_hex_ch(ch) && count<5 )
					++count;
				else if(ch=='\n' && 4<=count && count<=5)
				{
					count=0;
					st=0;
					++L;
					++nData;
				}
				else
					on_check_error;
				break;
			default:
				break;
		}
	}
	return err?-err:nData;
}
int CnsUniTable::ReadFiles(std::vector<std::string> &filenames)
{
	std::vector<FILE*> fps;
	FILE *fp;
	int nTotal=0,n;
	for(int i=0;i<filenames.size();++i)
	{
		fp=fopen(filenames[i].c_str(),"r");
		if(fp==NULL)
		{
			error_log<<"File "<<filenames[i]<<" open error\n";
			for(int j=0;j<fps.size();++j)
				fclose(fps[j]);
			return -1;
		}
		n=check(fp,filenames[i].c_str());
		if(n<0)
		{
			fclose(fp);
			for(int j=0;j<fps.size();++j)
				fclose(fps[j]);
			return -1;
		}
		nTotal+=n;
		fps.push_back(fp);
	}
	if(nTotal>0)
	{
		table.reserve(nTotal);
		uint32_t a,b,c;
		for(int i=0,k;i<fps.size();++i)
		{
			fseek(fps[i],0,SEEK_SET);
			k=fscanf(fps[i],"%x %x %x",&a,&b,&c);
			while(k==3)
			{
				table.push_back(CnsUniData(a<<16|b,c));
				k=fscanf(fps[i],"%x %x %x",&a,&b,&c);
			}
		}
		stable_sort(table.begin(),table.end(),CnsUniDataCmp);
		for(int i=1;i<table.size();++i)
		{
			if(table[i].Cns==table[i-1].Cns)
			{
				char buf[16];
				sprintf(buf,"%X\t%04X",table[i].Cns>>16,table[i].Cns&0xFFFF);
				error_log<<"the same Cns:"<<buf<<"\n";;
			}
		}
	}
	for(int j=0;j<fps.size();++j)
		fclose(fps[j]);
	return nTotal;
}
const std::string CnsUniTable::GetErrorLog()const
{
	return error_log.str();
}
uint32_t CnsUniTable::GetUnicode(uint32_t cns)
{
	std::vector<CnsUniData>::iterator it;
	it=lower_bound(table.begin(),table.end(),CnsUniData(cns,0),CnsUniDataCmp);
	int k=it-table.begin();
	return table[k].Cns==cns?table[k].Uni:0;
}

bool CnsUniDataCmp(CnsUniData a,CnsUniData b)
{
	return a.Cns==b.Cns?(a.Uni<b.Uni):(a.Cns<b.Cns);
}
bool UniCnsCmp(CnsUniData a,CnsUniData b)
{
	return a.Uni==b.Uni?(a.Cns<b.Cns):(a.Uni<b.Uni);
}



