#include <iostream>
#include <cmath>
#include <ctime>
#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
///buttons+



char buttons[6][25] = { "buttons\\play.bmp", "buttons\\Highscore.bmp", "buttons\\Instructions.bmp", "buttons\\Options.bmp", "buttons\\Credit.bmp", "buttons\\Exit.bmp" };
char optionbutton[4][20] = { "RED.bmp", "GREEN.bmp", "Easy.bmp", "Hard.bmp" };
int gamestate = -1;

int optionState = 0;
char resume[10] = { "" }; // find a way to start game
char play[10] = { "" };
bool over = false;

/*Button Co-ordinate Structure*/
struct buttonCoOrdinate
{
	int x;
	int y;

}buttonCordinate[8];

struct optionbuttonCoOrdinate
{
	int x;
	int y;

}obuttonCordinate[4];


bool green=true;
bool hard = false;//Select difficulty: a binary choice (either hard or easy)//
int x, y,loader,v,up,down,lt,rt;
int bullet_x, bullet_y;

bool bullet_visible = false;
int bullet_speed = 5;
int bullet_dx, bullet_dy;
int spaceship_dx, spaceship_dy;
bool is_space_down = false;
int asteroid_x[20];
int asteroid_y[20];
bool asteroid_visible[20];
int asteroid_radius[20];
int asteroid_count = 20;//chnage count for how many asteroids you want to destroy
int hit_count = 3;
float asteroid_dx[1000];
float asteroid_dy[10000];
int high_score = 0;
int  current_time;
int start_time = time(NULL);
using namespace std;






void readHighScore() {
	FILE *file;
	int x;
	x = fopen_s(&file, "highscore.txt", "r");
	if (file == NULL) {
		printf("Error opening file!\n");
		return;
	}
	fscanf_s(file, "%d", &high_score);
	fclose(file);
}


void saveHighScore() {
	FILE *file;
	int f1;
	f1 = fopen_s(&file, "highscore.txt", "w");
	if (file == NULL) {
		printf("Error opening file!\n");
		return;
	}
	fprintf(file, "%d", high_score);
	fclose(file);
}
void gameOver() {
	saveHighScore();
	// add code to exit the game
	exit(0);
}




// Call initTimer() at the start of the game and displayTime() in the main function to display the remaining time









void displayHighScore() {
	iSetColor(255, 255, 255);
	iRectangle(10, 450, 100, 440); // draw a box to display high score
	char high_score_string[5];
	sprintf_s(high_score_string, "%d", high_score);
	iText(20, 440, high_score_string);
}

void move_asteroids() {//MOVING THE ASTEROIDS
	int ex = iLoadImage("explode.png");
	for (int i = 0; i<asteroid_count; i++) {
		asteroid_dx[i] = rand() % 5 -2;
		asteroid_dy[i] = rand() % 5 -2;

		if (asteroid_x[i] > 500 || asteroid_x[i] < 0)
			asteroid_dx[i] = -asteroid_dx[i];
		if (asteroid_y[i] > 480 || asteroid_y[i] < 0)
			asteroid_dy[i] = -asteroid_dy[i];

		asteroid_x[i] += asteroid_dx[i];
		asteroid_y[i] += asteroid_dy[i];
	}
}

int health = 100;
int rotate_angle = 90;
bool rotated = false;//{"model2.bmp","model2_90.bmp","model2_180.bmp", "model2_270.bmp"};
//char image_file[5][20] = { "model2.bmp", "model2_90.bmp", "model2_180.bmp", "model2_270.bmp" };
int score = 0; // add a score variable
//int high_score = 0; // add a high score variable
char back[3][100] = { "background.bmp", "background_03_static.bmp","background_04_static.bmp" };
//char back[2][100] = {"background_03_static.bmp", "background.bmp"  };// First background image file // Second background image file
static int current_image = 0;


void newRound() {//START OF A NEW ROUND//
	
	
	
	iShowBMP(0, 0, back[current_image]);
	
	
	asteroid_count = 20;

	

	// spawn new asteroids
	for (int i = 0; i < 20; i++) {
		asteroid_visible[i] = true;
		asteroid_x[i] = rand() % 480;
		asteroid_y[i] = rand() % 480;
		if (!hard){
			asteroid_dx[i] += 1;
			asteroid_dy[i] += 1;
			asteroid_x[i] += asteroid_dx[i]; // move the asteroid in a random direction
			asteroid_y[i] += asteroid_dy[i];
		}
		else{
			asteroid_dx[i] += 2;
			asteroid_dy[i] += 2;
			asteroid_x[i] += asteroid_dx[i]; // move the asteroid in a random direction
			asteroid_y[i] += asteroid_dy[i];
		}
		
	}
	
}


