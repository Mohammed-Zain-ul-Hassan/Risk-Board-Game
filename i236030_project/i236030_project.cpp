//============================================================================
// Name        : Mohammed Zain-ul-Hassan
// Section     : SE-B
// Roll no.    : i23-6030
//============================================================================

#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_
#include <GL/glew.h>
#include <GL/freeglut.h>
//#include "Board.h"
#include "External Lib/Soil2/include/SOIL2/SOIL2.h"
#include "util.h"
#include "CImg.h"
#include <iostream>
#include <fstream>
#include<string>
#include<cmath> // for basic math functions such as cos, sin, sqrt
using namespace std;
//using namespace cimg_library;

GLuint textureID = 0;
/*//////////////////////////////////////////////////////////////////// NOTE ///////////////////////////////////////////////////////////////////////////
		*******  Welcome to RISK IN SPACE by Mohammed Zain ul Hassan, A place where boundaries are beyond imagination,    ********
		*******  troops travel across the territories through a 3 dimensional space, their space shuttles allow them to   ******** 
		*******  launch Artillery beyond the biverse.																      ********
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
/* All The GAMESTATES
	menu
	Highscores
	selectpl
	start
	Instructions
	credits
	dboard
	pl1color
	pl2color
	pl3color
	pl4color
	pl5color
	pl6color
	pl1name
	pl2name
	pl3name
	pl4name
	pl5name
	pl6name
	phase2
	fortify 
	attack
	end
*/

//////////////////////////////////////////////////////////////// FUNCTIONS ////////////////////////////////////////////////////////////////////////////

// To save the progress of the game and load the previous one 

void save() {
	for (int i = 0; i < 10; i++) {
		scores[i] = -1;
	}
	// Read existing high scores
	fstream file("Highscores.txt", ios::in);
	if (file.is_open()) {
		int i = 0;
		while (i < 10 && file >> names[i] >> scores[i]) {
			i++;
		}
		file.close();  // Close the file for reading
	}
	else {
		cerr << "Error opening the file for reading." << endl;
	}

	file.open("Highscores.txt", ios::out);
	if (file.is_open()) {

		// Find the correct position to insert the new score
		int insertIndex = 10;
		for (int i = 0; i < 10; i++) {
			if (scores[i] == -1 || newscore > scores[i]) {
				insertIndex = i;
				break;
			}
		}

		// Shift scores to make room for the new score
		for (int i = 9; i > insertIndex; i--) {
			scores[i] = scores[i - 1];
			names[i] = names[i - 1];
		}

		// Insert the new score
		if (newscore >= 30) {
			scores[insertIndex] = newscore;
			names[insertIndex] = newname;
		}
		// Write the updated scores to the file
		file.seekp(0);
		for (int i = 0; i < 10; i++) {
			if (scores[i] != -1) {
				file << names[i] << " " << scores[i] << endl;
			}
		}

		if (file.fail()) {
			cerr << "Error writing to the file." << file.rdstate()
				<< (file.rdstate() & ios::failbit ? " failbit " : "")
				<< (file.rdstate() & ios::badbit ? " badbit " : "")
				<< (file.rdstate() & ios::eofbit ? " eofbit " : "") << endl;
		}
		else {
			cout << "High scores updated successfully." << endl;
		}
	}
	else {
		cerr << "Error opening the file for writing." << endl;
	}
}

void escape() {
	DrawRoundRect(7 + shadow, height - 53 - shadow, 140, 45, colors[71], 20);
	DrawRoundRect(10, height - 55, 140, 45, colors[72], 20);
	DrawString(17, height - 40, "< Press Esc", colors[BLACK]);
}

// For Dynamic Board

void dboard() {
	DrawRoundRect(60 + 300 + shadow, height - 250 - shadow, 300, 115, colors[71], 20);
	DrawRoundRect(60 + 300, height - 250, 300, 115, colors[72], 20);
	DrawString(435, height - 200, "Choose Board: ", colors[WHITE]);
	DrawRoundRect(60 + 300 + shadow, 150 - shadow, 300, 375, colors[71], 20);
	DrawRoundRect(60 + 300, 150, 300, 375, colors[72], 20);
	DrawRoundRect(100 + 330, height - 396, 150, 75, colors[71], 42);
	DrawString(470, height - 366, "Board 1", colors[WHITE]);
	DrawRoundRect(100 + 330, height - 506, 150, 75, colors[71], 42);
	DrawString(470, height - 474, "Board 2", colors[137]);
	DrawRoundRect(100 + 330, height - 620, 150, 75, colors[71], 42);
	DrawString(470, height - 590, "Board 3", colors[137]);
}

// Initializing the color of troops with background color as default

void initTerrColor() {
	for (int i = 0; i < 51; i++) {
		for (int j = 0; j < 3; j++) {
			terrcolor[i][j] = colors[120];  //120
		}
	}
	for (int i = 0; i < 51; i++) {
		for (int j = 0; j < 3; j++) {
			playercolor[i] = colors[120];  //120
		}
	}
}

// Shows Troop in a certain territory

void troops() {
	if (TroopShow) {
		DrawRoundRect(50 + 300 + shadow, height - 130 - shadow, 300, 115, colors[118], 20);
		DrawRoundRect(50 + 300, height - 130, 300, 115, colors[120], 20);
		for (int i = 0; i < 51; i++) {
			if (show[i]) {
				DrawString(380, height - 50, "Inft : " + to_string(terr[i][0]) + "     Cavl: " + to_string(terr[i][1]), colors[BLACK]);
				DrawString(380, height - 100, "Artil : " + to_string(terr[i][2]), colors[BLACK]);
			}
		}
	}

}

// Initializes the main gameplay

void start() {
	// One Dice for every player
	int maxroll = -1;

	for (int i = 0; i < playernum; i++) {
		dice[i] = rand() % 6 + 1;
		if (dice[i] > maxroll) {
			maxroll = dice[i];
			maximum = i + 1;
		}
	}
	for (int i = 0; i < 6; i++) {
		if (terrNum[i] == 51) {
			newscore = cscore[i];
			newname = cnames[i];
			gameState = "end";
		}
	}

	//DrawString(475, height - 100, "Player " + to_string(max) + " has rolled the highest.!", colors[WHITE]);
	//territory[chosen][1] += 1;
	//territory[chosen] = 1;
	//terrNum[max-1] +=1;
	//TroopNum[max-1][0] += 1;
	//cscore[max-1] += 1;
	//count++;
}

// A function to represent the credits in the game

void credit() {
	DrawRoundRect(50 + 300 + shadow, height - 150 - shadow, 300, 115, colors[71], 20);
	DrawRoundRect(50 + 300, height - 150, 300, 115, colors[72], 20);
	DrawString(450, height - 100, "Credits", colors[WHITE]);
	DrawRoundRect(325 + shadow, 330 - shadow, 350, 270, colors[71], 20);
	DrawRoundRect(325, 330, 350, 270, colors[72], 20);
	DrawString(370, height - 295, "Logic and Implementation:", colors[BLACK]);
	DrawString(350, height - 350, "-> Mohammed Zain ul Hassan", colors[WHITE]);
}

// A Function to explain instructions

void instructions() {
	DrawRoundRect(175 + shadow, 110 - shadow, 700, 650, colors[72], 20);
	DrawRoundRect(175, 110, 700, 650, colors[71], 20);
	DrawString(430, height - 100, "Instructions", colors[WHITE]);
	DrawLine(430, height - 108, 550, height - 108, 3, colors[WHITE]);
	DrawString(200, height - 160, "1. Hover with the mouse and click to select the desired option. ", colors[WHITE]);
	DrawString(200, height - 200, "2. Select the number of players , Enter their names and select colors. ", colors[WHITE]);
	DrawString(200, height - 240, "3. Occupy your desired Territory by clicking in it's middle. ", colors[WHITE]);
	DrawString(200, height - 280, "4. Occupy all the territories and Dominate the Kingdom . ", colors[WHITE]);
	DrawString(200, height - 320, "5. Circle Represents Infantry on the Board.", colors[WHITE]);
	DrawString(200, height - 360, "6. Triangle Represents Cavalry on the Board. ", colors[WHITE]);
	DrawString(200, height - 400, "7. Square Represents Artillery on the Board. ", colors[WHITE]);
	DrawString(200, height - 440, "8. Empty Troop space means , it's yet to be occupied. ", colors[WHITE]);
	DrawString(200, height - 480, "9. Win Cards by occupying territories and dominate by Attacking. ", colors[WHITE]);
	DrawString(200, height - 520, "10. In the End , Eliminate everyone else and win the game . ", colors[WHITE]);
}

// A Function to take the names of players as input on screen

void playernames(int i = 1) {
	DrawRoundRect(50 + 300 + shadow, height - 150 - shadow, 300, 115, colors[71], 20);
	DrawRoundRect(50 + 300, height - 150, 300, 115, colors[72], 20);
	DrawString(450, height - 100, "Player " + to_string(i), colors[WHITE]);
	DrawRoundRect(325 + shadow, 330 - shadow, 350, 270, colors[71], 20);
	DrawRoundRect(325, 330, 350, 270, colors[72], 20);
	DrawRoundRect(360, height - 375, 250, 70, colors[WHITE], 20);
	DrawString(370, height - 275, "Enter Name:", colors[WHITE]);
	DrawString(390, height - 350,cnames[i-1], colors[BLACK]);
	DrawRoundRect(250 + shadow, height - 575 - shadow, 500, 75, colors[118], 20);
	DrawRoundRect(250, height - 575, 500, 75, colors[120], 20);
	DrawString(320, height - 550, "Blank Spaces won't be considered!!!", colors[BLACK]);
}

// A function to get the choice of user whether he wants to Attack , Fortify or move

void phasor() {
	DrawRoundRect(360 + shadow, 250 - shadow, 300, 375, colors[71], 20);
	DrawRoundRect(360 , 250, 300, 375, colors[72], 20);
	DrawRoundRect(430, height - 266, 150, 50, colors[71], 20);
	DrawString(140 + 330, height - 250, "Select ", colors[WHITE]);
	DrawString(100 + 300, height - 330, cnames[currentPlayer - 1] + "'s Turn:", colors[143]);
	DrawRoundRect(440, height - 390, 120, 40, colors[71], 20);
	DrawRoundRect(440, height - 440, 120, 40, colors[71], 20);
	DrawRoundRect(440, height - 490, 120, 40, colors[71], 20);
	DrawString(470, height - 380, "Fortify", colors[137]);
	DrawString(470, height - 430, "Attack", colors[137]);
	DrawString(475, height - 480, "Pass", colors[137]);
	for (int i = 0; i < playernum; i++) {
		if (terrNum[i] == 51) {
			newname = cnames[i];
			newscore = cscore[i];
			gameState = "end";
			save();
		}
	}
}

// This Function is called at the end to display user's name and Score and how he has won 

void end() {
	DrawRoundRect(360 + shadow, 300 - shadow, 300, 325, colors[71], 20);
	DrawRoundRect(360, 300, 300, 325, colors[72], 20);
	DrawRoundRect(430, height - 266, 150, 50, colors[71], 20);
	DrawString(140 + 325, height - 250, "Winner!!!", colors[WHITE]);
	DrawString(100 + 300, height - 330, newname + " has Won!", colors[143]);
	DrawString(100 + 300, height - 380, "Score : " + to_string(newscore), colors[143]);
	DrawString(100 + 300, height - 430, "World Dominated !!", colors[143]);
}

// Sets the size of Canvas

void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

// To Draw the background of the game

void DrawBackground(){
	int top = height / 16;
	for (int i = 0; i < 3; i++) {
	}
	for (int i = 0; i < (width / 16); i++){
		for (int j = 0; j < top; j++){
			if (!bgDrawn){
				int x = rand() % 3;
				if (x == 1){
					//To set up Different spaces between lines
					background[i][j] = 141;
				}
				else if (x == 0){
					background[i][j] = 142;
					colorMod[i][j] = 1;
				}
				else{
					background[i][j] = 141;
				}

				if (gameState == "menu"){
					if (i > 25)
						background[i][j] = 141;
				}
				DrawSquare(i * 40 - colorMod[i][i], j * 40 - colorMod[j][i], 40 + (2 * colorMod[j][i]), colors[background[j][i]]);
			}
			else{
				DrawSquare(i * 40 - colorMod[j][i], j * 40 - colorMod[j][i], 40 + (2 * colorMod[j][i]), colors[background[j][i]]);
			}
		}
	}
	bgDrawn = 1;
	for (int i = 0; i < 3; i++) {
	}
}

// Draws BackGround Image

void DrawImage(const char* imagePath) {
	// Generate a simple 2x2 checkerboard image if the texture is not loaded
	if (textureID == 0) {
		int width, height, channels;
		unsigned char* imageData = SOIL_load_image(imagePath, &width, &height, &channels, SOIL_LOAD_RGBA);
		// Checking if the image data is point to something or not
		if (imageData == nullptr) {
			std::cerr << "Error loading image: " << SOIL_last_result() << std::endl;
			return;
		}
		for (int i = 0; i < 3; i++) {
		}
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

		// Set texture parameters (you may want to adjust these based on your needs)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(imageData);  // Free the loaded image data
	}

	// Enable blending for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Enable texture mapping
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Draw a textured quad with adjusted coordinates and dimensions
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, 0);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(1024, 0);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(1024, 800);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(0, 800);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
}


// To Show the Menu on the screen

