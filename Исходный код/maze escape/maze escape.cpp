// maze_escape.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <Windows.h>
#include "GL.h"
#include "glut.h"
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#include "GLAUX.H"
#include <Stringapiset.h>
#pragma comment (lib, "glaux.lib")

using namespace std;
//cells: 0 - поле "проход", 1- поле "финиш", 2 - поле "стена", 3 - поле "старт", 4 - поле "монетка", 5 - враг

const int amount_w = 50, amount_h = 50, amount_enemy_const=15, total_types_of_cells=5, amount_of_skins=16; //кол-во клеток лабиринта, максимальное количество монстров,  всего видов полей, количество скинов
int width = 1000, height = 640, enable_fullscreen= 1, num_window; //ширина окна, высота окна, полноранный режим, номер окна
int current_version; //номер текущей версии программы
int amount_of_created_levels, page=0, last_page; //кол-во уровней, созданных пользователем, текущая страница, номер последней страницы 
int menu_select = 0, menu_buffer=0, click_x, click_y, me_x = ((amount_w / 2)*(-1)), me_y = ((amount_h / 2)*(-1)); //селектор меню,буферизация значения значения предыдущего меню,  координаты окна при нажатии мыши, координаты персоонажа
int maze[amount_w][amount_h], maze_shadow[amount_w][amount_h], maze_created[amount_w][amount_h], width_cell, height_cell; //лабиринт, покрытие тенью, массив для перемещения врагов, ширина клетки, высота клетки
int character_pos = 1, coins = 0, shadow_r = 0, coin_viewer = 0, character_num = 0, current_level=1, played_level;//направление персоонажа(1 - вверх, 2 - право, 3 - вниз, 4 - влево), количество собранных монет, радиус тени, видимость монет, номер скина персоонажа, последний пройденный уровень, текущий уровень(относится только к стандартным уровням) 
int amount_enemy = 0, enemy_x[amount_enemy_const], enemy_y[amount_enemy_const], enemy_pos[amount_enemy_const]; //кол-во врагов, позиция врагов X и Y, направление врага
int mc_tap_x, mc_tap_y, cell_selector=0; //mc==maze_create, нажатие по x и y
int fullscreen_button_w, fullscreen_button_h, office_button_size = 30, fullscr_location_x = -52, fullscr_location_y = -12;  //кнопка полноэкранного режима
int exit_button_w, exit_button_h, exit_location_x = -12, exit_location_y = -12; //кнопка выхода
int menu_button_w, menu_button_h[3], back_to_menu_h, back_to_menu_w;//кнопки главного меню
int play_select = 0, pm_b_w, pm_b_h[2], pm_b_size_w, pm_b_size_h;  //проверка на активность кнопки "play", местоположение кнопок игрового меню и их размеры       pm=play_menu
int ml_b_w, ml_b_h, ml_b_size, total_number_of_cells = 8; //ml==my_levels, положение кнопок в меню лабиринтов, созданных пользователем, их размеры, кол-во клеток на странице
int shop_select = 0, sm_b_w, sm_b_h[2], sm_b_size_w, sm_b_size_h; //проверка на активность кнопки "shop", местоположение кнопок игрового меню и их размеры      sm=shop menu
int updage_b_w[1], updage_b_h[1], updage_size_w, updage_size_h; //кнопки "updage" в магазине 
int onoff_b_w, onoff_b_h, onoff_size; //кнопки on/off в магазине
int yesno_b_w[2], yesno_b_h = -60, yesno_b_size = 43, ok_button_w; //кнопки yes/no
int cross_w, cross_h, cross_size; //крестик для удаления
int coin_viewer_cost = 50, shadow_r_cost[3]{20, 50, 100}, character_onoff[16], character_cost[16]{0, 5, 10, 10, 20, 20, 20, 20, 30, 35, 35, 35, 35, 35, 35, 35}; //стоимость покупки видимости монет, увеличения радиуса видимости, проверка на куплен ли скин, стоимость скинов
int mcr_select = 0, mcm_b_w, mcm_b_h[2], mcm_b_size_w, mcm_b_size_h;  //проверка на активность кнопки "maze_creator", местоположение кнопок игрового меню и их размеры       mcm=maze_creator_menu
int mc_button_w, mc_button_h, mc_button_size, save_b_h, save_b_w[2], save_b_size_w, save_b_size_h, save_selector=0, opened_maze; //расположение кнопок в редакторе, их размер, координаты кнопки "save", ее размер, номер лабиринта открытого для редактирования
int total_starts=0, total_finishes=0, center_text=0;//всего стартов, финишей, переменная для центрования текста по ширине окна
int arrows_w, arrows_h; //стрелки для перехода на страницы
int about_h, about_w, about_size_w, about_size_h; //кнопка "about"
int best_time, games_played, games_win; //лучшее время на уровне, кол-во сыгранных игр, кол-во выигранных игр
int _message_box, deleting_lvl_name, enable_delete = 0;
char mc_str[total_types_of_cells][32], string_e[100];  //массив строк для кнопок в редакторе,  extra string
GLuint button[12], cell[6], character[16], screen[2], exit_b, back_arrow, mm_button, onoff[3], updage_button, save_button[2], prize, yesno_button[2], ok_button;
AUX_RGBImageRec *button_p, *button_shop, *button_mazecr, *button_standart_lvls, *button_my_lvls, *my_level_icon, *upgrade_e, *skins_e, *new_maze_e, *remake_old_e, *lock_e, *about_e, *grass, *goal, *wall, *start_pos, *coin, *enemy, *character_e[16], *fullscreen_e, *exitfullscreen_e, *exit_b_e, *back_arrow_e, *mm_button_e, *updage_button_e, *onoff_e[3], *save_button_e[2], *prize_e, *yesno_button_e[2], *ok_button_e; // указатели на память, где будет расположена текстура
HWND hWnd;
clock_t start_time, current_time;
string save_name = "_", open_file, stopwatch;

