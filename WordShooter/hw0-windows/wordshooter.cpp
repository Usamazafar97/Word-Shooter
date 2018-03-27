//============================================================================
// Name        : Word-shooter.cpp
// Author      : UsamaZafar
// Copyright   : (c) Reserved
// Description : Basic 2D game of Bubble Shooting...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include "util.h"
#include<fstream>
#include<math.h>


#define PI 3.14159265  
#define DegToRad(val) (val*PI/180)
#define RadToDeg(val) (val*180/PI)

using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

//////////////////////////////Declaration of Global Variables//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int x;
//int y;
//-------------------------------------------------------------------------
// this array contains the array indices of the start of all alphabets
int _indices[27]; // 27 = 26 + 1, to store the endValue which will be used in word search algorithm to make it fast and not have index out of bound exception
char _alphabets[26];
//-------------------------------------------------------------------------
int Mouse_click_X = 0;
int Mouse_click_Y = 0;
int InitialAlphabetArray[100][3] = { 0 };
float c = 0;
int row_count;
int row;

float XPos = 460;
float YPos = 10;
int MouseX = 0;
int MouseY = 0;
float Current_Time = 0.0;
float Timeleft = 0.0;
double theta;
float Delta_X = 0.0;
float Delta_Y = 0.0;
float Del_X = 0;
int Del_Y = 0;
int chk = 0;
float theta1 = 0.0;
float slope = 0.0;
float Distance_from_ball_to_mouse = 0.0;
float vilocity = 0.0;
float angle = 0.0;
int Current_AlphabetName = 0;
float Current_AlphabetXPos = 0;
float Current_AlphabetYPos = 0;

int Shooter_AlphabetName = 0;
float Shooter_AlphabetXPos = 0;
float Shooter_AlphabetYPos = 0;

float FiredAlphabets[100][3] = { 0 };
int FiredAplhabetsCount = 0;
///////////////////////////////////////////////**********************************************************///////////////////////////////
int alphabet_array[9][15]; int shoot[100];

string ErasedWord[50] = { "\0" };
int ErasedWordCount = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
////////////////////////////////////////////////////////////////////

bool SearchInDictionary(string wordToSearch);
void AlphabetsToBeErased(int px, int py);
void initialize();
/////////////////////////////////////////////////////////////////////
const int nalphabets = 26;

enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
string alphabet_char_array = "abcdefghijklmnopqrstuvwxyz";
char AlphabetEnumToString(alphabets to_convert)
{
	return alphabet_char_array[to_convert];
}
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...

bool permutate(string s);

void StopFiredBall();
bool FindCollision();
void MatchWords(int cx, int cy);
float MovingVelocityDirection;
//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char)* data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		//cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy)
{
	cx = (px) / 60;
	cy = (py) / 60;
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
	px = cx * 60;
	py = cy * 60;
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}



void MoveShooterBall()
{
	
	double speed = 1;          // Random speed.
	double dx = (cos(MovingVelocityDirection))* speed;
	double dy = (sin(MovingVelocityDirection)) * speed;
	////cout << theta1 << "---" << dx << "---" << dy << endl;


	bool xDestReached = false;
	bool yDestReached = false;
	
	if (FindCollision() || Current_AlphabetYPos >= height - 60)
	{
		int cellx, celly;
		int px, py;
		Pixels2Cell(Current_AlphabetXPos + 30, Current_AlphabetYPos + 30, cellx, celly);
		Cell2Pixels(cellx, celly, px, py);
		Current_AlphabetXPos = px;
		Current_AlphabetYPos = py;

		StopFiredBall();
		MatchWords(cellx, celly);
		if (py <= 120) Timeleft = 0;
	}
	else
	{
		Current_AlphabetXPos = Current_AlphabetXPos + dx;
		Current_AlphabetYPos = Current_AlphabetYPos + dy;
		if (Current_AlphabetXPos >= width - 60)
		{
			MovingVelocityDirection = DegToRad(180) - MovingVelocityDirection;
		}
		else if (Current_AlphabetXPos <= 0)
		{
			MovingVelocityDirection = DegToRad(180) - MovingVelocityDirection;
		}
		else if (Current_AlphabetYPos >= 540 - 60)
		{
			StopFiredBall();
		}
	}

	//	//cout << XPos<<endl;
	//	//cout << YPos<<endl;



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	glutPostRedisplay();
}







