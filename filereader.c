#include<stdio.h>
#include<conio.h>
#include<stdbool.h>
#include<stdlib.h>
#include "keys.c"
#include "types.h"

//clears the screen
void clearScreen() {
    system("cls");
}
void copy_grid(Board src,Board destination)
{
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            destination[i][j]=src[i][j];
        }
    }
}
typedef struct Gamesdata{
    int lastmodified;
    int id;
    struct Settings{
    int ai;
    int hint;
    int gamemode;
    int slow;
    }settings;

    int initialgrid[9][9];
    int grid[9][9];
    int errorgrid[9][9];
}Game;

// Function to copy game settings from source to destination 
void CopyGame(Game *src, Game *destination) {
    *destination = *src;  // Copy the entire struct, including arrays
}


int* emptyboardinit(){
    int a[9][9]={
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
    return a;
}

bool completevalid(int a[9][9])
{
    //check 
    for(int i=0;i<9;i++)
        {
            int row_sum=0;
            int row_product=1;
            int col_sum=0;
            int col_product=1;

            for(int j=0;j<9;j++)
            {
            row_sum+=a[i][j];
            row_product*=a[i][j];
            col_sum+=a[j][i];
            col_product*=a[j][i];
            }

            if(row_sum!=45 ||col_sum!=45||row_product!=362880 ||col_product!=362880)
            {
            printf("row or column");
            return false;
            }
        }

    for(int row=0;row<9;row+=3)
    {   
        for(int col=0;col<9;col+=3)
        { int sum=0,product=1;
            for(int i=row;i<row+3;i++)
            {
                    for(int j=col;j<col+3;j++)
                    {
                        sum+=a[i][j];
                        product*=a[i][j];
                    }
            }
            if(sum!=45 || product!=362880)
            {
                printf("square");
                return false;
            }
        } 
    }

    return true;
}

Game* read_games(int* numgames) {
    FILE* fs = fopen("pastgames.txt", "rb");
    if (fs == NULL) {
    return NULL;
    }
    // Get the total number of records (size of file / size of a book structure)
    fseek(fs, 0, SEEK_END);
    long fileSize = ftell(fs);
    *numgames = fileSize / sizeof(Game);

    // Allocate memory for the array of games
    Game* games = (Game*)malloc((*numgames) * sizeof(Game));
    if (games == NULL) {
        perror("Memory allocation error");
        fclose(fs);
        return NULL;
    }

    // Read the data from the file into the array of structures
    rewind(fs); // Move file pointer back to the beginning
    fread(games, sizeof(Game), *numgames, fs);

    fclose(fs);
    return games;
}



void SavedGamesMenu(Game *Self)
{
    int selectedgame=0;
    Game Game1;
    int grid[9][9]= 
        {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
        };
        
        FILE *pastgames;
        pastgames=fopen("pastgames.txt","rb");
        int numgames;

        Game* games = read_games(&numgames);
        FILE *fs;
        fs=fopen("pastgames.txt","a+");
        fseek(fs,0,SEEK_END);

        //empty Game init
        Game bk;
        bk.lastmodified=200;
        bk.id=10;
        bk.settings.ai=bk.settings.gamemode=bk.settings.hint=bk.settings.slow=0;
        copy_grid(emptyboardinit,bk.errorgrid);
        copy_grid(emptyboardinit,bk.grid);


        fwrite(&bk,sizeof(bk),1,fs);
        fclose(fs);
        
        int test_grid[9][9]={
        {5, 3, 4, 6, 7, 8, 9, 1, 2},
        {6, 7, 2, 1, 9, 5, 3, 4, 8},
        {1, 9, 8, 3, 4, 2, 5, 6, 7},
        {8, 5, 9, 7, 6, 1, 4, 2, 3},
        {4, 2, 6, 8, 5, 3, 7, 9, 1},
        {7, 1, 3, 9, 2, 4, 8, 5, 6},
        {9, 6, 1, 5, 3, 7, 2, 8, 4},
        {2, 8, 7, 4, 1, 9, 6, 3, 5},
        {3, 4, 5, 2, 8, 6, 1, 7, 9}
    };
        // printf("%d",completevalid(test_grid)); //for testing
        if (games != NULL) {
            int key=1;
            while(key!=ESCKEY)
            {   clearScreen();
                printf("+ID----Last Modified----Ai--+\n");
                for (int i = 0; i < numgames; i++)
                {
                        if(i==selectedgame)
                        {
                        printf("\033[47;30m %d \t %d \t\t %d \033[0m\n",games[i].id ,games[i].lastmodified,games[i].settings.ai);
                        }
                        else{
                        printf(" %d \t %d \t\t %d \n",games[i].id ,games[i].lastmodified,games[i].settings.ai);
                        }
                }
                printf("\nPress backspace to go back to main menu");
            key=getch();
            if(key==UPARROW && selectedgame>0)
            {
                selectedgame-=1;
            }
            if(key==DOWNARROW && selectedgame<numgames-1)
            {
                selectedgame+=1;
            }
            if(key==ENTERKEY)
            {
            CopyGame(&games[selectedgame],&Game1);
            printf("\n copied Game data");
            //and send this Game to start menu
            *Self=bk;
            break;
            }
            if(key==BACKSPACE)
            {
                free(games);
                break;
            }

            }
            
    }
    else{
        printf("No Saved games\nPress backspace to go back to main menu...");
    }
    free(games); // Free the dynamically allocated memory for the array of books
}

