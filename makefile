CXX = g++
DBG = -g
OUTOPT = -o 
LD = ${CXX}
CXXFLAGS = ${DBG}
LDFLAGS = ${DBG}
PIPELINE_OBJS = pipeline.o stage.o fetch.o decode.o execute.o memaccess.o writeback.o instruction.o
ALL_BINARIES = pipeline

all: ${ALL_BINARIES}

stage.o: stage.cpp stage.h fetch.h decode.h execute.h memaccess.h writeback.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

fetch.o: fetch.cpp fetch.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

decode.o: decode.cpp decode.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

execute.o: execute.cpp execute.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

memaccess.o: memaccess.cpp memaccess.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

writeback.o: writeback.cpp writeback.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

instruction.o: instruction.cpp instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

pipeline.o: pipeline.cpp writeback.h memaccess.h execute.h decode.h fetch.h stage.h instruction.h
	${CXX} -c ${CXXFLAGS} ${OUTOPT}$@ $<

pipeline: ${PIPELINE_OBJS}
	${LD} ${LDFLAGS} ${OUTOPT}$@  ${PIPELINE_OBJS}

## cleaning
clean:
	rm -rf *.o ${ALL_BINARIES}
