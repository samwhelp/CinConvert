#!/usr/bin/env bash

# 下載「CNS11643中文標準交換碼全字庫(簡稱全字庫)」
# http://data.gov.tw/node/5961
# http://data.gov.tw/node/gov/resource/27234
# http://www.cns11643.gov.tw/

## 執行下載
wget -c http://www.cns11643.gov.tw/AIDB/Open_Data.zip

## 刪除資料夾「Open_Data」(若已經存在的話)
rm ./Open_Data -rf

## 解壓縮
unzip -O big5 Open_Data.zip

## 建立data資料夾
mkdir ./data -p

## 複製需要的檔案到「data」資料夾

cp Open_Data/Properties/CNS_phonetic.txt data/CNS_phonetic.txt
cp 'Open_Data/MapingTables/Unicode/CNS2UNICODE_Unicode 2.txt' data/CNS2UNICODE_Unicode_2.txt
cp 'Open_Data/MapingTables/Unicode/CNS2UNICODE_Unicode 15.txt' data/CNS2UNICODE_Unicode_15.txt
cp 'Open_Data/MapingTables/Unicode/CNS2UNICODE_Unicode BMP.txt' data/CNS2UNICODE_Unicode_BMP.txt
