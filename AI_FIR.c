# include "common.h"																				

st_case the_case[8];
int usr_points[150]; 
int usr_top = -1;
int defence_priority;

int ai_points[150];  
int ai_top = -1;
int charge_priority;

/* save the points around the point that just put */
int search_line(grid_info *grid_inf, int loc, int flag);
int double_sides_search(grid_info *grid_inf, int loc_1, int loc_2, int flag, int the_direct);

int defence(fb_info *fb_inf, grid_info *grid_inf, int enemy_flag, int my_flag);
int charge(fb_info *fb_inf, grid_info *grid_inf, int enemy_flag, int my_flag);

/* different cases */
int four_or_above(fb_info *fb_inf, grid_info *grid_inf, int flag);
int double_sides_three(fb_info *fb_inf, grid_info *grid_inf, int flag);
int double_sides_two(fb_info *fb_inf, grid_info *grid_inf, int flag);

/* main function of AI */
int ai_fir(fb_info *fb_inf, grid_info *grid_inf)
{
	int loc_defence, loc_charge, loc_t;
	int enemy_flag = 1, my_flag = 2;

	/* init the priority */
	defence_priority = -1;
	charge_priority = -1;

	loc_defence = defence(fb_inf, grid_inf, enemy_flag, my_flag);
	loc_charge = charge(fb_inf, grid_inf, enemy_flag, my_flag);
	printf("loc_defence = %d, loc_charge = %d!\n", loc_defence, loc_charge);

	/* defence or charge, depend on the priority */
	if(-1 == loc_defence){
		loc_t = loc_charge;
	}else{
		defence_priority > charge_priority ? (loc_t = loc_defence) : (loc_t = loc_charge);
	}
	
	if(loc_t > -1 && loc_t < GRID_MAX_POINTS && 0 == pos[loc_t].flag){
		fb_hollow_circle(fb_inf, pos[loc_t].x, pos[loc_t].y, 10, 0xcccccc);
		pos[loc_t].flag = 2;
		return loc_t;
	}

	printf("loc_t = %d!\n", loc_t);
	printf("AI Error!\n");
	return -1;
}

int help(fb_info *fb_inf, grid_info *grid_inf)
{

	int loc_defence, loc_charge, loc_t;
	int enemy_flag = 2, my_flag = 1;

	/* init the priority */
	defence_priority = -1;
	charge_priority = -1;

	loc_defence = defence(fb_inf, grid_inf, enemy_flag, my_flag);
	loc_charge = charge(fb_inf, grid_inf, enemy_flag, my_flag);
	printf("loc_defence = %d, loc_charge = %d!\n", loc_defence, loc_charge);

	/* defence or charge, depend on the priority */
	if(-1 == loc_defence){
		loc_t = loc_charge;
	}else{
		defence_priority > charge_priority ? (loc_t = loc_defence) : (loc_t = loc_charge);
	}
	
	if(loc_t > -1 && loc_t < GRID_MAX_POINTS && 0 == pos[loc_t].flag){
		return loc_t;
	}

	printf("loc_t = %d!\n", loc_t);
	printf("AI Error!\n");
	return -1;

}

