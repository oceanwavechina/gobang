# include "common.h"

const int direct[8][2] = {{0, -GRID_SPACE}, {-GRID_SPACE, -GRID_SPACE}, {-GRID_SPACE, 0}, {-GRID_SPACE, GRID_SPACE}, 
					   {0, GRID_SPACE},  {GRID_SPACE, GRID_SPACE}, {GRID_SPACE, 0},  {GRID_SPACE, -GRID_SPACE}};

int check(fb_info *fb_inf, grid_info *grid_inf, int loc, int flag)
{
	int i_direct, n;
	int loc_t, x_t, y_t;

	for(i_direct = 0; i_direct < 4; i_direct++){
		loc_t = loc;
		n = 0;
		while(is_inside_grid(grid_inf, pos[loc_t].x, pos[loc_t].y)){
			if(flag == pos[loc_t].flag){
				n++;
				x_t = pos[loc_t].x + direct[i_direct][0];
				y_t = pos[loc_t].y + direct[i_direct][1];
				loc_t = locate_position(grid_inf, x_t, y_t);
				if(loc_t < 0){
					break;
				}
			}else{
				break;
			}
		}

		loc_t = loc;
		while(is_inside_grid(grid_inf, pos[loc_t].x, pos[loc_t].y)){
			if(flag == pos[loc_t].flag){
				n++;
				x_t = pos[loc_t].x + direct[i_direct + 4][0];
				y_t = pos[loc_t].y + direct[i_direct + 4][1];
				loc_t = locate_position(grid_inf, x_t, y_t);
				if(loc_t < 0){
					break;
				}
			}else{
				break;
			}
		}
		
		if((--n) >= 5){
			if(1 == flag){
				printf("you win!\n");
				return GAME_OVER;
			}
			if(2 == flag){
				printf("computer win!\n");
				return GAME_OVER;
			}
		}
	}

	return 0;
}