void Menu()
{
	DrawRoundRect(50 + 300 + shadow, height - 150 - shadow, 300, 115, colors[71], 20);
	DrawRoundRect(50 + 300, height - 150, 300, 115, colors[72], 20);
	DrawString(415, height - 100, "RISK IN SPACE", colors[WHITE]);
	DrawRoundRect(250 + shadow, height - 250 - shadow, 500, 75, colors[118], 20);
	DrawRoundRect(250, height - 250, 500, 75, colors[120], 20);
	DrawString(325, height - 220, "It's Time to Transcend Boundaries!!!", colors[BLACK]);
	//DrawLine()
	DrawRoundRect(60 + 300+ shadow, 150 - shadow, 300, 375, colors[71], 20);
	DrawRoundRect(60 + 300, 150, 300, 375, colors[72], 20);
	DrawRoundRect(100 + 330, height - 366, 150, 50, colors[71], 20);
	DrawString(140 + 330, height - 350, "Menu", colors[WHITE]);
	DrawString(100 + 300, height - 430, "New Game", colors[143]);
	DrawString(100 + 300, height - 480, "LeaderBoard", colors[143]);
	DrawString(100 + 300, height - 530, "Instructions", colors[143]);
	DrawString(100 + 300, height - 580, "Credits", colors[143]);


	/*DrawString(100 + 300, height - 600, "Game History", colors[143]);
	DrawString(100 + 300, height - 650, "Exit", colors[143]);*/
}

//Shows a image
//void showimage() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	glDrawPixels(imgData.size() / 3, imgData.size() / (3 * 3), GL_RGB, GL_UNSIGNED_BYTE, &imgData[0]);
//	glFlush();
//}

// To Select the number of Players

void selectpl() {
	DrawRoundRect(200 + shadow, height - 200 - shadow, 600, 115, colors[71], 20);
	DrawRoundRect(200, height - 200, 600, 115, colors[72], 20);
	DrawString(435, height - 150, "Select Players", colors[WHITE]);
	DrawRoundRect(280 + shadow, height - 350 - shadow, 200, 115, colors[71], 20);
	DrawRoundRect(280, height - 350, 200, 115, colors[72], 20);
	DrawString(330, height - 300, "2 Players", colors[BLACK]);
	DrawRoundRect(530 + shadow, height - 350 - shadow, 200, 115, colors[71], 20);
	DrawRoundRect(530, height - 350, 200, 115, colors[72], 20);
	DrawString(580, height - 300, "3 Players", colors[BLACK]);
	DrawRoundRect(280 + shadow, height - 500 - shadow, 200, 115, colors[71], 20);
	DrawRoundRect(280, height - 500, 200, 115, colors[72], 20);
	DrawString(330, height - 450, "4 Players", colors[BLACK]);
	DrawRoundRect(530 + shadow, height - 500 - shadow, 200, 115, colors[71], 20);
	DrawRoundRect(530, height - 500, 200, 115, colors[72], 20);
	DrawString(580, height - 450, "5 Players", colors[BLACK]);
	DrawRoundRect(410 + shadow, height - 650 - shadow, 200, 115, colors[71], 20);
	DrawRoundRect(410, height - 650, 200, 115, colors[72], 20);
	DrawString(460, height - 600, "6 Players", colors[BLACK]);
}

bool colorchose = 1;

// To Select the color for each player

void color(int i) {
	DrawRoundRect(375 + shadow, height - 150 - shadow, 300, 115, colors[118], 20);
	DrawRoundRect(375, height - 150, 300, 115, colors[120], 20);
	DrawString(465, height - 100, "Player " + to_string(i), colors[BLACK]);
	DrawRoundRect(60 + 300 + shadow, 150 - shadow, 350, 420, colors[118], 20);
	DrawRoundRect(60 + 300, 150, 350, 420, colors[120], 20);
	DrawRoundRect(410, height - 320, 250, 50, colors[118], 20);
	DrawString(140 + 320, height - 305, "Choose Color", colors[WHITE]);
	DrawString(100 + 300, height - 380, "1. Blue", colors[143]);
	DrawCircle(570, height - 370, 17.0, colors[BLACK]);
	DrawCircle(570,height - 370 , 15.0 , colors[BLUE]);
	DrawString(100 + 300, height - 420, "2. Yellow", colors[143]);
	DrawCircle(570, height - 410, 17.0, colors[BLACK]);
	DrawCircle(570, height - 410, 15.0, colors[22]);
	DrawString(100 + 300, height - 460, "3. Red", colors[143]);
	DrawCircle(570, height - 450, 17.0, colors[BLACK]);
	DrawCircle(570, height - 450, 15.0, colors[RED]);
	DrawString(100 + 300, height - 500, "4. Green", colors[143]);
	DrawCircle(570, height - 490, 17.0, colors[BLACK]);
	DrawCircle(570, height - 490, 15.0, colors[30]);
	DrawString(100 + 300, height - 540, "5. Pink", colors[143]);
	DrawCircle(570, height - 530, 17.0, colors[BLACK]);
	DrawCircle(570, height - 530, 15.0, colors[90]);
	DrawString(100 + 300, height - 580, "6. Purple", colors[143]);
	DrawCircle(570, height - 570, 17.0, colors[BLACK]);
	DrawCircle(570, height - 570, 15.0, colors[70]);
}

// To Draw the shape of one territory

void DrawTerritory(int tx = 100,int ty = 700, float* color1 = terrcolor[0][0], float* color2 = terrcolor[0][0], float* color3 = terrcolor[0][0]) {

	DrawHexagon2(tx, ty, 48-factor, colors[WHITE]);
	DrawHexagon2(tx, ty, 45-factor, color1);
	/*DrawCircle(tx - 15, ty - 15, 8, colors[BLACK]);
	DrawCircle(tx - 15, ty - 15, 6, color1);
	DrawTriangle(tx - 9, ty + 8, tx + 9, ty + 8, tx + 1, ty + 23, colors[BLACK]);
	DrawTriangle(tx - 6, ty + 10, tx + 6, ty + 10, tx + 1, ty + 20, color2);
	DrawSquare(tx + 1, ty - 28, 18, colors[BLACK]);
	DrawSquare(tx + 3, ty - 25, 13, color3);*/
}

// To Draw the whole map floating in space

void Board() {
	DrawTerritory(60, 730, terrcolor[0][0], terrcolor[0][1], terrcolor[0][2]);			// Territory 1
	DrawTerritory(140, 730, terrcolor[1][0], terrcolor[1][1], terrcolor[1][2]);			// Territory 2
	DrawTerritory(100, 660, terrcolor[2][0], terrcolor[2][1], terrcolor[2][2]);			// Territory 3
	DrawTerritory(180, 660, terrcolor[3][0], terrcolor[3][1], terrcolor[3][2]);			// Territory 4
	DrawTerritory(260, 660, terrcolor[4][0], terrcolor[4][1], terrcolor[4][2]);			// Territory 5
	DrawTerritory(340, 660, terrcolor[5][0], terrcolor[5][1], terrcolor[5][2]);			// Territory 6
	DrawTerritory(420, 660, terrcolor[6][0], terrcolor[6][1], terrcolor[6][2]);			// Territory 7
	DrawTerritory(380, 730, terrcolor[7][0], terrcolor[7][1], terrcolor[7][2]);			// Territory 8
	DrawTerritory(140, 590, terrcolor[8][0], terrcolor[8][1], terrcolor[8][2]);			// Territory 9
	DrawTerritory(220, 590, terrcolor[9][0], terrcolor[9][1], terrcolor[9][2]);			// Territory 10
	DrawTerritory(300, 590, terrcolor[10][0], terrcolor[10][1], terrcolor[10][2]);		// Territory 11
	DrawTerritory(100, 520, terrcolor[11][0], terrcolor[11][1], terrcolor[11][2]);		// Territory 12
	DrawTerritory(340, 520, terrcolor[12][0], terrcolor[12][1], terrcolor[12][2]);		// Territory 13
	DrawTerritory(580, 740, terrcolor[13][0], terrcolor[13][1], terrcolor[13][2]);		// Territory 14
	DrawTerritory(660, 740, terrcolor[14][0], terrcolor[14][1], terrcolor[14][2]);		// Territory 15
	DrawTerritory(700, 670, terrcolor[15][0], terrcolor[15][1], terrcolor[15][2]);		// Territory 16
	DrawTerritory(580, 600, terrcolor[16][0], terrcolor[16][1], terrcolor[16][2]);		// Territory 17
	DrawTerritory(660, 600, terrcolor[17][0], terrcolor[17][1], terrcolor[17][2]);		// Territory 18
	DrawTerritory(740, 600, terrcolor[18][0], terrcolor[18][1], terrcolor[18][2]);		// Territory 19
	DrawTerritory(460, 530, terrcolor[19][0], terrcolor[19][1], terrcolor[19][2]);		// Territory 20
	DrawTerritory(540, 530, terrcolor[20][0], terrcolor[20][1], terrcolor[20][2]);		// Territory 21
	DrawTerritory(620, 530, terrcolor[21][0], terrcolor[21][1], terrcolor[21][2]);		// Territory 22
	DrawTerritory(700, 530, terrcolor[22][0], terrcolor[22][1], terrcolor[22][2]);		// Territory 23
	DrawTerritory(660, 460, terrcolor[23][0], terrcolor[23][1], terrcolor[23][2]);		// Territory 24
	DrawTerritory(420, 440, terrcolor[24][0], terrcolor[24][1], terrcolor[24][2]);		// Territory 25
	DrawTerritory(500, 440, terrcolor[25][0], terrcolor[25][1], terrcolor[25][2]);		// Territory 26
	DrawTerritory(380, 370, terrcolor[26][0], terrcolor[26][1], terrcolor[26][2]);		// Territory 27
	DrawTerritory(300, 370, terrcolor[27][0], terrcolor[27][1], terrcolor[27][2]);		// Territory 28
	DrawTerritory(220, 370, terrcolor[28][0], terrcolor[28][1], terrcolor[28][2]);		// Territory 29
	DrawTerritory(140, 370, terrcolor[29][0], terrcolor[29][1], terrcolor[29][2]);		// Territory 30
	DrawTerritory(180, 440, terrcolor[30][0], terrcolor[30][1], terrcolor[30][2]);		// Territory 31
	DrawTerritory(340, 300, terrcolor[31][0], terrcolor[31][1], terrcolor[31][2]);		// Territory 32
	DrawTerritory(420, 300, terrcolor[32][0], terrcolor[32][1], terrcolor[32][2]);		// Territory 33
	DrawTerritory(580, 350, terrcolor[33][0], terrcolor[33][1], terrcolor[33][2]);		// Territory 34
	DrawTerritory(660, 350, terrcolor[34][0], terrcolor[34][1], terrcolor[34][2]);		// Territory 35
	DrawTerritory(740, 350, terrcolor[35][0], terrcolor[35][1], terrcolor[35][2]);		// Territory 36
	DrawTerritory(140, 120, terrcolor[36][0], terrcolor[36][1], terrcolor[36][2]);		// Territory 37
	DrawTerritory(220, 120, terrcolor[37][0], terrcolor[37][1], terrcolor[37][2]);		// Territory 38
	DrawTerritory(180, 190, terrcolor[38][0], terrcolor[38][1], terrcolor[38][2]);		// Territory 39
	DrawTerritory(260, 190, terrcolor[39][0], terrcolor[39][1], terrcolor[39][2]);		// Territory 40
	DrawTerritory(140, 260, terrcolor[40][0], terrcolor[40][1], terrcolor[40][2]);		// Territory 41
	DrawTerritory(500, 190, terrcolor[41][0], terrcolor[41][1], terrcolor[41][2]);		// Territory 42
	DrawTerritory(580, 190, terrcolor[42][0], terrcolor[42][1], terrcolor[42][2]);		// Territory 43
	DrawTerritory(660, 190, terrcolor[43][0], terrcolor[43][1], terrcolor[43][2]);		// Territory 44
	DrawTerritory(740, 190, terrcolor[44][0], terrcolor[44][1], terrcolor[44][2]);		// Territory 45
	DrawTerritory(700, 260, terrcolor[45][0], terrcolor[45][1], terrcolor[45][2]);		// Territory 46
	DrawTerritory(700, 120, terrcolor[46][0], terrcolor[46][1], terrcolor[46][2]);		// Territory 47
	DrawTerritory(620, 120, terrcolor[47][0], terrcolor[47][1], terrcolor[47][2]);		// Territory 48
	DrawTerritory(540, 120, terrcolor[48][0], terrcolor[48][1], terrcolor[48][2]);		// Territory 49
	DrawTerritory(420, 120, terrcolor[49][0], terrcolor[49][1], terrcolor[49][2]);		// Territory 50
	DrawTerritory(340, 120, terrcolor[50][0], terrcolor[50][1], terrcolor[50][2]);		// Territory 51
	// To check if the territories are filled
	if (terrcount == 51) {
		currentPlayer = maximum;
		gameState = "phase2";
		terrcount += 1;
	}
	//DrawLine(100, 652, 100, 670, 3, colors[WHITE]);
}

// Increments troops based on territories

void increment() {
	for (int i = 0; i < playernum; i++) {
		troopNum[i][0] += ((terrNum[i] * terrNum[i])/6);
	}
	for (int i = 0; i < 51; i++) {
		for (int j = 0; j < playernum; j++) {
			if (territory[i] == j+1) {
				terr[i][0] += (terrNum[j])/6;
			}
		}
	}
}

// To Display the Player ID along with the map

