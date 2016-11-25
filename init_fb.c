# include "common.h"

int init_fb(fb_info *fb_inf)
{
	int fd;
	
	fd = Open(FB_DEVICE, O_RDWR);

	Ioctl_save(fd, FBIOGET_VSCREENINFO, fb_inf);
	
	Mmap(fb_inf, fd);

	close(fd);

	return 0;
}

