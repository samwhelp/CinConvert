#include <vector>
#include <cstdint>
#include <cstdio>
#include "futf8.h"
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
