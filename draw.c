# include "common.h"

inline int pixel(const fb_info *fb_inf, int x, int y, color_t color)
{
	*((int *)fb_inf->fbmem + y * fb_inf->w + x) = color;

	return 0;
}

inline int row(const fb_info *fb_inf, int x, int y, color_t color, int len)
{
	int i;

	for(i = 0; i < len; i++){
		pixel(fb_inf, x+i, y, color);
	}
	return 0;
}

inline int collum(const fb_info *fb_inf, int x, int y, color_t color, int len)
{
	int j;
	
	for(j = 0; j < len; j++){
		pixel(fb_inf, x, y+j, color);
	}

	return 0;
}

int grid(const fb_info *fb_inf, int x, int y, int space, color_t color, int len)
{
	int k;
	int tmp_y = y;
/*
	if(0 != len % space || 9 != len / space){
		printf("len must be 9 times of width!\n");
		exit(0);
	}
*/
	for(k = 0; k < len / space+1; k++){
		if(y > fb_inf->h){
			printf("wrong position!\n");
			exit(0);
		}
		row(fb_inf, x, y, color, len);
		y += space;
	}
	y = tmp_y;
	for(k = 0; k < len/space+1; k++){
		if(x > fb_inf->w){
			printf("wrong position!\n");
			exit(0);
		}
		collum(fb_inf, x, y, color, len);
		x += space;
	}

	return 0;
}

int tip(fb_info *fb_inf)
{
	
	row(fb_inf, fb_inf->w - 40, 40, 0xcccccc, 39);
	collum(fb_inf, fb_inf->w - 40, 0, 0xcccccc, 39);
	collum(fb_inf, fb_inf->w-1 , 0, 0xcccccc, 39);

	return 0;
}

int back_ground(const fb_info *fb_inf, color_t color)
{
	int j;

	for(j = 0; j < fb_inf->h; j++){
			row(fb_inf, 0, j, color, fb_inf->w);
	}

	return 0;
}

static inline int fb_pixel_symmetric(const fb_info *fb_inf, int x0, int y0, int x, int y, color_t color)
{
	pixel(fb_inf, x0+x, y0+y, color);
	pixel(fb_inf, x0+x, y0-y, color);
	pixel(fb_inf, x0-x, y0+y, color);
	pixel(fb_inf, x0-x, y0-y, color);

	collum(fb_inf, x0-x, y0-y, color, 2*y);
	collum(fb_inf, x0+x, y0-y, color, 2*y);

//	getchar();
	return 0;
}

static inline int circle_symmetric(const fb_info *fb_inf, int x0, int y0, int x, int y, color_t color)
{
	fb_pixel_symmetric(fb_inf, x0, y0, x, y, color);
	fb_pixel_symmetric(fb_inf, x0, y0, y, x, color);

	return 0;
}

int fb_hollow_circle(const fb_info *fb_inf, int x0, int y0, int r, color_t color)
{
	int x = 0;
	int y = r;
	int p = 3 - 2*r;
/*
	if(fb_inf->h - x0 < r || x0 < r){
		printf("wrong position!\n");
		return 1;
	}
*/
	while(x < y){
		circle_symmetric(fb_inf, x0, y0, x, y, color);
		if(p < 0){
			p += 4*x + 6;
		}else{
			p += 4*(x - y) + 10;
			y--;
		}
		x++;
	}
	if(x == y){
		circle_symmetric(fb_inf, x0, y0, x, y, color);
	}

	return 0;
}