void initPlayer() {
	//drawroundrect
	//drawstring -> current player
	//drawstring -> terrNum
	//drawstring -> troopNum each on seperate line
	DrawRoundRect(800 + shadow, height-400 - shadow, 210, 350, colors[118], 20);
	DrawRoundRect(800 , height - 400, 210, 350, colors[120], 20);
	DrawRoundRect(805 + shadow, height - 170, 190, 110, colors[WHITE], 20);
	DrawRoundRect(808 + shadow, height - 165, 184, 100, colors[BLACK], 20);
	DrawRoundRect(822, height - 160, 170, 90, playercolor[currentPlayer - 1], 20);
	DrawRoundRect(823, height - 145, 168, 60, colors[BLACK], 20);
	DrawString(826,height - 120 , "PLAYER INFO:", colors[WHITE]);
	DrawLine(826, height - 130, 990, height - 130, 2, colors[WHITE]);
	DrawString(816, height - 200," * "+ cnames[currentPlayer - 1] + " *", colors[BLACK]);
	DrawString(826, height - 235,"Territories:  "+ to_string(terrNum[currentPlayer - 1]), colors[BLACK]);
	DrawString(826, height - 270,"Score :  " + to_string(cscore[currentPlayer - 1]), colors[BLACK]);
	DrawString(826, height - 305, "Troops:" , colors[BLACK]);
	DrawString(826, height - 340, "Inft: "+ to_string(troopNum[currentPlayer-1][0]) + "   Cavl: " + to_string(troopNum[currentPlayer - 1][1]), colors[BLACK]);
	DrawString(826, height - 375, "Artil: " + to_string(troopNum[currentPlayer - 1][2]), colors[BLACK]);
	//cout << cscore[currentPlayer - 1] << endl;
}

// To Display if a certain player has won a card and benefitting them with it , by randomly assigning the winner a card

void dispCard() {
	string* cards = new string[3];
	cards[0] = "Infantry";
	cards[1] = "Cavalry";
	cards[2] = "Artillery";
	int random = rand() % 3;
	cardss[currentPlayer - 1][random] += 1;
	DrawRoundRect(360 + shadow, 250 - shadow, 300, 375, colors[71], 20);
	DrawRoundRect(360, 250, 300, 375, colors[72], 20);
	DrawRoundRect(430, height - 266, 150, 50, colors[71], 20);
	DrawString(140 + 330, height - 250, "CARD ", colors[WHITE]);
	DrawString(100 + 300, height - 330, "You have been rewarded with,", colors[143]);
	DrawString(100 + 300, height - 380, "A card for Domination", colors[143]);
	DrawString(500, height - 430, "of", colors[143]);
	DrawString(100 + 300, height - 480,cards[random], colors[143]);
	delete[] cards;
	card = 0;
	for (int i = 0; i < 3; i++) {
		if (cardss[currentPlayer - 1][i] == 3) {
			troopNum[currentPlayer - 1][i] += 5;
		}
	}

}

// To Show instructions while a player is attacking

void attack() {
	if (attacking) {
		DrawRoundRect(300 + shadow, height - 130 - shadow, 400, 120, colors[118], 20);
		DrawRoundRect(300, height - 130, 400, 120, colors[120], 20);
		DrawString(330, height - 50, "One can't choose their own territory", colors[BLACK]);
		attacking = 0;
	}
	else if (attack2) {
		DrawRoundRect(300 + shadow, height - 130 - shadow, 400, 120, colors[118], 20);
		DrawRoundRect(300, height - 130, 400, 120, colors[120], 20);
		DrawString(330, height - 50, "Attack Successful!!", colors[BLACK]);
		attack2 = 0;
	}
	else if (attack3) {
		DrawRoundRect(300 + shadow, height - 130 - shadow, 400, 120, colors[118], 20);
		DrawRoundRect(300, height - 130, 400, 120, colors[120], 20);
		DrawString(330, height - 50, "Defended Successfully!!", colors[BLACK]);
		attack3 = 0;
	}
}

// To Show instructions while player is fortifying his territory

void fortify() {
	if (fortification) {
		DrawRoundRect(300 + shadow, height - 130 - shadow, 400, 120, colors[118], 20);
		DrawRoundRect(300, height - 130, 400, 120, colors[120], 20);
		DrawString(330, height - 50, "One must choose their own territory", colors[BLACK]);
	}
}

// To Display Player Troops

void initDice() {
	DrawRoundRect(800 + shadow, height - 570 - shadow, 210, 150, colors[118], 20);
	DrawRoundRect(800, height - 570, 210, 150, colors[120], 20);
	DrawString(826, height - 500,"Rolled "+ to_string(dice[currentPlayer-1]), colors[BLACK]);
}

// A Funbox , collection of all the colors showed on the board

void funbox() {
	DrawRoundRect(830, 70, 170, 140, colors[120], 20);
	DrawRoundRect(840, 80, 150, 120, colors[BLUE], 20);
	DrawRoundRect(850, 90, 130, 100, colors[70], 20);
	DrawRoundRect(860, 100, 110, 80, colors[RED], 20);
	DrawRoundRect(870, 110, 90, 60, colors[30], 20);
	DrawRoundRect(880, 120, 70, 40, colors[90], 20);
	DrawRoundRect(890, 130, 50, 20, colors[22], 10);
	//DrawString(890,133,"WIN!",colors[BLACK]);
}

// To Display the top 10 highscores

void highscore() {
	DrawRoundRect(250 + shadow, 110 - shadow, 500, 650, colors[72], 20);
	DrawRoundRect(250, 110, 500, 650, colors[71], 20);
	DrawString(427, height - 100, "Highscores", colors[WHITE]);
	DrawLine(427,height - 108 , 540 , height - 108 , 3 , colors[WHITE]);
	DrawString(325, height - 150, "Names", colors[WHITE]);
	DrawLine(325, height - 158, 395, height - 158, 3, colors[WHITE]);
	DrawString(580, height - 150, "Scores", colors[WHITE]);
	DrawLine(580, height - 158, 645, height - 158, 3, colors[WHITE]);
	DrawString(325, height - 200, names[0], colors[WHITE]);
	if(!(names[0].empty()))
		DrawString(580, height - 200, to_string(scores[0]), colors[WHITE]);
	DrawString(325, height - 250, names[1], colors[WHITE]);
	if (!(names[1].empty()))
		DrawString(580, height - 250, to_string(scores[1]), colors[WHITE]);
	DrawString(325, height - 300, names[2], colors[WHITE]);
	if (!(names[2].empty()))
		DrawString(580, height - 300, to_string(scores[2]), colors[WHITE]);
	DrawString(325, height - 350, names[3], colors[WHITE]);
	if (!(names[3].empty()))
		DrawString(580, height - 350, to_string(scores[3]), colors[WHITE]);
	DrawString(325, height - 400, names[4], colors[WHITE]);
	if (!(names[4].empty()))
		DrawString(580, height - 400, to_string(scores[4]), colors[WHITE]);
	DrawString(325, height - 450, names[5], colors[WHITE]);
	if (!(names[5].empty()))
		DrawString(580, height - 450, to_string(scores[5]), colors[WHITE]);
	DrawString(325, height - 500, names[6], colors[WHITE]);
	if (!(names[6].empty()))
		DrawString(580, height - 500, to_string(scores[6]), colors[WHITE]);
	DrawString(325, height - 550, names[7], colors[WHITE]);
	if (!(names[7].empty()))
		DrawString(580, height - 550, to_string(scores[7]), colors[WHITE]);
	DrawString(325, height - 600, names[8], colors[WHITE]);
	if (!(names[8].empty()))
		DrawString(580, height - 600, to_string(scores[8]), colors[WHITE]);

}

// To Implement all the Drawing on the Board

void GameDisplay(){
	srand(time(0));
	if (currentPlayer == playernum + 1) {
		currentPlayer = 1;
	}
	glClearColor(0.0f, 0.0f, 0.5451f, 1.0f);  // Dark Navy Blue

	// Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Your game rendering code goes here...
	if (gameState == "menu") {
		//backgroundimage();
		DrawBackground();
		escape();
		Menu();
	}
	else if (gameState == "selectpl") {
		DrawBackground();
		selectpl();
	}
	else if (gameState == "pl1color") {
		DrawBackground();
		color(1);
	}
	else if (gameState == "pl2color") {
		DrawBackground();
		color(2);
	}
	else if (gameState == "pl3color") {
		DrawBackground();
		color(3);
	}
	else if (gameState == "pl4color") {
		DrawBackground();
		color(4);
	}
	else if (gameState == "pl5color") {
		DrawBackground();
		color(5);
	}
	else if (gameState == "pl6color") {
		DrawBackground();
		color(6);
	}
	else if (gameState == "start") {
		DrawBackground();
		Board();
		initPlayer();
		initDice();
		funbox();
		troops();
		if (flag) {
			start();
			flag = 0;
		}
	}
	else if (gameState == "Highscores") {
		DrawBackground();
		escape();
		highscore();
	}
	else if (gameState == "Instructions") {
		DrawBackground();
		escape();
		instructions();
	}
	else if (gameState == "dboard") {
		//showimage();
		//DrawBackground();
		DrawImage("Imgs/BG1.jpg");
		dboard();
	}
	else if (gameState == "credits") {
		DrawBackground();
		escape();
		credit();
	}
	else if (gameState == "pl1name") {
		DrawBackground();
		playernames(1);
	}
	else if (gameState == "pl2name") {
		DrawBackground();
		playernames(2);
	}
	else if (gameState == "pl3name") {
		DrawBackground();
		playernames(3);
	}
	else if (gameState == "pl4name") {
		DrawBackground();
		playernames(4);
	}
	else if (gameState == "pl5name") {
		DrawBackground();
		playernames(5);
	}
	else if (gameState == "pl6name") {
		DrawBackground();
		playernames(6);
	}
	else if (gameState == "phase2") {
		if (inc) {
			increment();
			inc = 0;
		}
		DrawBackground();
		Board();
		initPlayer();
		initDice();
		funbox();
		//troops();
		phasor();
	}
	else if (gameState == "attack") {
		DrawBackground();
		Board();
		initPlayer();
		initDice();
		funbox();
		if (!card) {
			troops();
		}
		attack();
		if (card) {
			dispCard();
		}
	}
	else if (gameState == "fortify") {
		DrawBackground();
		Board();
		initPlayer();
		initDice();
		funbox();
		troops();
		fortify();
	}
	else if (gameState == "end") {
		DrawBackground();
		escape();
		save();
		end();
	}

	glutSwapBuffers(); // do not modify this line..
}

// This function is called whenever a non printable key is pressed

void NonPrintableKeys(int key, int x, int y) {
	//if (key
	//		== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
	//	// what to do when left key is pressed...

	//} else if (key
	//		== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	//} else if (key
	//		== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {

	//}

	//else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {

	//}



	glutPostRedisplay();

}

// This function is called whenever a printable key is pressed

void PrintableKeys(unsigned char key, int x, int y) {
	if (key == 27/* Escape key ASCII*/) {
		if(gameState == "menu")
			exit(1);
		else if (gameState == "Highscores") {
			gameState = "menu";
		}
		else if (gameState == "selectpl") {
			gameState = "menu";
		}
		else if (gameState == "credits") {
			gameState = "menu";
		}
		else if (gameState == "pl1color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl2color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl3color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl4color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl5color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl6color") {
			gameState = "selectpl";
		}
		else if (gameState == "pl1name") {
			gameState = "selectpl";
		}
		else if (gameState == "pl2name") {
			gameState = "selectpl";
		}
		else if (gameState == "pl3name") {
			gameState = "selectpl";
		}
		else if (gameState == "pl4name") {
			gameState = "selectpl";
		}
		else if (gameState == "pl5name") {
			gameState = "selectpl";
		}
		else if (gameState == "pl6name") {
			gameState = "selectpl";
		}
		else if (gameState == "Instructions") {
			gameState = "menu";
		}
		else if (gameState == "start") {
			gameState = "menu";
		}
		else if (gameState == "dboard") {
			gameState = "selectpl";
		}
		else if (gameState == "phase2") {
			gameState = "menu";
		}
		else if (gameState == "attack" || gameState == "fortify") {
			gameState = "phase2";
		}
		else if (gameState == "end") {
			gameState = "menu";
		}
	}
	if (gameState == "pl1name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[0].length() < 10)) {
			//cnames[0].clear();
			cnames[0] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[0].empty()) {
				cnames[0].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl1color";
		}
			
	}
	else if (gameState == "pl2name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[1].length() < 10)) {
			//cnames[1].clear();
			cnames[1] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[1].empty()) {
				cnames[1].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl2color";
		}
	}
	else if (gameState == "pl3name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[2].length() < 10)) {
			//cnames[2].clear();
			cnames[2] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[2].empty()) {
				cnames[2].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl3color";
		}
	}
	else if (gameState == "pl4name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[3].length() < 10)) {
			//cnames[3].clear();
			cnames[3] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[3].empty()) {
				cnames[3].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl4color";
		}
	}
	else if (gameState == "pl5name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[4].length() < 10)) {
			//cnames[4].clear();
			cnames[4] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[4].empty()) {
				cnames[4].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl5color";
		}
	}
	else if (gameState == "pl6name") {
		if (((key > 47 && key < 58) || (key > 64 && key < 91) || (key > 96 && key < 123) || key == 95 || key == 45) && (cnames[5].length() < 10)) {
			//cnames[5].clear();
			cnames[5] += key;
		}
		else if (key == 8) { // For Backspace 
			if (!cnames[5].empty()) {
				cnames[5].pop_back();
			}
		}
		else if (key == 13) {
			gameState = "pl6color";
		}
	}


	glutPostRedisplay();
}

// This function is called after a certain interval of time