int defence(fb_info *fb_inf, grid_info *grid_inf, int enemy_flag, int my_flag)
{
	int loc_t, i;  
	int *stack, top = -1;

	if(1 == enemy_flag){
		stack = usr_points;
		top = usr_top;
	}else if(2 == enemy_flag){
		stack = ai_points;
		top = ai_top;
	}

	/* handle the case of four-or-above in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], enemy_flag);
		loc_t = four_or_above(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			defence_priority = 4;
			return loc_t;
		}
	}

	/* handle the case of double-sides-three in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], enemy_flag);
		loc_t = double_sides_three(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			defence_priority = 3;
			return loc_t;
		}
	}

	/* handle the case of double-sides-two in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], enemy_flag);
		loc_t = double_sides_two(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			defence_priority = 2;
			return loc_t;
		}
	}
	
	return -1;
}

#if 1
int charge(fb_info *fb_inf, grid_info *grid_inf, int enemy_flag, int my_flag)
{
	int i, j;
	int loc_t, x_t, y_t;
	int *stack, top = -1;

	if(2 == enemy_flag){
		stack = usr_points;
		top = usr_top;
	}else if(1 == enemy_flag){
		stack = ai_points;
		top = ai_top;
	}

	/* handle the case of the first time */
	if(-1 == top){
		if(0 == pos[112].flag){
			loc_t = 112;
		}else{
			loc_t = 113;
		}
		charge_priority = 0;
		return loc_t;
	}

	/* handle the case of four-or-above in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], my_flag);
		loc_t = four_or_above(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			charge_priority = 4;
			return loc_t;
		}
	}

	/* handle the case of double-sides-three in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], my_flag);
		loc_t = double_sides_three(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			charge_priority = 3;
			return loc_t;
		}
	}

	/* handle the case of double-sides-two in a line */
	for(i = top; i > -1; i--){
		search_line(grid_inf, stack[i], my_flag);
		loc_t = double_sides_two(fb_inf, grid_inf, my_flag);
		if(-1 != loc_t){
			charge_priority = 2;
			return loc_t;
		}
	}
	
	/* handle the normal case */
	for(j = top; j > -1; j--){
		for(i = 0; i < 8; i++){
			loc_t = stack[j];
			x_t = pos[loc_t].x + direct[i][0];
			y_t = pos[loc_t].y + direct[i][1];
			loc_t = locate_position(grid_inf, x_t, y_t);
			if(-1 != loc_t && 0 == pos[loc_t].flag){
				charge_priority = 1;
				return loc_t;
			}
		}
	}
	
	for(loc_t = 0; loc_t < GRID_MAX_POINTS; loc_t++){
		if(0 == pos[loc_t].flag){
			charge_priority = 1;
			return loc_t;	
		}
	}
	
	printf("charge Error!\n");
	return -1;
}
#endif

int four_or_above(fb_info *fb_inf, grid_info *grid_inf, int flag)
{	
	int max = -1, i, j = -1, loc_t = -1;

	for(i = 0; i < 8; i++){
		if(max < the_case[i].n1 + the_case[i].n2){
			max = the_case[i].n1 + the_case[i].n2;
			j = i;
		}
	}

	if(max >= 4){
		if(0 == pos[the_case[j].loc_middle].flag){
			loc_t = the_case[j].loc_middle;
		}else if(0 == pos[the_case[j].loc_1].flag){
			loc_t = the_case[j].loc_1;
		}else if(0 == pos[the_case[j].loc_2].flag){
			loc_t = the_case[j].loc_2;
		}

	}	
	return loc_t;
}

int double_sides_three(fb_info *fb_inf, grid_info *grid_inf, int flag)
{
	int i, loc_t = -1;
	int val, x_t, y_t;

	/* middle point is blank */
	for(i = 0; i < 8; i++){
		if(3 == the_case[i].n1 + the_case[i].n2 &&
		   0 == pos[the_case[i].loc_1].flag && 0 == pos[the_case[i].loc_2].flag){
			if(the_case[i].loc_middle != the_case[i].loc_1 && 
			   the_case[i].loc_middle != the_case[i].loc_2){
				if(0 == pos[the_case[i].loc_middle].flag){
					loc_t = the_case[i].loc_middle;
					break;
				}
			}
		}
	}

	/* middle point is not blank */
	if(-1 == loc_t){
		for(i = 0; i < 8; i++){	
			if(3 == the_case[i].n1 + the_case[i].n2 &&
			   0 == pos[the_case[i].loc_1].flag && 0 == pos[the_case[i].loc_2].flag){
					val = double_sides_search(grid_inf, the_case[i].loc_1, the_case[i].loc_2, 
												1, the_case[i].the_direct);
				if(-1 != val){
					loc_t = val;
					break;
				}else{
					x_t = pos[the_case[i].loc_1].x;
					y_t = pos[the_case[i].loc_1].y;
					if(0 == is_on_boundry(grid_inf, x_t, y_t)){
						loc_t = the_case[i].loc_1;
						printf("loc_1!\n");
						break;
					}else{
						loc_t = the_case[i].loc_2;
						printf("loc_2!\n");
						break;
					}
				}
			}
		}
	}
	
	return loc_t;
}