/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
	/////////////////////////////First line of alphabets///////////////////////////
	///////////////////////////////////*************************************************************///////////////////////////////////

	if (Timeleft <= 0){
		//DrawString(width / 2 - 65, height - 325, width, height,
		//"GAME ENDED", colors[RED]);
		DrawString(width / 2 - 65, height - 325, width, height,
			"Game over!", colors[RED]);
		DrawString(width / 2 - 75, height - 425, width, height,
			"Your score is " + Num2Str(score), colors[RED]);
	}
	else
	{

		{
			

			//DrawString(width / 2 - 130, height - 325, width, height,
			//"Time Left to Game End:" + Num2Str(Timeleft) + " secs", colors[RED]);


			/////////////////////////////Alphabet on shooter///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			DrawAlphabet((alphabets)Shooter_AlphabetName, Shooter_AlphabetXPos, Shooter_AlphabetYPos, awidth, aheight);
			////////////////////////////////Fired Alphabets //////////////////////////////////////////////////////////////////////////////////////////

			for (int j = 0; j < FiredAplhabetsCount; j++)
			{
				if (FiredAlphabets[j][0] != -1)
				{
					DrawAlphabet((alphabets)((int)FiredAlphabets[j][0]), FiredAlphabets[j][1], FiredAlphabets[j][2], awidth, aheight);
				}

			}
			//////////////////////////Work For Ball Move////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



			if (chk == 1)
			{
				MoveShooterBall();
				DrawAlphabet((alphabets)Current_AlphabetName, Current_AlphabetXPos, Current_AlphabetYPos, awidth, aheight);
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);

			DrawString(width / 2 - 30, height - 25, width, height,
				"Current Time:" + Num2Str(Current_Time) + " secs", colors[BLUE]);
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			// #----------------- Displaying  time on of top Screen ----------------------------#

			DrawString(width / 2 + 190, height - 25, width, height,
				"Time Left:" + Num2Str(Timeleft) + " secs", colors[RED]);

			//DrawString(140, height - 20, width, height + 5, "X= " + Num2Str(MouseX) + "    Y= " + Num2Str(MouseY), colors[BLUE_VIOLET]);
			///////////////////////////////////////////extra middle text/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//DrawString(width / 2 - 130, height - 325, width, height,
				//"Time Left to Game End:" + Num2Str(Timeleft) + " secs", colors[RED]);

			DrawString(40, height - 325, width, height,
				"ErasedWord:", colors[RED]);

			for (int j = 0, ErasedWordheight = 0; j < ErasedWordCount; j++, ErasedWordheight += 20)
			{
				DrawString(80, height - 345 - ErasedWordheight, width, height,
					ErasedWord[j], colors[RED]);
			}






		}
	}

	// #-------------------------------------------------------------------------------------#
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES

	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}


/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		if (XPos >= 12){
			XPos = XPos - 5;
		}
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		if (XPos <= 845){
			XPos = XPos + 5;
		}
	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
		if (YPos <= height - 222){
			YPos = YPos + 5;
		}
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
		if (YPos < height){
			YPos = YPos - 5;
		}
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/

	glutPostRedisplay();

}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls
	MouseX = x;
	MouseY = y;
	////cout << x << y << endl;

}


