PORT=COM6
BOARD=arduino:avr:nano:cpu=atmega328old
BUILD_DIR=build

all: compile 

compile:
	arduino-cli compile --fqbn ${BOARD} --output-dir ${BUILD_DIR}

upload: compile
	arduino-cli upload --input-dir ${BUILD_DIR} -p ${PORT} --fqbn ${BOARD}

clean:
	rmdir /Q /S ${BUILD_DIR}
