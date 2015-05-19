#ifndef __API_FB__
#define __API_FB__

struct fb_info{
	unsigned int		w;	
	unsigned int		h;	
	char				bpp;	
	char				state;	
	unsigned int*		vmem;	
};

enum{
	FB_NOT_ACTIVE=0,
	FB_ACTIVE=1,
};

#define API_FB_IS_AVAILABLE			0x801
#define API_FB_GET_INFO				0x802	
#define API_FB_GET_BINFO			0x803
#define API_FB_SET_INFO				0x804


#endif
