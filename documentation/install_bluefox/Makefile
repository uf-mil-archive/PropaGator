all: installed

BUILD_DIR   = build
TARBALL_PREFIX = mvVirtualDevice-x86_64_ABI2-2.4.2
TARBALL     = $(BUILD_DIR)/$(TARBALL_PREFIX).tgz
TARBALL_URL = http://www.matrix-vision.com/mvvirtualdevice-442.html?file=tl_files/mv11/support/mvIMPACT_Acquire/01/$(TARBALL_PREFIX).tgz
UNPACK_CMD  = tar zxf
SOURCE_DIR  = $(BUILD_DIR)/mvIMPACT_acquire-x86_64-2.4.2
MD5SUM_FILE = $(TARBALL_PREFIX).tgz.md5sum

include $(shell rospack find mk)/download_unpack_build.mk

installed: $(SOURCE_DIR)/unpacked
	cd $(SOURCE_DIR) && make
	cd $(rospack find ccny_mvVirtualDevice) 
	mkdir -p ./include/ccny_mvVirtualDevice
	mkdir -p ./bin
	cp -r $(SOURCE_DIR)/DriverBase ./include/
	cp -r $(SOURCE_DIR)/mvDeviceManager ./include/
	cp -r $(SOURCE_DIR)/mvIMPACT_CPP ./include/ccny_mvVirtualDevice/
	cp -r $(SOURCE_DIR)/mvPropHandling ./include/
	mkdir -p ./lib
	cp $(SOURCE_DIR)/lib/x86_64/* ./lib
	sudo cp $(SOURCE_DIR)/lib/x86_64/* /usr/local/lib
	sudo ldconfig
	cp $(SOURCE_DIR)/apps/mvPropView/x86_64/wxPropView ./bin	
	touch installed

clean:
	rm -rf $(CURDIR)/lib $(CURDIR)/include $(CURDIR)/bin installed 
	sudo rm -rf /usr/local/lib/libmv* /usr/local/lib/libwxPropGrid*

wipe: clean
	rm -rf $(SOURCE_DIR)
	rm -rf build