bool bullet_fired = false;


void move_spaceship() {//NOTE: IT DOES NOT MOVE THE SHIP, THIS IS FOR THE GAMEPLAY
	iClear();

	iShowBMP(0, 0, back[current_image]);
	iSetColor(255, 255, 255);
	
	//DISPLAY HEALTH AND LEVELS
	iSetColor(255, 0, 0);
	iRectangle(550, 550, 580, 570);
	iFilledRectangle(450, 450, (health / 10) * 5, 10);
	char life_string[5];
	sprintf_s(life_string, "%.0lf", health);
	iSetColor(0, 0, 0);
	//leveling the level
	char level_text[50];
	sprintf_s(level_text, "Level: %d", current_image+1);
	iSetColor(255, 0, 0);
	iText(217, 50, level_text);
	//iSetColor(255, 0, 0);


	//SPACEHSIP IMAGE
	int image_file[4] = { down,rt,up,lt };

	if (1){
		
		iShowImage(x - 25, y - 15,50,50,image_file[rotate_angle/90]);//change to 0
		rotated = true;
	}
	

	//iFilledCircle(x, y, 30); //draw spaceship
	//BULLETS FIRE AND VISBLITY//
	if (bullet_visible) {
		iSetColor(255, 0, 0);
		iFilledCircle(bullet_x, bullet_y, 2); //draw bullet
		bullet_x += bullet_dx * bullet_speed;
		bullet_y += bullet_dy * bullet_speed;
		if (bullet_y > 480 || bullet_y < 0 || bullet_x > 500 || bullet_x < 0) bullet_visible = false; // if bullet is out of screen, hide it
	}

	//SPAWN ASTEROIDS AND ALSO CHECKS IF BULLETS HIT THE ASTEROID//
	for (int i = 0; i < 20; i++) {//Change count for visible asteroids
		if (asteroid_visible[i]) {
			v = iLoadImage("transback.png");
			iShowImage(asteroid_x[i] - asteroid_radius[i], asteroid_y[i] - asteroid_radius[i], asteroid_radius[i] * 2, asteroid_radius[i] * 2, v);

			//double asteroid_dx[i] = rand() % 3 - 1;

			asteroid_x[i] += asteroid_dx[i]; // move the asteroid in a random direction
			asteroid_y[i] += asteroid_dy[i];

			//Check if asteroid hits the edges of the screen and change direction
			//CHECKS IF BULLET HITS THE ASTEROID, IF SO THEN ASTEROID VANISHES WITH EXPLOSION ALSO SCORE INCREASES AS WELL
			if (asteroid_x[i] > 480 || asteroid_x[i] < 0) asteroid_dx[i] *= -1;
			if (asteroid_y[i] > 480 || asteroid_y[i] < 0) asteroid_dy[i] *= -1;

			if (bullet_visible && (sqrt((bullet_x - asteroid_x[i]) * (bullet_x - asteroid_x[i]) + (bullet_y - asteroid_y[i]) * (bullet_y - asteroid_y[i])) <= asteroid_radius[i])) {
				int ex = iLoadImage("explode.png");
				asteroid_visible[i] = false;
				iShowImage(asteroid_x[i] - asteroid_radius[i]-1, asteroid_y[i] - asteroid_radius[i]-1, asteroid_radius[i] * 3, asteroid_radius[i] * 3, ex);
				/////////////
				bullet_visible = false;
				rotated = false;
				
				PlaySound("Damage.wav", NULL, SND_LOOP | SND_ASYNC);
				
				asteroid_count--;
				
				if (asteroid_radius[i] < 10) {
					if (!hard){
						score += 2;
					}//this will be 2
					else{
						score += 1;
					}
				}
				else {
					if (!hard){
						score += 4;
					}///this wil be 4
					else{
						score += 2;
					}
				}
				if (score > high_score) {
					high_score = score;
					saveHighScore();
				}

				if (asteroid_count == 0) {
					printf("You win! Your score is: %d\n", score);
					
					current_image += 1;
					if (current_image > 0){
						gamestate = 6;
						PlaySound(NULL, NULL, 0);
						PlaySound("youwon.wav", NULL, SND_LOOP | SND_ASYNC);
						health = 100;
						current_image = 0;
						score = 0;
					}

					//youwin
					else
					newRound();
				}
			}
			iSetColor(255, 255, 255);
			////////////////////////////coloring the bar on top
			iSetColor(255, 255, 255);
			
			iSetColor(255, 0, 0);
			iText(20, 470, "Current Score:");
			iSetColor(0, 255, 0);
			char score_string[5];
			sprintf_s(score_string, "%d", score);
			iText(20, 450, score_string, GLUT_BITMAP_HELVETICA_18);
			iSetColor(255, 0, 0);
			iText(20, 420, "High Score:");
			iSetColor(0, 255, 0);
			char high_score_string[5];
			sprintf_s(high_score_string, "%d", high_score);
			iText(20, 400, high_score_string, GLUT_BITMAP_HELVETICA_18);
			//BELOW CODES CHECK IF THE SPACESHIP GETS DAMAGED AND ALSO DECREASE THE LIFE
			if (sqrt((x - asteroid_x[i]) * (x - asteroid_x[i]) + (y - asteroid_y[i]) * (y - asteroid_y[i])) <= asteroid_radius[i] + 15) {
				if (hard){
					if (asteroid_radius[i] > 16) {
						health -= 4;
					}
					else {
						health -= 2;
					}
				}
				else{
					if (asteroid_radius[i] > 16) {
						health -= 2;
					}
					else {
						health -= 1;
					}
				}
				if (health <= 0) {
					printf("You lost!\n");
					gamestate = 7;
					PlaySound(NULL, NULL, 0);
					PlaySound("gameover.wav", NULL, SND_LOOP | SND_ASYNC);
					health = 100;
					current_image = 0;
					score = 0;
					

					
				}
			}
		}
	}
	
}


