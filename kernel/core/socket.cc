
#include <os.h>

// Socket deconstructor
Socket::~Socket(){
	
}

// Socket constructor
Socket::Socket(char* n) : File(n,TYPE_FILE)
{
	fsm.addFile("/sys/sockets/",this);
}

// Open IO socket
u32	Socket::open(u32 flag){
	return RETURN_OK;
}

// Close IO socket
u32	Socket::close(){
	return RETURN_OK;
}

// Read from IO socket
u32	Socket::read(u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Write to IO socket
u32	Socket::write(u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Initialise IO to sockets
u32	Socket::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

// Remove IO socket from listing
u32	Socket::remove(){
	delete this;
}

// Scan for IO sockets
void Socket::scan(){

}

