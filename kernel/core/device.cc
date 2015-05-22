
#include <os.h>


// Device deconstructor
Device::~Device(){
	
}

// Device constructor
Device::Device(char* n) : File(n,TYPE_DEVICE)
{
	fsm.addFile("/dev",this);
}

// Open connection to device
u32	Device::open(u32 flag){
	return NOT_DEFINED;
}

// Close connection to device
u32	Device::close(){
	return NOT_DEFINED;
}

// Read from a device connection
u32	Device::read(u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Write to a device connection
u32	Device::write(u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Initialise IO to device connection
u32	Device::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

// Remove device connection
u32	Device::remove(){
	delete this;
}

// Scan for devices
void Device::scan(){

}

