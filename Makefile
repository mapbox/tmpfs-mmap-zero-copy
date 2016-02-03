default: all

BUILD_TYPE ?= Release
BUILD_DIR := build/${BUILD_TYPE}
ROOT_DIR := $(shell pwd)

${BUILD_DIR}/Makefile: CMakeLists.txt
	@mkdir -p ${BUILD_DIR}
	@cd ${BUILD_DIR} && cmake ${ROOT_DIR} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -G Ninja

all: ${BUILD_DIR}/Makefile
	@ninja -C ${BUILD_DIR}

watch:
	while inotifywait -e modify CMakeLists.txt Makefile *.cc; do make; done

clean:
	@${RM} -rf ${BUILD_DIR}

.PHONE: all watch clean
