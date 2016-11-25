# include "common.h"

int grid_info_set(grid_info *grid_inf)
{
	/* attention: len must be 9 times of space */
	grid_inf -> start_x = START_X;
	grid_inf -> start_y = START_Y;
	grid_inf -> grid_space = GRID_SPACE;
	grid_inf -> color = GRID_COLOR;
	grid_inf -> grid_len = GRID_LEN;

	return 0;
}


grid_pos pos[];
int get_points_position(const grid_info *grid_inf)
{
	int i;
	int tmp_x = grid_inf -> start_x;
	int tmp_y = grid_inf -> start_y;

	for(i = 0; i < GRID_MAX_POINTS; i++){
		if(0 == i % ((grid_inf->grid_len / grid_inf->grid_space) + 1) && 0 != i){
			tmp_y += grid_inf -> grid_space;
			tmp_x = grid_inf -> start_x;
		}
		pos[i].x = tmp_x;
		pos[i].y = tmp_y;
		pos[i].flag = 0;
		tmp_x += grid_inf -> grid_space;
	}

	return 0;
}

int is_inside_grid(grid_info *grid_inf, int x, int y)
{
	if((x >= grid_inf->start_x && x <= grid_inf->start_x + grid_inf->grid_len) &&
	   (y >= grid_inf->start_y && y <= grid_inf->start_y + grid_inf->grid_len)){
		return 1;
	}
	
	return 0;
}

int is_on_boundry(grid_info *grid_inf, int x, int y)
{
	if((x == grid_inf->start_x || x == grid_inf->start_x + grid_inf->grid_len) ||
	   (y == grid_inf->start_y || y == grid_inf->start_y + grid_inf->grid_len)){
		return 1;
	}

	return 0;
}

int locate_position(grid_info *grid_inf, int x, int y)
{
	int i, n_x, n_y;
	int tmp_x, tmp_y;

	if(0 == is_inside_grid(grid_inf, x, y)){
		return -1;
	}

	n_x = (x - grid_inf->start_x) / grid_inf->grid_space;
	i = (x - grid_inf->start_x) % grid_inf->grid_space;
	if(i < (grid_inf->grid_space / 2)){
		tmp_x = grid_inf->start_x + n_x * grid_inf->grid_space;
	}else{
		tmp_x = grid_inf->start_x + (++n_x) * grid_inf->grid_space;
	}

	n_y = (y - grid_inf->start_y) / grid_inf->grid_space;
	i = (y - grid_inf->start_y) % grid_inf->grid_space;
	if(i < (grid_inf->grid_space / 2)){
		tmp_y = grid_inf->start_y + n_y * grid_inf->grid_space;
	}else{
		tmp_y = grid_inf->start_y + (++n_y) * grid_inf->grid_space;
	}

//	printf("after locate x : %d, y : %d\n", tmp_x, tmp_y);
	
	n_y *= 15;

	return n_x + n_y;
#if 0
	for(i = 0; i < 100; i++){
		if(tmp_x == pos[i].x && tmp_y == pos[i].y){
			return i;
		}
	}
#endif	
}
