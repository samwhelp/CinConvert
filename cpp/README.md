# CinConvert (c++版)

## 來源

原始版本是從這篇「[回覆](https://www.ubuntu-tw.org/modules/newbb/viewtopic.php?post_id=354524#forumpost354524)」的附檔，下載下來的。

原始作者的「[GitHub](https://github.com/ren1244)」，可以參考這篇「[回覆](https://www.ubuntu-tw.org/modules/newbb/viewtopic.php?post_id=354600#forumpost354600)」。

這個專案就是clone原始作者的「GitHub / [CinConvert](https://github.com/ren1244/CinConvert)」過來的。


下載原始版本

``` sh
$ wget -c 'https://www.ubuntu-tw.org/modules/newbb/dl_attachment.php?attachid=1471522084&post_id=354524' -O CinConvert1.0.7z
```

解壓縮

``` sh
$ 7z x CinConvert1.0.7z
```

執行

``` sh
$ tree CinConvert1.0
```

顯示

```
CinConvert1.0
├── CinConvert.exe
├── data
│   ├── CNS2UNICODE_Unicode 15.txt
│   ├── CNS2UNICODE_Unicode 2.txt
│   ├── CNS2UNICODE_Unicode BMP.txt
│   ├── CNS_phonetic.txt
│   └── other_phonetic.txt
├── log.txt
├── phonetic.cin
├── setting.txt
├── src
│   ├── CNS_phon.cpp
│   ├── CNS_phon.h
│   ├── CNS_Uni.cpp
│   ├── CNS_Uni.h
│   ├── futf8.cpp
│   ├── futf8.h
│   ├── main.cpp
│   ├── main_full.cpp
│   ├── makefile
│   ├── obj
│   └── 編譯說明.txt
└── 說明.txt

3 directories, 20 files
```

## 修改目的

修改成在Linux環境底下也能編譯。

## 修改摘要

* 在Linux上因為檔名有分大小寫，所以把檔名大小寫相關的修正，才能執行make編譯，檔名有修正過的，主要是跟「CNS_Phon」「CNS_Uni」這兩個字相關的地方。

* 修改 main.cpp 這個檔的 「int ReadSettingFile(FileList& fs1,FileList& fs2)」，因為原始的「setting.txt」這個檔的斷行是使用「"\r\n"」，所以讀每行，然後做比對的時候，不是預期的結果，只要是因為還會殘留「\r」，所以對「ReadSettingFile」做了修改。


## 開發環境

* Xubuntu 16.04 (64位元) 英文界面

執行

``` sh
$ g++ --version
```

顯示

```
g++ (Ubuntu 5.4.0-6ubuntu1~16.04.2) 5.4.0 20160609
Copyright (C) 2015 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

## 操作步驟

請依照下面順序操作。

### 編譯

執行下面指令，切換到「src」資料夾。

``` sh
$ cd src
```

執行下面指令編譯。

``` sh
$ make
```

執行下面指令，切回到「README.md(本文件)」所在的那個資料夾。

``` sh
$ cd ..
```

### 下載資源檔

執行下面指令，下載資源檔。

``` sh
$ ./download.sh
```

主要會產生

* data/CNS_phonetic.txt
* data/CNS2UNICODE_Unicode_2.txt
* data/CNS2UNICODE_Unicode_15.txt
* data/CNS2UNICODE_Unicode_BMP.txt

註：原本就有存在下面這個檔

* data/CNS_phonetic.txt

### 轉檔

執行下面指令，會執行轉檔的動作。

``` sh
$ ./CinConvert.exe
```

執行完畢後，會產生一個檔案「phonetic.cin」。


### 觀看「phonetic.cin」

執行下面指令，觀看「phonetic.cin」這個檔的內容

``` sh
$ less phonetic.cin
```

也可以執行下面指令，計算「phonetic.cin」這個檔的行數。

``` sh
$ wc -l phonetic.cin
```
