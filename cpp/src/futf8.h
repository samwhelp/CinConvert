#ifndef FUTF8_H
#define FUTF8_H

#include <cstdio>
#include <cstdint>
#include <vector>

#define UNIEOF 0xFFFFFFFF
#define UNIERROR 0xFFFFFFFE

uint32_t fgetUTF8c(FILE* fp);
void fputUTF8s(FILE *fp,std::vector<uint32_t> str);
int GetLines(FILE *fp);

#endif
