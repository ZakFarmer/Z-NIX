SDKDIR=./sdk
VERSION=1

help:
	@echo "Z-NIX Makefile"
	@echo "Usage: make [ all | clean | help | build] " 
	@echo ""
	@echo "Version" $(VERSION)
	@echo

all: 
	@echo "Building Kernel"
	make -C ./kernel
	@echo "Building SDK"
	make -C ./sdk
	@echo "Building Userland"
	make -C ./userland
	

build:
	zip -r devos-$(VERSION).zip ./


emulate:
	@echo "Running Z-NIX in emulated mode"
	cd ./sdk && sudo ./diskimage.sh
	cd ./sdk && ./qemu.sh

clean:
	make -C ./kernel clean
	make -C ./userland clean
