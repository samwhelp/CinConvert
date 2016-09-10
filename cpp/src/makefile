obj_dir=obj/
exe_dir=../
#target
main:$(obj_dir)futf8.o $(obj_dir)cns_uni.o $(obj_dir)cns_phon.o main.cpp
	g++ -o $(exe_dir)CinConvert.exe $(obj_dir)futf8.o $(obj_dir)cns_uni.o $(obj_dir)cns_phon.o main.cpp -std=c++11 -static
clean:
	rm obj/*.o
#obj files
$(obj_dir)futf8.o:futf8.cpp futf8.h
	g++ -c -o $(obj_dir)futf8.o futf8.cpp -std=c++11
$(obj_dir)cns_uni.o:CNS_Uni.cpp CNS_Uni.h futf8.h
	g++ -c -o $(obj_dir)CNS_Uni.o CNS_Uni.cpp -std=c++11
$(obj_dir)cns_phon.o:CNS_Phon.cpp CNS_Phon.h CNS_Uni.h futf8.h
	g++ -c -o $(obj_dir)cns_phon.o CNS_Phon.cpp -std=c++11
