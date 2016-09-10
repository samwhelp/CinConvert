#include "CNS_Phon.h"
#include "CNS_Uni.h"
#include "futf8.h"
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
