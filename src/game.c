
#include "game.h"


void g_new_game(void) {
	points = 0;

	bl_reset(&current_block);

	net_send_ready();

	// If we are in manual mode (auto_mode = 0), generate a block
	if (auto_mode == 0) {
		srand ( time(NULL) );
		bl_set_block_type(&current_block, rand()%9);
	}
	else {
		// Wait for 130 GO <code_pirce> <code_piece_suivant>
		net_wait_for_go();

		ai_suggest_best_block_location();
	}
}

void g_cycle(int kb_input) {
	int x, y;

	if (auto_mode == 0) {
		g_manage_kb(kb_input);
	}
	else {
		// TODO : Add network manage code here to manage all commands !!!!!!!!!

		// Sleep half a second before sending code
		usleep(500);
	
		// Move the block to the best location
		ai_move_block_to_best_location(&current_block);
	}

	bl_move_down(&current_block);

	if (b_does_collide(&current_block) == 1) {
		if (current_block.y <= 1) {
			g_game_over();
			return;
		}
		for (y = 0; y < current_block.sizeY; y++) {
			for (x = 0; x < current_block.sizeX; x++) {
				if (current_block.tab[x][y] == 1) {
					board[x+current_block.x][y+current_block.y] = 1;
				}
			}
		}

		points += b_remove_lines();

		bl_reset(&current_block);
		bl_set_block_type(&current_block, rand()%9);
		if (auto_mode == 1) {
			// Get a new block and calculate the best location
			ai_suggest_best_block_location();
		}
	}
}

void g_manage_kb(int kb_input) {
	switch (kb_input) {
		case 'q':
		case 'Q':
			done = 1;
			return;
		case 'a':
		case 'A':
			bl_rotate_anti_clockwise(&current_block);
			return;
		case 'z':
		case 'Z':
			bl_rotate_clockwise(&current_block);
			return;
		case 'r':
		case 'R':
			bl_reflect(&current_block);
			return;
		case KEY_LEFT:
			bl_move_left(&current_block);
			return;
		case KEY_RIGHT:
			bl_move_right(&current_block);
			return;
		case KEY_DOWN:
			b_drop_block(&current_block);
			return;
		default:
			break;
	}
}

void g_manage_net_command(int * code, char * command, char * data) {
	int currentPiece = 0;
	int nextPiece = 0;
	char buffer[NET_BUFFER_LENGTH];
	int piece, x, y, rotation;

	if (!code || !command) {
		return;
	}

	if (*code == 130 && strcmp("GO", command) == 0) {
		if (!data) {
			return;
		} else {
			sscanf(data, "%d %d", &currentPiece, &nextPiece);
			bl_set_current_block(currentPiece-1);
			bl_set_next_block(nextPiece-1);
		}

		return;
	} else if (*code == 140) {
		g_game_over();
		return;
	} else if (*code == 301) {
		if (!data) {
			return;
		} else {
			bzero(buffer, sizeof(buffer));
			sscanf(data, "%s %d %d %d %d", buffer, piece, x, y, rotation);

			b_set_board_from_string(data);
			bl_set_block_type(&current_block, piece - 1);
			current_block.x = x;
			current_block.y = y;
			current_block.rotation = rotation;
		}
	}
}

void g_draw(void) {
	clear();
	b_draw_board();
	bl_draw(&current_block);
	mvprintw(2, BOARD_DRAW_OFFSET + BOARD_WIDTH + 2, "Points: %d", points);
	if (auto_mode == 1) {
		// Draw the ai block in color
		start_color();
		init_pair(1, COLOR_BLUE, COLOR_BLACK);
		attron(COLOR_PAIR(1));
		bl_draw(&ai_block);
		attroff(COLOR_PAIR(1));
	}
	//usleep(50000);
	refresh();
}

void g_game_over(void) {
	clear();
	mvprintw( 1,1,"   _____                      ");
	mvprintw( 2,1,"  / ____|                     ");
	mvprintw( 3,1," | |  __  __ _ _ __ ___   ___ ");
	mvprintw( 4,1," | | |_ |/ _` | /_ ` _ \\ / _ \\");
	mvprintw( 5,1," | |__| | (_| | | | | | |  __/");
	mvprintw( 6,1,"  \\_____|\\__,_|_| |_| |_|\\___|");
	mvprintw( 8,1,"   ____                 ");
	mvprintw( 9,1,"  / __ \\                ");
	mvprintw(10,1," | |  | |_   _____ _ __ ");
	mvprintw(11,1," | |  | \\ \\ / / _ \\ |__|");
	mvprintw(12,1," | |__| |\\ V /  __/ |   ");
	mvprintw(13,1,"  \\____/  \\_/ \\___|_|   ");
	refresh();
	done = 1;
	sleep(2);
	g_log_result();
}

void g_log_result(void) {
	FILE *file_pointer;
	file_pointer = fopen("results.log", "a");
	if (file_pointer == NULL) {
		error("unable to open log file");
	}
	fprintf(file_pointer, "ai_height_weight=%f;ai_line_weight=%f;ai_empty_blocks_weight=%f;points=%d\n",
		ai_height_weight,
		ai_line_weight,
		ai_empty_blocks_weight,
		points
	);
	fclose(file_pointer);	
}