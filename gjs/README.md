# CinConvert (gjs版)

## 來源

原始版本是根據「[CinConvert](https://github.com/ren1244/CinConvert)」這個專案來修改的。

原始作者的相關討論，可以參考「[這篇](https://www.ubuntu-tw.org/modules/newbb/viewtopic.php?post_id=354600#forumpost354600)」和「[這篇](https://www.ubuntu-tw.org/modules/newbb/viewtopic.php?post_id=354546#forumpost354546)」。

## 修改目的

將「[網頁版](https://github.com/ren1244/CinConvert/blob/master/FileRead.js)」修改成「[gjs](https://wiki.gnome.org/Projects/Gjs)版」。

## 修改摘要

* 主要修改的是讀寫檔案的部份。
原本的網頁版是使用「async read」的方式。
而這個修改的版本，則是使用「sync read」的方式。


## 開發環境

* Xubuntu 16.04 (64位元) 英文界面
* [gjs](http://packages.ubuntu.com/xenial/gjs) 1.44.0-1ubuntu1


### 安裝「gjs」

執行下面指令，安裝「[gjs](http://packages.ubuntu.com/xenial/gjs)」

``` sh
$ sudo apt-get install gjs
```

執行下面指令，觀看安裝的版本

``` sh
$ dpkg -l gjs
```

顯示

```
Desired=Unknown/Install/Remove/Purge/Hold
| Status=Not/Inst/Conf-files/Unpacked/halF-conf/Half-inst/trig-aWait/Trig-pend
|/ Err?=(none)/Reinst-required (Status,Err: uppercase=bad)
||/ Name                          Version             Architecture        Description
+++-=============================-===================-===================-================================================================
ii  gjs                           1.44.0-1ubuntu1     amd64               Mozilla-based javascript bindings for the GNOME platform

```


## 操作步驟

請依照下面順序操作。

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
$ ./cinconvert.js
```

執行完畢後會顯示一些訊息，並且產生一個檔案「CnsPhonetic.cin」。


### 觀看「CnsPhonetic.cin」

執行下面指令，觀看「CnsPhonetic.cin」這個檔的內容

``` sh
$ less CnsPhonetic.cin
```

也可以執行下面指令，計算「CnsPhonetic.cin」這個檔的行數。

``` sh
$ wc -l CnsPhonetic.cin
```


## 相關專案

* [util-cns11643-to-cin-gjs](https://github.com/samwhelp/util-cns11643-to-cin-gjs.git)