int double_sides_two(fb_info *fb_inf, grid_info *grid_inf, int flag)
{
	int loc_t = -1;
	int i;
	
	if(-1 == loc_t){
		for(i = 0; i < 8; i++){
			if(2 == the_case[i].n1 + the_case[i].n2 && 
			   0 == pos[the_case[i].loc_1].flag && 0 == pos[the_case[i].loc_2].flag){
				if(0 == pos[the_case[i].loc_middle].flag && 
				   the_case[i].loc_middle != the_case[i].loc_1 && the_case[i].loc_middle != the_case[i].loc_2){
					loc_t = the_case[i].loc_middle;
				}
			}
		}
	}
	if(-1 == loc_t){
		for(i = 0; i < 8; i++){
			if(2 == the_case[i].n1 + the_case[i].n2 && 
			   0 == pos[the_case[i].loc_1].flag && 0 == pos[the_case[i].loc_2].flag){
				loc_t = double_sides_search(grid_inf, the_case[i].loc_1, the_case[i].loc_2, 
												flag, the_case[i].the_direct);
				if(-1 != loc_t){	
					return loc_t;
				}else{
					loc_t = the_case[i].loc_1;
				}
			}
		}
	}
		
	return loc_t;
}

int double_sides_search(grid_info *grid_inf, int loc_1, int loc_2, int flag, int the_direct)
{
	int i_direct, n1, n2;
	int loc_t;
	int x_t, y_t;

	for(i_direct = 0; i_direct < 8; i_direct++){
		n1 = 0; n2 = 0;

		if(i_direct == the_direct || i_direct == (the_direct + 4) % 8){
			continue;
		}
//		printf("in direct:%d\n", i_direct);

		/* search from loc_1 */
		loc_t = loc_1;
		x_t = pos[loc_t].x + direct[i_direct][0];
		y_t = pos[loc_t].y + direct[i_direct][1];
		loc_t = locate_position(grid_inf, x_t, y_t);

		if(-1 ==loc_t){
			continue;
		}
		while(flag == pos[loc_t].flag){
			n1++;
			x_t = pos[loc_t].x + direct[i_direct][0];
			y_t = pos[loc_t].y + direct[i_direct][1];
			loc_t = locate_position(grid_inf, x_t, y_t);
			if(-1 == loc_t){
				break;
			}
		}
		if(4 == n1){
			return loc_1;
//			printf("1. in research n1 = %d\n", n1);
		}else if(n1 >= 2 && (loc_t >-1 && loc_t < GRID_MAX_POINTS) && 0 == pos[loc_t].flag){
//			printf("2. in research n1 = %d\n", n1);
			return loc_1;
		}

		/* search from loc_2 */
		loc_t = loc_2;
		x_t = pos[loc_t].x + direct[i_direct][0];
		y_t = pos[loc_t].y + direct[i_direct][1];
		loc_t = locate_position(grid_inf, x_t, y_t);

		if(-1 == loc_t){
			continue;
		}
		while(flag == pos[loc_t].flag){
			n2++;
			x_t = pos[loc_t].x + direct[i_direct][0];
			y_t = pos[loc_t].y + direct[i_direct][1];
			loc_t = locate_position(grid_inf, x_t, y_t);
			if(-1 == loc_t){
				break;
			}
		}
		if(4 == n2){
//			printf("3. in research n2 = %d\n", n2);
			return loc_2;
		}else if(n2 >= 2 && (loc_t >-1 && loc_t < GRID_MAX_POINTS) && 0 == pos[loc_t].flag){
//			printf("4. in research n2 = %d\n", n2);
			return loc_2;
		}
	}

	return -1;
}

