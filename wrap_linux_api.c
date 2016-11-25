# include "common.h"

int Open(const char *device_name, int flag)
{
	int fd;

	fd = open(device_name, flag);
	if(0 > fd){
		fprintf(stderr, "open device %s Error:%s\n", device_name, strerror(errno));
		exit(1);
	}

	return fd;
}

void Ioctl_save(int fd, int request, fb_info *fb_inf)
{
	int val;
	struct fb_var_screeninfo fb_var;

	val = ioctl(fd, request, &fb_var);
	if(0 > val){
		fprintf(stderr, "ioctl Error :%s!\n", strerror(errno));
		exit(1);
	}

	fb_inf -> w = fb_var.xres;
	fb_inf -> h = fb_var.yres;
	fb_inf -> bpp = fb_var.bits_per_pixel;
	printf("w:%d, h:%d, bpp:%d\n", fb_inf->w, fb_inf->h, fb_inf->bpp);
}

void Mmap(fb_info *fb_inf, int fd)
{
	fb_inf ->fbmem = mmap(0, fb_inf->w * fb_inf->h * fb_inf->bpp / 8,
						  PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(fb_inf->fbmem == MAP_FAILED){
		fprintf(stderr, "fb mmap Error:%s\n", strerror(errno));
		exit(1);
	}
}

void Read(int fd, char *buf, int n)
{
	int val;

	val = read(fd, buf, n);
	if(val < 0){
		fprintf(stderr, "read Error:<%s>\n", strerror(errno));
		exit(1);
	}
}
