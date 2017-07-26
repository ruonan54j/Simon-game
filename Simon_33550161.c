/*
Name:				Nancy (Ruonan) Jia
Date:				2016-12-02
bonus assignment:	Simon Game
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <DAQlib.h>
#include <Windows.h>

#define TOTAL_SWITCH 4

#define MAX_LENGTH 5
#define DELAY 500 

#define TRUE 1
#define FALSE 0
#define DEFAULT -1

#define ON 1
#define OFF 0

/*functions*/
void runSimon ( void );
void generateSequence (int length, int data[]);
void clearArray (int data[]);
void flashLED (int channel);
int readSwitch(void);
int randInt( void ) ;

/*setting up simulator*/
int main (void)
{
	int setupNum;
	
	printf ("Welcome! Enter 6 to begin the Simon Game on the simulator: \n");
	scanf ("%d", &setupNum);

	if (setupDAQ(setupNum)==TRUE)
		runSimon();

	else
		printf ("Error. Simon game could not be initiated...");
	
	system ("PAUSE");
	return 0;
}

/*Runs Simon Game*/
void runSimon ( void )
{
	int gameOver;
	int pattern[5]={0};
	int length = 1;
	int n;
	int count;
	int switchNum=DEFAULT;
	int channel;
	int allOff;
	
	while(continueSuperLoop()==TRUE)
	{
		clearArray(pattern);
		gameOver = FALSE;

		generateSequence (MAX_LENGTH, pattern);
		Sleep (DELAY*2);

		while (length <= MAX_LENGTH && gameOver == FALSE) 
		{	
			/*resetting lights pattern*/
			n=0;
			while (n < length)
			{ 
				channel = pattern[n];
				flashLED(channel);
				Sleep (40);
				n++;
			}
			
			/*checking user entered pattern*/
			count=0;
			while (count < length && gameOver==FALSE)
			{   
				switchNum = DEFAULT;
				while (switchNum == DEFAULT)
				{
					switchNum = readSwitch();
				}
				
				allOff = ON;
				/*check switch was released*/
				while (allOff != DEFAULT)
				{
					allOff = readSwitch();
				}

				/*pattern does not match, ending game*/
				if (pattern[count] != switchNum)
				{
					gameOver=TRUE;
				}
			
				/*pattern matches, continue game*/
				else if (pattern[count] == switchNum)
				{
					count++;
					switchNum = DEFAULT;
				}
			}
			length++;
		}

			/*user has won*/
			 if (gameOver==FALSE)
			 {	
				for (count = 0; count < 3; count++)
				{
					flashLED(0);
					Sleep (50);
				}
				gameOver = DEFAULT;
				length = 1;
			 }
			 /*user has lost*/
			 if (gameOver==TRUE)
			{	
				for (count = 0; count < 3; count++)
				{
					flashLED(1);
					Sleep (50);
				}
					gameOver=FALSE;
					length = 1;
			 }
	}
	}
	
/*	
	Takes no parameters
	Returns a random value between 0 and 3 inclusive
*/
int randInt( void ) 
{
    return rand() % 4;
}

/*
	Parameters: length: size of array to be generates
				pattern[]: array of integers
	Assigns "length" number of random integers to an array
*/

void generateSequence (int length, int pattern[])
{ 
	int j;
	srand( (unsigned) time( NULL ) );
	
	for (j=0; j < length; j++)
		pattern[j] = randInt();
	
}

/* 
	Parameters: data[]: array of integers
	sets all values in array to default number (-1)
*/
void clearArray (int data[])
{
	int n;
	for (n = 0; n < MAX_LENGTH; n++)
	{ 
		data[n] = DEFAULT;
	}
}

/*
	Parameters: channel: channel of light to turn on
	flashes light for entered channel for 500ms before turning it off
*/
void flashLED (int channel)
{
	digitalWrite (channel, ON);
	Sleep (DELAY);
	digitalWrite (channel, OFF);
}
/*
	Takes no parameters
	Reads from switches until one of the switch is turned on. Returns the integer number of the switch that was turned on.
*/
int readSwitch(void)
{
	int status0, status1, status2, status3;
	int onSwitch=DEFAULT;
			status0 = digitalRead(0);
			status1 = digitalRead(1);
			status2 = digitalRead(2);
			status3 = digitalRead(3);
			if (status0 ==ON)	
			{
				onSwitch = 0;
			}
			else if (status1 ==ON)	
			{
				onSwitch = 1;
			}
			else if (status2 ==ON)	
			{
				onSwitch = 2;
			}
			else if (status3 ==ON)	
			{
				onSwitch = 3;
			}

		return onSwitch;
	
}
