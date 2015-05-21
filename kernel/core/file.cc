
#include <os.h>


static void strreplace(char* s,char a,char to){
	if (s==NULL)
		return;
	while (*s){
		if (*s==a)
			*s=to;
		s++;
	}
}


u32	File::inode_system=0;	

// File constructor
File::File(char* n,u8 t){
	name=(char*)kmalloc(strlen(n)+1);
	memset(name,0,strlen(n));
	memcpy(name,n,strlen(n));
	name[strlen(n)]=0;
	
	checkName();
	master=arch.pcurrent;	
	inode=inode_system;
	inode_system++;
	size=0;
	type=t;
	parent=NULL;
	child=NULL;
	next=NULL;
	prec=NULL;
	link=NULL;
	map_memory=NULL;
}

// File destructor
File::~File(){
	kfree(name);
		
	if (prec==NULL){
		parent->setChild(next);
		next->setPrec(NULL);
	}
	else if (next==NULL){
		prec->setNext(NULL);
	}
	else if (next==NULL && prec==NULL){
		parent->setChild(NULL);
	}
	else{
		io.print("prec (%s) next is now %s\n",prec->getName(),next->getName());
		io.print("next (%s) prec is now %s\n",next->getName(),prec->getName());
		prec->setNext(next);
		next->setPrec(prec);
	}

	File* n=child;
	File* nn=NULL;
	while (n!=NULL){
		//io.print("delete %s \n",n->getName());
		nn=n->getNext();
		delete n;
		n=nn;
	}
	
}

#define CAR_REPLACE '_'

// Check name of files in filesystem
void File::checkName(){
	//Adapte le nom
	strreplace(name,'/',CAR_REPLACE);
	strreplace(name,'\ ',CAR_REPLACE);
	strreplace(name,'?',CAR_REPLACE);
	strreplace(name,':',CAR_REPLACE);
	strreplace(name,'>',CAR_REPLACE);
	strreplace(name,'<',CAR_REPLACE);
	strreplace(name,'*',CAR_REPLACE);
	strreplace(name,'"',CAR_REPLACE);
	strreplace(name,':',CAR_REPLACE);
}

// Add child of a file
u32 File::addChild(File* n){
	if (!n){
		return PARAM_NULL;
	}
	n->setParent(this);
	n->setPrec(NULL);
	n->setNext(child);
	if (child != NULL)
		child->setPrec(n);
	child=n;
	return RETURN_OK;
}

// Create child of a file
File*	File::createChild(char* n,u8 t){
	File* fp=new File(n,t);
	addChild(fp);
	return fp;
}

// Get the parent of a file
File*	File::getParent(){
	return parent;
}

// Get the child of a file
File*	File::getChild(){
	return child;
}

// Get the next file in the filesystem
File*	File::getNext(){
	return next;
}

// Get the previous file in the filesystem
File*	File::getPrec(){
	return prec;
}

// Get linked file aliases
File*	File::getLink(){
	return link;
}

// Get size of file in filesystem
u32	File::getSize(){
	return size;
}

// Get INODEs
u32	File::getInode(){
	return inode;
}

// Scan for files in filesystem
void File::scan(){

}

//Returns a list of files in the directory, defined.
File * File::getFilesInDir(char* dir)
{
	return NULL;
}

// Set type of file in filesystem
void	File::setType(u8 t){
	type=t;
}

// Set size of file in filesystem
void	File::setSize(u32 t){
	size=t;
}

// Set parent of file in filesystem
void	File::setParent(File* n){
	parent=n;
}

// Set linked aliases of file in filesystem
void	File::setLink(File* n){
	link=n;
}

// Set child of file in filesystem
void	File::setChild(File* n){
	child=n;
}

// Set next file of file in filesystem
void	File::setNext(File* n){
	next=n;
}

// Set previous file of file in filesystem
void	File::setPrec(File* n){
	prec=n;
}

// Set name of file in filesystem
void	File::setName(char* n){
	kfree(name);
	name=(char*)kmalloc(strlen(n));
	memcpy(name,n,strlen(n));
	checkName();
}

// Get filetype of file in filesystem
u8	File::getType(){
	return type;
}

// Get name of file in filesystem
char* File::getName(){
	return name;
}

// Find files in filesystem
File* File::find(char* n){
	File* fp=child;
	while (fp!=0){
		if (!strcmp(fp->getName(),n))
			return fp;
		
		fp=fp->next;
	}
	return NULL;
}

// Open file in filesystem
u32	File::open(u32 flag){
	return NOT_DEFINED;
}

// Close file in filesystem
u32	File::close(){
	return NOT_DEFINED;
}

// Read from file in filesystem
u32	File::read(u32 pos,u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Write to file in filesystem
u32	File::write(u32 pos,u8* buffer,u32 size){
	return NOT_DEFINED;
}

// Initialise IO between filesystem
u32	File::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

// Remove file in filesystem
u32	File::remove(){
	delete this;
	return NOT_DEFINED;
}

// Get status of file in filesystem
stat_fs File::stat(){
	stat_fs st;
	
	return st;
}

// Get NMAP of filesystem
u32 File::mmap(u32 sizee,u32 flags,u32 offset,u32 prot){
	if (map_memory!=NULL){
		int i=0;
		unsigned int adress;
		struct page *pg;
		process_st* current=(arch.pcurrent)->getPInfo();
		for (i=0;i<sizee;i++){
				adress=(unsigned int)(map_memory+i*PAGESIZE);
				//io.print("mmap : %x %d\n",adress,sizee);
				pg = (struct page *) kmalloc(sizeof(struct page));
				pg->p_addr = (char*) (adress);
				pg->v_addr = (char *) (adress & 0xFFFFF000);
				list_add(&pg->list, &current->pglist);
				pd_add_page(pg->v_addr, pg->p_addr, PG_USER, current->pd);
		}
		return (u32)map_memory;
	}
	else{
		return -1;
	}
}