void LoadGLTextures()  // Загрузка картинок в память приложения
{  // создание текстур
	button_p = auxDIBImageLoad(L"files//textures//buttons//play.bmp"); // кнопка "play"
	glGenTextures(1, &button[0]); 
	glBindTexture(GL_TEXTURE_2D, button[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, button_p->sizeX, button_p->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, button_p->data); 

	button_shop = auxDIBImageLoad(L"files//textures//buttons//shop.bmp"); // кнопка "play"
	glGenTextures(1, &button[1]);
	glBindTexture(GL_TEXTURE_2D, button[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, button_shop->sizeX, button_shop->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, button_shop->data);

	button_mazecr = auxDIBImageLoad(L"files//textures//buttons//map_creator.bmp"); // кнопка "play"
	glGenTextures(1, &button[2]);
	glBindTexture(GL_TEXTURE_2D, button[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, button_mazecr->sizeX, button_mazecr->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, button_mazecr->data);

	button_standart_lvls = auxDIBImageLoad(L"files//textures//buttons//standart_levels.bmp"); // кнопка "standart levels"
	glGenTextures(1, &button[3]);
	glBindTexture(GL_TEXTURE_2D, button[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, button_standart_lvls->sizeX, button_standart_lvls->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, button_standart_lvls->data);

	button_my_lvls = auxDIBImageLoad(L"files//textures//buttons//my levels.bmp"); // кнопка "my levels"
	glGenTextures(1, &button[4]);
	glBindTexture(GL_TEXTURE_2D, button[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, button_my_lvls->sizeX, button_my_lvls->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, button_my_lvls->data);

	my_level_icon = auxDIBImageLoad(L"files//textures//buttons//my_level_icon.bmp"); // кнопка, изображающая иконку уровня
	glGenTextures(1, &button[5]);
	glBindTexture(GL_TEXTURE_2D, button[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, my_level_icon->sizeX, my_level_icon->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, my_level_icon->data);

	upgrade_e = auxDIBImageLoad(L"files//textures//buttons//upgrade.bmp"); // кнопка "upgrade"
	glGenTextures(1, &button[6]);
	glBindTexture(GL_TEXTURE_2D, button[6]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, upgrade_e->sizeX, upgrade_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, upgrade_e->data);

	skins_e = auxDIBImageLoad(L"files//textures//buttons//skins.bmp"); // кнопка "skins"
	glGenTextures(1, &button[7]);
	glBindTexture(GL_TEXTURE_2D, button[7]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, skins_e->sizeX, skins_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, skins_e->data);

	new_maze_e = auxDIBImageLoad(L"files//textures//buttons//new maze.bmp"); // кнопка "new maze"
	glGenTextures(1, &button[8]);
	glBindTexture(GL_TEXTURE_2D, button[8]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, new_maze_e->sizeX, new_maze_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, new_maze_e->data);
	
	remake_old_e = auxDIBImageLoad(L"files//textures//buttons//remake old.bmp"); // кнопка "remake_old"
	glGenTextures(1, &button[9]);
	glBindTexture(GL_TEXTURE_2D, button[9]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, remake_old_e->sizeX, remake_old_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, remake_old_e->data);

	lock_e = auxDIBImageLoad(L"files//textures//buttons//locked.bmp"); // заблокированный уровень
	glGenTextures(1, &button[10]);
	glBindTexture(GL_TEXTURE_2D, button[10]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, lock_e->sizeX, lock_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, lock_e->data);

	about_e = auxDIBImageLoad(L"files//textures//buttons//about.bmp"); // кнопка "about"
	glGenTextures(1, &button[11]);
	glBindTexture(GL_TEXTURE_2D, button[11]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, about_e->sizeX, about_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, about_e->data);

	
	grass = auxDIBImageLoad(L"files//textures//cells//grass.bmp");  // поле "проход" (трава)
	glGenTextures(1, &cell[0]);
	glBindTexture(GL_TEXTURE_2D, cell[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, grass->sizeX, grass->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, grass->data);

	goal = auxDIBImageLoad(L"files//textures//cells//goal.bmp");  // поле "финиш"
	glGenTextures(1, &cell[1]); 
	glBindTexture(GL_TEXTURE_2D, cell[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, goal->sizeX, goal->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, goal->data);

	wall = auxDIBImageLoad(L"files//textures//cells//wall.bmp");  // поле "стена"
	glGenTextures(1, &cell[2]); 
	glBindTexture(GL_TEXTURE_2D, cell[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wall->sizeX, wall->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, wall->data);

	start_pos = auxDIBImageLoad(L"files//textures//cells//start.bmp");  // поле "стена"
	glGenTextures(1, &cell[3]);
	glBindTexture(GL_TEXTURE_2D, cell[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, start_pos->sizeX, start_pos->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, start_pos->data);

	coin = auxDIBImageLoad(L"files//textures//cells//coin.bmp");  // монетка
	glGenTextures(1, &cell[4]);
	glBindTexture(GL_TEXTURE_2D, cell[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, coin->sizeX, coin->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, coin->data);

	enemy = auxDIBImageLoad(L"files//textures//cells//enemy.bmp");  //враг
	glGenTextures(1, &cell[5]);
	glBindTexture(GL_TEXTURE_2D, cell[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, enemy->sizeX, enemy->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, enemy->data);

	string open_character;
	char temp[64];
	int temp_len = 0;
	for (int i = 0; i < 16; i++)
	{
		open_character = "files//textures//character//character";
		open_character+=to_string(i); 
		open_character += ".bmp";
		strcpy_s(temp, open_character.c_str());
		LPWSTR lpwstr = new wchar_t[64];;
		temp_len = strlen(open_character.c_str());
		for (int j = 0; j <temp_len + 1; j++) lpwstr[j] = temp[j];
		character_e[i] = auxDIBImageLoad(lpwstr);  // персоонаж
		glGenTextures(1, &character[i]);
		glBindTexture(GL_TEXTURE_2D, character[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, character_e[i]->sizeX, character_e[i]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, character_e[i]->data);
	}

	exitfullscreen_e = auxDIBImageLoad(L"files//textures//buttons//exitfullscreen.bmp");  // кнопка "exit fullscreen"
	glGenTextures(1, &screen[0]);
	glBindTexture(GL_TEXTURE_2D, screen[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, exitfullscreen_e->sizeX, exitfullscreen_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, exitfullscreen_e->data);

	fullscreen_e = auxDIBImageLoad(L"files//textures//buttons//fullscreen.bmp");  // кнопка "fullscreen"
	glGenTextures(1, &screen[1]);
	glBindTexture(GL_TEXTURE_2D, screen[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, fullscreen_e->sizeX, fullscreen_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, fullscreen_e->data);

	exit_b_e = auxDIBImageLoad(L"files//textures//buttons//blackX.bmp");  //кнопка "exit"
	glGenTextures(1, &exit_b);
	glBindTexture(GL_TEXTURE_2D, exit_b);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, exit_b_e->sizeX, exit_b_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, exit_b_e->data);

	back_arrow_e = auxDIBImageLoad(L"files//textures//buttons//back_arrow.bmp");  //кнопка "back_arrow"
	glGenTextures(1, &back_arrow);
	glBindTexture(GL_TEXTURE_2D, back_arrow);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, back_arrow_e->sizeX, back_arrow_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, back_arrow_e->data);

	mm_button_e = auxDIBImageLoad(L"files//textures//buttons//back_to_menu.bmp");  //кнопка "back to main menu"
	glGenTextures(1, &mm_button);
	glBindTexture(GL_TEXTURE_2D, mm_button);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, mm_button_e->sizeX, mm_button_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, mm_button_e->data);

	updage_button_e = auxDIBImageLoad(L"files//textures//buttons//updage.bmp");  //кнопка "updage"
	glGenTextures(1, &updage_button);
	glBindTexture(GL_TEXTURE_2D, updage_button);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, updage_button_e->sizeX, updage_button_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, updage_button_e->data);

	onoff_e[0] = auxDIBImageLoad(L"files//textures//buttons//buy.bmp");  //кнопка "buy"
	glGenTextures(1, &onoff[0]);
	glBindTexture(GL_TEXTURE_2D, onoff[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, onoff_e[0]->sizeX, onoff_e[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, onoff_e[0]->data);

	onoff_e[1] = auxDIBImageLoad(L"files//textures//buttons//off.bmp");  //кнопка "off"
	glGenTextures(1, &onoff[1]);
	glBindTexture(GL_TEXTURE_2D, onoff[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, onoff_e[1]->sizeX, onoff_e[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, onoff_e[1]->data);

	onoff_e[2] = auxDIBImageLoad(L"files//textures//buttons//on.bmp");  //кнопка "on"
	glGenTextures(1, &onoff[2]);
	glBindTexture(GL_TEXTURE_2D, onoff[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, onoff_e[2]->sizeX, onoff_e[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, onoff_e[2]->data);

	save_button_e[0] = auxDIBImageLoad(L"files//textures//buttons//save as new.bmp");  //кнопка "save as new"
	glGenTextures(1, &save_button[0]);
	glBindTexture(GL_TEXTURE_2D, save_button[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, save_button_e[0]->sizeX, save_button_e[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, save_button_e[0]->data);

	save_button_e[1] = auxDIBImageLoad(L"files//textures//buttons//save.bmp");  //кнопка "save"
	glGenTextures(1, &save_button[1]);
	glBindTexture(GL_TEXTURE_2D, save_button[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, save_button_e[1]->sizeX, save_button_e[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, save_button_e[1]->data);
	
	prize_e = auxDIBImageLoad(L"files//textures//prize.bmp");  //кнопка "save"
	glGenTextures(1, &prize);
	glBindTexture(GL_TEXTURE_2D, prize);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, prize_e->sizeX, prize_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, prize_e->data);

	yesno_button_e[0] = auxDIBImageLoad(L"files//textures//buttons//yes.bmp");  //кнопка "yes"
	glGenTextures(1, &yesno_button[0]);
	glBindTexture(GL_TEXTURE_2D, yesno_button[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, yesno_button_e[0]->sizeX, yesno_button_e[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, yesno_button_e[0]->data);

	yesno_button_e[1] = auxDIBImageLoad(L"files//textures//buttons//no.bmp");  //кнопка "no"
	glGenTextures(1, &yesno_button[1]);
	glBindTexture(GL_TEXTURE_2D, yesno_button[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, yesno_button_e[1]->sizeX, yesno_button_e[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, yesno_button_e[1]->data);

	ok_button_e = auxDIBImageLoad(L"files//textures//buttons//ok.bmp");  //кнопка "ok"
	glGenTextures(1, &ok_button);
	glBindTexture(GL_TEXTURE_2D, ok_button);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, ok_button_e->sizeX, ok_button_e->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, ok_button_e->data);
}

void RenderBitmapString(float x, float y, float z, void *font, char *string)
{
	char *c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void coins_to_file()
{
	ofstream f;
	f.open("files//programm files//coins.txt", ios::out | ios::trunc);
	f << coins;
	f.close(); //запись количества монет в файл
}

void progress_to_file()
{
	ofstream _open_file;
	_open_file.open("files//programm files//progress.txt", ios::out | ios::trunc);
	_open_file << current_level <<" " << coin_viewer << " " << shadow_r;
	_open_file.close();
}

void count_time(int cur_sec)
{
	stopwatch = "";
	int cur_min = 0, cur_hour = 0;  //текущее время: секунды, минуты, часы

	if (cur_sec > 3599)  //подсчет кол-ва часов
	{
		cur_hour = cur_sec / 3600;
		cur_sec = cur_sec - (cur_hour * 3600);
	}
	if (cur_sec > 59)  //подсчет кол-ва минут
	{
		cur_min = cur_sec / 60;
		cur_sec = cur_sec - (cur_min * 60);
	}
	if (cur_hour != 0) //если у нас есть какое-либо кол-во часов, записываем это в строку
	{
		stopwatch += to_string(cur_hour);
		stopwatch += " h ";
	}
	if (cur_min != 0) //если у нас есть какое-либо кол-во минут, записываем это в строку
	{
		stopwatch += to_string(cur_min);
		stopwatch += " min ";
	}
	stopwatch += to_string(cur_sec);
	stopwatch += " sec ";
}

void time_to_window(int x, int y, int z, int cur_sec)
{
	stopwatch = "";
	count_time(cur_sec);
	glColor3f(0.0, 0.0, 1.0);
	RenderBitmapString(x, y, z, GLUT_BITMAP_TIMES_ROMAN_24, (char *)stopwatch.c_str());
}

void arrows_move()
{
	if ((click_y > -arrows_h) && (click_y < arrows_h))
	{
		if ((click_x > arrows_w) && (click_x < arrows_w + (3 * width / 50)))
			if (page != last_page)
				page++;   //переход на страницу вперед

		if ((click_x < -arrows_w) && (click_x > -(arrows_w + (3 * width / 50))))
			if (page != 0)
				page--;	//переход на страницу назад
	}
}

void reshape(int w, int h)  //функция обработки изменения размеров окна
{
	glViewport(0.0, 0.0, w, h);
	width = w; height = h;
	glClearColor(0.15, 0.1, 0.2, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2, width/2, -height/2, height/2, -width/2, width/2);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();

	fullscreen_button_w = (width / 2) + fullscr_location_x;
	fullscreen_button_h = (height / 2) + fullscr_location_y;  //вычисление расположения кнопок "fullscreen"
	exit_button_w = (width / 2) + exit_location_x;
	exit_button_h = (height / 2) + exit_location_y;
	
	if (height <= width) { height_cell = (height - 100) / amount_h; width_cell = height_cell; }
	else { width_cell = (width - 100) / amount_w; height_cell = width_cell; } //расчет размеров одной клетки лабиринта
}

void shop()
{
	updage_b_w[0] = width/8;
	updage_b_h[0] = 0;
	updage_size_w = width * 3 / 10;
	updage_size_h = width*15/100;

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, updage_button);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(updage_b_w[0], updage_b_h[0], 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(updage_b_w[0] + updage_size_w, updage_b_h[0], 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(updage_b_w[0] + updage_size_w, updage_b_h[0] + updage_size_h, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(updage_b_w[0], updage_b_h[0] + updage_size_h, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0, 0.5, 0.5);
	sprintf_s(string_e, "radius of overview: level%2d", shadow_r);
	RenderBitmapString(updage_b_w[0] + updage_b_w[0]/10, updage_b_h[0] + updage_size_h + 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
	if (shadow_r == 3)
		sprintf_s(string_e, "(max level)");  //добавить к концу строки
	else
	{
		sprintf_s(string_e, "(next level: %d", shadow_r_cost[shadow_r]);
		strcat_s(string_e, ")");
	}
	RenderBitmapString(updage_b_w[0] + updage_b_w[0] / 5, updage_b_h[0] - 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);

	onoff_b_w = -(width / 2 - width/2*1.5 / 4);
	onoff_b_h = 0;
	onoff_size = updage_size_h;

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, onoff[coin_viewer]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(onoff_b_w, onoff_b_h, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(onoff_b_w + onoff_size, onoff_b_h, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(onoff_b_w + onoff_size, onoff_b_h + onoff_size, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(onoff_b_w, onoff_b_h + onoff_size, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0, 0.5, 0.5);
	RenderBitmapString(onoff_b_w + onoff_size/10, onoff_b_h + onoff_size + 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "coin viewer");
	if (coin_viewer == 0)
	{
		sprintf_s(string_e, "buy: %d", coin_viewer_cost);
		RenderBitmapString(onoff_b_w + onoff_size / 10, onoff_b_h - 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
	}
	else RenderBitmapString(onoff_b_w - onoff_size / 5, onoff_b_h - 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "press to turn on/off");
	
	back_to_menu_h = -height / 2 + (height / 10 * 3);
	back_to_menu_w = height / 2 - height / 10 * 3;

	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mm_button); 
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-back_to_menu_w, back_to_menu_h + back_to_menu_h, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(back_to_menu_w, back_to_menu_h + back_to_menu_h, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(back_to_menu_w, back_to_menu_h, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-back_to_menu_w, back_to_menu_h, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0, 0.5, 0.5);
	sprintf_s(string_e, "Your coins: %2d", coins);
	RenderBitmapString(-width / 2 + 50, -height / 2 + 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
}

void shop_menu()
{
	int i;
	static float motion_x = 0.0, motion_y = 12.0, motion_plus_x = 0.1, motion_plus_y = 0.1;  //движение кнопок
	static int motion_time = -200;
	sm_b_h[0] = height / 2 * 3 / 10 + motion_y;
	sm_b_h[1] = height / 2 * -1 / 10 - motion_y;
	//sm_b_w = width / 2 * 9 / 20;
	sm_b_w = -(menu_button_w + 30 + height / 2 * 11 / 50 + motion_x);

	sm_b_size_h = height / 2 * 22 / 100;
	sm_b_size_w = sm_b_size_h * 2.3;

	motion_x = motion_x - motion_plus_x;
	motion_y = motion_y + motion_plus_y;
	if (motion_time == 200)
	{
		motion_plus_x = -motion_plus_x;
		motion_time = -200;
	}
	if (motion_time == 0) motion_plus_y = -motion_plus_y;
	motion_time++;

	for (i = 6; i < 8; i++)
	{
		glColor3f(0.0, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex3f(-menu_button_w, 0, 0);
		glVertex3f(sm_b_w, sm_b_h[i - 6] - sm_b_size_h / 2, 0);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);                 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, button[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(sm_b_w - sm_b_size_w, sm_b_h[i - 6], 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sm_b_w , sm_b_h[i - 6], 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(sm_b_w , sm_b_h[i - 6] - sm_b_size_h, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(sm_b_w - sm_b_size_w, sm_b_h[i - 6] - sm_b_size_h, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void skins_shop()
{
	int  number_of_cells, k = 1; //кол-во страниц, кол-во клеток на последней странице, переменная для подсчета номера клетки
	int i, j;

	arrows_w = 4 * width / 2 / 4.8;
	arrows_h = height / 2 / 4;

	if (page != 0) i = -1;
	else i = 1;
	glColor3f(1.0, 0.0, 0.0);
	for (i; i < 2; i = i + 2)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(arrows_w*i, arrows_h, 0);
		glVertex3f(arrows_w*i, -arrows_h, 0);
		glVertex3f((arrows_w + (3 * width / 50))*i, 0, 0);
		glEnd();
		if (page == last_page) i = 3;
	}  //прорисовка стрелочек

	number_of_cells = amount_of_skins % total_number_of_cells;

	ml_b_w = width / 9 - width / 2;
	ml_b_h = height / 2 * 2 / 5;
	ml_b_size = width / 9;  //подсчет размеров клетки уровня

	k = total_number_of_cells*page + 1;

	for (j = 1; j > -2; j = j - 2)  //прорисовка клеток уровней
	{
		for (i = 0; i < total_number_of_cells / 2; i++)
		{
			if (character_num == k-1) //подсветка выбранного скина
			{
				glColor3f(1.0, 0.5, 0.0);
				glBegin(GL_TRIANGLES);
				glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size / 2, j*ml_b_h + ml_b_size / 2 + ml_b_size / 8, 0);
				glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size+ml_b_size / 8, j*ml_b_h - ml_b_size / 2 - ml_b_size / 10, 0);
				glVertex3f(ml_b_w + ml_b_size*i * 2 - ml_b_size / 8, j*ml_b_h - ml_b_size / 2 - ml_b_size / 10, 0);
				glEnd();
			}

			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, character[k-1]);
			glBegin(GL_TRIANGLES);
			glTexCoord2f(0.5f, 1.0f); glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size/2, j*ml_b_h + ml_b_size / 2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size, j*ml_b_h - ml_b_size / 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(ml_b_w + ml_b_size*i * 2, j*ml_b_h - ml_b_size / 2, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			if (character_onoff[k-1]!=1)
			{
				glColor3f(0.0, 0.5, 0.5);
				sprintf_s(string_e, "%d", character_cost[k - 1]);
				RenderBitmapString(ml_b_w + ml_b_size*i * 2 + 3, j*ml_b_h - ml_b_size / 2 - 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
			}

			if (k == amount_of_skins)
			{
				i = total_number_of_cells / 2;
				j = -2;
			}
			k++;
		}
	}

	glColor3f(0.0, 0.5, 0.5);
	sprintf_s(string_e, "Your coins: %2d", coins);
	RenderBitmapString(-width / 2 + 50, -height / 2 + 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
}

void maze_creator()
{
	int i, j, pl_w, pl_h;
	pl_w = (-1)*(amount_w / 2)*width_cell;
	pl_h = (-1)*(amount_h / 2)*height_cell; //определиние местоположения относительно центра координат OpenGl

	POINT mouse_pos;  
	GetCursorPos(&mouse_pos);
	hWnd = FindWindow(L"glut", L"Maze escape");
	ScreenToClient(hWnd, &mouse_pos); //координаты мыши

	int mouse_pos_x, mouse_pos_y;
	if (mouse_pos.x > width / 2) mouse_pos_x = mouse_pos.x - width / 2;
	else mouse_pos_x = width / 2 * (-1) + mouse_pos.x;
	if (mouse_pos.y > height / 2) mouse_pos_y = (mouse_pos.y - height / 2)*(-1);
	else  mouse_pos_y = height / 2 - mouse_pos.y; //перевод из координат окна в координаты opengl

	for (i = 0; i < amount_w; i++) //прорисовка лабиринта
	{
		for (j = 0; j < amount_h; j++)
		{
			if ((mouse_pos_x>=i*width_cell + pl_w) && (mouse_pos_x<i*width_cell + width_cell + pl_w) && (mouse_pos_y>=j*height_cell + pl_h) && (mouse_pos_y<j*height_cell + height_cell + pl_h))
				glColor3f(0.5, 0.5, 0.3);
			else glColor3f(1.0, 1.0, 1.0);  //подсветка клетки, на которой находится мышка

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, cell[maze[i][j]]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + pl_h, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + pl_h, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}

	//if (width >= height)  //если ширина больше
	{		
		mc_button_w = -width/2 + 17;
		mc_button_h = height/2 - 100;
		mc_button_size = 2*width/100;
		
		for (i = 0; i < total_types_of_cells*2-2; i=i+2)
		{		
			if (i / 2 == cell_selector) glColor3f(0.0, 0.5, 0.5);
			else glColor3f(1.0, 0.1, 0.0);
			RenderBitmapString(mc_button_w + mc_button_size, mc_button_h + -i*mc_button_size - mc_button_size*6/7 , 0, GLUT_BITMAP_TIMES_ROMAN_24, mc_str[i / 2]);

			if (i / 2 == cell_selector) glColor3f(0.5, 0.5, 0.3);
			else glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, cell[i/2]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mc_button_w, mc_button_h + -i*mc_button_size, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mc_button_w + mc_button_size, mc_button_h + -i*mc_button_size, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mc_button_w + mc_button_size, mc_button_h + -i*mc_button_size - mc_button_size, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(mc_button_w, mc_button_h + -i*mc_button_size - mc_button_size, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}//расчет положения кнопок в редакторе
	/*else
	{
		mc_button_w = -width / 2 + 200;
		mc_button_h = -height / 2 + 17;
		mc_button_size = 2 * height / 100;

		for (i = 0; i < total_types_of_cells * 2 - 2; i = i + 2)
		{
			glColor3f(1.0, 1.0, 1.0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, cell[i / 2]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(mc_button_w + -i*mc_button_size, mc_button_h, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(mc_button_w + -i*mc_button_size, mc_button_h + mc_button_size, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(mc_button_w+ -i*mc_button_size - mc_button_size, mc_button_h + mc_button_size , 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(mc_button_w + -i*mc_button_size - mc_button_size, mc_button_h, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	} */

	save_b_w[1] = width/2 - width / 10;
	save_b_h = -(height/2 - height / 11);
	save_b_size_w = width/13;
	save_b_size_h = -save_b_size_w/2.5;
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, save_button[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(save_b_w[1], save_b_h, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(save_b_w[1] + save_b_size_w, save_b_h, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(save_b_w[1] + save_b_size_w, save_b_h + save_b_size_h, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(save_b_w[1], save_b_h + save_b_size_h, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void maze_creator_menu()
{
	int i;
	static float motion_x = 0.0, motion_y = 12.0, motion_plus_x = 0.1, motion_plus_y = 0.1;  //движение кнопок
	static int motion_time = -200;
	mcm_b_h[0] = -(height / 2 * 4 / 5 + motion_y);
	mcm_b_h[1] = -(height / 2 * 2.7 / 5 - motion_y);
	//mcm_b_w = width / 2 * 9 / 20;
	mcm_b_w = menu_button_w + 30 + motion_x;

	mcm_b_size_h = height / 2 * 22 / 100;
	mcm_b_size_w = mcm_b_size_h * 2.3;

	motion_x = motion_x + motion_plus_x;
	motion_y = motion_y + motion_plus_y;
	if (motion_time == 200)
	{
		motion_plus_x = -motion_plus_x;
		motion_time = -200;
	}
	if (motion_time == 0) motion_plus_y = -motion_plus_y;
	motion_time++;

	for (i = 8; i < 10; i++)
	{
		glColor3f(0.0, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex3f(menu_button_w, menu_button_h[2] + (menu_button_h[2] / 2) + i/2, 0);
		glVertex3f(mcm_b_w, mcm_b_h[i - 8] + mcm_b_size_h / 2, 0);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);       
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, button[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(mcm_b_w, mcm_b_h[i - 8] + mcm_b_size_h, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(mcm_b_w + mcm_b_size_w, mcm_b_h[i - 8] +  mcm_b_size_h, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(mcm_b_w + mcm_b_size_w, mcm_b_h[i - 8] , 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(mcm_b_w, mcm_b_h[i - 8] , 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void save_created_maze()  //меню сохранения уровней
{
	save_b_w[0] = width / 2 - (width / 13*3);
	save_b_w[1] = width / 2 - width / 10;
	save_b_h = -(height / 2 - height / 11);
	save_b_size_w = width / 13;
	save_b_size_h = -save_b_size_w / 2.5;
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < 2; i++)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, save_button[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(save_b_w[i], save_b_h, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(save_b_w[i] + save_b_size_w, save_b_h, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(save_b_w[i] + save_b_size_w, save_b_h + save_b_size_h, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(save_b_w[i], save_b_h + save_b_size_h, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	int centep_pos, pos_y;  //центровка названия по ширине, по высоте
	centep_pos = center_text * 4.7;
	pos_y = 30;

	glColor3f(0.0, 0.5, 0.5);
	RenderBitmapString(-5 * 32/* примерный размер символов умножить на кол-вo символов*/, -height / 2 + height * 7 / 10, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Please, enter name of your maze:");
	RenderBitmapString(-centep_pos, pos_y, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char *)save_name.c_str());
	RenderBitmapString( -5*43  , -height / 2 + height * 3 / 10, 0, GLUT_BITMAP_TIMES_ROMAN_24, "P.s.: you can write only on english keyboard");
}

void saving_maze()  //сохранение
{
	int i, j;                                                                                                                                             //not work
			
	save_name.erase(save_name.end() - 1);
	ofstream f;	
	f.open("files//my levels//" + open_file + ".txt", ios::out);
	f << (char *)save_name.c_str()<<endl;
	for (i = 0; i < amount_w; i++)
	{
		for (j = 0; j < amount_h; j++)
		{
			f << maze[j][i] << " ";
			if ((j + 1) % 5 == 0) f << " ";
		}
		f << endl;
		if ((i + 1) % 5 == 0) f << endl;
	} 
	f.close();  //запись созданного лабиринта в файл
	save_name += "_";

	f.open("files//my levels//" + open_file + " inf.txt", ios::out);
	f << 0 << " " << 0<< " "<<0;
	f.close();

	menu_select = 4;
}

void deleting_level()
{
	int i, j, k;
	ifstream fin;
	ofstream fout;
	string del_str;
	enable_delete = 0;
	_message_box = 0;
	for (i = deleting_lvl_name; i < amount_of_created_levels; i++)
	{
		del_str = to_string(i+1);
		fin.open("files//my levels//" + del_str + ".txt", ios::in);
		fin >> string_e; //имя лабиринта
		for (k = 0; k < amount_h; k++)
		{
			for (j = 0; j < amount_w; j++)
			{
				fin >> maze[j][k];
			}
		}
		fin.close();//копирование данных следущего лабиринта в буфер

		fin.open("files//my levels//" + del_str + " inf.txt", ios::in);
		fin >> best_time;
		fin >> games_played;
		fin >> games_win;
		fin.close();  //чтение лучшего времени и кол-ва сыгранных раз

		del_str = to_string(i);
		fout.open("files//my levels//" + del_str + ".txt", ios::out | ios::trunc);
		fout << string_e << endl; //имя лабиринта
		for (k = 0; k < amount_w; k++)
		{
			for (j = 0; j < amount_h; j++)
			{
				fout << maze[j][k] << " ";
				if ((j + 1) % 5 == 0) fout << " ";
			}
			fout << endl;
			if ((k + 1) % 5 == 0) fout << endl;
		}
		fout.close(); //перенос данных из буфера в файл удаленного лабирина

		fout.open("files//my levels//" + del_str + " inf.txt", ios::out | ios::trunc);
		fout << best_time << " " << games_played << " " << games_win;
		fout.close();  //запись лучшего времени и кол-ва сыгранных раз
	}

	amount_of_created_levels = amount_of_created_levels - 1;
	fout.open("files//my levels//amount of created levels.txt");
	fout << amount_of_created_levels;
	fout.close();
}

void back_func()
{
	if (menu_select == 0)
		_message_box = 1;

	if (menu_select == 1)
		_message_box = 2;

	if ( (menu_select == 2) || (menu_select == 3) || (menu_select == 7)||(menu_select==8))
		menu_select = 0; //возврат в главное меню

	if (menu_select == 4)
		_message_box = 3;

	if (menu_select == 6) //выход из редактора лабиринта
	{
		menu_buffer = 0;
		menu_select = 0; //возврат в главное меню
	}

	if (menu_select == 5)
		menu_select = 4; //возврат в редактор карты
}

void fullscreen_mode()
{
	if (enable_fullscreen == 0)
	{
		glutPositionWindow(10, 30);
		width = 900; height = 650;
		glutReshapeWindow(width, height);
		enable_fullscreen = 1;
	}
	else
	{
		glutFullScreen();
		enable_fullscreen = 0;
	}
}

void discover_maze() //удаление тени с пройденного пути
{
	maze_shadow[me_x + (amount_w / 2)][me_y + (amount_h / 2)] = 1;
	if (me_x != amount_w / 2 - 1)
		maze_shadow[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2)] = 1;
	if (me_x != amount_w / 2 * (-1))
		maze_shadow[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2)] = 1;
	if (me_y != amount_h / 2 - 1)
		maze_shadow[me_x + (amount_w / 2)][me_y + (amount_h / 2) + 1] = 1;
	if (me_y != amount_h / 2 * (-1))
		maze_shadow[me_x + (amount_w / 2)][me_y + (amount_h / 2) - 1] = 1;
	if ((me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 - 1))
		maze_shadow[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2) + 1] = 1;
	if ((me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 * (-1)))
		maze_shadow[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2) - 1] = 1;
	if ((me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 * (-1)))
		maze_shadow[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2) - 1] = 1;
	if ((me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 - 1))
		maze_shadow[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2) + 1] = 1;

	if (shadow_r > 0)
	{
		if ((me_x != amount_w / 2 - 2) && (me_x != amount_w / 2 - 1))
			maze_shadow[me_x + (amount_w / 2) + 2][me_y + (amount_h / 2)] = 1;
		if ((me_x != amount_w / 2 * (-1) + 1) && (me_x != amount_w / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) - 2][me_y + (amount_h / 2)] = 1;
		if ((me_y != amount_h / 2 - 2) && ((me_y != amount_h / 2 - 1)))
			maze_shadow[me_x + (amount_w / 2)][me_y + (amount_h / 2) + 2] = 1;
		if ((me_y != amount_h / 2 * (-1) + 1) && (me_y != amount_h / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2)][me_y + (amount_h / 2) - 2] = 1;
	}

	if (shadow_r > 1)
	{
		if ((me_x != amount_w / 2 - 2) && (me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 - 1))
			maze_shadow[me_x + (amount_w / 2) + 2][me_y + (amount_h / 2)+1] = 1;
		if ((me_x != amount_w / 2 * (-1) + 1) && (me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 - 1))
			maze_shadow[me_x + (amount_w / 2) - 2][me_y + (amount_h / 2)+1] = 1;
		if ((me_x != amount_w / 2 - 2) && (me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) + 2][me_y + (amount_h / 2)-1] = 1;
		if ((me_x != amount_w / 2 * (-1) + 1) && (me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) - 2][me_y + (amount_h / 2)-1] = 1;

		if ((me_y != amount_h / 2 - 2) && ((me_y != amount_h / 2 - 1)) && (me_x != amount_w / 2 - 1))
			maze_shadow[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2) + 2] = 1;
		if ((me_y != amount_h / 2 * (-1) + 1) && (me_y != amount_h / 2 * (-1)) && (me_x != amount_w / 2 - 1))
			maze_shadow[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2) - 2] = 1;
		if ((me_y != amount_h / 2 - 2) && ((me_y != amount_h / 2 - 1)) && (me_x != amount_w / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2) + 2] = 1;
		if ((me_y != amount_h / 2 * (-1) + 1) && (me_y != amount_h / 2 * (-1)) && (me_x != amount_w / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2) - 2] = 1;
	}

	if (shadow_r > 2)
	{
		if ((me_x != amount_w / 2 - 2) && (me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 - 2) && ((me_y != amount_h / 2 - 1)))
			maze_shadow[me_x + (amount_w / 2) + 2][me_y + (amount_h / 2)+2] = 1;
		if ((me_x != amount_w / 2 - 2) && (me_x != amount_w / 2 - 1) && (me_y != amount_h / 2 * (-1) + 1) && (me_y != amount_h / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) + 2][me_y + (amount_h / 2)-2] = 1;
		if ((me_x != amount_w / 2 * (-1) + 1) && (me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 - 2) && ((me_y != amount_h / 2 - 1)))
			maze_shadow[me_x + (amount_w / 2) - 2][me_y + (amount_h / 2)+2] = 1;
		if ((me_x != amount_w / 2 * (-1) + 1) && (me_x != amount_w / 2 * (-1)) && (me_y != amount_h / 2 * (-1) + 1) && (me_y != amount_h / 2 * (-1)))
			maze_shadow[me_x + (amount_w / 2) - 2][me_y + (amount_h / 2)-2] = 1;
	}
}

void maze_action()
{
	discover_maze(); //удаление тени

	if (maze[me_x + (amount_w / 2)][me_y + (amount_h / 2)] == 4)
	{
		maze[me_x + (amount_w / 2)][me_y + (amount_h / 2)] = 0;
		coins++;
		coins_to_file();
	}  //проверка на сбор призов

	if (maze[me_x + (amount_w / 2)][me_y + (amount_w / 2)] == 1)  
	{

		if (best_time == 0)  //запись лучшего времени в файл
			best_time = current_time - start_time;
		else
			if (current_time - start_time < best_time)
				best_time = current_time - start_time;
        games_win++;

		ofstream _open_file;
		_open_file.open("files//" + open_file + " inf.txt", ios::out | ios::trunc);
		_open_file << best_time << " " << games_played<< " "<< games_win;

		if (menu_buffer == 7)
		{
			if (current_level == played_level)
			{
				current_level++;
				progress_to_file();
			} 
		} //открытие нового уровня
		menu_select = 2;
	}//проверка на выигрыш
}

void keyboard(unsigned char key, int localx, int localy)
{
	if (menu_select == 5)
	{
		if (key == 13)  //если "enter"
			saving_maze();		
		else
		{
			if (key == 8)  //если  "back_space"
			{
				if (center_text >= 1)  //проверка, чтобы не нельзя было удалить символ, если в строке данных нет
				{
					save_name.erase(save_name.end() - 2);
					center_text--;
				}
			}
			else //приписываем нажатый символ к строке с названием
			{ 
				/*if ((key == '?') || (key == ':') || (key == '*') || (key == '/') || (key == '\\') || (key == '<') || (key == '>') || (key == '.') || (key == ',') || (key == '\"') || (key == '\'') || (key == ';')||(key==' ')||(key>100))  //проверка на использование символов
				{
					hWnd = FindWindow(L"glut", L"Maze escape");
					MessageBox(hWnd, L"Error: you cannot use this symbols: \n / \\ * . , ? ; : \' \" > < ", L"Error", MB_OK | MB_ICONSTOP | MB_SYSTEMMODAL);
				}
				else*/
				if (((key >= 65) && (key <= 90)) || ((key>=97) && (key<=122))||((key>=48)&&(key<=57)))
				{
					save_name.erase(save_name.end() - 1);
					save_name += key;
					save_name += "_";
					center_text++;
				}
			}
		}
	}
	if (menu_select == 1)
	{
		if (('d' == key) || ('D' == key))
		{
			character_pos = 2;
			if 	(maze[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2)] != 2)
			{
				if (me_x < (amount_w / 2) - 1)   me_x = me_x + 1;
			}
		}

		if (('a' == key) || ('A' == key))
		{
			character_pos = 4;
			if 	(maze[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2)] != 2) 
			{
				if (me_x > (amount_w / 2)*(-1))   me_x = me_x - 1;
			}
		}

		if (('w' == key) || ('W' == key))
		{
			character_pos = 1;
			if  (maze[me_x + (amount_w / 2)][me_y + (amount_h / 2) + 1] != 2)
			{
				if (me_y < (amount_h / 2) - 1)   me_y = me_y + 1;
			}
		}

		if (('s' == key) || ('S' == key))
		{
			character_pos = 3;
			if 	(maze[me_x + (amount_w / 2)][me_y + (amount_h / 2) - 1] != 2) 
			{
				if (me_y > (amount_h / 2)*(-1))   me_y = me_y - 1;
			}
		}//движение персоонажа

		maze_action();
	}

	if (27 == key)
		back_func(); //кнопка назад
}

void keyboard2(int key, int localx, int localy)
{
	if (key == GLUT_KEY_F11)  // вкл/откл fullscreen
		fullscreen_mode();

	if (menu_select == 1)
	{
		if (GLUT_KEY_RIGHT == key)
		{
			character_pos = 2;
			if (maze[me_x + (amount_w / 2) + 1][me_y + (amount_h / 2)] != 2)
			{
				if (me_x < (amount_w / 2) - 1)   me_x = me_x + 1;
			}
		}

		if (GLUT_KEY_LEFT == key)
		{
			character_pos = 4;
			if (maze[me_x + (amount_w / 2) - 1][me_y + (amount_h / 2)] != 2)
			{
				if (me_x > (amount_w / 2)*(-1))   me_x = me_x - 1;
			}
		}

		if (GLUT_KEY_UP == key)
		{
			character_pos = 1;
			if (maze[me_x + (amount_w / 2)][me_y + (amount_h / 2) + 1] != 2)
			{
				if (me_y < (amount_h / 2) - 1)   me_y = me_y + 1;
			}
		}
		if (GLUT_KEY_DOWN == key)
		{
			character_pos = 3;
			if (maze[me_x + (amount_w / 2)][me_y + (amount_h / 2) - 1] != 2)
			{
				if (me_y > (amount_h / 2)*(-1))   me_y = me_y - 1;
			}
		}
	}//движение персоонажа
	maze_action();
}

void character_position() //положение персоонажа (верх, право, право, лево)
{
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, character[character_num]);
	glBegin(GL_TRIANGLES);
	if (character_pos == 1)
	{
		glTexCoord2f(0.0f, 0.0f); glVertex3f(me_x*width_cell + width_cell / 12, me_y*height_cell, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(me_x*width_cell + width_cell - width_cell / 12, me_y*height_cell, 0);
		glTexCoord2f(0.5f, 1.0f); glVertex3f(me_x*width_cell + (width_cell/2), me_y*height_cell + height_cell, 0);

	}
	if (character_pos == 2)
	{
		glTexCoord2f(1.0f, 0.0f); glVertex3f(me_x*width_cell, me_y*height_cell + height_cell / 12, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(me_x*width_cell, me_y*height_cell + height_cell - height_cell / 12, 0);
		glTexCoord2f(0.5f, 1.0f); glVertex3f(me_x*width_cell + width_cell, me_y*height_cell + height_cell/2, 0);

	}
	if (character_pos == 3)
	{
		glTexCoord2f(1.0f, 0.0f); glVertex3f(me_x*width_cell + width_cell / 12, me_y*height_cell + height_cell, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(me_x*width_cell + width_cell - width_cell / 12, me_y*height_cell + height_cell, 0);
		glTexCoord2f(0.5f, 1.0f); glVertex3f(me_x*width_cell + (width_cell / 2), me_y*height_cell, 0);
	}
	if (character_pos == 4)
	{
		glTexCoord2f(1.0f, 0.0f); glVertex3f(me_x*width_cell+height_cell, me_y*height_cell+width_cell-height_cell/12, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(me_x*width_cell+width_cell, me_y*height_cell+height_cell/12, 0);
		glTexCoord2f(0.5f, 1.0f); glVertex3f(me_x*width_cell, me_y*height_cell + height_cell / 2, 0);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}  

void preparing_level()
{
	int i, j, mp_x, mp_y, max_coins;//mp == maze prizes

	ifstream f1;
	f1.open("files//" + open_file + ".txt", ios::in);
	if (menu_select == 6)
		f1 >> string_e;
	for (i = 0; i < amount_h; i++)
	{
		for (j = 0; j < amount_w; j++)
		{
			f1 >> maze[j][i];
			if (maze[j][i] == 3)
			{
				me_x = ((amount_w / 2)*(-1) + j);
				me_y = ((amount_h / 2)*(-1) + i);
			}
		}
	}
	f1.close(); //чтение лабиринта из файла

	f1.open("files//" + open_file + " inf.txt", ios::in);
	f1 >> best_time;
	f1 >> games_played;
	f1.close();  //чтение лучшего времени и кол-ва сыгранных раз

	games_played++;
	ofstream _open_file;
	_open_file.open("files//" + open_file + " inf.txt", ios::out | ios::trunc);
	_open_file << best_time << " " << games_played<< " " << games_win; //запись кол-ва сыгранных уровней

	for (i = 0; i < amount_h; i++)
		for (j = 0; j < amount_w; j++)
			maze_shadow[j][i] = 0;   //cоздание маски(тени) лабиринта

	srand(time(NULL));
	if (menu_select == 7)
		max_coins = 15;
	else max_coins = 7;
	for (i = 0; i < max_coins; i++)
	{
		mp_x = 1 + rand() % amount_w;
		mp_y = 1 + rand() % amount_h;
		if ((maze[mp_x][mp_y] == 0) && (mp_x != me_x) && (mp_y != me_y))
			maze[mp_x][mp_y] = 4;
		else i--;
	} //раскладка призов на карту

	character_pos = 1;
	menu_select = 1;
	discover_maze();
	start_time = clock();
}

void preparing_maze_creator()
{
	menu_select = 4;

	cell_selector = 0;

	sprintf_s(mc_str[0], " -walkway");
	sprintf_s(mc_str[1], " -finish");
	sprintf_s(mc_str[2], " -wall");
	sprintf_s(mc_str[3], " -start");

	total_finishes = 0;
	total_starts = 0;  //обнуление количества общих стартов и финишей
}

void mouse(int button, int state, int x, int y)
{
	int i, j, enable_save = 0;  // проверяет схождение кнопки "save" во время сохранения
	int pl_w, pl_h, k = 0; //необходимо для определиния местоположения относительно центра координат OpenGl
	if (x > width / 2) click_x = x - width / 2;
	else click_x = width / 2 * (-1) + x;
	if (y > height / 2) click_y = (y - height / 2)*(-1);
	else  click_y = height / 2 - y; //перевод из координат окна в координаты opengl
	
	if (_message_box != 0)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{			
			if ((click_y > yesno_b_h) && (click_y < yesno_b_h + yesno_b_size))
			{
				if ((_message_box > 0) && (_message_box <= 4) && (enable_delete == 0))
				{
					if ((click_x > yesno_b_w[0]) && (click_x < yesno_b_w[0] + yesno_b_size*2.5))
					{
						switch (_message_box)
						{
						case 1:
							glutDestroyWindow(num_window); //уничтожить окно
							exit(0); //выход из приложения
							break;
						case 2: menu_select = 0; break;
						case 3: menu_select = 0; break;
						case 4:
							enable_delete++;
							break;

						}
						if (enable_delete != 1)
							_message_box = 0;
					}
					if ((click_x > yesno_b_w[1]) && (click_x < yesno_b_w[1] + yesno_b_size*2.5))
						_message_box = 0;
				}

				else
				{
					if ((_message_box > 4) || (_message_box == -1))
					{
						if ((click_x > ok_button_w) && (click_x < ok_button_w + yesno_b_size*2.5))
							_message_box = 0;
					}
				}
			}
		}
	}
	else
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if ((click_x > -width / 2 + 12) && (click_x<-width / 2 + 12 + office_button_size * 3 / 2) && (click_y>height / 2 - 12 - office_button_size) && (click_y < height / 2 - 12))
				back_func();  //кнопка назад

			if ((click_x<fullscreen_button_w) && (click_x>fullscreen_button_w - office_button_size) && (click_y<fullscreen_button_h) && (click_y>fullscreen_button_h - office_button_size))  //fullscreen
			{
				fullscreen_mode();
			}

			if ((click_x<exit_button_w) && (click_x>exit_button_w - office_button_size) && (click_y<exit_button_h) && (click_y>exit_button_h - office_button_size))
				_message_box = 1;

			if (menu_select == 7)  //переход в игру
			{
				int num_lvl = 1;

				for (j = 1; j > -2; j = j - 2)  //открытие нужного уровня
				{
					for (i = -1; i < 2; i = i + 2)
					{
						if ((num_lvl <= current_level) && (click_x > width / 4 * i - ml_b_size / 2) && (click_x<width / 4 * i + ml_b_size / 2) && (click_y>height / 4 * j - ml_b_size / 2) && (click_y < height / 4 * j + ml_b_size / 2))
						{
							open_file = "programm files//levels//level ";
							open_file.operator+=(to_string(num_lvl)); //добавление числа к строке
							preparing_level();
							menu_buffer = 7;
							played_level = num_lvl;
						}
						num_lvl++;
					}
				}
			}

			if (menu_select == 6)
			{
				int num_lvl = page*total_number_of_cells + 1;
				for (j = 1; j > -2; j = j - 2)
				{
					cross_h = j*ml_b_h + ml_b_size / 2 - ml_b_size / 25;
					for (i = 0; i < total_number_of_cells / 2; i++)
					{
						if ((click_x>ml_b_w + ml_b_size*i * 2) && (click_x<ml_b_w + ml_b_size*i * 2 + ml_b_size) && (click_y>j*ml_b_h - ml_b_size / 2) && (click_y < j*ml_b_h + ml_b_size / 2))
						{
							if (menu_buffer == 0) //если мы переходим в список уровней, созданных пользователем из главного меню
							{
								open_file = "my levels//";
								open_file.operator+=(to_string(num_lvl));
								preparing_level();
							}
							if (menu_buffer == 4) //если мы хотим редактировать уровень
							{
								preparing_maze_creator();
								open_file = (to_string(num_lvl));
								ifstream f;
								f.open("files//my levels//" + open_file + ".txt", ios::in);
								f >> string_e;
								for (i = 0; i < amount_h; i++)
									for (j = 0; j < amount_w; j++)
									{
										f >> maze[j][i];
										if (maze[j][i] == 1) total_finishes++;
										if (maze[j][i] == 3) total_starts++;
									}
								opened_maze = num_lvl;
								f.close();
							}
						}
						cross_w = ml_b_w + ml_b_size*i * 2 + ml_b_size + ml_b_size / 50;
						if ((click_x >cross_w) && (click_x<cross_w + cross_size) && (click_y > cross_h - cross_size) && (click_y < cross_h))
						{
							_message_box = 4;
							deleting_lvl_name = num_lvl;
						}
						num_lvl++;
					}
				}
				arrows_move();
			}

			if (menu_select == 4)  //действия в редакторе
			{
				int buffer;
				pl_w = (-1)*(amount_w / 2)*width_cell;
				pl_h = (-1)*(amount_h / 2)*height_cell; //определиние местоположения относительно центра координат OpenGl

				for (i = 0; i < total_types_of_cells * 2 - 2; i = i + 2)
					if ((click_x>mc_button_w) && (click_x < mc_button_w + mc_button_size) && (click_y<mc_button_h - i*mc_button_size) && (click_y > mc_button_h - i*mc_button_size - mc_button_size))
						cell_selector = i / 2;

				for (j = 0; j < amount_h; j++)
				{
					for (i = 0; i < amount_w; i++)
					{
						if ((click_x >= pl_w) && (click_x < pl_w + width_cell) && (-click_y >= pl_h) && (-click_y < pl_h + height_cell))
						{
							buffer = maze[i][amount_h - 1 - j];  //временно запоминает предыдущее значение клетки, которое мы хотим изменить
							maze[i][amount_h - 1 - j] = cell_selector;

							if ((j == 0) || (j == 49) || (i == 0) || (i == 49))  //ошибка: нельзя менять стены по краям поля на что-либо еще
							{
								_message_box = 5;
								maze[i][amount_h - 1 - j] = 2;
							}

							if (buffer == 1)  //если мы удалили финиш, то уменьшаем текушее кол-во финишей
								total_finishes--;

							if (maze[i][amount_h - 1 - j] == 1)  //ошибка: нельзя ставить больше одного финиша
							{
								total_finishes++;
								if (total_finishes >= 2)
								{
									maze[i][amount_h - 1 - j] = buffer;
									total_finishes--;
									_message_box = 6;
								}
							}

							if (buffer == 3)  //если мы удалили старт, то уменьшаем текушее кол-во финишей
								total_starts--;

							if (maze[i][amount_h - 1 - j] == 3)  //ошибка: нельзя ставить больше одного финиша
							{
								total_starts++;
								if (total_starts >= 2)
								{
									maze[i][amount_h - 1 - j] = buffer;
									total_starts--;
									_message_box = 7;
								}
							}
						}
						pl_w = pl_w + width_cell;
					}
					pl_w = (-1)*(amount_w / 2)*width_cell;
					pl_h = pl_h + height_cell;
				}
				if ((click_x > save_b_w[1]) && (click_x < save_b_w[1] + save_b_size_w) && (click_y < save_b_h) && (click_y > save_b_h + save_b_size_h))
				{
					if ((total_starts == 1) && (total_finishes == 1))
					{
						menu_select = 5;
						enable_save = -1;
					}
					else
					{
						if (total_finishes != 1)
							_message_box = 9;
						if (total_starts != 1)
							_message_box = 8;
					}
				}
			}

			if (menu_select == 5)  //сохранение созданного лабиринта
			{
				enable_save++;
				if ((enable_save == 1) && (click_y < save_b_h) && (click_y > save_b_h + save_b_size_h))
				{
					if ((click_x > save_b_w[0]) && (click_x < save_b_w[0] + save_b_size_w))  //save as new
					{
						int next_number;
						next_number = amount_of_created_levels + 1;
						open_file = (to_string(next_number)); //добавление числа к строке
						saving_maze();
						ofstream f2;
						f2.open("files//my levels//amount of created levels.txt");
						f2 << next_number;
						f2.close();
						amount_of_created_levels++;
					}

					if ((click_x > save_b_w[1]) && (click_x < save_b_w[1] + save_b_size_w))  //save
					{
						if (menu_buffer == 4)
							open_file = (to_string(opened_maze));
						else
						{
							int next_number;
							next_number = amount_of_created_levels + 1;
							open_file = (to_string(next_number));
							ofstream f2;
							f2.open("files//my levels//amount of created levels.txt");
							f2 << next_number;
							f2.close();
							amount_of_created_levels++;
						}
						saving_maze();
					}
				}
			}

			if ((menu_select == 0) && (click_x > -menu_button_w) && (click_x < menu_button_w) && (click_y < menu_button_h[2]) && (click_y>menu_button_h[2] * 2))   //подключение меню редактора лабиринта
			{
				if (mcr_select == 0)
					mcr_select = 1;
				else mcr_select = 0;
			}

			if ((menu_select == 0) && (click_x > -menu_button_w) && (click_x < menu_button_w) && (click_y < menu_button_h[1]) && (click_y>-menu_button_h[1]))  //подключение меню магазина
			{
				if (shop_select == 0)
					shop_select = 1;
				else shop_select = 0;
			}

			if (menu_select == 3)
			{
				if ((click_x>-back_to_menu_w) && (click_x < back_to_menu_w) && (click_y<back_to_menu_h) && (click_y>back_to_menu_h + back_to_menu_h))
					menu_select = 0;  //переход из магазина в главное меню

				if ((click_x>updage_b_w[0]) && (click_x<updage_b_w[0] + updage_size_w) && (click_y>updage_b_h[0]) && (click_y < updage_b_h[0] + updage_size_h))
				{
					if (shadow_r != 3)  //увеличение радиуса обзора
					{

						if (coins - shadow_r_cost[shadow_r] >= 0)
						{
							coins = coins - shadow_r_cost[shadow_r];
							coins_to_file();
							shadow_r++;
							progress_to_file();
						}
						else
							_message_box = 10;
					}
				}

				if ((click_x > onoff_b_w) && (click_x<onoff_b_w + onoff_size) && (click_y>onoff_b_h) && (click_y < onoff_b_h + onoff_size))
				{
					if (coin_viewer == 0)
					{
						if (coins >= coin_viewer_cost)
						{
							coins = coins - coin_viewer_cost;
							coins_to_file();
							coin_viewer = 2;
							progress_to_file();
						}
						else
							_message_box = 10;
					}
					else
					{
						if (coin_viewer == 1)
							coin_viewer = 2;
						else coin_viewer = 1;
						progress_to_file();
					}
				}//покупка и переключение видимости монет
			}

			if (menu_select == 8)
			{
				int num_skin = page*total_number_of_cells + 1;
				for (j = 1; j > -2; j = j - 2)
				{
					for (i = 0; i < total_number_of_cells / 2; i++)
					{
						if ((click_x>ml_b_w + ml_b_size*i * 2) && (click_x<ml_b_w + ml_b_size*i * 2 + ml_b_size) && (click_y>j*ml_b_h - ml_b_size / 2) && (click_y < j*ml_b_h + ml_b_size / 2))
						{
							if (character_onoff[num_skin - 1] != 1)
							{
								if (coins >= character_cost[num_skin - 1])
								{
									coins = coins - character_cost[num_skin - 1];
									coins_to_file();
									character_onoff[num_skin - 1] = 1;
									character_num = num_skin - 1;
								}
								else
									_message_box = 10;
							}
							else character_num = num_skin - 1;

							ofstream open_file;
							open_file.open("files//programm files//skins.txt", ios::trunc | ios::in);
							open_file << character_num << endl;
							for (int k = 0; k < amount_of_skins; k++)
								open_file << character_onoff[k] << " ";
							open_file.close();
						}
						num_skin++;
					}
				}

				arrows_move();
			}

			if ((menu_select == 2) && (click_x > -back_to_menu_w) && (click_x<back_to_menu_w) && (click_y<back_to_menu_h) && (click_y>back_to_menu_h + back_to_menu_h))
				menu_select = 0; //переход из окна victory в main menu

			if (menu_select == 0)
			{
				if ((click_x>about_w) && (click_x<about_w + about_size_w) && (click_y>about_h) && (click_y<about_h + about_size_h))
				{
					_message_box = -1;
					//hWnd = FindWindow(L"glut", L"Maze escape");
					//MessageBox(hWnd, L"program: Maze escape \nmaked by Pavel Bezkorovainiy \nin 2016 \nusing Visual Studio \nversion: 1.0", L"About programm", MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
				}  //about

				if ((click_x > -menu_button_w) && (click_x < menu_button_w) && (click_y < menu_button_h[0]) && (click_y>menu_button_h[0] / 2))
				{
					if (play_select == 0)
						play_select = 1;
					else play_select = 0;
				}//подключение игрового меню

				if ((play_select == 1) && (click_x > pm_b_w) && (click_x < pm_b_w + pm_b_size_w) && (click_y < pm_b_h[1]) && (click_y > pm_b_h[1] - pm_b_size_h))
				{
					page = 0;
					last_page = amount_of_created_levels / total_number_of_cells;
					if (amount_of_created_levels % total_number_of_cells == 0)
						last_page--;
					menu_buffer = 0;
					menu_select = 6;  //переход в меню уровней, созданных пользователем
				}

				if ((play_select == 1) && (click_x > pm_b_w) && (click_x < pm_b_w + pm_b_size_w) && (click_y < pm_b_h[0]) && (click_y > pm_b_h[0] - pm_b_size_h))
					menu_select = 7; //переход в меню стандартных уровней

				if (shop_select == 1)
				{
					if ((click_x >(sm_b_w - sm_b_size_w)) && (click_x <sm_b_w))
					{
						if ((click_y>sm_b_h[0] - sm_b_size_h) && (click_y < sm_b_h[0]))
						{
							menu_select = 3;
						}//переход в магазин

						if ((click_y>sm_b_h[1] - sm_b_size_h) && (click_y < sm_b_h[1]))
						{
							menu_select = 8;
							page = 0;
							last_page = amount_of_skins / total_number_of_cells;
							if (amount_of_skins % total_number_of_cells == 0)
								last_page--;
						}//переход в магазин скинов
					}
				}

				if (mcr_select == 1)
				{
					if ((click_x > mcm_b_w) && (click_x < mcm_b_w + mcm_b_size_w))
					{
						if ((click_y > mcm_b_h[0]) && (click_y < mcm_b_h[0] + mcm_b_size_h)) //переход в редактор карты (новая карта)
						{
							for (j = 0; j < amount_h; j++)
								for (i = 0; i < amount_w; i++)
								{
									if ((j == 0) || (j == 49) || (i == 0) || (i == 49))
										maze[j][i] = 2;
									else maze[j][i] = 0;
								}
							preparing_maze_creator();
						}

						if ((click_y > mcm_b_h[1]) && (click_y < mcm_b_h[1] + mcm_b_size_h)) //переход в редактор карты (редактировать старую карту)
						{
							page = 0;
							last_page = amount_of_created_levels / total_number_of_cells;
							if (amount_of_created_levels % total_number_of_cells == 0)
								last_page--;
							menu_select = 6;
							menu_buffer = 4;
						}
					}
				}
			}
		}


		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		{
			if (menu_select == 4)  //удаление клетки в лабиринте (сделать клетку "проходом" (травой)	
			{
				int buffer;

				pl_w = (-1)*(amount_w / 2)*width_cell;
				pl_h = (-1)*(amount_h / 2)*height_cell; //определиние местоположения относительно центра координат OpenGl

				for (j = 0; j < amount_h; j++)
				{
					for (i = 0; i < amount_w; i++)
					{
						if ((click_x >= pl_w) && (click_x < pl_w + width_cell) && (-click_y >= pl_h) && (-click_y < pl_h + height_cell))
						{
							buffer = maze[i][amount_h - 1 - j];  //временно запоминает предыдущее значение клетки, которое мы хотим изменить
							maze[i][amount_h - 1 - j] = 0;
							if ((j == 0) || (j == 49) || (i == 0) || (i == 49))  //по краям должны соять стены
								maze[i][amount_h - 1 - j] = 2;

							if (buffer == 1)  //если мы удалили финиш, то уменьшаем текушее кол-во финишей
								total_finishes--;

							if (buffer == 3)  //если мы удалили старт, то уменьшаем текушее кол-во финишей
								total_starts--;
						}
						pl_w = pl_w + width_cell;
					}

					pl_w = (-1)*(amount_w / 2)*width_cell;
					pl_h = pl_h + height_cell;
				}
			}
		}
	}
}

void main_menu()
{
	menu_button_w = height / 2 - height / 10 * 3;
	menu_button_h[0] = height / 2 - height / 10;	
	menu_button_h[1] = height / 2 - height / 10 * 4;
	menu_button_h[2] = height / 2 - height / 10 * 7;//размеры кнопок главного меню

	glEnable(GL_TEXTURE_2D);    // включаем обработку текстур
	glBindTexture(GL_TEXTURE_2D, button[0]);    // говорим системе связаться с текстурой с таким-то номером
	glBegin(GL_QUADS); // начало рисования прямоугольника, z == const
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-menu_button_w, menu_button_h[0] - (menu_button_h[0] / 2), 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(menu_button_w, menu_button_h[0] - (menu_button_h[0] / 2), 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(menu_button_w, menu_button_h[0], 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-menu_button_w, menu_button_h[0], 0.0); // привязываем точки полигона к углам текстуры при рисовании
	glEnd();
	glDisable(GL_TEXTURE_2D);  //"play"
	
	glEnable(GL_TEXTURE_2D);  
	glBindTexture(GL_TEXTURE_2D, button[1]);   
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-menu_button_w, -menu_button_h[1] , 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(menu_button_w, -menu_button_h[1], 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(menu_button_w, menu_button_h[1], 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-menu_button_w, menu_button_h[1], 0.0); 
	glEnd();
	glDisable(GL_TEXTURE_2D); //"shop"

	glEnable(GL_TEXTURE_2D);    
	glBindTexture(GL_TEXTURE_2D, button[2]);   
	glBegin(GL_QUADS); 
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-menu_button_w, menu_button_h[2] + menu_button_h[2], 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(menu_button_w, menu_button_h[2] + menu_button_h[2], 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(menu_button_w, menu_button_h[2], 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-menu_button_w, menu_button_h[2], 0.0); 
	glEnd();
	glDisable(GL_TEXTURE_2D);  //"map_creator"

	static float motion_x = 0.0, motion_y = 12.0, motion_plus_x = 0.1, motion_plus_y = 0.1;  //движение кнопок
	static int motion_time = -400;

	about_h = -(height / 2 * 33 / 50 + motion_y);
	about_w = -(menu_button_w + 30 + height / 2 * 30 / 50 + motion_x);
	about_size_h = height / 2 * 22 / 100;
	about_size_w = about_size_h * 2.3;

	motion_x = motion_x - motion_plus_x;
	motion_y = motion_y + motion_plus_y;
	if ((motion_time == -200) || (motion_time == 200))
		motion_plus_x = -motion_plus_x;
	if ((motion_time == 0) || (motion_time == 400))
	{
		motion_plus_y = -motion_plus_y;
		if (motion_time == 400)
			motion_time = 0;
	}
	motion_time++;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, button[11]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(about_w, about_h + about_size_h, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(about_w + about_size_w, about_h + about_size_h, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(about_w + about_size_w, about_h, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(about_w, about_h, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glColor3f(0.0, 0.5, 0.5);
	sprintf_s(string_e, "Your coins: %2d", coins);
	RenderBitmapString(-width / 2 + 50, -height / 2 + 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
}

void play_menu()
{
	int i;
	static float motion_x = 0.0, motion_y = 12.0, motion_plus_x = 0.1, motion_plus_y = 0.1;  //движение кнопок
	static int motion_time = -200;  
	pm_b_h[0] = height / 2 * 4 / 5 + motion_y;
	pm_b_h[1] = height / 2 * 2.7 / 5 - motion_y;
	//pm_b_w = width / 2 * 9 / 20;
	pm_b_w = menu_button_w + 30 + motion_x;
	
	pm_b_size_h = height / 2 * 22 / 100 ;
	pm_b_size_w = pm_b_size_h * 2.3 ;

	motion_x = motion_x + motion_plus_x;
	motion_y = motion_y + motion_plus_y;
	if (motion_time == 200)
	{
		motion_plus_x = -motion_plus_x;
		motion_time = -200;
	}
	if (motion_time == 0) motion_plus_y = -motion_plus_y;
	motion_time++;

	for (i = 3; i < 5; i++)
	{
		glColor3f(0.0, 0.5, 0.5);
		glBegin(GL_LINES);
		glVertex3f(menu_button_w, menu_button_h[0] - (menu_button_h[0] / 4)-i , 0);
		glVertex3f(pm_b_w , pm_b_h[i - 3] - pm_b_size_h / 2, 0);
		glEnd();

		glColor3f(1.0, 1.0, 1.0);                   /////////////////////////////////////////////////////
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, button[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(pm_b_w , pm_b_h[i - 3] , 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(pm_b_w + pm_b_size_w , pm_b_h[i - 3] , 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(pm_b_w + pm_b_size_w , pm_b_h[i - 3] - pm_b_size_h , 0);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(pm_b_w , pm_b_h[i - 3] - pm_b_size_h , 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

void standart_levels()
{
	int i, j, num_lvl=1, lock_level=0, k=1;
	ml_b_size = width / 5;

	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	hWnd = FindWindow(L"glut", L"Maze escape");
	ScreenToClient(hWnd, &mouse_pos); //координаты мыши

	int mouse_pos_x, mouse_pos_y;
	if (mouse_pos.x > width / 2) mouse_pos_x = mouse_pos.x - width / 2;
	else mouse_pos_x = width / 2 * (-1) + mouse_pos.x;
	if (mouse_pos.y > height / 2) mouse_pos_y = (mouse_pos.y - height / 2)*(-1);
	else  mouse_pos_y = height / 2 - mouse_pos.y; //перевод из координат окна в координаты opengl

	for (j = 1; j > -2; j = j - 2)
		for (i = -1; i < 2; i = i + 2)
		{
			{
				if (num_lvl <= current_level)
					lock_level = 5;
				else lock_level = 10;

				glColor3f(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, button[lock_level]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(width / 4 * i - ml_b_size / 2, height / 4 * j + ml_b_size / 2, 0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(width / 4 * i + ml_b_size / 2, height / 4 * j + ml_b_size / 2, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(width / 4 * i + ml_b_size / 2, height / 4 * j - ml_b_size / 2, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(width / 4 * i - ml_b_size / 2, height / 4 * j - ml_b_size / 2, 0);
				glEnd();
				glDisable(GL_TEXTURE_2D);

				glColor3f(0.0, 0.5, 0.5);
				sprintf_s(string_e, "   level %d", num_lvl);
				RenderBitmapString(width / 4 * i - ml_b_size / 2, height / 4 * j - ml_b_size / 2 - 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
				num_lvl++;

				if ((mouse_pos_x > width / 4 * i - ml_b_size / 2) && (mouse_pos_x<width / 4 * i + ml_b_size / 2) && (mouse_pos_y > height / 4 * j - ml_b_size / 2) && (mouse_pos_y < height / 4 * j + ml_b_size / 2))
				{
					string info_string="";
					glColor4f(0.7, 0.7, 0.7, 0.5);
					glBegin(GL_QUADS);
					glVertex3f(mouse_pos_x, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 220, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 220, mouse_pos_y - 60, 0);
					glVertex3f(mouse_pos_x, mouse_pos_y - 60, 0);
					glEnd();
					
					ifstream _open_file;
					open_file = to_string(k);
					_open_file.open("files//programm files//levels//level " + open_file + " inf.txt", ios::in);
					_open_file >> best_time;
					_open_file >> games_played;
					_open_file >> games_win;
					_open_file.close();  //чтение лучшего времени и кол-ва сыгранных раз

					glColor3f(0.0, 0.0, 0.7);
					count_time(best_time/1000);
					info_string += "Best time: ";
					if (best_time == 0)
						info_string += "-";
					else info_string += stopwatch;
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 15, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());

					info_string = "Games played: ";
					info_string += to_string(games_played);
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 35, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());

					info_string = "Wins: ";
					info_string += to_string(games_win);
					if (games_played == 0)
						info_string += " (0%)";
					else
					{
						info_string += " (";
						int _percent = games_win * 100 / games_played;
						info_string += to_string(_percent);
						info_string += "%)";
					}
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 55, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());
				}
				k++;
			}
		}

}

void my_created_levels()
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	hWnd = FindWindow(L"glut", L"Maze escape");
	ScreenToClient(hWnd, &mouse_pos); //координаты мыши

	int mouse_pos_x, mouse_pos_y;
	if (mouse_pos.x > width / 2) mouse_pos_x = mouse_pos.x - width / 2;
	else mouse_pos_x = width / 2 * (-1) + mouse_pos.x;
	if (mouse_pos.y > height / 2) mouse_pos_y = (mouse_pos.y - height / 2)*(-1);
	else  mouse_pos_y = height / 2 - mouse_pos.y; //перевод из координат окна в координаты opengl

	if (amount_of_created_levels == 0)
	{
		glColor3f(0.0, 0.5, 0.5);
		RenderBitmapString(-width / 2 + 50, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You haven't create any levels yet");
	}
	else
	{
		int  number_of_cells, k = 1; //кол-во страниц, кол-во клеток на последней странице, переменная для подсчета номера клетки
		int i, j;

		glColor3f(0.0, 0.5, 0.5);
		string num_of_page;
		num_of_page = to_string(page+1) += " of ";
		num_of_page += to_string(last_page+1);
		RenderBitmapString(-29, -height / 2 + 20, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char *)num_of_page.c_str());

		arrows_w = 4 * width / 2 / 5;
		arrows_h = height / 2 / 4;

		if (page != 0) i = -1;
		else 
		{
			if (last_page != 0)
				i = 1;
			else i = 3;
		}
		glColor3f(1.0, 0.0, 0.0);
		for (i; i < 2; i = i + 2)
		{
			glBegin(GL_TRIANGLES);
			glVertex3f(arrows_w*i, arrows_h, 0);
			glVertex3f(arrows_w*i, -arrows_h, 0);
			glVertex3f((arrows_w + (3 * width / 50))*i, 0, 0);
			glEnd();
			if (page == last_page) i = 3;
		}  //прорисовка стрелочек

		number_of_cells = amount_of_created_levels % total_number_of_cells;

		ml_b_w = width / 9 - width / 2;
		ml_b_h = height / 2 * 2 / 5;
		ml_b_size = width / 9;  //подсчет размеров клетки уровня
		cross_size = ml_b_size / 10;

		k = total_number_of_cells*page + 1;

		ifstream level_name;
		for (j = 1; j > -2; j = j - 2)  //прорисовка клеток уровней
		{
			cross_h = j*ml_b_h + ml_b_size / 2 - ml_b_size / 25;
			for (i = 0; i < total_number_of_cells / 2; i++)
			{
				glColor3f(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, button[5]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(ml_b_w + ml_b_size*i * 2, j*ml_b_h + ml_b_size / 2, 0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size, j*ml_b_h + ml_b_size / 2, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(ml_b_w + ml_b_size*i * 2 + ml_b_size, j*ml_b_h - ml_b_size / 2, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(ml_b_w + ml_b_size*i * 2, j*ml_b_h - ml_b_size / 2, 0);
				glEnd();
				glDisable(GL_TEXTURE_2D);

				cross_w = ml_b_w + ml_b_size*i * 2 + ml_b_size + ml_b_size / 50;

				if ((mouse_pos_x >cross_w) && (mouse_pos_x<cross_w + cross_size) && (mouse_pos_y > cross_h - cross_size) && (mouse_pos_y < cross_h))
				{
					glLineWidth(7);
					glColor3f(1.0, 1.0, 0.0);
					glBegin(GL_LINES);
					glVertex3f(cross_w, cross_h, 0);
					glVertex3f(cross_w + cross_size, cross_h - cross_size, 0);
					glVertex3f(cross_w, cross_h - cross_size, 0);
					glVertex3f(cross_w + cross_size, cross_h, 0);
					glEnd();

					glColor4f(0.7, 0.7, 0.7, 0.5);
					glBegin(GL_QUADS);
					glVertex3f(mouse_pos_x, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 60, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 60, mouse_pos_y - 20, 0);
					glVertex3f(mouse_pos_x, mouse_pos_y - 20, 0);
					glEnd();

					glColor3f(0.0, 0.0, 0.7);
					RenderBitmapString(mouse_pos_x + 5, mouse_pos_y - 15, 0, GLUT_BITMAP_9_BY_15, "Delete");
				}  //подсветка крестика
				glLineWidth(3);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_LINES);
				glVertex3f(cross_w, cross_h, 0);
				glVertex3f(cross_w + cross_size, cross_h - cross_size, 0);
				glVertex3f(cross_w, cross_h - cross_size, 0);
				glVertex3f(cross_w + cross_size, cross_h, 0);
				glEnd(); //прорисовка крестика

				open_file = to_string(k);
				level_name.open("files//my levels//" + open_file + ".txt", ios::in);
				level_name >> string_e;
				level_name.close();

				glColor3f(0.0, 0.5, 0.5);
				RenderBitmapString(ml_b_w + ml_b_size*i * 2 + 3, j*ml_b_h - ml_b_size / 2 - 17, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);

				if ((mouse_pos_x > ml_b_w + ml_b_size*i * 2) && (mouse_pos_x<ml_b_w + ml_b_size*i * 2 + ml_b_size) && (mouse_pos_y > j*ml_b_h - ml_b_size / 2) && (mouse_pos_y < j*ml_b_h + ml_b_size / 2))
				{
					string info_string = "";
					glColor4f(0.7, 0.7, 0.7, 0.5);
					glBegin(GL_QUADS);
					glVertex3f(mouse_pos_x, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 220, mouse_pos_y, 0);
					glVertex3f(mouse_pos_x + 220, mouse_pos_y - 60, 0);
					glVertex3f(mouse_pos_x, mouse_pos_y - 60, 0);
					glEnd();

					ifstream _open_file;
					open_file = to_string(k);
					_open_file.open("files//my levels//" + open_file + " inf.txt", ios::in);
					_open_file >> best_time;
					_open_file >> games_played;
					_open_file >> games_win;
					_open_file.close();  //чтение лучшего времени и кол-ва сыгранных раз 

					glColor3f(0.0, 0.0, 0.7);
					count_time(best_time / 1000);
					info_string += "Best time: ";
					if (best_time == 0)
						info_string += "-";
					else info_string += stopwatch;
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 15, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());

					info_string = "Games played: ";
					info_string += to_string(games_played);
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 35, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());

					info_string = "Wins: ";
					info_string += to_string(games_win);
					if (games_played == 0)
						info_string += " (0%)";
					else
					{
						info_string += " (";
						int _percent = games_win * 100 / games_played;
						info_string += to_string(_percent);
						info_string += "%)";
					}
					RenderBitmapString(mouse_pos_x + 2, mouse_pos_y - 55, 0, GLUT_BITMAP_9_BY_15, (char *)info_string.c_str());
				}

				if (k == amount_of_created_levels)
				{
					i = total_number_of_cells / 2;
					j = -2;
				}
				k++;
			}
		}
	}
}

void create_maze()
{
	int i, j, pl_w, pl_h;

	glColor3f(0.0, 0.5, 0.5);
	sprintf_s(string_e, "Your coins: %2d", coins);
	RenderBitmapString(-width / 2 + 50, -height / 2 + 30, 0, GLUT_BITMAP_TIMES_ROMAN_24, string_e);
	
	pl_w = (-1)*(amount_w / 2)*width_cell;
	pl_h = (-1)*(amount_h / 2)*height_cell; //определиние местоположения относительно центра координат OpenGl

	for (i = 0; i < amount_w; i++) //прорисовка лабиринта
	{
		for (j = 0; j < amount_h; j++)
		{
			if (maze_shadow[i][j] == 1)
			{
				
				glColor3f(1.0, 1.0, 1.0);
				glEnable(GL_TEXTURE_2D);  
				glBindTexture(GL_TEXTURE_2D, cell[maze[i][j]]);
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + pl_h, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + pl_h, 0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}
			else 
			{
				glColor3f(0.0, 0.0, 0.0);
				glBegin(GL_POLYGON);
				glVertex3f(i*width_cell + pl_w, j*height_cell + pl_h, 0);
				glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + pl_h, 0);
				glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
				glVertex3f(i*width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
				glEnd();
			}
			if (coin_viewer == 2)
			{
				if (maze[i][j] == 4)
				{
					glColor3f(1.0, 1.0, 1.0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, cell[4]);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + pl_h, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + pl_h, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
				if (maze[i][j] == 5)
				{
					glColor3f(1.0, 1.0, 1.0);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, cell[5]);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + pl_h, 0);
					glTexCoord2f(1.0f, 0.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + pl_h, 0);
					glTexCoord2f(1.0f, 1.0f); glVertex3f(i*width_cell + width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
					glTexCoord2f(0.0f, 1.0f); glVertex3f(i*width_cell + pl_w, j*height_cell + height_cell + pl_h, 0);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}
			}

		}
	}

	character_position(); //прорисовка персоонажа

	glColor3f(0.0, 0.0, 1.0);
	RenderBitmapString(pl_w, height / 2 - 31, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Best time:");  //вывод лучшего  времени
	if (best_time != 0)
		time_to_window(pl_w, height / 2 - 54, 0, best_time / 1000);
	else RenderBitmapString(pl_w, height / 2 - 54, 0, GLUT_BITMAP_TIMES_ROMAN_24, "-");

	RenderBitmapString(0, height / 2 - 31, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Your time:");
	current_time = clock();
	time_to_window(0, height / 2 - 54, 0, (current_time - start_time) / 1000);  //вывод секундомера
}

void victory()
{
	int prize_location; //расположение кубка
	
	back_to_menu_h = -height/2 + (height / 10 * 3);
	back_to_menu_w = height / 2 - height / 10 * 3;
	
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mm_button);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-back_to_menu_w, back_to_menu_h + back_to_menu_h, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(back_to_menu_w, back_to_menu_h + back_to_menu_h, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(back_to_menu_w, back_to_menu_h, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-back_to_menu_w, back_to_menu_h, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if (width > height)
		prize_location = height / 6;
	else prize_location = width / 6;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, prize);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-prize_location, -prize_location + prize_location/2, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(prize_location, -prize_location + prize_location/2, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(prize_location, prize_location + prize_location/2, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-prize_location, prize_location + prize_location/2, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glColor3f(0.7, 0.5, 0.0);
	RenderBitmapString(-52, -prize_location+17, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You win!!!");

	string str_e;
	glColor3f(0.0, 0.0, 1.0);
	str_e = "Best time: ";
	int cur_sec=0, cur_min = 0, cur_hour = 0;  //текущее время: секунды, минуты, часы
	cur_sec = best_time / 1000;
	if (cur_sec > 3599)  //подсчет кол-ва часов
	{
		cur_hour = cur_sec / 3600;
		cur_sec = cur_sec - (cur_hour * 3600);
	}
	if (cur_sec > 59)  //подсчет кол-ва минут
	{
		cur_min = cur_sec / 60;
		cur_sec = cur_sec - (cur_min * 60);
	}
	if (cur_hour != 0) //если у нас есть какое-либо кол-во часов, записываем это в строку
	{
		str_e += to_string(cur_hour);
		str_e += " h ";
	}
	if (cur_min != 0) //если у нас есть какое-либо кол-во минут, записываем это в строку
	{
		str_e += to_string(cur_min);
		str_e += " min ";
	}
	str_e += to_string(cur_sec);
	str_e += " sec";
	RenderBitmapString(prize_location + 20, 45, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char *)str_e.c_str());

	str_e = "Your time: ";
	str_e += stopwatch;
	RenderBitmapString(prize_location + 20, 15, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char *)str_e.c_str());
}

void display_message_box()
{
	if (enable_delete == 2)
		deleting_level();

	int i, k = 0;

	glColor3f(0.3, 0.9, 0.9);
	glBegin(GL_POLYGON);
	if (_message_box == -1)
	{
		glVertex3f(-165, 140, 0);
		glVertex3f(165, 140, 0);
	}
	else
	{
		glVertex3f(-165, 90, 0);
		glVertex3f(165, 90, 0);
	}
	glVertex3f(165, -90, 0);
	glVertex3f(-165, -90, 0);
	glEnd();

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	if (_message_box == -1)
	{
		glVertex3f(-155, 130, 0);
		glVertex3f(155, 130, 0);
	}
	else
	{
		glVertex3f(-155, 80, 0);
		glVertex3f(155, 80, 0);
	}
	glVertex3f(155, -80, 0);
	glVertex3f(-155, -80, 0);
	glEnd();

	glColor3f(0.05, 0.1, 0.1);

	string version_info;
	switch (_message_box)
	{
	case -1:
		RenderBitmapString(-150, 100, 0, GLUT_BITMAP_TIMES_ROMAN_24, "program: Maze escape");
		RenderBitmapString(-150, 75, 0, GLUT_BITMAP_TIMES_ROMAN_24, "maked by Pavel Bezkorovainiy");
		RenderBitmapString(-150, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "in 2016");
		RenderBitmapString(-150, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "using Visual Studio");
		version_info = "version: 1.0 (";
		version_info += to_string(current_version);
		version_info += ")";
		RenderBitmapString(-150, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, (char *)version_info.c_str());
		break;

	case 1:
		RenderBitmapString(-130, 45, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Do you really want to exit?");
		break;

	case 2:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Your progress will lost.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Exit to menu?");
		break;

	case 3:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "All unsaved data will be lost.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Exit to menu?");
		break;

	case 4:
		if (enable_delete == 1)
		{
			RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Please wait.");
			RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Deleting...");
			enable_delete = 2;
		}
		else
		{
			RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Do you really want to delete");
			RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "this level?");
		}
		break;

	case 5:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "There should be a wall.");
		break;

	case 6:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You cannot place more than");
		RenderBitmapString(-135, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, "one finish.");
		break;

	case 7:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You cannot place more than");
		RenderBitmapString(-135, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24, "one start.");
		break;

	case 8:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You forgot to place start.");
		break;

	case 9:
		RenderBitmapString(-135, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-135, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You forgot to place finish.");
		break;

	case 10:
		RenderBitmapString(-138, 50, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Error.");
		RenderBitmapString(-138, 25, 0, GLUT_BITMAP_TIMES_ROMAN_24, "You haven't so much money.");
		break;

	}

	glColor3f(1.0, 1.0, 1.0);
	if ((_message_box > 0) && (_message_box <= 4) && (enable_delete == 0))
	{
		for (i = -135; i < 50; i = i + 163)
		{
			yesno_b_w[k] = i;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, yesno_button[k]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(yesno_b_w[k], yesno_b_h, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(yesno_b_w[k] + yesno_b_size*2.5, yesno_b_h, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(yesno_b_w[k] + yesno_b_size*2.5, yesno_b_h + yesno_b_size, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(yesno_b_w[k], yesno_b_h + yesno_b_size, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
			k++;
		}
	}
	else
	{
		if ((_message_box > 4) || (_message_box == -1))
		{
			ok_button_w = -54;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ok_button);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(ok_button_w, yesno_b_h, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(ok_button_w + yesno_b_size*2.5, yesno_b_h, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(ok_button_w + yesno_b_size*2.5, yesno_b_h + yesno_b_size, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(ok_button_w, yesno_b_h + yesno_b_size, 0);
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_QUADS);
	glVertex3f(-width / 2 + 10, -height / 2 + 10, 0);
	glVertex3f(width / 2 - 10, -height / 2 + 10, 0);
	glVertex3f(width / 2 - 10, height / 2 - 10, 0);
	glVertex3f(-width / 2 + 10, height / 2 - 10, 0);
	glEnd();  //большой белый квадрат

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, screen[enable_fullscreen]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(fullscreen_button_w, fullscreen_button_h, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(fullscreen_button_w - office_button_size, fullscreen_button_h, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(fullscreen_button_w - office_button_size, fullscreen_button_h - office_button_size, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(fullscreen_button_w, fullscreen_button_h - office_button_size, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D); //прорисовка кнопки "fullscreen"

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, exit_b);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(exit_button_w, exit_button_h, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(exit_button_w - office_button_size, exit_button_h, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(exit_button_w - office_button_size, exit_button_h - office_button_size, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(exit_button_w, exit_button_h - office_button_size, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);  //прорисовка кнопки "exit"

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, back_arrow);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width / 2 + 12 , height / 2 - 12 - office_button_size, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width / 2 + 12 + office_button_size*3/2, height / 2 - 12 - office_button_size, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width / 2 + 12 + office_button_size*3/2, height / 2 - 12, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width / 2 + 12 , height/2 - 12 , 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);  //прорисовка кнопки "back arrow"
	
	switch (menu_select)
	{
	   case 0:
			if (play_select == 1)
				play_menu(); //подключение игрового меню
			if (shop_select == 1)
				shop_menu();//подключение меню магазина
			if (mcr_select == 1)
				maze_creator_menu();//подключение меню редактора карты
			main_menu(); 
			break;
	    case 1:create_maze(); break;
	    case 2:victory(); break;
	    case 3:shop(); break;  
	    case 4:maze_creator(); break;
		case 5:save_created_maze(); break;
		case 6:my_created_levels(); break;
		case 7:standart_levels(); break;
		case 8:skins_shop(); break;

		default: main_menu(); break;
	}

	if (_message_box != 0)
		display_message_box();

	glFlush();
	glFinish();
}

void start_loading()
{
	ifstream _open_file;

	_open_file.open("files//programm files//version info.txt", ios::in);
	_open_file >> current_version;
	_open_file.close(); //сбор текущей версии программы

	_open_file.open("files//my levels//amount of created levels.txt", ios::in);
	_open_file >> amount_of_created_levels;
	_open_file.close();  //сбор кол-ва уровней, созданных пользователем

	_open_file.open("files//programm files//coins.txt", ios::in);
	_open_file >> coins;
	_open_file.close(); //сбор количества монет из файла 

	_open_file.open("files//programm files//progress.txt", ios::in);
	_open_file >> current_level; //сбор текущего уровня
	_open_file >> coin_viewer;  //сбор активности видимости монет
	_open_file >> shadow_r;  //сбор радиуса видимости
	_open_file.close();  

	_open_file.open("files//programm files//skins.txt", ios::in);
	_open_file >> character_num;
	for (int i = 0; i < amount_of_skins; i++)
		_open_file >> character_onoff[i];
	_open_file.close(); //сбор номера активного скина
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Loading..." << endl << "Please wait";
    start_loading();
	ShowWindow(GetConsoleWindow(), SW_HIDE);  //спрятать консоль

	glutInit(&argc, (char**)argv);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	num_window = glutCreateWindow("Maze escape");    
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard2);	
	glutMouseFunc(mouse);
    LoadGLTextures();
	glutMainLoop();
	return 0;
}
