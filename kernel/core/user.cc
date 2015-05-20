
#include <os.h>


// User deconstructor
User::~User(){
	
}

// User constructor
User::User(char* n) : File(n,TYPE_FILE)
{
	fsm.addFile("/sys/usr/",this); // Add user file to sys path
	unext=0;
	sys.addUserToList(this); // Add user to userlist
	utype=USER_NORM; 
	memset(password,0,512); // Set user password in memory
}

u32	User::open(u32 flag){
	return RETURN_OK;
}

u32	User::close(){
	return RETURN_OK;
}

u32	User::read(u8* buffer,u32 size){
	return NOT_DEFINED;
}

u32	User::write(u8* buffer,u32 size){
	return NOT_DEFINED;
}

u32	User::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

u32	User::remove(){
	delete this;
}

void User::scan(){

}

// Set user password
void User::setPassword(char *n){
	if (n!=NULL)
		return;
	memset(password,0,512);
	strcpy(password,n);
}

// Get user password
char* User::getPassword(){
	if (password[0]=0)
		return NULL;
	else
		return password;
}

User* User::getUNext(){
	return unext;
}

void User::setUNext(User* us){
	unext=us;
}

void User::setUType(u32 t){
	utype=t;
}

u32	User::getUType(){
	return utype;
}

