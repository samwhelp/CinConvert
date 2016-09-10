//futf8.h
#include <cstdio>
#include <cstdint>
#include <vector>

#define UNIEOF 0xFFFFFFFF
#define UNIERROR 0xFFFFFFFE

uint32_t fgetUTF8c(FILE* fp);
void fputUTF8s(FILE *fp,std::vector<uint32_t> str);
int GetLines(FILE *fp);

//CNS_Uni.h
#include <cstdio>
#include <sstream>
#include <vector>

#define is_hex_ch(ch) (('0'<=(ch) && (ch)<='9')||('A'<=(ch) && (ch)<='F'))
#define on_check_error do{ \
	if(err++) \
		error_log<<"Line "<<L++ <<"\n"; \
	else \
		error_log<<filename<<" error at:\nLine "<<L++ <<"\n"; \
	for(;ch!=EOF && ch!='\n';ch=fgetc(fp)); \
	st=0;count=0; \
}while(0)

class CnsUniData
{
public:
	uint32_t Cns;
	uint32_t Uni;
	CnsUniData();
	CnsUniData(const CnsUniData& CUD);
	CnsUniData(uint32_t cns,uint32_t uni);
};
bool CnsUniDataCmp(CnsUniData a,CnsUniData b);
bool UniCnsCmp(CnsUniData a,CnsUniData b);
class CnsUniTable
{
private:
public:
	//變數
	std::stringstream error_log; //儲存錯誤
	std::vector<CnsUniData> table;
	//方法
	int check(FILE *fp,const char* filename); //回傳>=0，無誤，資料總數；-1，有錯誤
public:
	CnsUniTable();
	int ReadFiles(std::vector<std::string> &filenames); //讀取檔案
	const std::string GetErrorLog()const; //回傳錯誤資訊，若無，回傳空字串
	uint32_t GetUnicode(uint32_t cns); //回傳查詢的UNICODE，若無回傳0
};

//CNS_phon.h
#include <cstdio>
#include <cstdint>
#include <string>

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

//main.cpp
#include <algorithm>
#include <cstdio>
#include <vector>
#include <cstdint>
#include <cstring>
#include <string>

typedef std::vector<std::string> FileList;

int ReadSettingFile(FileList& fs1,FileList& fs2)
{
	FILE *fp;
	char ch;
	int f=0;
	std::string ss;
	fp=fopen("setting.txt","r");
	if(fp==NULL)
		return -1;
	do
	{
		ss="";
		for(ch=fgetc(fp);ch!='\n' && ch!=EOF;ch=fgetc(fp))
			ss+=ch;
		if(ss.size()>0)
		{
			if(ss=="#CNS-phonetic:")
				f=1;
			else if(ss=="#CNS-unicode:")
				f=2;
			else
			{
				if(f==1)
					fs1.push_back(ss);
				else if(f==2)
					fs2.push_back(ss);
				else
					return -2;
			}
		}
	}while(ch!=EOF);
	fclose(fp);
	return 0;
}