int search_line(grid_info *grid_inf, int loc, int flag)
{	
	int i_direct, n1, n2;
	int loc_t, loc_bak, loc_1, loc_middle, loc_2;
	int x_t, y_t;

	for(i_direct = 0; i_direct < 8; i_direct++){
		loc_t = loc; loc_1 = -1; loc_middle = -1; loc_2 = -1; loc_bak = -1;
		n1 = 0; n2 = 0;
		
		/* 1. come to the head */
		while(-1 != loc_t){
			if(flag == pos[loc_t].flag){
				loc_bak = loc_t;
				x_t = pos[loc_t].x + direct[i_direct][0];
				y_t = pos[loc_t].y + direct[i_direct][1];
				loc_t = locate_position(grid_inf, x_t, y_t);
			}else{
				break;
			}
		}
		if(-1 == loc_t){
			the_case[i_direct].loc_1 = loc_bak;
		}else{
			if(0 == pos[loc_t].flag){
				the_case[i_direct].loc_1 = loc_t;
			}else{
				the_case[i_direct].loc_1 = loc_bak;
			}
		}

		/* 2. from the head */
		loc_t = loc_bak;
		while(-1 != loc_t){
			if(flag == pos[loc_t].flag){
				n1++;
				loc_bak = loc_t;
				x_t = pos[loc_t].x + direct[(i_direct + 4) % 8][0];
				y_t = pos[loc_t].y + direct[(i_direct + 4) % 8][1];
				loc_t = locate_position(grid_inf, x_t, y_t);
			}else{
				break;
			}
		}
		if(-1 == loc_t){
			the_case[i_direct].loc_middle = loc_bak;
			the_case[i_direct].loc_2 = loc_bak;
			the_case[i_direct].n1 = n1;
			the_case[i_direct].n2 = n2;
			the_case[i_direct].the_direct = i_direct;
			continue;
		}else{
			if(0 != pos[loc_t].flag){
				the_case[i_direct].loc_middle = loc_bak;
				the_case[i_direct].loc_2 = loc_bak;
				the_case[i_direct].n1 = n1;
				the_case[i_direct].n2 = n2;
				the_case[i_direct].the_direct = i_direct;
				continue;
			}else{
				the_case[i_direct].n1 = n1;
				the_case[i_direct].loc_middle = loc_t;
			}
		}

		/* 3.from the middle */
		loc_bak = loc_t;
		x_t = pos[loc_t].x + direct[(i_direct + 4) % 8][0];
		y_t = pos[loc_t].y + direct[(i_direct + 4) % 8][1];
		loc_t = locate_position(grid_inf, x_t, y_t);
		if(-1 == loc_t){
			the_case[i_direct].loc_2 = loc_bak;
			the_case[i_direct].n2 = n2;
			the_case[i_direct].the_direct = i_direct;
			continue;
		}

		while(-1 != loc_t){
			if(flag == pos[loc_t].flag){
				n2++;
				loc_bak = loc_t;
				x_t = pos[loc_t].x + direct[(i_direct + 4) % 8][0];
				y_t = pos[loc_t].y + direct[(i_direct + 4) % 8][1];
				loc_t = locate_position(grid_inf, x_t, y_t);
			}else{
				break;
			}
		}
		if(0 == n2){
			loc_t = loc_bak;
		}
		if(-1 == loc_t){
			the_case[i_direct].loc_2 = loc_bak;
			the_case[i_direct].n2 = n2;
			the_case[i_direct].the_direct = i_direct;
			continue;
		}else{
			if(0 == pos[loc_t].flag){
				the_case[i_direct].loc_2 = loc_t;
				the_case[i_direct].the_direct = i_direct;
				the_case[i_direct].n2 = n2;
				continue;
			}else{
				the_case[i_direct].loc_2 = loc_bak;
				the_case[i_direct].the_direct = i_direct;
				the_case[i_direct].n2 = n2;
			}
		}
	}

	printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^\n");
	return 0;
}
