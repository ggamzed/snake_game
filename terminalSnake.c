#include <stdio.h>
#include <stdlib.h>
#include <termios.h> // for changemode()
#include <unistd.h> // for changemode() and kbhit()
// Screen constants
#define HEIGHT 20
#define WIDTH 20

// Function to get user input
int keyBoardHit(char* keyHit)
{
    scanf("%c", keyHit);
    return *keyHit;
}

// Change the buffer mode to get user input without pressing enter
void changeMode(int dir)
{
  static struct termios oldt, newt;
 
  if (dir == 1)
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

// kbhit() func is not allowed for linux
// so this is our manual kbhit function
int kbHit(void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

// Function to initalize values,
// and bait position 
void initialize_game(int* gameOver, int* x, int* y, int* xBait, int* yBait, int* score) 
{ 
    // game is not over
    *gameOver = 0; 

    // for snake's initial position
    *x = HEIGHT / 2; 
    *y = WIDTH / 2; 

    // randomize the bait's position
    do
    {
        *xBait = rand() % 20;

    } while(*xBait==0);
    
    do
    {
        *yBait = rand() % 20;

    } while(*yBait == 0);

    // initialize score
    *score = 0; 
} 
  
// Function to print snake's head, bait,
// and the boundaries 
void print_game(int* x, int* y, int* xBait, int* yBait, int* score) 
{ 
    system("clear"); 
    int i, j;
    for (i=0; i<HEIGHT; i++) 
    { 
        for (j=0; j<WIDTH; j++) 
        { 
            if (i==0 || i==WIDTH-1 || j==0 || j==HEIGHT-1) 
            { 
                printf("#"); 
            } 
            else 
            { 
                if (i == *x && j == *y) 
                    printf("0"); 

                else if (i == *xBait && j == *yBait) 
                    printf("*"); 

                else
                    printf(" "); 
            } 
        } 
        printf("\n"); 
    } 
   
    printf("score = %d", *score); 
    printf("\n"); 
    printf("press X to quit the game"); 
} 
  
// Function to determine which key has been pressed
void input(int* gameOver, char* keyHit, int* flag) 
{ 
    *gameOver = 0;
    
    // For windows kbhit() and getch() which are conio.h functions can be used
    if (keyBoardHit(keyHit)) 
    { 
        switch (*keyHit) 
        { 
        case 'a': 
            *flag = 1; 
            break; 
        case 's': 
            *flag = 2; 
            break; 
        case 'd': 
            *flag = 3; 
            break; 
        case 'w': 
            *flag = 4; 
            break; 
        case 'x': 
            *gameOver = 1; 
            break; 
        } 
    } 
} 
  
// Function for the game logic
void game_logic(int* gameOver, int* x, int* y, int* xBait, int* yBait, int* score, int* flag) 
{ 
    // For movements
    switch (*flag) 
    { 
    case 1: 
        (*y)--; 
        break; 
    case 2: 
        (*x)++; 
        break; 
    case 3: 
        (*y)++; 
        break; 
    case 4: 
        (*x)--; 
        break; 
    default: 
        break; 
    } 
  
    // If the game is over 
    if (*x < 0 || *x > HEIGHT || *y < 0 || *y > WIDTH) 
        *gameOver = 1; 
  
    // If snake eats the bait
    if (*x == *xBait && *y == *yBait) 
    { 
    // Generate new bait
    do
    { 
        *xBait = rand() % 20; 

    } while(*xBait == 0);
     
    do
    {
        *yBait = rand() % 20;

    } while(*yBait==0);

    // Update the score
    *score += 10; 
    } 
} 

// Function for print the score,
// and game over message
void GameOver(int* score)
{
    system("clear");
    printf("-------------------------------\n");
    printf("GAME OVER\n");
    printf("Score: %d\n", *score);
    printf("-------------------------------\n");
}
  
// Driver Code 
int main() 
{  
    int gameOver;
    char keyHit;
    int x, y, xBait, yBait;
    int score;
    int flag;
  
    // Initialize values
    initialize_game(&gameOver, &x, &y, &xBait, &yBait, &score); 
  
    // Until the game is over 
    while (!gameOver) { 
  
        // Function Call 
        print_game(&x, &y, &xBait, &yBait, &score); 
        changeMode(1);
        input(&gameOver, &keyHit, &flag); 
        changeMode(0);
        game_logic(&gameOver, &x, &y, &xBait, &yBait, &score, &flag); 
    } 
    GameOver(&score);
} 