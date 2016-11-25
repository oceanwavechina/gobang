# include "common.h"

/*  
 *  1. a globle var:pos[100] is declared(in  grid_info.c ) to save point position;  
 */

int main(void)
{
	fb_info fb_inf;
	grid_info grid_inf;

	/* framebuffer init  */
	init_fb(&fb_inf);

	/*  draw grid & get point position */
	grid_info_set(&grid_inf);
	grid(&fb_inf,grid_inf.start_x, grid_inf.start_y, grid_inf.grid_space, grid_inf.color, grid_inf.grid_len);
	get_points_position(&grid_inf);
	tip(&fb_inf);

	/* draw circle with mouse */
	grid_pos cur_pos;
	mouse(&cur_pos, &fb_inf, &grid_inf);

	printf("success!\n");
	return 0;
}
