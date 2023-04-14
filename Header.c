#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <string.h>
typedef struct Cell{
	int x;
	int y;
	int w;
	int h;
	int State;
	SDL_Texture * Texture;
} Cell;
typedef struct Hoverer{
	int x;
	int y;
	int w;
	int h;
} Hoverer;
typedef struct Circle{
	int x;
	int y;
	int w;
	int h;
	SDL_Texture * Texture;
} Circle;
typedef struct Coordinates{
	int i;
	int j;
} Coordinates;
typedef struct Player{
	int score;
    char name[9];
    char date[11];
} Player;
int RandomNumber(int nr_min, int nr_max){
    static bool initialized = false;
    if(!initialized)
    {
        initialized = true;
        srand(time(NULL));
    }
    // generate the random number
    return rand() % nr_max + nr_min;
}
int RandomColor(){
	return RandomNumber(0,255);
}
SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(file_name);
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    return image_texture;
}
SDL_Texture *initialize_texture_for_text(TTF_Font *Font, SDL_Color color, const char* Text, SDL_Renderer *renderer) {
	SDL_Surface * TextSurface = TTF_RenderText_Solid(Font, Text, color);
	SDL_Texture * TextTexture = SDL_CreateTextureFromSurface(renderer, TextSurface);
    SDL_FreeSurface(TextSurface);
    return TextTexture;
}
void generate_set(int n, int (*Solution)[n]){
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			Solution[i][j] = RandomNumber(1,2);
		}		
	}
}
void show_set(int n, int (*Tab)[n]){
	printf("\n");
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			printf("%d|",Tab[i][j]);
		}
		printf("\n");
	}
}
void create_numbers(int n, int m, int (*Mat)[n], int(*Pos)[m]){
    for (int i = 0; i < m; i++){
        for (int j = 0; j < m; j++){
            Pos[i][j] = 0;
        }    
    }    
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (Mat[i][j] == 2){
                Pos[i][j]++;
                Pos[i+1][j+1]++;
            }
            else{
                Pos[i+1][j]++;
                Pos[i][j+1]++;
            }
        }    
    }
}
void copy_set(int n, int (*Mat)[n], int (*New)[n]){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
			New[i][j] = Mat[i][j];
		}
	}
}
bool is_set_zero(int n, int (*Mat)[n]){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
			if (Mat[i][j] != 0)
				return false;
		}
	}
	return true;
}
bool is_same_set(int n, int (*Mat)[n], int (*Mat2)[n]){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
			if (Mat[i][j] != Mat2[i][j]){
				return false;
			}
		}
	}
	return true;
}
Coordinates Next_State0(struct Cell (*Cells)[6]){
	Coordinates Val;
    for (int j = 0; j < 6; j++){
        for (int i = 0; i < 6; i++){
			if (Cells[i][j].State == 0){
				Val.i = j;
				Val.j = i;
				return Val;
			}
		}
	}
	Val.i = -1;
	Val.j = -1;
	return Val;
}
Player* get_scores(Player Tab[]){
    FILE *fptr;
    if ((fptr = fopen("file.bin","rb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for(int n = 0; n < 5; n++)
        fread(&Tab[n], sizeof(Player), 1, fptr); 
    fclose(fptr); 
    return Tab;
}
int leaderboard_check(Player Tab2[], int score){
    for (int i = 0; i < 5; i++){
        if (Tab2[i].score > score){
            return i;
        }
    }
    return -1;
}
Player* update_PlayersArray(Player TopPlayers[],Player User){
	int i;
	Player Tab[5];
	if (leaderboard_check(TopPlayers, User.score) != -1){
		for (i = 0 ; i < leaderboard_check(TopPlayers, User.score); i++){
			Tab[i] = TopPlayers[i];
		}
		Tab[i] = User;
		for (i ; i < 4; i++){
			Tab[i+1] = TopPlayers[i];
		}
		for (i = 0; i < 5; i++){
			TopPlayers[i] = Tab[i];
		}
	}
	return TopPlayers;
}
Player* update_scores(Player Tab[]){
    FILE *fptr;
    if ((fptr = fopen("file.bin","wb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for(int n = 0; n < 5; n++)
        fwrite(&Tab[n], sizeof(struct Player), 1, fptr); 
    fclose(fptr); 
    return Tab;
}
Hoverer set_hoverer_to_zero(struct Hoverer hover){
	hover.x = 0;
	hover.y = 0;
	hover.h = 0;
	hover.w = 0;
	return hover;
}
void initialize_set(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], struct Cell (*Cells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize){
	create_numbers(6, 7, Solution, Numbers);
	copy_set(7, Numbers, UserTemp);
	for (int i = 0; i < 7; i++){
		for (int j = 0; j < 7; j++){
			Circles[i][j].x = 258-14 + j*cellsize;
			Circles[i][j].y = 108-14 + i*cellsize;
			Circles[i][j].w = 28;
			Circles[i][j].h = 28;
		}
	}
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			Cells[i][j].x = 258+ i*cellsize;
			Cells[i][j].y = 108 + j*cellsize;
			Cells[i][j].w = cellsize;
			Cells[i][j].h = cellsize;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Cells[i][j].State = 0;
			UserStates[i][j] = 0;
			Cells[i][j].Texture = Cell0;
		}
	}
	for (int i = 0; i < n+1; i++){
		for (int j = 0; j < n+1; j++){
			switch (Numbers[i][j]){
			case 0:
				Circles[i][j].Texture = Text0;
				break;
			case 1:
				Circles[i][j].Texture = Text1;
				break;
			case 2:
				Circles[i][j].Texture = Text2;
				break;
			case 3:
				Circles[i][j].Texture = Text3;
				break;
			case 4:
				Circles[i][j].Texture = Text4;
				break;
			}
		}
	}
}
void initialize_player_vs_bot(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], int (*BotTemp)[7], struct Cell (*Cells)[6], struct Cell (*BotCells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize){
	create_numbers(6, 7, Solution, Numbers);
	copy_set(7, Numbers, UserTemp);
	copy_set(7, Numbers, BotTemp);
	for (int i = 0; i < 7; i++){
		for (int j = 0; j < 7; j++){
			Circles[i][j].x = 44-14 + j*cellsize;
			Circles[i][j].y = 138-14 + i*cellsize;
			Circles[i][j].w = 28;
			Circles[i][j].h = 28;
		}
	}
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			Cells[i][j].x = 44 + i*cellsize;
			Cells[i][j].y = 138 + j*cellsize;
			Cells[i][j].w = cellsize;
			Cells[i][j].h = cellsize;
		}
	}
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			BotCells[i][j].x = 88 + 6*cellsize + i*cellsize;
			BotCells[i][j].y = 138 + j*cellsize;
			BotCells[i][j].w = cellsize;
			BotCells[i][j].h = cellsize;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Cells[i][j].State = 0;
			UserStates[i][j] = 0;
			Cells[i][j].Texture = Cell0;
			BotCells[i][j].State = 0;
			BotCells[i][j].Texture = Cell0;
		}
	}
	for (int i = 0; i < n+1; i++){
		for (int j = 0; j < n+1; j++){
			switch (Numbers[i][j]){
			case 0:
				Circles[i][j].Texture = Text0;
				break;
			case 1:
				Circles[i][j].Texture = Text1;
				break;
			case 2:
				Circles[i][j].Texture = Text2;
				break;
			case 3:
				Circles[i][j].Texture = Text3;
				break;
			case 4:
				Circles[i][j].Texture = Text4;
				break;
			}
		}
	}
}
void initialize_save(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], struct Cell (*Cells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Cell1, SDL_Texture *Cell2, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize){
	create_numbers(6, 7, Solution, Numbers);
	for (int i = 0; i < 7; i++){
		for (int j = 0; j < 7; j++){
			Circles[i][j].x = 258-14 + j*cellsize;
			Circles[i][j].y = 108-14 + i*cellsize;
			Circles[i][j].w = 28;
			Circles[i][j].h = 28;
		}
	}
	for (int i = 0; i < 6; i++){
		for (int j = 0; j < 6; j++){
			Cells[i][j].x = 258+ i*cellsize;
			Cells[i][j].y = 108 + j*cellsize;
			Cells[i][j].w = cellsize;
			Cells[i][j].h = cellsize;
		}
	}
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			Cells[i][j].State = UserStates[i][j];
			switch (Cells[i][j].State){
			case 0:
				Cells[i][j].Texture = Cell0;
				break;
			case 1:
				Cells[i][j].Texture = Cell1;
				break;
			case 2:
				Cells[i][j].Texture = Cell2;
				break;
			}
		}
	}
	for (int i = 0; i < n+1; i++){
		for (int j = 0; j < n+1; j++){
			switch (Numbers[i][j]){
			case 0:
				Circles[i][j].Texture = Text0;
				break;
			case 1:
				Circles[i][j].Texture = Text1;
				break;
			case 2:
				Circles[i][j].Texture = Text2;
				break;
			case 3:
				Circles[i][j].Texture = Text3;
				break;
			case 4:
				Circles[i][j].Texture = Text4;
				break;
			}
		}
	}
}
void update_save(int n, int m, int (*Solution)[n], int (*UserStates)[n], int(*UserTemp)[m], unsigned int Time){
    FILE *fptr;
    int i,j;
    if ((fptr = fopen("save.bin","wb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            fwrite(&UserTemp[i][j], sizeof(int), 1, fptr);
        }
    }
    for (i = 0; i < 6; i++){
        for (j = 0; j < 6; j++){
            fwrite(&Solution[i][j], sizeof(int), 1, fptr);
        }
    }
    for (i = 0; i < 6; i++){
        for (j = 0; j < 6; j++){
            fwrite(&UserStates[i][j], sizeof(int), 1, fptr);
        }
    }
    fwrite(&Time, sizeof(unsigned int), 1, fptr);

    fclose(fptr);
}
void PutNull(Cell (*Cells)[6], Cell (*BotCells)[6], Circle (*Circles)[7], SDL_Texture *Names[], SDL_Texture *Scores[], SDL_Texture *Dates[]){
	int i, j;
	for (i = 0; i < 6; i++){
		for (j = 0; j < 6; j++){
			Cells[i][j].Texture = NULL;
			BotCells[i][j].Texture = NULL;
		}
	}
	for (i = 0; i < 7; i++){
		for (j = 0; j < 7; j++){
			Circles[i][j].Texture = NULL;
		}
	}
	for (i = 0; i < 5; i++){
		Names[i] = NULL;
		Scores[i] = NULL;
		Dates[i] = NULL;
	}
}
int read_save(int n, int m, int (*Solution)[n], int (*UserStates)[n], int(*UserTemp)[m]){
    FILE *fptr;
    int i,j;
    unsigned int Time;
    if ((fptr = fopen("save.bin","rb")) == NULL){
        printf("Error! opening file");
        exit(1);
    }
    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            fread(&UserTemp[i][j], sizeof(int), 1, fptr);
        }
    }
    for (i = 0; i < 6; i++){
        for (j = 0; j < 6; j++){
            fread(&Solution[i][j], sizeof(int), 1, fptr);
        }
    }
    for (i = 0; i < 6; i++){
        for (j = 0; j < 6; j++){
            fread(&UserStates[i][j], sizeof(int), 1, fptr);
        }
    }
    fread(&Time, sizeof(unsigned int), 1, fptr);
    fclose(fptr);
    return Time;
}