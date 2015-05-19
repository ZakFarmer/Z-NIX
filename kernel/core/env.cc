#include <os.h>

Variable::~Variable(){
	if (value!=NULL)
		kfree(value);
}

Variable::Variable(char* n,char* v) : File(n,TYPE_FILE)
{
	fsm.addFile("/sys/env/",this);
	if (v!=NULL){
		io.print("env: create %s (%s) \n",n,v);
		value=(char*)kmalloc(strlen(v)+1);
		memcpy(value,v,strlen(v)+1);
		setSize(strlen(v)+1);
	}
	else{
		value=NULL;
	}
}

u32	Variable::open(u32 flag){
	return RETURN_OK;
}

u32	Variable::close(){
	return RETURN_OK;
}

u32	Variable::read(u32 pos,u8* buffer,u32 size){
	if (value==NULL)
		return NOT_DEFINED;
	else{
		strncpy((char*)buffer,value,size);
		return size;		
	}
}

u32	Variable::write(u32 pos,u8* buffer,u32 size){
	if (value!=NULL)
		kfree(value);
	value=(char*)kmalloc(size+1);
	memset((char*)value,0,size+1);
	memcpy(value,(char*)buffer,size+1);
	value[size]=0;	
	setSize(size+1);
	return size;
}

u32	Variable::ioctl(u32 id,u8* buffer){
	return NOT_DEFINED;
}

u32	Variable::remove(){
	delete this;
	return NOT_DEFINED;
}

void Variable::scan(){

}