int main()
{
	//從setting.txt讀取檔案名稱
	FileList fs1;
	FileList fs2;
	if(ReadSettingFile(fs1,fs2)<0)
	{
		printf("error\n");
		return 0;
	}
	if(fs1.size()==0 || fs2.size()==0)
	{
		printf("empty\n");
		return 0;
	}
	printf("[1]%d\n",fs1.size());
	for(int i=0;i<fs1.size();++i)
		printf("%s\n",fs1[i].c_str());
	printf("[2]%d\n",fs2.size());
	for(int i=0;i<fs2.size();++i)
		printf("%s\n",fs2[i].c_str());
	
	const char *head_of_file="\
%gen_inp\n\
%ename english_name\n\
%cname 中文名稱\n\
%selkey 0123456789\n\
%keyname begin\n\
, ㄝ\n\
- ㄦ\n\
. ㄡ\n\
/ ㄥ\n\
0 ㄢ\n\
1 ㄅ\n\
2 ㄉ\n\
3 ˇ\n\
4 ˋ\n\
5 ㄓ\n\
6 ˊ\n\
7 ˙\n\
8 ㄚ\n\
9 ㄞ\n\
; ㄤ\n\
a ㄇ\n\
b ㄖ\n\
c ㄏ\n\
d ㄎ\n\
e ㄍ\n\
f ㄑ\n\
g ㄕ\n\
h ㄘ\n\
i ㄛ\n\
j ㄨ\n\
k ㄜ\n\
l ㄠ\n\
m ㄩ\n\
n ㄙ\n\
o ㄟ\n\
p ㄣ\n\
q ㄆ\n\
r ㄐ\n\
s ㄋ\n\
t ㄔ\n\
u ㄧ\n\
v ㄒ\n\
w ㄊ\n\
x ㄌ\n\
y ㄗ\n\
z ㄈ\n\
%keyname end\n\
%endkey 3467\n\
%chardef begin\n\
";
	const char *end_of_file="%chardef end";
	CnsUniTable CUT;
	
	int k=CUT.ReadFiles(fs2);
	std::string err=CUT.GetErrorLog();
	if(err.size()!=0)
	{
		printf("%s",err.c_str());
		return 0;
	}
	std::vector<CnsPhonData> CPT;
	char buf[8];
	int x=GetCinFile(CPT,CUT,fs1,"log.txt",0x0A);
	if(x!=0)
		printf("%d",x);
	//檢查是不是有重覆的UNICODE-PHON
	/*for(int k=0,n=CPT.size();k<n;++k)
	{
		printf("%X %X %X %s\n",CPT[k].phon,CPT[k].cns,CPT[k].uni,CPT[k].phon_str(buf));
	}*/
	stable_sort(CPT.begin(),CPT.end(),CnsPhonCmp_PU);
	
	for(int i=0,k=0,r,n=CPT.size();k<n;k+=r)
	{//i:寫入位置,k:讀取位置
		for(r=k+1;r<n && CPT[r].uni==CPT[k].uni && CPT[r].phon==CPT[k].phon;++r);
		if(r-k>1)
			printf("%X %X %X %s\n",CPT[k].phon,CPT[k].cns,CPT[k].uni,CPT[k].phon_str(buf));
		//CPT[i++]=CPT[k];
	}
	stable_sort(CPT.begin(),CPT.end(),CnsPhonCmp_PC);
	FILE *fp;
	std::vector<uint32_t> v;
	fp=fopen("phonetic.cin","wb");
	if(fp==NULL)
		return 0;
	for(int i=0,n=strlen(head_of_file),rt=0x0A0D;i<n;++i)
	{
		if(head_of_file[i]!='\n')
			fwrite(head_of_file+i,1,1,fp);
		else
			fwrite(&rt,1,2,fp);
	}
	for(int i=0,n=CPT.size();i<n;++i)
	{
		v.clear();
		CPT[i].phon_str(buf);
		for(int j=0,m=strlen(buf);j<m;++j)
			v.push_back(buf[j]);
		v.push_back('\t');
		v.push_back(CPT[i].uni);
		v.push_back(0x0D);
		v.push_back(0x0A);
		fputUTF8s(fp,v);
		//printf("%s\t%X\n",CPT[i].phon.c_str(),CPT[i].uni);
	}
	for(int i=0,n=strlen(end_of_file),rt=0x0A0D;i<n;++i)
	{
		if(end_of_file[i]!='\n')
			fwrite(end_of_file+i,1,1,fp);
		else
			fwrite(&rt,1,2,fp);
	}
	fclose(fp);
	return 0;
}


//futf8.cpp
#include <vector>
#include <cstdint>
#include <cstdio>
uint32_t fgetUTF8c(FILE* fp)
{
	char ch=fgetc(fp);
	uint32_t r,t;
	int i,n;
	if(ch==EOF)
		return UNIEOF;
	if(ch>0)
		return ch;
	r=*(unsigned char*)&ch;
	if((r>>5)==6)
	{
		n=1;
		r&=0x1F;
	}
	else if((r>>4)==14)
	{
		n=2;
		r&=0x0F;
	}
	else if((r>>3)==30)
	{
		n=3;
		r&=0x07;
	}
	else
		return UNIERROR;
	for(i=0;i<n;++i)
	{
		ch=fgetc(fp);
		t=*(unsigned char*)&ch;
		if(ch==EOF || (t>>6)!=2)
			return UNIERROR;
		r=((r<<6)|(t&0x3F));
	}
	return r;
}
void fputUTF8s(FILE *fp,std::vector<uint32_t> str)
{
	uint32_t uc;
	int i,n;
	for(i=0,n=str.size();i<n;++i)
	{
		uc=str[i];
		if(uc<0x80)
			fwrite(&uc,1,1,fp);
		else if(uc<0x800)
		{
			uc=uc<<8&0x3F00|uc>>6&0x1F|0x80C0;
			//printf("[%X]",uc);
			fwrite(&uc,1,2,fp);
		}
		else if(uc<0x10000)
		{
			uc=uc<<16&0x3F0000|uc<<2&0x3F00|uc>>12&0xF|0x8080E0;
			fwrite(&uc,1,3,fp);
		}
		else
		{
			uc=uc<<24&0x3F000000|uc<<10&0x3F0000|uc>>4&0x3F00|uc>>18&0x7|0x808080F0;
			fwrite(&uc,1,4,fp);
		}
	}
}
int GetLines(FILE *fp)
{
	int count=1;
	uint32_t uc;
	while((uc=fgetUTF8c(fp))!=UNIEOF)
		if(uc=='\n')
			++count;
	return count;
}

//CNS_phon.cpp
#include <cstdio>
#include <cstdint>
#include <string>

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

//CNS_Uni.cpp
//讀取檔案，確認正確性，建立對映表，提供查詢函數(CNS->UNICODE)

#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>


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