/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{

		if (state == GLUT_UP)
		{
			YPos = 10;
			XPos = 465;
			chk = 1;
			Mouse_click_X = x;
			//Mouse_click_Y = height-y;
			Mouse_click_Y = 660 - y;

			float xDiff = Mouse_click_X - Current_AlphabetXPos;
			float yDiff = Mouse_click_Y - Current_AlphabetYPos;
			MovingVelocityDirection = atan2f(yDiff, xDiff);
			/*slope = (Mouse_click_X - XPos) / (Mouse_click_Y - YPos);
			c = YPos - (slope*XPos);*/
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{


	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.

	}

}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */
void Timer(int m) {

	Current_Time++;

	Timeleft--;
	glutPostRedisplay();
	glutTimerFunc(1000, Timer, 0);
}

void StopFiredBall()
{
	chk = 0;

	FiredAlphabets[FiredAplhabetsCount][0] = Current_AlphabetName;
	FiredAlphabets[FiredAplhabetsCount][1] = Current_AlphabetXPos;
	FiredAlphabets[FiredAplhabetsCount][2] = Current_AlphabetYPos;
	FiredAplhabetsCount++;

	Shooter_AlphabetName = InitialAlphabetArray[30 + FiredAplhabetsCount][0];
	Shooter_AlphabetXPos = 460;
	Shooter_AlphabetYPos = 10;

	Current_AlphabetName = Shooter_AlphabetName;
	Current_AlphabetXPos = Shooter_AlphabetXPos;
	Current_AlphabetYPos = Shooter_AlphabetYPos;
	XPos = 460;
	YPos = 10;

}
/*
* our gateway main function
* */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SetTimeLeft(int t)
{
	Timeleft = t;//to display time left
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	ReadWords("words_alpha.txt", dictionary); // dictionary is an InitialAlphabetArray of strings
	//print first 5 words from the dictionary
	for (int i = 0; i < 5; ++i)
		//cout << " word " << i << " =" << dictionary[i] << endl;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	

	// sorting the alphabets
	std::sort(dictionary, dictionary + dictionarysize);

	_alphabets[0] = 'a';
	_alphabets[1] = 'b';
	_alphabets[2] = 'c';
	_alphabets[3] = 'd';
	_alphabets[4] = 'e';
	_alphabets[5] = 'f';
	_alphabets[6] = 'g';
	_alphabets[7] = 'h';
	_alphabets[8] = 'i';
	_alphabets[9] = 'j';
	_alphabets[10] = 'k';
	_alphabets[11] = 'l';
	_alphabets[12] = 'm';
	_alphabets[13] = 'n';
	_alphabets[14] = 'o';
	_alphabets[15] = 'p';
	_alphabets[16] = 'q';
	_alphabets[17] = 'r';
	_alphabets[18] = 's';
	_alphabets[19] = 't';
	_alphabets[20] = 'u';
	_alphabets[21] = 'v';
	_alphabets[22] = 'w';
	_alphabets[23] = 'x';
	_alphabets[24] = 'y';
	_alphabets[25] = 'z';

	int alphabetCount = 0;
	for (int i = 0; i < dictionarysize; i++)
	{
		if (dictionary[i][0] == _alphabets[alphabetCount]) {
			_indices[alphabetCount] = i;
			alphabetCount++;
		}
	}


	// just for debug purpose
	for (int j = 0; j < 26; j++)
	{
		//cout << endl << "The Alphabet " << _alphabets[j] << " starts at : " << _indices[j];
	}



	for (int i = 0; i < 100; i++)//geting random alphabets
	{
		InitialAlphabetArray[i][0] = GetAlphabet();
	}

	
	SetTimeLeft(Current_Time + 120);

	

	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("OSAMA ZAFAR WORD SHOOTER GAME"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(10000, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	initialize();

	Current_AlphabetName = InitialAlphabetArray[30 + FiredAplhabetsCount][0];
	Current_AlphabetXPos = 460;
	Current_AlphabetYPos = 10;


	Shooter_AlphabetName = InitialAlphabetArray[30 + FiredAplhabetsCount][0];
	Shooter_AlphabetXPos = 460;
	Shooter_AlphabetYPos = 10;


	glutMainLoop();




	return 1;
}


void initialize()
{

	for (int i = 0, x_axis = 0; i < 15; i++, x_axis += 60)
	{
		//InitialAlphabetArray[i][0] = i;
		InitialAlphabetArray[i][1] = x_axis;
		InitialAlphabetArray[i][2] = 540;

		FiredAlphabets[FiredAplhabetsCount][0] = InitialAlphabetArray[i][0];
		FiredAlphabets[FiredAplhabetsCount][1] = x_axis;
		FiredAlphabets[FiredAplhabetsCount][2] = 540;
		FiredAplhabetsCount++;


	}
	for (int i = 15, x_axis = 0; i < 30; i++, x_axis += 60)
	{
		//InitialAlphabetArray[i][0] = i;
		InitialAlphabetArray[i][1] = x_axis;
		InitialAlphabetArray[i][2] = 480;

		FiredAlphabets[FiredAplhabetsCount][0] = InitialAlphabetArray[i][0];
		FiredAlphabets[FiredAplhabetsCount][1] = x_axis;
		FiredAlphabets[FiredAplhabetsCount][2] = 480;
		FiredAplhabetsCount++;
	}


}



bool CirclesCollide(float x1, float y1, float x2, float y2)
{
	// Radius = 28;
	float SumOfRadiuses = 56;

	float xDiff = x1 - x2;
	float yDiff = y1 - y2;

	float Dist = sqrt((xDiff * xDiff) + (yDiff * yDiff));
	if (Dist <= SumOfRadiuses)
	{
		return true;
	}
	return false;

}
bool FindCollision()//to find collision
{
	bool Collide = false;
	for (int i = 0; i < 30; i++)
	{
		if (InitialAlphabetArray[i][0] == -1)
		{
			continue;
		}
		float x1 = InitialAlphabetArray[i][1] + 30;
		float y1 = InitialAlphabetArray[i][2] + 30;
		float x2 = Current_AlphabetXPos + 30;
		float y2 = Current_AlphabetYPos + 30;

		if (CirclesCollide(x1, y1, x2, y2)) {
			Collide = true;
			break;
		}


	}

	for (int i = 0; i < FiredAplhabetsCount; i++)
	{
		if (FiredAlphabets[i][0] == -1)
		{
			continue;
		}

		float x1 = FiredAlphabets[i][1] + 30;
		float y1 = FiredAlphabets[i][2] + 30;
		float x2 = Current_AlphabetXPos + 30;
		float y2 = Current_AlphabetYPos + 30;

		if (CirclesCollide(x1, y1, x2, y2)) {
			Collide = true;
			break;
		}

	}
	return Collide;
}

bool AlphabetExistAtCell(int cx, int cy)//existance of any alphabet
{
	bool exist = false;

	int px, py;
	Cell2Pixels(cx, cy, px, py);
	for (int i = 0; i < FiredAplhabetsCount; i++)
	{
		if (FiredAlphabets[i][0] == -1)
		{
			continue;
		}

		if (px >= FiredAlphabets[i][1] && px < FiredAlphabets[i][1] + 60 &&
			py >= FiredAlphabets[i][2] && py < FiredAlphabets[i][2] + 60

			)
		{
			exist = true;
			break;
		}
	}

	return exist;
}
alphabets GetAlphabetForColumn(int cx, int cy)
{
	alphabets result = alphabets::AL_A;

	int px, py;
	Cell2Pixels(cx, cy, px, py);
	for (int i = 0; i < FiredAplhabetsCount; i++)
	{
		if (FiredAlphabets[i][0] == -1)
		{
			continue;
		}

		if (px >= FiredAlphabets[i][1] && px < FiredAlphabets[i][1] + 60 &&
			py >= FiredAlphabets[i][2] && py < FiredAlphabets[i][2] + 60

			)
		{
			result = (alphabets)(int)FiredAlphabets[i][0];
			break;
		}
	}

	return result;
}
void MatchWordHorizontal(int cx, int cy)
{
	// check left to right
	int StartCol = cx;
	for (int i = cx; i >= cx - 4; i--)
	{
		if (AlphabetExistAtCell(i, cy))
		{
			StartCol = i;
		}
		else
		{
			break;
		}
	}
	int EndCol = cx;
	for (int i = cx; i < cx + 4; i++)
	{
		if (AlphabetExistAtCell(i, cy))
		{
			EndCol = i;
		}
		else
		{

			break;
		}
	}

	//cout << " Start COl: " << StartCol << " end COl : " << EndCol << endl;
	//cout << " To Match: ";
	string word = "";
	for (int i = StartCol; i <= EndCol; i++)
	{
		word += AlphabetEnumToString(GetAlphabetForColumn(i, cy));
	}
	//cout << word << endl;
}
void MatchWordVertical(int cx, int cy)
{
	int px, py;
	// check left to right
	int StartRow = cy;
	for (int i = cy; i >= cy - 4; i--)
	{
		if (AlphabetExistAtCell(cx, i))
		{
			StartRow = i;
		}
		else
		{
			break;
		}
	}
	int EndRow = cy;
	for (int i = cy; i < cy + 4; i++)
	{
		if (AlphabetExistAtCell(cx, i))
		{
			EndRow = i;
		}
		else
		{

			break;
		}
	}

	//cout << " Start Row: " << StartRow << " end Row : " << EndRow << endl;
	//cout << " To Match: ";
	string word = "";
	for (int i = StartRow; i <= EndRow; i++)
	{
		word += AlphabetEnumToString(GetAlphabetForColumn(cx, i));
	}
	//cout << word << endl;
	if (word.length() >= 2)
	{
		if (permutate(word))
		{
			for (int i = StartRow; i <= EndRow; i++)
			{
				Cell2Pixels(cx, i, px, py);
				AlphabetsToBeErased(px, py);
			}
		}
	}

}
void AlphabetsToBeErased(int px, int py)
{
	for (int i = 0; i< FiredAplhabetsCount; i++)
	{
		if (FiredAlphabets[i][1] == px && FiredAlphabets[i][2] == py)
		{
			FiredAlphabets[i][0] = -1;
		}
	}

}
void MatchWords(int cx, int cy)
{
	MatchWordHorizontal(cx, cy);
	MatchWordVertical(cx, cy);
	
}

bool permutate(string s)
{
	std::string default = s;

	int perm = 1, digits = default.size();
	for (int i = 1; i <= digits; perm *= i++); // perm = !default.size()
	for (int a = 0; a<perm; a++)
	{
		std::string avail = default; // initialize string
		std::string ToCompareStr = "";
		// b counts the columns
		for (int b = digits, div = perm; b>0; b--)
		{
			// compute the number of repetitions for one character in the actual column
			div /= b;
			//compute the index of the character in the string 
			int index = (a / div) % b;

			//cout << avail[index] << "\n";;
			ToCompareStr += avail[index];

			//remove the used character
			avail.erase(index, 1);
		}

		if (ToCompareStr.length()>0)
		{
			if (SearchInDictionary(ToCompareStr))
			{
				return true;
			}
		}
		//cout << endl;
	}
	//cout << "permutations:" << perm << "\n";
	return false;
}


bool SearchInDictionary(string wordToSearch)
{
	//cout << "Time before Searching: " << Current_Time << "\n";
	bool isFound = false;
	int index = wordToSearch[0] - 97;
	char temp = _alphabets[index];
	int start = _indices[index];
	int end = _indices[index + 1];

	/*for (int i = 0; i<dictionarysize; i++)
	{*/
	// search will be reduces in the portion from start to end
	for (int i = start; i<end; i++)
	{
		//string tempStr = string(dictionary[i]); // should be removed for performance jz for debug purpose!
		if (wordToSearch == dictionary[i])
		{

			ErasedWord[ErasedWordCount] = wordToSearch;
			score += wordToSearch.length();
			//cout << "found: " << dictionary[i] << "  \t at  \t " << i << "\n";
			isFound = true;
			ErasedWordCount++;
			break;
		}

	}
	//cout << "Time after Searching: " << Current_Time << "\n";
	return isFound;
}

#endif /* */
