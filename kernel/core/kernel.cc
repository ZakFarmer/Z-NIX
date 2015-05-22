// Z-NIX Kernel
// Created by Zak Farmer

// Include core header files
#include <os.h>
#include <boot.h>


static char* init_argv[2]={"init","-i"};

// Load core system modules function
static void load_modules(multiboot_info* mbi){
	if (mbi->mods_count>0){
		u32 initrd_location = *((u32*)mbi->mods_addr);
		u32 initrd_end = *(u32*)(mbi->mods_addr+4);
		u32	initrd_size=initrd_end-initrd_location;
		io.print(" >load module:  location=%x, size=%d \n",initrd_location,initrd_end-initrd_location);
		int i=0;
		unsigned int adress;
	
		for (i=0;i<(initrd_size/4072)+1;i++){	
				adress=(initrd_location+i*4096);
				vmm.kmap(adress,adress);
		}
		execv_module(initrd_location,1,init_argv);
	}
}

extern "C" void kmain(multiboot_info* mbi){
	io.clear();
	// Print kernel information
	io.print("%s - %s -- %s %s \n",	KERNEL_NAME,
									KERNEL_VERSION,
									KERNEL_DATE,
									KERNEL_TIME);
	
	// Print kernel licence (not needed yet, not yet defined)
	io.print("%s \n",KERNEL_LICENCE);
	arch.init();
	
	// Load VMem management
	io.print("Loading Virtual Memory Management... \n");
	vmm.init(mbi->high_mem);
	
	// Load FS management
	io.print("Loading FileSystem Management... \n");
	fsm.init();
	
	// Load syscalls interface
	io.print("Loading syscalls interface... \n");
	syscall.init();
	
	// Load system core
	io.print("Loading system... \n");
	sys.init();
	
	// Load system modules
	io.print("Loading modules... \n");
	modm.init();
	modm.initLink();
	
	// Print clarification of kernel boot
	io.print("Kernel booted... \n");
	
	// Mount disks to filesystem
	modm.install("hda0","module.dospartition",0,"/dev/hda");
	modm.install("hda1","module.dospartition",1,"/dev/hda");
	modm.install("hda2","module.dospartition",2,"/dev/hda");
	modm.install("hda3","module.dospartition",3,"/dev/hda");
	modm.mount("/dev/hda0","boot","module.ext2",NO_FLAG);

	arch.initProc();
	
	// Load extra binary modules for system operation
	io.print("Loading binary modules \n");
	load_modules(mbi);
	
	fsm.link("/mnt/boot/bin/","/bin/");

	// Print ready message (if system initialised correctly)
	io.print("\n");
	io.print("==== Z-NIX ==== System is ready (%s - %s) ==== \n",KERNEL_DATE,KERNEL_TIME); // Print the date and time inside brackets
	io.print("==== Z-NIX is in development mode ====");
	// Enable interrupts to handle errors nicely without damaging the OS or system itself
	arch.enable_interrupt();
	for (;;);
	// If system is halted before shutdown command is run, run shutdown on the kernel before damage is done
	arch.shutdown();
}

