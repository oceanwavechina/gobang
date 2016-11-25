# ifndef __COMMON_H__
# define __COMMON_H__

# include <stdio.h>
# include <linux/fb.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/mman.h>
# include <unistd.h>
# include <math.h>
# include <time.h>

typedef unsigned int color_t;
extern const int direct[8][2];

/* ************ AI *******************  */
# define GAME_OVER -1
typedef struct search_line_case{
	int loc_1;
	int loc_middle;
	int loc_2;
	int n1;
	int n2;
	int the_direct;
}st_case;
extern st_case the_case[8];

extern int usr_points[150]; 
extern int usr_top;
extern int ai_points[150];  
extern int ai_top;

/* *********** grid_info_set struct ********  */
# define START_X 300
# define START_Y 100
# define GRID_SPACE 40
# define GRID_LEN 560
# define GRID_COLOR 0x00ff00
# define GRID_MAX_POINTS 225

typedef struct grid{
	int start_x;
	int start_y;
	int grid_space;
	color_t color;
	int grid_len;
}grid_info;


/* **************** grid_position struct ********  */

typedef struct grid_position{
	int x;
	int y;
	int flag;
}grid_pos;

/* the direction follows:		 0
 *							  1	   7
 *							2	loc	 6
 *							  3	   5		
 *								 4
 *
 */

/* ************** fb_info struct ***************** */
typedef struct fb_information{
	int w;
	int h;
	int bpp;
	char *fbmem;
}fb_info;

#define FB_DEVICE "/dev/fb0"

/* ************** mouse ****************** */
# define T 0xeeeeee
# define B 0xffffff
# define X 0x000000
# define M_ROW 17
# define M_COLLUM 11


/* ******************** wrap_linux_api.c **************  */
extern int Open(const char *device_name, int flag);
extern void Ioctl_save(int fd, int request, fb_info *fb_inf);
extern void Mmap(fb_info *fb_inf, int fd);
extern void Read(int fd, char *buf, int n);

/* ********************* init_fb.c ******************* */
extern int init_fb(fb_info *fb_inf);

/* ******************** draw.c *********************** */
extern inline int pixel(const fb_info *fb_inf, int x, int y, color_t color);
extern inline int row(const fb_info *fb_inf, int x, int y, color_t color, int len);
extern inline int collum(const fb_info *fb_inf, int x, int y, color_t color, int len);
extern int grid(const fb_info *fb_inf, int x, int y, int space, color_t color, int len);
extern int back_ground(const fb_info *fb_inf, color_t color);
extern int fb_hollow_circle(const fb_info *fb_inf, int x0, int y0, int r, color_t color);
extern int tip(fb_info *fb_inf);

/* ************************** grid_info.c *********************************  */
extern grid_pos pos[225];

extern int grid_info_set(grid_info *grid_inf);
extern int get_points_position(const grid_info *grid_inf);
extern int is_inside_grid(grid_info *gird_inf, int x, int y);
extern int is_on_boundry(grid_info *grid_inf, int x, int y);
extern int locate_position(grid_info *grid_inf, int x, int y);

/* ************************ mouse.c *******************  */
extern int mouse(grid_pos *cur_pos, fb_info *fb_inf, grid_info *grid_inf);
extern int draw_mouse(const fb_info *fb_inf, int x, int y, color_t tmp[M_ROW][M_COLLUM]);
extern int left_key_action(fb_info *fb_inf, grid_info *grid_inf, int x, int y);
extern int right_key_action(fb_info *fb_inf, grid_info *grid_inf, int x, int y);

/* *********************** AI_FIR.c ********************  */
extern int ai_fir(fb_info *fb_inf, grid_info *grid_inf);
extern int help(fb_info *fb_inf, grid_info *grid_inf);

/* ********************** check.c *************** */


extern int check(fb_info *fb_inf, grid_info *grid_inf, int loc, int flag);
#endif
