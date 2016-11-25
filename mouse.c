# include "common.h"

static void get_pos_handle_boundry(int *x, int *y, fb_info *fb_inf, char buf[3]);
static void save_screen(const fb_info *fb_inf, int x, int y, color_t tmp[M_ROW][M_COLLUM]);
static void write_back_screen(fb_info *fb_inf, int x, int y, color_t tmp[M_ROW][M_COLLUM]);

unsigned int mouse_pixel[M_ROW][M_COLLUM] = 
{
	{B,T,T,T,T,T,T,T,T,T,T},
	{B,B,T,T,T,T,T,T,T,T,T},
	{B,X,B,T,T,T,T,T,T,T,T},
	{B,X,X,B,T,T,T,T,T,T,T},
	{B,X,X,X,B,T,T,T,T,T,T},
	{B,X,X,X,X,B,T,T,T,T,T},
	{B,X,X,X,X,X,B,T,T,T,T},
	{B,X,X,X,X,X,X,B,T,T,T},
	{B,X,X,X,X,X,X,X,B,T,T},
	{B,X,X,X,X,X,X,X,X,B,T},
	{B,X,X,X,X,X,B,B,B,B,B},
	{B,X,X,B,X,X,B,T,T,T,T},
	{B,X,B,T,B,X,X,B,T,T,T},
	{B,B,T,T,B,X,X,B,T,T,T},
	{B,T,T,T,T,B,X,B,T,T,T},
	{T,T,T,T,T,B,B,B,T,T,T},
};

int mouse(grid_pos *cur_pos, fb_info *fb_inf, grid_info *grid_inf)
{
	int fd, loc, val, help_loc;
	char buf[3];
	int x = 500, y = 365;
	color_t tmp[M_ROW][M_COLLUM];

	fd = Open("/dev/input/mice", O_RDONLY);
s:	draw_mouse(fb_inf, x, y, tmp);

	while(1){
		Read(fd, buf, 3);
		write_back_screen(fb_inf, x, y, tmp);
		get_pos_handle_boundry(&x, &y, fb_inf, buf);

		if(1 == (buf[0] & 1)){
			Read(fd, buf, 3);
			if(0 == (buf[0] & 1)){
				loc = left_key_action(fb_inf, grid_inf, x, y);
				if(-1 == loc) {
					goto s;
				}
				if(-2 == loc){
					help_loc = help(fb_inf, grid_inf);
					printf("need help:loc = %d!\n", help_loc);
					if(-1 != help_loc){
						fb_hollow_circle(fb_inf, pos[help_loc].x, pos[help_loc].y, 10, 0xff0000);
						pos[help_loc].flag = 1;
						loc = help_loc;
						sleep(1);
					}else{
						printf("help error!\n");
					}
				}
				usr_points[++usr_top] = loc;
				val = check(fb_inf, grid_inf, loc, 1);
				if(GAME_OVER == val){
					printf("game over!\n");
					return -1;
				}
				loc = ai_fir(fb_inf, grid_inf);
				ai_points[++ai_top] = loc;
				val = check(fb_inf, grid_inf, loc, 2);
				if(GAME_OVER == val){
					printf("game_over!\n");
					return -1;
				}
			}
		}

		draw_mouse(fb_inf, x, y, tmp);
	}

	close(fd);
	return 0;
}

int left_key_action(fb_info *fb_inf, grid_info *grid_inf, int x, int y)
{
	int i_loc;

	if(x > fb_inf->w-40 && x < fb_inf->w && y > 0 && y < 40){
		return -2;
	}

	i_loc = locate_position(grid_inf, x, y);
	if(i_loc < 0 || 0 != pos[i_loc].flag){
		printf("wrong position!\n");
		return -1;
	}

	fb_hollow_circle(fb_inf, pos[i_loc].x, pos[i_loc].y, 10, 0xff0000);
	pos[i_loc].flag = 1;

//	printf("left key pressed!\n");

	return i_loc;
}


int right_key_action(fb_info *fb_inf, grid_info *grid_inf, int x, int y)
{
	int i_loc;

	i_loc = locate_position(grid_inf, x, y);
	if(i_loc < 0 || 0 != pos[i_loc].flag){
		printf("wrong position!\n");
		return -1;
	}
	fb_hollow_circle(fb_inf, pos[i_loc].x, pos[i_loc].y, 10, 0x0000ff);
	pos[i_loc].flag = 1;

	printf("right key pressed!\n");
	return i_loc;
}

int draw_mouse(const fb_info *fb_inf, int x, int y, unsigned int tmp[M_ROW][M_COLLUM])
{
	int i, j;
	
	save_screen(fb_inf, x, y, tmp);
	for(j = 0; j < M_ROW; j++){
		for(i = 0; i < M_COLLUM; i++){
			if(mouse_pixel[j][i] != T){
				*((color_t *)fb_inf->fbmem + (y+j)*fb_inf->w + x +i) = mouse_pixel[j][i];
			}
		}
	}

	return 0;
}

static void save_screen(const fb_info *fb_inf, int x, int y, color_t tmp[M_ROW][M_COLLUM])
{
	int i, j;
	color_t tmp_color;

	for(j = 0; j < M_ROW; j++){
		for(i = 0; i < M_COLLUM; i++){
			tmp_color = *((color_t *)fb_inf->fbmem + (y+j)*fb_inf->w + x +i);
			tmp[j][i] = tmp_color;
		}
	}
}


static void write_back_screen(fb_info *fb_inf, int x, int y, color_t tmp[M_ROW][M_COLLUM])
{
	int i, j;
	color_t tmp_color;

	for(j = 0; j < M_ROW; j++){
		for(i = 0; i < M_COLLUM; i++){
			tmp_color = tmp[j][i];
			*((color_t *)fb_inf->fbmem + (y+j)*fb_inf->w + x +i) = tmp_color;
		}
	}
}

static void get_pos_handle_boundry(int *x, int *y, fb_info *fb_inf, char buf[3])
{

	*x += buf[1];
	*y += -buf[2];
	if(*x < 0){
		*x = 0;
	}
	if(*x >= fb_inf->w - M_COLLUM){
		*x = fb_inf->w - (M_COLLUM + 1);
	}
	if(*y < 0){
		*y = 0;
	}
	if(*y >= fb_inf->h - M_ROW){
		*y = fb_inf->h - (M_ROW + 1);
	}
//	printf("x = %d, y = %d\n", *x, *y);
}

