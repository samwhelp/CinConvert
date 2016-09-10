//讀取檔案，確認正確性，建立對映表，提供查詢函數(CNS->UNICODE)
#ifndef CNSUNI_H
#define CNSUNI_H

#include <cstdio>
#include <sstream>
#include <vector>
#include <cstdint>

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

#endif