void Timer(int m) {
	glutPostRedisplay();
	glutTimerFunc(1000.0 / FPS, Timer, 0);
}

// This function is called when mouse is pressed and moved and prints those coordinates

void MousePressedAndMoved(int x, int y) {
	cout << "X: " << x << endl;
	cout<<"Y: " << height - y << endl;

	glutPostRedisplay();
}

// This function is called when mouse is moved

void MouseMoved(int x, int y) {
	y = height - y;
	//if ((x > 23 && x < 98 && y > 719 && y < 758) || (x > 100 && x < 179 && y > 719 && y < 758) || (x > 341 && x < 417 && y > 719 && y < 758) || (x > 542 && x < 617 && y > 729 && y < 768) || (x > 620 && x < 698 && y > 729 && y < 768) || (x > 61 && x < 138 && y > 658 && y < 697) || (x > 142 && x < 219 && y > 658 && y < 697) || (x > 222 && x < 299 && y > 658 && y < 697) || (x > 301 && x < 378 && y > 658 && y < 697) || (x > 381 && x < 458 && y > 658 && y < 697) || (x > 661 && x < 738 && y > 668 && y < 707) || (x > 101 && x < 178 && y > 597 && y < 636) || (x > 181 && x < 258 && y > 597 && y < 636) || (x > 261 && x < 338 && y > 597 && y < 636) || (x > 541 && x < 618 && y > 607 && y < 646) || (x > 621 && x < 698 && y > 607 && y < 646) || (x > 701 && x < 778 && y > 607 && y < 646) || (x > 61 && x < 138 && y > 536 && y < 575) || (x > 302 && x < 379 && y > 536 && y < 575) || (x > 422 && x < 499 && y > 546 && y < 585) || (x > 502 && x < 579 && y > 546 && y < 585) || (x > 582 && x < 659 && y > 546 && y < 585) || (x > 662 && x < 739 && y > 546 && y < 585) || (x > 141 && x < 218 && y > 465 && y < 504) || (x > 380 && x < 457 && y > 465 && y < 504) || (x > 460 && x < 537 && y > 465 && y < 504) || (x > 622 && x < 699 && y > 483 && y < 522) || (x > 100 && x < 177 && y > 405 && y < 444) || (x > 180 && x < 257 && y > 405 && y < 444) || (x > 260 && x < 337 && y > 405 && y < 444) || (x > 340 && x < 417 && y > 405 && y < 444) || (x > 541 && x < 618 && y > 387 && y < 426) || (x > 621 && x < 698 && y > 387 && y < 426) || (x > 701 && x < 778 && y > 387 && y < 426) || (x > 100 && x < 177 && y > 308 && y < 347) || (x > 302 && x < 379 && y > 343 && y < 382) || (x > 382 && x < 459 && y > 343 && y < 382) || (x > 662 && x < 739 && y > 308 && y < 347) || (x > 142 && x < 219 && y > 247 && y < 286) || (x > 222 && x < 299 && y > 247 && y < 286) || (x > 462 && x < 539 && y > 248 && y < 287) || (x > 542 && x < 619 && y > 248 && y < 287) || (x > 622 && x < 699 && y > 248 && y < 287) || (x > 702 && x < 779 && y > 248 && y < 287) || (x > 102 && x < 179 && y > 186 && y < 225) || (x > 182 && x < 259 && y > 186 && y < 225) || (x > 262 && x < 339 && y > 186 && y < 225) || (x > 422 && x < 499 && y > 186 && y < 225) || (x > 502 && x < 579 && y > 186 && y < 225) || (x > 582 && x < 659 && y > 186 && y < 225) || (x > 662 && x < 739 && y > 186 && y < 225)) {
		if (x > 23 && x < 98 && y > 719 && y < 758 && !(terrcolor[0][0] == colors[120]))		//Territory 1
		{
			TroopShow = 1;
			show[0] = 1;
		}
		else if (x > 100 && x < 179 && y > 719 && y < 758 && !(terrcolor[1][0] == colors[120]))		//Territory 2
		{
			TroopShow = 1;
			show[1] = 1;
		}
		else if (x > 341 && x < 417 && y > 719 && y < 758 && !(terrcolor[7][0] == colors[120]))		//Territory 3
		{
			TroopShow = 1;
			show[7] = 1;
		}
		else if (x > 542 && x < 617 && y > 729 && y < 768 && !(terrcolor[13][0] == colors[120]))		//Territory 4
		{
			TroopShow = 1;
			show[13] = 1;
		}
		else if (x > 620 && x < 698 && y > 729 && y < 768 && !(terrcolor[14][0] == colors[120]))		//Territory 5
		{
			TroopShow = 1;
			show[14] = 1;
		}
		else if (x > 61 && x < 138 && y > 658 && y < 697 && !(terrcolor[2][0] == colors[120]))		//Territory 6
		{
			TroopShow = 1;
			show[2] = 1;
		}
		else if (x > 142 && x < 219 && y > 658 && y < 697 && !(terrcolor[3][0] == colors[120]))		//Territory 7
		{
			TroopShow = 1;
			show[3] = 1;
		}
		else if (x > 222 && x < 299 && y > 658 && y < 697 && !(terrcolor[4][0] == colors[120]))		//Territory 8
		{
			TroopShow = 1;
			show[4] = 1;
		}
		else if (x > 301 && x < 378 && y > 658 && y < 697 && !(terrcolor[5][0] == colors[120]))		//Territory 9
		{
			TroopShow = 1;
			show[5] = 1;
		}
		else if (x > 381 && x < 458 && y > 658 && y < 697 && !(terrcolor[6][0] == colors[120]))		//Territory 10
		{
			TroopShow = 1;
			show[6] = 1;
		}
		else if (x > 661 && x < 738 && y > 668 && y < 707 && !(terrcolor[15][0] == colors[120]))		//Territory 11
		{
			TroopShow = 1;
			show[15] = 1;
		}
		else if (x > 101 && x < 178 && y > 597 && y < 636 && !(terrcolor[8][0] == colors[120]))		//Territory 12
		{
			TroopShow = 1;
			show[8] = 1;
		}
		else if (x > 181 && x < 258 && y > 597 && y < 636 && !(terrcolor[9][0] == colors[120]))		//Territory 13
		{
			TroopShow = 1;
			show[9] = 1;
		}
		else if (x > 261 && x < 338 && y > 597 && y < 636 && !(terrcolor[10][0] == colors[120]))		//Territory 14
		{
			TroopShow = 1;
			show[10] = 1;
		}
		else if (x > 541 && x < 618 && y > 607 && y < 646 && !(terrcolor[16][0] == colors[120]))		//Territory 15
		{
			TroopShow = 1;
			show[16] = 1;
		}
		else if (x > 621 && x < 698 && y > 607 && y < 646 && !(terrcolor[17][0] == colors[120]))		//Territory 16
		{
			TroopShow = 1;
			show[17] = 1;
		}
		else if (x > 701 && x < 778 && y > 607 && y < 646 && !(terrcolor[18][0] == colors[120]))		//Territory 17
		{
			TroopShow = 1;
			show[18] = 1;
		}
		else if (x > 61 && x < 138 && y > 536 && y < 575 && !(terrcolor[11][0] == colors[120]))		//Territory 18
		{
			TroopShow = 1;
			show[11] = 1;
		}
		else if (x > 302 && x < 379 && y > 536 && y < 575 && !(terrcolor[12][0] == colors[120]))		//Territory 19
		{
			TroopShow = 1;
			show[12] = 1;
		}
		else if (x > 422 && x < 499 && y > 546 && y < 585 && !(terrcolor[19][0] == colors[120]))		//Territory 20
		{
			TroopShow = 1;
			show[19] = 1;
		}
		else if (x > 502 && x < 579 && y > 546 && y < 585 && !(terrcolor[20][0] == colors[120]))		//Territory 21
		{
			TroopShow = 1;
			show[20] = 1;
		}
		else if (x > 582 && x < 659 && y > 546 && y < 585 && !(terrcolor[21][0] == colors[120]))		//Territory 22
		{
			TroopShow = 1;
			show[21] = 1;
		}
		else if (x > 662 && x < 739 && y > 546 && y < 585 && !(terrcolor[22][0] == colors[120]))		//Territory 23
		{
			TroopShow = 1;
			show[22] = 1;
		}
		else if (x > 141 && x < 218 && y > 465 && y < 504 && !(terrcolor[30][0] == colors[120]))		//Territory 24
		{
			TroopShow = 1;
			show[30] = 1;
		}
		else if (x > 380 && x < 457 && y > 465 && y < 504 && !(terrcolor[24][0] == colors[120]))		//Territory 25
		{
			TroopShow = 1;
			show[24] = 1;
		}
		else if (x > 460 && x < 537 && y > 465 && y < 504 && !(terrcolor[25][0] == colors[120]))		//Territory 26
		{
			TroopShow = 1;
			show[25] = 1;
		}
		else if (x > 622 && x < 699 && y > 483 && y < 522 && !(terrcolor[23][0] == colors[120]))		//Territory 27
		{
			TroopShow = 1;
			show[23] = 1;
		}
		else if (x > 100 && x < 177 && y > 405 && y < 444 && !(terrcolor[29][0] == colors[120]))		//Territory 28
		{
			TroopShow = 1;
			show[29] = 1;
		}
		else if (x > 180 && x < 257 && y > 405 && y < 444 && !(terrcolor[28][0] == colors[120]))		//Territory 29
		{
			TroopShow = 1;
			show[28] = 1;
		}
		else if (x > 260 && x < 337 && y > 405 && y < 444 && !(terrcolor[27][0] == colors[120]))		//Territory 30
		{
			TroopShow = 1;
			show[27] = 1;
		}
		else if (x > 340 && x < 417 && y > 405 && y < 444 && !(terrcolor[26][0] == colors[120]))		//Territory 31
		{
			TroopShow = 1;
			show[26] = 1;
		}
		else if (x > 541 && x < 618 && y > 387 && y < 426 && !(terrcolor[33][0] == colors[120]))		//Territory 32
		{
			TroopShow = 1;
			show[33] = 1;
		}
		else if (x > 621 && x < 698 && y > 387 && y < 426 && !(terrcolor[34][0] == colors[120]))		//Territory 33
		{
			TroopShow = 1;
			show[34] = 1;
		}
		else if (x > 701 && x < 778 && y > 387 && y < 426 && !(terrcolor[35][0] == colors[120]))		//Territory 34
		{
			TroopShow = 1;
			show[35] = 1;
		}
		else if (x > 100 && x < 177 && y > 308 && y < 347 && !(terrcolor[40][0] == colors[120]))		//Territory 35
		{
			TroopShow = 1;
			show[40] = 1;
		}
		else if (x > 302 && x < 379 && y > 343 && y < 382 && !(terrcolor[31][0] == colors[120]))		//Territory 36
		{
			TroopShow = 1;
			show[31] = 1;
		}
		else if (x > 382 && x < 459 && y > 343 && y < 382 && !(terrcolor[32][0] == colors[120]))		//Territory 37
		{
			TroopShow = 1;
			show[32] = 1;
		}
		else if (x > 662 && x < 739 && y > 308 && y < 347 && !(terrcolor[45][0] == colors[120]))		//Territory 38
		{
			TroopShow = 1;
			show[45] = 1;
		}
		else if (x > 142 && x < 219 && y > 247 && y < 286 && !(terrcolor[38][0] == colors[120]))		//Territory 39
		{
			TroopShow = 1;
			show[38] = 1;
		}
		else if (x > 222 && x < 299 && y > 247 && y < 286 && !(terrcolor[39][0] == colors[120]))		//Territory 40
		{
			TroopShow = 1;
			show[39] = 1;
		}
		else if (x > 462 && x < 539 && y > 248 && y < 287 && !(terrcolor[41][0] == colors[120]))		//Territory 41
		{
			TroopShow = 1;
			show[41] = 1;
		}
		else if (x > 542 && x < 619 && y > 248 && y < 287 && !(terrcolor[42][0] == colors[120]))		//Territory 42
		{
			TroopShow = 1;
			show[42] = 1;
		}
		else if (x > 622 && x < 699 && y > 248 && y < 287 && !(terrcolor[43][0] == colors[120]))		//Territory 43
		{
			TroopShow = 1;
			show[43] = 1;
		}
		else if (x > 702 && x < 779 && y > 248 && y < 287 && !(terrcolor[44][0] == colors[120]))		//Territory 44
		{
			TroopShow = 1;
			show[44] = 1;
		}
		else if (x > 102 && x < 179 && y > 186 && y < 225 && !(terrcolor[36][0] == colors[120]))		//Territory 45
		{
			TroopShow = 1;
			show[36] = 1;
		}
		else if (x > 182 && x < 259 && y > 186 && y < 225 && !(terrcolor[37][0] == colors[120]))		//Territory 46
		{
			TroopShow = 1;
			show[37] = 1;
		}
		else if (x > 262 && x < 339 && y > 186 && y < 225 && !(terrcolor[50][0] == colors[120]))		//Territory 47
		{
			TroopShow = 1;
			show[50] = 1;
		}
		else if (x > 422 && x < 499 && y > 186 && y < 225 && !(terrcolor[49][0] == colors[120]))		//Territory 48
		{
			TroopShow = 1;
			show[49] = 1;
		}
		else if (x > 502 && x < 579 && y > 186 && y < 225 && !(terrcolor[48][0] == colors[120]))		//Territory 49
		{
			TroopShow = 1;
			show[48] = 1;
		}
		else if (x > 582 && x < 659 && y > 186 && y < 225 && !(terrcolor[47][0] == colors[120]))		//Territory 50
		{
			TroopShow = 1;
			show[47] = 1;
		}
		else if (x > 662 && x < 739 && y > 186 && y < 225 && !(terrcolor[46][0] == colors[120]))		//Territory 51 
		{
			TroopShow = 1;
			show[46] = 1;
		}
	//}
	else {
		TroopShow = 0;
		for (int i = 0; i < 51; i++) {
			show[i] = 0;
		}
	}
	glutPostRedisplay();
}