void iDraw()// 
{
	iClear();

	//Show Menu -start
	if (gamestate == -1)
	{
		//main
		iShowBMP(0, 0, "bgmenu\\bgmain.bmp");
		iText(165, 25, "Press HOME Key to return Main Menu", GLUT_BITMAP_8_BY_13);

		//Menu button code -start
		int sum = 75;
		for (int i = 5; i >= 0; i--)
		{
			//y: 75px to 450px
			buttonCordinate[i].x = 235;
			buttonCordinate[i].y = sum;
			sum += 50;
		}
		//Menu button code -Ends

		for (int i = 0; i < 6; i++)
		{
			iShowBMP(buttonCordinate[i].x, buttonCordinate[i].y, buttons[i]);
		}



	}

	else if (gamestate == 0)	 //play
	{
		//game codes
		iSetColor(255, 255, 255);
		int n = iLoadImage("transback.png");

		iShowImage(0, 0, 50, 50, n);
		move_spaceship();

	}
	else if (gamestate == 1)
	{
		iShowBMP(0, 0, "bgmenu\\bgHighscore.bmp");	 //highscore

		iText(230, 250, "High Score", GLUT_BITMAP_TIMES_ROMAN_24);
		//iSetColor(0, 255, 0);
		char high_score_string[5];
		sprintf_s(high_score_string, "%d", high_score);
		iText(280, 210, high_score_string, GLUT_BITMAP_HELVETICA_18);

	}
	else if (gamestate == 2)
	{
		iShowBMP(0, 0, "bgmenu\\bgInstructions.bmp");	//	instructions

		iText(0, 250, "  use W, A, S, D, SPACE to fire and control bullet", GLUT_BITMAP_TIMES_ROMAN_24); //write instructions



	}
	else if (gamestate == 3)
	{
		iShowBMP(0, 0, "bgmenu\\bgOptions.bmp");	 //options

		iText(220, 350, "Select SpaceShip", GLUT_BITMAP_HELVETICA_18);

		for (int j = 0; j < 4; j++)
		{

			iShowBMP(obuttonCordinate[j].x, obuttonCordinate[j].y, optionbutton[j]);
		}

		int sum2 = 170; //y axis place
		for (int j = 1; j >= 0; j--)
		{
			//y: 75px to 450px
			obuttonCordinate[j].x = sum2;
			obuttonCordinate[j].y = 230;
			sum2 += 150;
		}
		sum2 = 170;
		for (int j = 3; j >= 2; j--)
		{
			//y: 75px to 450px
			obuttonCordinate[j].x = sum2;
			obuttonCordinate[j].y =125;
			sum2 += 150;




		}



		if (optionState == 0)
		{
			green = false;
			up = iLoadImage("model.png");//unused items
			lt = iLoadImage("model_90.png");
			down = iLoadImage("model_180.png");
			rt = iLoadImage("model_270.png");

		}
		else if (optionState == 1)
		{
			green = false;
			up = iLoadImage("Red0.png");//unused items
			lt = iLoadImage("Red90.png");
			down = iLoadImage("Red180.png");
			rt = iLoadImage("Red270.png");
		}
		else if (optionState == 2)
		{
			hard = false;
		}
		else if (optionState == 3)
		{
			hard = true;
		}

		iText(220, 50, "Select Difficulty Level", GLUT_BITMAP_HELVETICA_18);
		


	}
	else if (gamestate == 4)
	{
		iShowBMP(0, 0, "bgmenu\\bgCredit.bmp");	 //credit
		iText(230, 360, "Welcome!", GLUT_BITMAP_HELVETICA_18);

		iText(150, 340, "TEAM PROJECT- DOOM EXPLODER", GLUT_BITMAP_HELVETICA_18);

		iText(200, 310, "PREPARED BY", GLUT_BITMAP_HELVETICA_18);

		iText(100, 280, "NAEEM BEG, 35", GLUT_BITMAP_HELVETICA_18);
		iText(100, 250, "ABU DOJANA, 39", GLUT_BITMAP_HELVETICA_18);
		iText(100, 220, "ANANTO NAYNA BALA, 28", GLUT_BITMAP_HELVETICA_18);

		iText(165, 25, "Press HOME Key to return Main Menu", GLUT_BITMAP_8_BY_13);
	}
	else if (gamestate == 5)
	{
		gameOver();	 //exit
	}
	else if (gamestate == 6)
	{
		iShowBMP(0, 0, "background.bmp");	 //youwin
		iText(200, 250, "YOU  WIN !!!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(165, 25, "Press HOME Key to return Main Menu", GLUT_BITMAP_8_BY_13);
	}
	else if (gamestate == 7)
	{
		iShowBMP(0, 0, "background.bmp");	 //youlose
		iText(200, 250, "GAME OVER !!!", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(165, 25, "Press HOME Key to return Main Menu", GLUT_BITMAP_8_BY_13);
	}

	//Show Menu -end



}







void iKeyboard(unsigned char key) {
	if (key == 'w') {
		bullet_dx = 0;
		bullet_dy = -1;
		rotate_angle = 0;
	}
	else if (key == 'a') {
		bullet_dx = -1;
		bullet_dy = 0;
		rotate_angle = 90;
	}
	else if (key == 's') {
		bullet_dx = 0;
		bullet_dy = 1;
		rotate_angle = 180;
	}
	else if (key == 'd') {
		bullet_dx = 1;
		bullet_dy = 0;
		rotate_angle = 270;
	}
	else if (key == ' ') {
		bullet_visible = true;

		bullet_x = x;
		bullet_y = y;

		if (!bullet_fired){
			PlaySound("pew.wav", NULL, SND_LOOP | SND_ASYNC);

			bullet_fired = true;

		}
		
			bullet_fired = false;
			//PlaySound(NULL, NULL, 0);
		
	}

}




void iMouseMove(int mx, int my)
{

}

void iSpecialKeyboard(unsigned char key)
{
	if (key == GLUT_KEY_HOME)
	{
		gamestate = -1;
	}
}
void iMouse(int button, int state, int mx, int my)
{

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		for (int i = 0; i < 8; i++)
		{

			//150x25 is button dimension
			if (mx >= buttonCordinate[i].x && mx <= buttonCordinate[i].x + 150 && my >= buttonCordinate[i].y && my <= buttonCordinate[i].y + 25)
			{
				gamestate = i;
			}

		}

		for (int j = 0; j < 4; j++)
		{

			//150x25 is button dimension
			if (mx >= obuttonCordinate[j].x && mx <= obuttonCordinate[j].x + 90 && my >= obuttonCordinate[j].y && my <= obuttonCordinate[j].y + 90)
			{
				optionState = j;
			}

		}

	}


	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

void iPassiveMouseMove(int mx, int my)
{

}



int main()
{

	int sum = 35;
	for (int i = 7; i >= 0; i--)
	{
		//y: 75px to 450px
		buttonCordinate[i].x = 150;
		buttonCordinate[i].y = sum;
		sum += 50;
	}

	iInitialize(500, 480, "DOOM EXPLODER");
	
	


	readHighScore();
	PlaySound("youwon.wav", NULL, SND_LOOP | SND_ASYNC);

	x = 250;
	y = 240;
	

	if (green)
	{
		green = false;
		up = iLoadImage("model.png");//unused items
		lt = iLoadImage("model_90.png");
		down = iLoadImage("model_180.png");
		rt = iLoadImage("model_270.png");

	}

	for (int i = 0; i < 20; i++) {
		asteroid_x[i] = rand() % 480;
		asteroid_y[i] = rand() % 480;
		move_asteroids();
		asteroid_visible[i] = true;
		asteroid_radius[i] = rand() % 10 + 5;
	}
	spaceship_dx = 0;
	spaceship_dy = 0;
	iStart();
	
	
	return 0;
}



