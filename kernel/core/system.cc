
#include <os.h>
#include <boot.h>

// System constructor
System::System(){
	
}

// System destructor
System::~System(){

}


void System::init(){
	// Declare system path
	var=fsm.path("/sys/env/");

	// Declare root user and set as superuser permissions
	root=new User("root");
	root->setUType(USER_ROOT);
	
	// Create another user with default permissions, used for permission testing
	actual=new User("liveuser");

	
	// Declare system env variables
	uservar=new Variable("USER","liveuser"); // Standard user
	new Variable("OS_NAME",KERNEL_NAME); // Name of the kernel
	new Variable("OS_VERSION",KERNEL_VERSION); // Kernel version
	new Variable("OS_DATE",KERNEL_DATE); // Kernel date
	new Variable("OS_TIME",KERNEL_TIME); // Kernel time
	new Variable("OS_LICENCE",KERNEL_LICENCE); // Kernel license key
	new Variable("COMPUTERNAME",KERNEL_COMPUTERNAME); // Computer name
	new Variable("PROCESSOR_IDENTIFIER",KERNEL_PROCESSOR_IDENTIFIER); // ID of the CPU
	new Variable("PROCESSOR_NAME",arch.detect()); // CPU architecture
	new Variable("PATH","/bin/"); // PATH dir
	new Variable("SHELL","/bin/sh"); // SHELL dir
}

// User login function
int	System::login(User* us,char* pass){
	if (us==NULL)
		return ERROR_PARAM;
	if (us->getPassword() != NULL){
		
		if (pass==NULL)	
			return PARAM_NULL;
			//
		if (strncmp(pass,us->getPassword(),strlen(us->getPassword())))
			return RETURN_FAILURE;
		//io.print("login %s with %s (%s)\n",us->getName(),pass,us->getPassword());
	}
		
	uservar->write(0,(u8*)us->getName(),strlen(us->getName()));
	actual=us;
	return RETURN_OK;
}

// Get system variables, used to return vars to any class that requires them
char* System::getvar(char* name){
	char* varin;
	File* temp=var->find(name);
	if (temp==NULL){
		return NULL;
	}
	varin=(char*)kmalloc(temp->getSize());
	memset(varin,0,temp->getSize());
	temp->read(0,(u8*)varin,temp->getSize());
	return varin;
}

// Get user details, used to return details to any class that requires them
User* System::getUser(char* nae){
	User* us=listuser;
	while (us!=NULL){
		//io.print("test '%s' with '%s'\n",nae,us->getName());
		if (!strncmp(nae,us->getName(),strlen(us->getName())))
			return us;
		us=us->getUNext();
	}
	return NULL;
}

// Add users to userlist
void System::addUserToList(User* us){
	if (us==NULL)
		return;
	us->setUNext(listuser);
	listuser=us;
}

// Check if user is root on the system
u32 System::isRoot(){
	if (actual!=NULL){
		if (actual->getUType() == USER_ROOT)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

/* Base kernel panic function (some of it is not actual code but you get the idea)
extern C void System::panic(int panicCode){
	if (panicCode == 1){
		io.print("Testing connection to kernel panic function call...");
		io.print("Successful!");
	}
	else if (panicCode == 2){
		io.print("PANICKING!");
		vmm.close();
		fsm.close();
		syscall.close();
		sys.close();
		modm.close();
		arch.close();
		arch.shutdown();
		if (!arch.shutdown()){
			io.print("Unable to shutdown, running CPU halt command. Could damage the system but it is the only way.");
			asm("hlt");
		}
	}
}
*/