// This function is called whenever mouse is clicked and it works in different game states for different coordinates

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		// Check if the click coordinates are within a specific area
		if (gameState == "menu") {
			// Example: Check if the click is within the "PLAY" button
			if (x > 395 && x < 500 && y > 354 && y < 381) {
				for (int i = 0; i < 51; i++) {
					for (int j = 0; j < 3; j++) {
						terrcolor[i][j] = colors[120];
						terr[i][j] = 0;
					}
					territory[i] = 0;
				}
				terrcount = 0;
				chosen = 0;
				newscore = 1;
				newname = "";
				factor = 0;
				for (int i = 0; i < 6; i++) {
					cscore[i] = 0;
					cnames[i] = "";
					terrNum[i] = 0;
					for (int j = 0; j < 3; j++) {
						troopNum[i][j] = 0;
					}
				}
				gameState = "selectpl";  // Transition to the next state
			}
			else if (x > 395 && x < 530 && y > 399 && y < 426) {
				gameState  = "Highscores";
			}
			else if (x>395 && x < 520 && y > 443 && y < 470) {
				gameState = "Instructions";
			}
			else if (x > 395 && x < 480 && y > 490 && y < 513) {
				gameState = "credits";
			}
		}
		else if (gameState == "selectpl") {
			if (x > 279 && x < 477 && y > 207 && y < 307) {
				gameState = "pl1name";  // Transition to the next state
				playernum = 2;
			}
			else if (x > 533 && x < 728 && y > 207 && y < 307) {
				gameState = "pl1name";
				playernum = 3;
			}
			else if (x > 279 && x < 477 && y > 337 && y < 436) {
				gameState = "pl1name";
				playernum = 4;
			}
			else if (x > 533 && x < 728 && y > 337 && y < 436) {
				gameState = "pl1name";
				playernum = 5;
			}
			else if (x > 410 && x < 612 && y > 469 && y < 571) {
				gameState = "pl1name";
				playernum = 6;
			}
			//cout << playernum << endl;
		}
		else if (gameState == "pl1color") {
			//y = height - y;
			if (x > 421 && x < 589 && y > 306 && y < 335) {
				//gameState = "start";  // Transition to the next state
				playercolor[0] = colors[BLUE];
				gameState = "pl2name";
			}
			else if (x > 421 && x < 589 && y > 350 && y < 369) {
				//gameState = "start";
				playercolor[0] = colors[22];
				gameState = "pl2name";
			}
			else if (x > 421 && x < 589 && y > 383 && y < 406) {
				//gameState = "start";
				playercolor[0] = colors[RED];
				gameState = "pl2name";
			}
			else if (x > 421 && x < 589 && y > 420 && y < 441) {
				//gameState = "start";
				playercolor[0] = colors[30];
				gameState = "pl2name";
			}
			else if (x > 421 && x < 589 && y > 453 && y < 477) {
				//gameState = "start";
				playercolor[0] = colors[90];
				gameState = "pl2name";
			}
			else if (x > 421 && x < 589 && y > 489 && y < 513) {
				//gameState = "start";
				playercolor[0] = colors[70];
				gameState = "pl2name";
			}
		}
		else if (gameState == "pl2color") {
			//y = height - y;
			if (x > 421 && x < 589 && y > 306 && y < 335) {
				//gameState = "start";  // Transition to the next state
				if (!(playercolor[0] == colors[BLUE])) {
					playercolor[1] = colors[BLUE];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
			else if (x > 421 && x < 589 && y > 350 && y < 369) {
				//gameState = "start";
				if (!(playercolor[0] == colors[22])) {
					playercolor[1] = colors[22];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
			else if (x > 421 && x < 589 && y > 383 && y < 406) {
				//gameState = "start";
				if (!(playercolor[0] == colors[RED])) {
					playercolor[1] = colors[RED];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
			else if (x > 421 && x < 589 && y > 420 && y < 441) {
				//gameState = "start";
				if (!(playercolor[0] == colors[30])) {
					playercolor[1] = colors[30];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
			else if (x > 421 && x < 589 && y > 453 && y < 477) {
				//gameState = "start";
				if (!(playercolor[0] == colors[90])) {
					playercolor[1] = colors[90];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
			else if (x > 421 && x < 589 && y > 489 && y < 513) {
				//gameState = "start";
				if (!(playercolor[0] == colors[70])) {
					playercolor[1] = colors[70];
					if (playernum == 2) {
						gameState = "dboard";
					}
					else
						gameState = "pl3name";
				}
			}
		}
		else if (gameState == "pl3color") {
			if (x > 421 && x < 589 && y > 306 && y < 335) {
				//gameState = "start";  // Transition to the next state
				if (!(playercolor[0] == colors[BLUE] || playercolor[1] == colors[BLUE])) {
					playercolor[2] = colors[BLUE];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			else if (x > 421 && x < 589 && y > 350 && y < 369) {
				//gameState = "start";
				if (!(playercolor[0] == colors[22] || playercolor[1] == colors[22])) {
					playercolor[2] = colors[22];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			else if (x > 421 && x < 589 && y > 383 && y < 406) {
				//gameState = "start";
				if (!(playercolor[0] == colors[RED] || playercolor[1] == colors[RED])) {
					playercolor[2] = colors[RED];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			else if (x > 421 && x < 589 && y > 420 && y < 441) {
				//gameState = "start";
				if (!(playercolor[0] == colors[30] || playercolor[1] == colors[30])) {
					playercolor[2] = colors[30];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			else if (x > 421 && x < 589 && y > 453 && y < 477) {
				//gameState = "start";
				if (!(playercolor[0] == colors[90] || playercolor[1] == colors[90])) {
					playercolor[2] = colors[90];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			else if (x > 421 && x < 589 && y > 489 && y < 513) {
				//gameState = "start";
				if (!(playercolor[0] == colors[70] || playercolor[1] == colors[70])) {
					playercolor[2] = colors[70];
					if (playernum == 3) {
						gameState = "dboard";
					}
					else
						gameState = "pl4name";
				}
			}
			}
		else if (gameState == "pl4color") {
				if (x > 421 && x < 589 && y > 306 && y < 335) {
					//gameState = "start";  // Transition to the next state
					if (!(playercolor[0] == colors[BLUE] || playercolor[1] == colors[BLUE] || playercolor[2] == colors[BLUE])) {
						playercolor[3] = colors[BLUE];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				else if (x > 421 && x < 589 && y > 350 && y < 369) {
					//gameState = "start";
					if (!(playercolor[0] == colors[22] || playercolor[1] == colors[22] || playercolor[2] == colors[22])) {
						playercolor[3] = colors[22];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				else if (x > 421 && x < 589 && y > 383 && y < 406) {
					//gameState = "start";
					if (!(playercolor[0] == colors[RED] || playercolor[1] == colors[RED] || playercolor[2] == colors[RED])) {
						playercolor[3] = colors[RED];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				else if (x > 421 && x < 589 && y > 420 && y < 441) {
					//gameState = "start";
					if (!(playercolor[0] == colors[30] || playercolor[1] == colors[30] || playercolor[2] == colors[30])) {
						playercolor[3] = colors[30];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				else if (x > 421 && x < 589 && y > 453 && y < 477) {
					//gameState = "start";
					if (!(playercolor[0] == colors[90] || playercolor[1] == colors[90] || playercolor[2] == colors[90])) {
						playercolor[3] = colors[90];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				else if (x > 421 && x < 589 && y > 489 && y < 513) {
					//gameState = "start";
					if (!(playercolor[0] == colors[70] || playercolor[1] == colors[70] || playercolor[2] == colors[70])) {
						playercolor[3] = colors[70];
						if (playernum == 4) {
							gameState = "dboard";
						}
						else
							gameState = "pl5name";
					}
				}
				}
		else if (gameState == "pl5color") {
			if (x > 421 && x < 589 && y > 306 && y < 335) {
				//gameState = "start";  // Transition to the next state
				if (!(playercolor[0] == colors[BLUE] || playercolor[1] == colors[BLUE] || playercolor[2] == colors[BLUE] || playercolor[3] == colors[BLUE])) {
					playercolor[4] = colors[BLUE];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			else if (x > 421 && x < 589 && y > 350 && y < 369) {
				//gameState = "start";
				if (!(playercolor[0] == colors[22] || playercolor[1] == colors[22] || playercolor[2] == colors[22] || playercolor[3] == colors[22])) {
					playercolor[4] = colors[22];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			else if (x > 421 && x < 589 && y > 383 && y < 406) {
				//gameState = "start";
				if (!(playercolor[0] == colors[RED] || playercolor[1] == colors[RED] || playercolor[2] == colors[RED] || playercolor[3] == colors[RED])) {
					playercolor[4] = colors[RED];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			else if (x > 421 && x < 589 && y > 420 && y < 441) {
				//gameState = "start";
				if (!(playercolor[0] == colors[30] || playercolor[1] == colors[30] || playercolor[2] == colors[30] || playercolor[3] == colors[30])) {
					playercolor[4] = colors[30];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			else if (x > 421 && x < 589 && y > 453 && y < 477) {
				//gameState = "start";
				if (!(playercolor[0] == colors[90] || playercolor[1] == colors[90] || playercolor[2] == colors[90] || playercolor[3] == colors[90])) {
					playercolor[4] = colors[90];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			else if (x > 421 && x < 589 && y > 489 && y < 513) {
				//gameState = "start";
				if (!(playercolor[0] == colors[70] || playercolor[1] == colors[70] || playercolor[2] == colors[70] || playercolor[3] == colors[70])) {
					playercolor[4] = colors[70];
					if (playernum == 5) {
						gameState = "dboard";
					}
					else
						gameState = "pl6name";
				}
			}
			}
		else if (gameState == "pl6color") {
			if (x > 421 && x < 589 && y > 306 && y < 335) {
				if (!(playercolor[0] == colors[BLUE] || playercolor[1] == colors[BLUE] || playercolor[2] == colors[BLUE] || playercolor[3] == colors[BLUE] || playercolor[4] == colors[BLUE])) {
					playercolor[5] = colors[BLUE];
					gameState = "dboard";  // Transition to the next state
				}
			}
			else if (x > 421 && x < 589 && y > 350 && y < 369) {
				if (!(playercolor[0] == colors[22] || playercolor[1] == colors[22] || playercolor[2] == colors[22] || playercolor[3] == colors[22] || playercolor[4] == colors[22])) {
					playercolor[5] = colors[22];
					gameState = "dboard";  // Transition to the next state
				}
			}
			else if (x > 421 && x < 589 && y > 383 && y < 406) {
				if (!(playercolor[0] == colors[RED] || playercolor[1] == colors[RED] || playercolor[2] == colors[RED] || playercolor[3] == colors[RED] || playercolor[4] == colors[RED])) {
					playercolor[5] = colors[RED];
					gameState = "dboard";  // Transition to the next state
				}
			}
			else if (x > 421 && x < 589 && y > 420 && y < 441) {
				if (!(playercolor[0] == colors[30] || playercolor[1] == colors[30] || playercolor[2] == colors[30] || playercolor[3] == colors[30] || playercolor[4] == colors[30])) {
					playercolor[5] = colors[30];
					gameState = "dboard";  // Transition to the next state;
				}
			}
			else if (x > 421 && x < 589 && y > 453 && y < 477) {
				if (!(playercolor[0] == colors[90] || playercolor[1] == colors[90] || playercolor[2] == colors[90] || playercolor[3] == colors[90] || playercolor[4] == colors[90])) {
					playercolor[5] = colors[90];
					gameState = "dboard";  // Transition to the next state
				}
			}
			else if (x > 421 && x < 589 && y > 489 && y < 513) {
				if (!(playercolor[0] == colors[70] || playercolor[1] == colors[70] || playercolor[2] == colors[70] || playercolor[3] == colors[70] || playercolor[4] == colors[70])) {
					playercolor[5] = colors[70];
					gameState = "dboard";  // Transition to the next state
				}
			}
		}
		else if (gameState == "dboard") {
			if (x > 429 && x < 580 && y > 453 && y < 522) {
				gameState = "start";
			}
			if (x > 429 && x < 580 && y > 356 && y < 422) {
				factor = 3;
				gameState = "start";
			}
			if (x > 429 && x < 580 && y > 257 && y < 322) {
				factor = 5;
				gameState = "start";
			}
		}
		else if (gameState == "start") {
			y = height - y;
			if (x > 23 && x < 98 && y > 719 && y < 758)		//Territory 1
			{	
				if (terrcolor[0][0] == colors[120]) {
					terr[0][0] += 1;
					terrcount += 1;
					territory[0] = currentPlayer;
					terrcolor[0][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;
					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}

			}
			else if (x > 100 && x < 179 && y > 719 && y < 758)		//Territory 2
			{
				if (terrcolor[1][0] == colors[120]) {
					terr[1][0] += 1;
					terrcount += 1;
					territory[1]=currentPlayer;
					terrcolor[1][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 341 && x < 417 && y > 719 && y < 758)		//Territory 3
			{
				//terrcolor[7][0] = playercolor[currentPlayer - 1];
				if (terrcolor[7][0] == colors[120]) {
					terr[7][0] += 1;
					terrcount += 1;
					territory[7]=currentPlayer;
					terrcolor[7][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 542 && x < 617 && y > 729 && y < 768)		//Territory 4
			{
				//	terrcolor[13][0] = playercolor[currentPlayer - 1];
				if (terrcolor[13][0] == colors[120]) {
					terr[13][0] += 1;
					terrcount += 1;
					territory[13]=currentPlayer;
					terrcolor[13][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 620 && x < 698 && y > 729 && y < 768)		//Territory 5
			{
				//	terrcolor[14][0] = playercolor[currentPlayer - 1];
				if (terrcolor[14][0] == colors[120]) {
					terr[14][0] += 1;
					terrcount += 1;
					territory[14]=currentPlayer;
					terrcolor[14][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 61 && x < 138 && y > 658 && y < 697)		//Territory 6
			{
				//	terrcolor[2][0] = playercolor[currentPlayer - 1];
				if (terrcolor[2][0] == colors[120]) {
					terr[2][0] += 1;
					terrcount += 1;
					territory[2]=currentPlayer;
					terrcolor[2][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 142 && x < 219 && y > 658 && y < 697)		//Territory 7
			{
				//	terrcolor[3][0] = playercolor[currentPlayer - 1];
				if (terrcolor[3][0] == colors[120]) {
					terr[3][0] += 1;
					terrcount += 1;
					territory[3]=currentPlayer;
					terrcolor[3][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 222 && x < 299 && y > 658 && y < 697)		//Territory 8
			{
				//	terrcolor[4][0] = playercolor[currentPlayer - 1];
				if (terrcolor[4][0] == colors[120]) {
					terr[4][0] += 1;
					terrcount += 1;
					territory[4]=currentPlayer;
					terrcolor[4][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 301 && x < 378 && y > 658 && y < 697)		//Territory 9
			{
				//	terrcolor[5][0] = playercolor[currentPlayer - 1];
				if (terrcolor[5][0] == colors[120]) {
					terr[5][0] += 1;
					terrcount += 1;
					territory[5]=currentPlayer;
					terrcolor[5][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 381 && x < 458 && y > 658 && y < 697)		//Territory 10
			{
				//	terrcolor[6][0] = playercolor[currentPlayer - 1];
				if (terrcolor[6][0] == colors[120]) {
					terr[6][0] += 1;
					terrcount += 1;
					territory[6]=currentPlayer;
					terrcolor[6][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 661 && x < 738 && y > 668 && y < 707)		//Territory 11
			{
				//	terrcolor[15][0] = playercolor[currentPlayer - 1];
				if (terrcolor[15][0] == colors[120]) {
					terr[15][0] += 1;
					terrcount += 1;
					territory[15]=currentPlayer;
					terrcolor[15][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 101 && x < 178 && y > 597 && y < 636)		//Territory 12
			{
				//	terrcolor[8][0] = playercolor[currentPlayer - 1];
				if (terrcolor[8][0] == colors[120]) {
					terr[8][0] += 1;
					terrcount += 1;
					territory[8]=currentPlayer;
					terrcolor[8][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 181 && x < 258 && y > 597 && y < 636)		//Territory 13
			{
				//	terrcolor[9][0] = playercolor[currentPlayer - 1];
				if (terrcolor[9][0] == colors[120]) {
					terr[9][0] += 1;
					terrcount += 1;
					territory[9]=currentPlayer;
					terrcolor[9][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 261 && x < 338 && y > 597 && y < 636)		//Territory 14
			{
				//	terrcolor[10][0] = playercolor[currentPlayer - 1];
				if (terrcolor[10][0] == colors[120]) {
					terr[10][0] += 1;
					terrcount += 1;
					territory[10]=currentPlayer;
					terrcolor[10][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 541 && x < 618 && y > 607 && y < 646)		//Territory 15
			{
				//	terrcolor[16][0] = playercolor[currentPlayer - 1];
				if (terrcolor[16][0] == colors[120]) {
					terr[16][0] += 1;
					terrcount += 1;
					territory[16]=currentPlayer;
					terrcolor[16][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 621 && x < 698 && y > 607 && y < 646)		//Territory 16
			{
				//	terrcolor[17][0] = playercolor[currentPlayer - 1];
				if (terrcolor[17][0] == colors[120]) {
					terr[17][0] += 1;
					terrcount += 1;
					territory[17]=currentPlayer;
					terrcolor[17][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 701 && x < 778 && y > 607 && y < 646)		//Territory 17
			{
				//	terrcolor[18][0] = playercolor[currentPlayer - 1];
				if (terrcolor[18][0] == colors[120]) {
					terr[18][0] += 1;
					terrcount += 1;
					territory[18]=currentPlayer;
					terrcolor[18][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 61 && x < 138 && y > 536 && y < 575)		//Territory 18
			{
				//	terrcolor[11][0] = playercolor[currentPlayer - 1];
				if (terrcolor[11][0] == colors[120]) {
					terr[11][0] += 1;
					terrcount += 1;
					territory[11]=currentPlayer;
					terrcolor[11][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 302 && x < 379 && y > 536 && y < 575)		//Territory 19
			{
				//	terrcolor[12][0] = playercolor[currentPlayer - 1];
				if (terrcolor[12][0] == colors[120]) {
					terr[12][0] += 1;
					terrcount += 1;
					territory[12]=currentPlayer;
					terrcolor[12][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 422 && x < 499 && y > 546 && y < 585)		//Territory 20
			{
				//	terrcolor[19][0] = playercolor[currentPlayer - 1];
				if (terrcolor[19][0] == colors[120]) {
					terr[19][0] += 1;
					terrcount += 1;
					territory[19]=currentPlayer;
					terrcolor[19][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 502 && x < 579 && y > 546 && y < 585)		//Territory 21
			{
				//	terrcolor[20][0] = playercolor[currentPlayer - 1];
				if (terrcolor[20][0] == colors[120]) {
					terr[20][0] += 1;
					terrcount += 1;
					territory[20]=currentPlayer;
					terrcolor[20][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 582 && x < 659 && y > 546 && y < 585)		//Territory 22
			{
				//	terrcolor[21][0] = playercolor[currentPlayer - 1];
				if (terrcolor[21][0] == colors[120]) {
					terr[21][0] += 1;
					terrcount += 1;
					territory[21]=currentPlayer;
					terrcolor[21][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 662 && x < 739 && y > 546 && y < 585)		//Territory 23
			{
				//	terrcolor[22][0] = playercolor[currentPlayer - 1];
				if (terrcolor[22][0] == colors[120]) {
					terr[22][0] += 1;
					terrcount += 1;
					territory[22]=currentPlayer;
					terrcolor[22][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 141 && x < 218 && y > 465 && y < 504)		//Territory 24
			{
				//	terrcolor[30][0] = playercolor[currentPlayer - 1];
				if (terrcolor[30][0] == colors[120]) {
					terr[30][0] += 1;
					terrcount += 1;
					territory[30]=currentPlayer;
					terrcolor[30][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 380 && x < 457 && y > 465 && y < 504)		//Territory 25
			{
				//	terrcolor[24][0] = playercolor[currentPlayer - 1];
				if (terrcolor[24][0] == colors[120]) {
					terr[24][0] += 1;
					terrcount += 1;
					territory[24]=currentPlayer;
					terrcolor[24][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 460 && x < 537 && y > 465 && y < 504)		//Territory 26
			{
				//	terrcolor[25][0] = playercolor[currentPlayer - 1];
				if (terrcolor[25][0] == colors[120]) {
					terr[25][0] += 1;
					terrcount += 1;
					territory[25]=currentPlayer;
					terrcolor[25][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 622 && x < 699 && y > 483 && y < 522)		//Territory 27
			{
				//	terrcolor[23][0] = playercolor[currentPlayer - 1];
				if (terrcolor[23][0] == colors[120]) {
					terr[23][0] += 1;
					terrcount += 1;
					territory[23]=currentPlayer;
					terrcolor[23][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 100 && x < 177 && y > 405 && y < 444)		//Territory 28
			{
				//	terrcolor[29][0] = playercolor[currentPlayer - 1];
				if (terrcolor[29][0] == colors[120]) {
					terr[29][0] += 1;
					terrcount += 1;
					territory[29]=currentPlayer;
					terrcolor[29][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 180 && x < 257 && y > 405 && y < 444)		//Territory 29
			{
				//	terrcolor[28][0] = playercolor[currentPlayer - 1];
				if (terrcolor[28][0] == colors[120]) {
					terr[28][0] += 1;
					terrcount += 1;
					territory[28]=currentPlayer;
					terrcolor[28][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 260 && x < 337 && y > 405 && y < 444)		//Territory 30
			{
				//	terrcolor[27][0] = playercolor[currentPlayer - 1];
				if (terrcolor[27][0] == colors[120]) {
					terr[27][0] += 1;
					terrcount += 1;
					territory[27]=currentPlayer;
					terrcolor[27][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 340 && x < 417 && y > 405 && y < 444)		//Territory 31
			{
				//	terrcolor[26][0] = playercolor[currentPlayer - 1];
				if (terrcolor[26][0] == colors[120]) {
					terr[26][0] += 1;
					terrcount += 1;
					territory[26]=currentPlayer;
					terrcolor[26][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 541 && x < 618 && y > 387 && y < 426)		//Territory 32
			{
				//	terrcolor[33][0] = playercolor[currentPlayer - 1];
				if (terrcolor[33][0] == colors[120]) {
					terr[33][0] += 1;
					terrcount += 1;
					territory[33]=currentPlayer;
					terrcolor[33][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 621 && x < 698 && y > 387 && y < 426)		//Territory 33
			{
				//	terrcolor[34][0] = playercolor[currentPlayer - 1];
				if (terrcolor[34][0] == colors[120]) {
					terr[34][0] += 1;
					terrcount += 1;
					territory[34]=currentPlayer;
					terrcolor[34][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 701 && x < 778 && y > 387 && y < 426)		//Territory 34
			{
				//	terrcolor[35][0] = playercolor[currentPlayer - 1];
				if (terrcolor[35][0] == colors[120]) {
					terr[35][0] += 1;
					terrcount += 1;
					territory[35]=currentPlayer;
					terrcolor[35][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 100 && x < 177 && y > 308 && y < 347)		//Territory 35
			{
				//	terrcolor[40][0] = playercolor[currentPlayer - 1];
				if (terrcolor[40][0] == colors[120]) {
					terr[40][0] += 1;
					terrcount += 1;
					territory[40]=currentPlayer;
					terrcolor[40][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 302 && x < 379 && y > 343 && y < 382)		//Territory 36
			{
				//	terrcolor[31][0] = playercolor[currentPlayer - 1];
				if (terrcolor[31][0] == colors[120]) {
					terr[31][0] += 1;
					terrcount += 1;
					territory[31]=currentPlayer;
					terrcolor[31][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 382 && x < 459 && y > 343 && y < 382)		//Territory 37
			{
				//	terrcolor[32][0] = playercolor[currentPlayer - 1];
				if (terrcolor[32][0] == colors[120]) {
					terr[32][0] += 1;
					terrcount += 1;
					territory[32]=currentPlayer;
					terrcolor[32][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 662 && x < 739 && y > 308 && y < 347)		//Territory 38
			{
				//	terrcolor[45][0] = playercolor[currentPlayer - 1];
				if (terrcolor[45][0] == colors[120]) {
					terr[45][0] += 1;
					terrcount += 1;
					territory[45]=currentPlayer;
					terrcolor[45][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 142 && x < 219 && y > 247 && y < 286)		//Territory 39
			{
				//	terrcolor[38][0] = playercolor[currentPlayer - 1];
				if (terrcolor[38][0] == colors[120]) {
					terr[38][0] += 1;
					terrcount += 1;
					territory[38]=currentPlayer;
					terrcolor[38][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 222 && x < 299 && y > 247 && y < 286)		//Territory 40
			{
				//	terrcolor[39][0] = playercolor[currentPlayer - 1];
				if (terrcolor[39][0] == colors[120]) {
					terr[39][0] += 1;
					terrcount += 1;
					territory[39]=currentPlayer;
					terrcolor[39][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 462 && x < 539 && y > 248 && y < 287)		//Territory 41
			{
				//	terrcolor[41][0] = playercolor[currentPlayer - 1];
				if (terrcolor[41][0] == colors[120]) {
					terr[41][0] += 1;
					terrcount += 1;
					territory[41]=currentPlayer;
					terrcolor[41][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 542 && x < 619 && y > 248 && y < 287)		//Territory 42
			{
				//	terrcolor[42][0] = playercolor[currentPlayer - 1];
				if (terrcolor[42][0] == colors[120]) {
					terr[42][0] += 1;
					terrcount += 1;
					territory[42]=currentPlayer;
					terrcolor[42][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 622 && x < 699 && y > 248 && y < 287)		//Territory 43
			{
				//	terrcolor[43][0] = playercolor[currentPlayer - 1];
				if (terrcolor[43][0] == colors[120]) {
					terr[43][0] += 1;
					terrcount += 1;
					territory[43]=currentPlayer;
					terrcolor[43][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 702 && x < 779 && y > 248 && y < 287)		//Territory 44
			{
				//	terrcolor[44][0] = playercolor[currentPlayer - 1];
				if (terrcolor[44][0] == colors[120]) {
					terr[44][0] += 1;
					terrcount += 1;
					territory[44]=currentPlayer;
					terrcolor[44][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 102 && x < 179 && y > 186 && y < 225)		//Territory 45
			{
				//	terrcolor[36][0] = playercolor[currentPlayer - 1];
				if (terrcolor[36][0] == colors[120]) {
					terr[36][0] += 1;
					terrcount += 1;
					territory[36]=currentPlayer;
					terrcolor[36][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 182 && x < 259 && y > 186 && y < 225)		//Territory 46
			{
				//	terrcolor[37][0] = playercolor[currentPlayer - 1];
				if (terrcolor[37][0] == colors[120]) {
					terr[37][0] += 1;
					terrcount += 1;
					territory[37]=currentPlayer;
					terrcolor[37][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 262 && x < 339 && y > 186 && y < 225)		//Territory 47
			{
				//	terrcolor[50][0] = playercolor[currentPlayer - 1];
				if (terrcolor[50][0] == colors[120]) {
					terr[50][0] += 1;
					terrcount += 1;
					territory[50]=currentPlayer;
					terrcolor[50][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 422 && x < 499 && y > 186 && y < 225)		//Territory 48
			{
				//	terrcolor[49][0] = playercolor[currentPlayer - 1];
				if (terrcolor[49][0] == colors[120]) {
					terr[49][0] += 1;
					terrcount += 1;
					territory[49]=currentPlayer;
					terrcolor[49][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 502 && x < 579 && y > 186 && y < 225)		//Territory 49
			{
				//	terrcolor[48][0] = playercolor[currentPlayer - 1];
				if (terrcolor[48][0] == colors[120]) {
					terr[48][0] += 1;
					terrcount += 1;
					territory[48]=currentPlayer;
					terrcolor[48][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 582 && x < 659 && y > 186 && y < 225)		//Territory 50
			{
				//	terrcolor[47][0] = playercolor[currentPlayer - 1];
				if (terrcolor[47][0] == colors[120]) {
					terr[47][0] += 1;
					terrcount += 1;
					territory[0]=currentPlayer;
					terrcolor[47][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;

					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
			else if (x > 662 && x < 739 && y > 186 && y < 225)		//Territory 51 
			{
				//	terrcolor[46][0] = playercolor[currentPlayer - 1];
				if (terrcolor[46][0] == colors[120]) {
					terr[46][0] += 1;
					terrcount += 1;
					territory[46]=currentPlayer;
					terrcolor[46][0] = playercolor[currentPlayer - 1];
					terrNum[currentPlayer - 1] += 1;
					troopNum[currentPlayer - 1][0] += 1;
					cscore[currentPlayer - 1] += 2;
					currentPlayer += 1;
					 
					if (currentPlayer == playernum + 1) {
						currentPlayer = 1;
					}
				}
			}
		}
		else if (gameState == "phase2") {
			y = height - y;
			if (x > 300 && x < 525 && y > 465 && y < 485) {
				gameState = "fortify";
			}
			else if (x > 300 && x < 525 && y > 420 && y < 440) {
				gameState = "attack";
			}
			else if (x > 300 && x < 525 && y > 380 && y < 400) {
				currentPlayer += 1;
				if (currentPlayer == playernum + 1) {
					currentPlayer = 1;
				}
			}
		}
		else if (gameState == "fortify") {
			y = height - y;
			if (x > 23 && x < 98 && y > 719 && y < 758 && !(terrcolor[0][0] == colors[120]))		//Territory 1
			{
				if (terrcolor[0][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[0][0] <= 1)) {
							terr[0][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[0][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else {
					fortification = 1;
				}
			}
			else if (x > 100 && x < 179 && y > 719 && y < 758 && !(terrcolor[1][0] == colors[120]))		//Territory 2
			{
				if (terrcolor[1][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[1][0] <= 1)) {
							terr[1][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[1][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 341 && x < 417 && y > 719 && y < 758 && !(terrcolor[7][0] == colors[120]))		//Territory 3
			{
				if (terrcolor[7][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[7][0] <= 1)) {
							terr[7][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[7][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 542 && x < 617 && y > 729 && y < 768 && !(terrcolor[13][0] == colors[120]))		//Territory 4
			{
				if (terrcolor[13][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[13][0] <= 1)) {
							terr[13][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[13][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 620 && x < 698 && y > 729 && y < 768 && !(terrcolor[14][0] == colors[120]))		//Territory 5
			{
				if (terrcolor[14][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[14][0] <= 1)) {
							terr[14][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[14][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 61 && x < 138 && y > 658 && y < 697 && !(terrcolor[2][0] == colors[120]))		//Territory 6
			{
				if (terrcolor[2][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[2][0] <= 1)) {
							terr[2][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[2][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 142 && x < 219 && y > 658 && y < 697 && !(terrcolor[3][0] == colors[120]))		//Territory 7
			{
				if (terrcolor[3][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[3][0] <= 1)) {
							terr[3][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[3][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 222 && x < 299 && y > 658 && y < 697 && !(terrcolor[4][0] == colors[120]))		//Territory 8
			{
				if (terrcolor[4][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[4][0] <= 1)) {
							terr[4][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[4][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 301 && x < 378 && y > 658 && y < 697 && !(terrcolor[5][0] == colors[120]))		//Territory 9
			{
				if (terrcolor[5][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[5][0] <= 1)) {
							terr[5][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[5][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 381 && x < 458 && y > 658 && y < 697 && !(terrcolor[6][0] == colors[120]))		//Territory 10
			{
				if (terrcolor[6][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[6][0] <= 1)) {
							terr[6][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[6][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 661 && x < 738 && y > 668 && y < 707 && !(terrcolor[15][0] == colors[120]))		//Territory 11
			{
				if (terrcolor[15][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[15][0] <= 1)) {
							terr[15][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[15][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 101 && x < 178 && y > 597 && y < 636 && !(terrcolor[8][0] == colors[120]))		//Territory 12
			{
				if (terrcolor[8][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[8][0] <= 1)) {
							terr[8][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[8][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 181 && x < 258 && y > 597 && y < 636 && !(terrcolor[9][0] == colors[120]))		//Territory 13
			{
				if (terrcolor[9][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[9][0] <= 1)) {
							terr[9][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[9][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 261 && x < 338 && y > 597 && y < 636 && !(terrcolor[10][0] == colors[120]))		//Territory 14
			{
				if (terrcolor[10][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[10][0] <= 1)) {
							terr[10][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[10][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 541 && x < 618 && y > 607 && y < 646 && !(terrcolor[16][0] == colors[120]))		//Territory 15
			{
				if (terrcolor[16][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[16][0] <= 1)) {
							terr[16][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[16][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 621 && x < 698 && y > 607 && y < 646 && !(terrcolor[17][0] == colors[120]))		//Territory 16
			{
				if (terrcolor[17][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[18][0] <= 1)) {
							terr[18][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[17][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 701 && x < 778 && y > 607 && y < 646 && !(terrcolor[18][0] == colors[120]))		//Territory 17
			{
				if (terrcolor[18][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[18][0] <= 1)) {
							terr[18][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[18][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 61 && x < 138 && y > 536 && y < 575 && !(terrcolor[11][0] == colors[120]))		//Territory 18
			{
				if (terrcolor[11][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[11][0] <= 1)) {
							terr[11][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[11][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 302 && x < 379 && y > 536 && y < 575 && !(terrcolor[12][0] == colors[120]))		//Territory 19
			{
				if (terrcolor[12][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[12][0] <= 1)) {
							terr[12][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[12][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 422 && x < 499 && y > 546 && y < 585 && !(terrcolor[19][0] == colors[120]))		//Territory 20
			{
				if (terrcolor[19][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[19][0] <= 1)) {
							terr[19][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[19][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 502 && x < 579 && y > 546 && y < 585 && !(terrcolor[20][0] == colors[120]))		//Territory 21
			{
				if (terrcolor[20][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[20][0] <= 1)) {
							terr[20][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[20][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 582 && x < 659 && y > 546 && y < 585 && !(terrcolor[21][0] == colors[120]))		//Territory 22
			{
				if (terrcolor[21][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[21][0] <= 1)) {
							terr[21][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[21][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 662 && x < 739 && y > 546 && y < 585 && !(terrcolor[22][0] == colors[120]))		//Territory 23
			{
				if (terrcolor[22][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[22][0] <= 1)) {
							terr[22][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[22][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 141 && x < 218 && y > 465 && y < 504 && !(terrcolor[30][0] == colors[120]))		//Territory 24
			{
				if (terrcolor[30][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[30][0] <= 1)) {
							terr[30][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[30][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 380 && x < 457 && y > 465 && y < 504 && !(terrcolor[24][0] == colors[120]))		//Territory 25
			{
				if (terrcolor[24][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[24][0] <= 1)) {
							terr[24][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[24][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 460 && x < 537 && y > 465 && y < 504 && !(terrcolor[25][0] == colors[120]))		//Territory 26
			{
				if (terrcolor[25][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[25][0] <= 1)) {
							terr[25][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[25][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 622 && x < 699 && y > 483 && y < 522 && !(terrcolor[23][0] == colors[120]))		//Territory 27
			{
				if (terrcolor[23][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[23][0] <= 1)) {
							terr[23][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[23][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 100 && x < 177 && y > 405 && y < 444 && !(terrcolor[29][0] == colors[120]))		//Territory 28
			{
				if (terrcolor[29][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[29][0] <= 1)) {
							terr[29][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[29][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 180 && x < 257 && y > 405 && y < 444 && !(terrcolor[28][0] == colors[120]))		//Territory 29
			{
				if (terrcolor[28][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[28][0] <= 1)) {
							terr[28][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[28][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 260 && x < 337 && y > 405 && y < 444 && !(terrcolor[27][0] == colors[120]))		//Territory 30
			{
				if (terrcolor[27][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[27][0] <= 1)) {
							terr[27][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[27][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 340 && x < 417 && y > 405 && y < 444 && !(terrcolor[26][0] == colors[120]))		//Territory 31
			{
				if (terrcolor[26][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[26][0] <= 1)) {
							terr[26][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[26][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 541 && x < 618 && y > 387 && y < 426 && !(terrcolor[33][0] == colors[120]))		//Territory 32
			{
				if (terrcolor[33][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[33][0] <= 1)) {
							terr[33][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[33][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 621 && x < 698 && y > 387 && y < 426 && !(terrcolor[34][0] == colors[120]))		//Territory 33
			{
				if (terrcolor[34][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[34][0] <= 1)) {
							terr[34][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[34][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 701 && x < 778 && y > 387 && y < 426 && !(terrcolor[35][0] == colors[120]))		//Territory 34
			{
				if (terrcolor[35][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[35][0] <= 1)) {
							terr[35][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[35][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 100 && x < 177 && y > 308 && y < 347 && !(terrcolor[40][0] == colors[120]))		//Territory 35
			{
				if (terrcolor[40][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[40][0] <= 1)) {
							terr[40][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[40][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 302 && x < 379 && y > 343 && y < 382 && !(terrcolor[31][0] == colors[120]))		//Territory 36
			{
				if (terrcolor[31][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[31][0] <= 1)) {
							terr[31][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[31][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 382 && x < 459 && y > 343 && y < 382 && !(terrcolor[32][0] == colors[120]))		//Territory 37
			{
				if (terrcolor[32][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[32][0] <= 1)) {
							terr[32][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[32][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 662 && x < 739 && y > 308 && y < 347 && !(terrcolor[45][0] == colors[120]))		//Territory 38
			{
				if (terrcolor[45][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[45][0] <= 1)) {
							terr[45][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[45][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 142 && x < 219 && y > 247 && y < 286 && !(terrcolor[38][0] == colors[120]))		//Territory 39
			{
				if (terrcolor[38][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[38][0] <= 1)) {
							terr[38][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[38][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 222 && x < 299 && y > 247 && y < 286 && !(terrcolor[39][0] == colors[120]))		//Territory 40
			{
				if (terrcolor[39][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[39][0] <= 1)) {
							terr[39][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[39][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 462 && x < 539 && y > 248 && y < 287 && !(terrcolor[41][0] == colors[120]))		//Territory 41
			{
				if (terrcolor[41][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[41][0] <= 1)) {
							terr[41][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[41][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 542 && x < 619 && y > 248 && y < 287 && !(terrcolor[42][0] == colors[120]))		//Territory 42
			{
				if (terrcolor[42][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[42][0] <= 1)) {
							terr[42][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[42][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 622 && x < 699 && y > 248 && y < 287 && !(terrcolor[43][0] == colors[120]))		//Territory 43
			{
				if (terrcolor[43][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[43][0] <= 1)) {
							terr[43][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[43][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 702 && x < 779 && y > 248 && y < 287 && !(terrcolor[44][0] == colors[120]))		//Territory 44
			{
				if (terrcolor[44][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[44][0] <= 1)) {
							terr[44][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[44][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 102 && x < 179 && y > 186 && y < 225 && !(terrcolor[36][0] == colors[120]))		//Territory 45
			{
				if (terrcolor[36][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[36][0] <= 1)) {
							terr[36][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[36][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 182 && x < 259 && y > 186 && y < 225 && !(terrcolor[37][0] == colors[120]))		//Territory 46
			{
				if (terrcolor[37][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[37][0] <= 1)) {
							terr[37][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[37][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 262 && x < 339 && y > 186 && y < 225 && !(terrcolor[50][0] == colors[120]))		//Territory 47
			{
				if (terrcolor[50][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[3][0] <= 1)) {
							terr[3][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[3][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 422 && x < 499 && y > 186 && y < 225 && !(terrcolor[49][0] == colors[120]))		//Territory 48
			{
				if (terrcolor[49][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[49][0] <= 1)) {
							terr[49][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[49][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 502 && x < 579 && y > 186 && y < 225 && !(terrcolor[48][0] == colors[120]))		//Territory 49
			{
				if (terrcolor[48][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[48][0] <= 1)) {
							terr[48][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[48][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 582 && x < 659 && y > 186 && y < 225 && !(terrcolor[47][0] == colors[120]))		//Territory 50
			{
				if (terrcolor[47][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[47][0] <= 1)) {
							terr[47][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[47][0] += 1;
						gameState = "phase2";
						//currentPlayer++;
						selectt = 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			else if (x > 662 && x < 739 && y > 186 && y < 225 && !(terrcolor[46][0] == colors[120]))		//Territory 51 
			{
				if (terrcolor[46][0] == playercolor[currentPlayer - 1]) {
					fortification = 0;
					if (selectt) {
						if (!(terr[46][0] <= 1)) {
							terr[46][0] -= 1;
							selectt = 0;
						}
					}
					else {
						terr[46][0] += 1;
						gameState = "phase2";
						currentPlayer++;
						selectt = 1;
						if (currentPlayer > playernum) {
							currentPlayer == 1;
						}
					}
				}
				else
					fortification = 1;
			}
			/*else {
				fortification = 1;
			}*/
		}
		else if (gameState == "attack") {
			y = height - y;
			if (x > 23 && x < 98 && y > 719 && y < 758 && !(terrcolor[0][0] == colors[120]))		//Territory 1
			{
				if (terrcolor[0][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					//just = 3;                            // For Debugging
					if (just % 2) {
						terrcolor[0][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[0] - 1]--;
						territory[0] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;
						
						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 100 && x < 179 && y > 719 && y < 758 && !(terrcolor[1][0] == colors[120]))		//Territory 2
			{
				if (terrcolor[1][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[1][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1] += 1;
						terrNum[territory[1] - 1] -= 1;
						territory[1] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;
						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 341 && x < 417 && y > 719 && y < 758 && !(terrcolor[7][0] == colors[120]))		//Territory 3
			{
				if (terrcolor[7][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[7][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[7] - 1]--;
						territory[7] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 542 && x < 617 && y > 729 && y < 768 && !(terrcolor[13][0] == colors[120]))		//Territory 4
			{
				if (terrcolor[13][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[13][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[13] - 1]--;
						territory[13] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 620 && x < 698 && y > 729 && y < 768 && !(terrcolor[14][0] == colors[120]))		//Territory 5
			{
				if (terrcolor[14][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[14][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[14] - 1]--;
						territory[14] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 61 && x < 138 && y > 658 && y < 697 && !(terrcolor[2][0] == colors[120]))		//Territory 6
			{
				if (terrcolor[2][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[2][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[2] - 1]--;
						territory[2] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 142 && x < 219 && y > 658 && y < 697 && !(terrcolor[3][0] == colors[120]))		//Territory 7
			{
				if (terrcolor[3][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[3][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[3] - 1]--;
						territory[3] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 222 && x < 299 && y > 658 && y < 697 && !(terrcolor[4][0] == colors[120]))		//Territory 8
			{
				if (terrcolor[4][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[4][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[4] - 1]--;
						territory[4] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 301 && x < 378 && y > 658 && y < 697 && !(terrcolor[5][0] == colors[120]))		//Territory 9
			{
				if (terrcolor[5][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[5][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[5] - 1]--;
						territory[5] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 381 && x < 458 && y > 658 && y < 697 && !(terrcolor[6][0] == colors[120]))		//Territory 10
			{
				if (terrcolor[6][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[6][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[6] - 1]--;
						territory[6] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 661 && x < 738 && y > 668 && y < 707 && !(terrcolor[15][0] == colors[120]))		//Territory 11
			{
				if (terrcolor[15][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[15][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[15] - 1]--;
						territory[15] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 101 && x < 178 && y > 597 && y < 636 && !(terrcolor[8][0] == colors[120]))		//Territory 12
			{
				if (terrcolor[8][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[8][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[8] - 1]--;
						territory[8] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 181 && x < 258 && y > 597 && y < 636 && !(terrcolor[9][0] == colors[120]))		//Territory 13
			{
				if (terrcolor[9][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[9][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[9] - 1]--;
						territory[9] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 261 && x < 338 && y > 597 && y < 636 && !(terrcolor[10][0] == colors[120]))		//Territory 14
			{
				if (terrcolor[10][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[10][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[10] - 1]--;
						territory[10] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 541 && x < 618 && y > 607 && y < 646 && !(terrcolor[16][0] == colors[120]))		//Territory 15
			{
				if (terrcolor[16][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[16][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[16] - 1]--;
						territory[16] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 621 && x < 698 && y > 607 && y < 646 && !(terrcolor[17][0] == colors[120]))		//Territory 16
			{
				if (terrcolor[17][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[17][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[17] - 1]--;
						territory[17] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 701 && x < 778 && y > 607 && y < 646 && !(terrcolor[18][0] == colors[120]))		//Territory 17
			{
				if (terrcolor[18][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[18][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[18] - 1]--;
						territory[18] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 61 && x < 138 && y > 536 && y < 575 && !(terrcolor[11][0] == colors[120]))		//Territory 18
			{
				if (terrcolor[11][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[11][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[11] - 1]--;
						territory[11] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 302 && x < 379 && y > 536 && y < 575 && !(terrcolor[12][0] == colors[120]))		//Territory 19
			{
				if (terrcolor[12][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[12][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[12] - 1]--;
						territory[12] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 422 && x < 499 && y > 546 && y < 585 && !(terrcolor[19][0] == colors[120]))		//Territory 20
			{
				if (terrcolor[19][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[19][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[19] - 1]--;
						territory[19] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 502 && x < 579 && y > 546 && y < 585 && !(terrcolor[20][0] == colors[120]))		//Territory 21
			{
				if (terrcolor[20][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[20][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[20] - 1]--;
						territory[20] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 582 && x < 659 && y > 546 && y < 585 && !(terrcolor[21][0] == colors[120]))		//Territory 22
			{
				if (terrcolor[21][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[21][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[21] - 1]--;
						territory[21] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 662 && x < 739 && y > 546 && y < 585 && !(terrcolor[22][0] == colors[120]))		//Territory 23
			{
				if (terrcolor[22][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[22][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[22] - 1]--;
						territory[22] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 141 && x < 218 && y > 465 && y < 504 && !(terrcolor[30][0] == colors[120]))		//Territory 24
			{
				if (terrcolor[30][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[30][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[30] - 1]--;
						territory[30] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 380 && x < 457 && y > 465 && y < 504 && !(terrcolor[24][0] == colors[120]))		//Territory 25
			{
				if (terrcolor[24][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[24][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[24] - 1]--;
						territory[24] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 460 && x < 537 && y > 465 && y < 504 && !(terrcolor[25][0] == colors[120]))		//Territory 26
			{
				if (terrcolor[25][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[25][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[25] - 1]--;
						territory[25] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 622 && x < 699 && y > 483 && y < 522 && !(terrcolor[23][0] == colors[120]))		//Territory 27
			{
				if (terrcolor[23][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[23][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[23] - 1]--;
						territory[23] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 100 && x < 177 && y > 405 && y < 444 && !(terrcolor[29][0] == colors[120]))		//Territory 28
			{
				if (terrcolor[29][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[29][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[29] - 1]--;
						territory[29] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 180 && x < 257 && y > 405 && y < 444 && !(terrcolor[28][0] == colors[120]))		//Territory 29
			{
				if (terrcolor[28][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[28][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[28] - 1]--;
						territory[28] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 260 && x < 337 && y > 405 && y < 444 && !(terrcolor[27][0] == colors[120]))		//Territory 30
			{
				if (terrcolor[27][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[27][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[27] - 1]--;
						territory[27] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 340 && x < 417 && y > 405 && y < 444 && !(terrcolor[26][0] == colors[120]))		//Territory 31
			{
				if (terrcolor[26][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[26][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[26] - 1]--;
						territory[26] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 541 && x < 618 && y > 387 && y < 426 && !(terrcolor[33][0] == colors[120]))		//Territory 32
			{
				if (terrcolor[33][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[33][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[33] - 1]--;
						territory[33] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 621 && x < 698 && y > 387 && y < 426 && !(terrcolor[34][0] == colors[120]))		//Territory 33
			{
				if (terrcolor[34][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[34][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[34] - 1]--;
						territory[34] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 701 && x < 778 && y > 387 && y < 426 && !(terrcolor[35][0] == colors[120]))		//Territory 34
			{
				if (terrcolor[35][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[35][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[35] - 1]--;
						territory[35] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 100 && x < 177 && y > 308 && y < 347 && !(terrcolor[40][0] == colors[120]))		//Territory 35
			{
				if (terrcolor[40][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[40][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[40] - 1]--;
						territory[40] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 302 && x < 379 && y > 343 && y < 382 && !(terrcolor[31][0] == colors[120]))		//Territory 36
			{
				if (terrcolor[31][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[31][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[31] - 1]--;
						territory[31] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 382 && x < 459 && y > 343 && y < 382 && !(terrcolor[32][0] == colors[120]))		//Territory 37
			{
				if (terrcolor[32][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[32][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[32] - 1]--;
						territory[32] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 662 && x < 739 && y > 308 && y < 347 && !(terrcolor[45][0] == colors[120]))		//Territory 38
			{
				if (terrcolor[45][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[45][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[45] - 1]--;
						territory[45] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 142 && x < 219 && y > 247 && y < 286 && !(terrcolor[38][0] == colors[120]))		//Territory 39
			{
				if (terrcolor[38][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[38][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[38] - 1]--;
						territory[38] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 222 && x < 299 && y > 247 && y < 286 && !(terrcolor[39][0] == colors[120]))		//Territory 40
			{
				if (terrcolor[39][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[39][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[39] - 1]--;
						territory[39] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 462 && x < 539 && y > 248 && y < 287 && !(terrcolor[41][0] == colors[120]))		//Territory 41
			{
				if (terrcolor[41][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[41][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[41] - 1]--;
						territory[41] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 542 && x < 619 && y > 248 && y < 287 && !(terrcolor[42][0] == colors[120]))		//Territory 42
			{
				if (terrcolor[42][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[42][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[42] - 1]--;
						territory[42] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 622 && x < 699 && y > 248 && y < 287 && !(terrcolor[43][0] == colors[120]))		//Territory 43
			{
				if (terrcolor[43][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[43][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[43] - 1]--;
						territory[43] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 702 && x < 779 && y > 248 && y < 287 && !(terrcolor[44][0] == colors[120]))		//Territory 44
			{
				if (terrcolor[44][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[44][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[44] - 1]--;
						territory[44] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 102 && x < 179 && y > 186 && y < 225 && !(terrcolor[36][0] == colors[120]))		//Territory 45
			{
				if (terrcolor[36][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[36][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[36] - 1]--;
						territory[36] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 182 && x < 259 && y > 186 && y < 225 && !(terrcolor[37][0] == colors[120]))		//Territory 46
			{
				if (terrcolor[37][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[37][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[37] - 1]--;
						territory[37] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 262 && x < 339 && y > 186 && y < 225 && !(terrcolor[50][0] == colors[120]))		//Territory 47
			{
				if (terrcolor[50][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[50][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[50] - 1]--;
						territory[50] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 422 && x < 499 && y > 186 && y < 225 && !(terrcolor[49][0] == colors[120]))		//Territory 48
			{
				if (terrcolor[49][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[49][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[49] - 1]--;
						territory[49] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 502 && x < 579 && y > 186 && y < 225 && !(terrcolor[48][0] == colors[120]))		//Territory 49
			{
				if (terrcolor[48][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[48][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[48] - 1]--;
						territory[48] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 582 && x < 659 && y > 186 && y < 225 && !(terrcolor[47][0] == colors[120]))		//Territory 50
			{
				if (terrcolor[47][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[47][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[47] - 1]--;
						territory[47] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			else if (x > 662 && x < 739 && y > 186 && y < 225 && !(terrcolor[46][0] == colors[120]))		//Territory 51 
			{
				if (terrcolor[46][0] == playercolor[currentPlayer - 1]) {
					attacking = 1;
				}
				else {
					attacking = 0;
					just = rand() % 10 + 1;
					if (just % 2) {
						terrcolor[46][0] = playercolor[currentPlayer - 1];
						terrNum[currentPlayer - 1]++;
						terrNum[territory[46] - 1]--;
						territory[46] = currentPlayer;
						//attack2 = 1;
						//attack3 = 0;
						card = 1;

						gameState = "phase2";
					}
					else {
						//attack2 = 0;
						//attack3 = 1;
						card = 0;
						gameState = "phase2";
						currentPlayer += 1;
						if (currentPlayer == playernum + 1) {
							currentPlayer = 1;
						}
					}
				}
			}
			}
	}
	

	glutPostRedisplay();
	
}

int main(int argc, char*argv[]) {
	//playernum = 2;
	save();
	initTerrColor();
	InitRandomizer(); // seed the random number generator...
	srand(time(0));  // Seed again
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(100, 100); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("RISK IN SPACE                   ~Mohammed Zain ul Hassan"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...                             
	glewInit();
	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutTimerFunc(1000.0, Timer, 0);
	//string imageFileName = "BG/BG1.jpg";
	//ReadImage(imageFileName, imgData);
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse
	glutMotionFunc(MousePressedAndMoved); // Mouse
	//glMatrixMode(GL_PROJECTION);
	//gluOrtho2D(-1, 1, -1, 1);


	glutMainLoop();
	return 1;
}
#endif /* AsteroidS_CPP_ */
