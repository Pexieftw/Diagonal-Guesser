#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <string.h>
#include "Header.h"
int main(int argc, char **argv)
{	
	// Variables
	int x, y, i, j, TextW = 0, TextH=0, cellsize = 64, NLetters, Solution[6][6], Numbers[7][7], UserTemp[7][7], BotTemp[7][7], UserStates[6][6];
	Hoverer Hover, CHover, MenuBHover;
	Cell Cells[6][6], BotCells[6][6];
	Circle Circles[7][7];
	Coordinates Val;
	Player TopPlayers[5], User;
	char Text[1], Time[50], BestScore[50], Score[50], Date[50], Name[50], UserName[8], date[11];
	unsigned int lastTime, currentTime, OutTime, saveTime = 0, pauseTime = 0;
	bool savegame = false;
	// local date
	time_t t = time(NULL);
	struct tm tm;
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	// Initialize SDL_Image
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	// Initialize SDL_TTF
	TTF_Init();
    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Diagonal Guesser", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// Game State
	state = MAINMENU;
	// Load Image files
	SDL_Texture * Cell0 = initialize_texture_from_file("Pictures/CellState0_64.png", renderer);
	SDL_Texture * Cell1 = initialize_texture_from_file("Pictures/CellState1_64.png", renderer);
	SDL_Texture * Cell2 = initialize_texture_from_file("Pictures/CellState2_64.png", renderer);
	SDL_Texture * CellHover = initialize_texture_from_file("Pictures/CellHover_64.png", renderer);
	SDL_Texture * CircleHover = initialize_texture_from_file("Pictures/CircleHover.png", renderer);
	SDL_Texture * MenuButtonHover = initialize_texture_from_file("Pictures/MenuButtonHover.png", renderer);
	SDL_Texture * Circle_28 = initialize_texture_from_file("Pictures/Circle_28.png", renderer);
	SDL_Texture * MenuButton = initialize_texture_from_file("Pictures/MenuButton.png", renderer);
	SDL_Texture * HomeButton = initialize_texture_from_file("Pictures/HomeButton.png", renderer);
	SDL_Texture * Background = initialize_texture_from_file("Pictures/FLATBACKGROUND.png", renderer);
	SDL_Texture * Shadow = initialize_texture_from_file("Pictures/BACKGROUNDSHADOW.png", renderer);
	SDL_Texture * PauseBar = initialize_texture_from_file("Pictures/PauseBar.png", renderer);
	SDL_Texture * GameOverBar = initialize_texture_from_file("Pictures/GameOverBar.png", renderer);
	SDL_Texture * GameOverBot = initialize_texture_from_file("Pictures/GameOverBot.png", renderer);
	SDL_Texture * Bar = initialize_texture_from_file("Pictures/bar.png", renderer);
	SDL_Texture * MainMenu = initialize_texture_from_file("Pictures/MainMenu.png", renderer);
	SDL_Texture * TopRankingsMenu = initialize_texture_from_file("Pictures/TopRankingsMenu.png", renderer);
	SDL_Texture * BackgroundTime = initialize_texture_from_file("Pictures/BackgroundTime.png", renderer);
	SDL_Texture * NameMenu = initialize_texture_from_file("Pictures/WriteName.png", renderer);
	SDL_Texture * SoloMenu = initialize_texture_from_file("Pictures/SoloMenu.png", renderer);
	SDL_Texture * YouWon = initialize_texture_from_file("Pictures/YouWon.png", renderer);
	SDL_Texture * BotWon = initialize_texture_from_file("Pictures/BotWon.png", renderer);
	SDL_Texture * Win = NULL;
	// Load Font / Color Font
    TTF_Font * Font = TTF_OpenFont("Fonts/OpenSans-Bold.ttf", 24);
	TTF_Font * Font2 = TTF_OpenFont("Fonts/OpenSans-Bold.ttf", 32);
	TTF_Font * Font3 = TTF_OpenFont("Fonts/OpenSans-Bold.ttf", 44);
    SDL_Color color = { 77+20, 84+20, 120+20 };
	SDL_Color color2 = { 255, 255, 255 };
	SDL_Texture * timer = initialize_texture_for_text(Font2, color, "Time: 0s", renderer);
	SDL_Texture * Text0 = initialize_texture_for_text(Font, color, "0", renderer);
	SDL_Texture * Text1 = initialize_texture_for_text(Font, color, "1", renderer);
	SDL_Texture * Text2 = initialize_texture_for_text(Font, color, "2", renderer);
	SDL_Texture * Text3 = initialize_texture_for_text(Font, color, "3", renderer);
	SDL_Texture * Text4 = initialize_texture_for_text(Font, color, "4", renderer);
	SDL_Texture * Best = NULL;
	SDL_Texture * Names[5];
	SDL_Texture * Scores[5];
	SDL_Texture * Dates[5];
	SDL_Texture * UserNameTexture = initialize_texture_for_text(Font2, color, "", renderer);
	PutNull(Cells, BotCells, Circles, Names, Scores, Dates);
	get_scores(TopPlayers);
	sprintf(Score,"%ds", TopPlayers[0].score);
	Scores[0] = initialize_texture_for_text(Font2, color, Score, renderer);
	// Initialize SDL mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // Load audio files
    Mix_Music *backgroundSound = Mix_LoadMUS("Music/SPUTNIK3.mp3");
    Mix_Chunk *click = Mix_LoadWAV("Music/normal-hitnormal.wav");
	// -1 to make it infinite
	Mix_PlayMusic(backgroundSound, -1);
    bool running = true;
    SDL_Event event;
    while(running){
		x = event.motion.x;
		y = event.motion.y;
		if (state == MAINMENU){
			OutTime = SDL_GetTicks() - pauseTime;
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 244 ) && ( x < 666 ) && ( y > 217 ) && ( y < 302 ) ){
						state = SOLOMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
					}
					else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87 ) && ( y < 302+87 ) ){
						savegame = false;
						lastTime=0;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						timer = initialize_texture_for_text(Font2, color, "0s", renderer);
						generate_set(6, Solution);
						initialize_set(Solution, Numbers, BotTemp, BotCells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = BOT;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87*2 ) && ( y < 302+87*2 ) ){
						savegame = false;
						lastTime=0;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						timer = initialize_texture_for_text(Font2, color, "0s", renderer);
						generate_set(6, Solution);
						initialize_player_vs_bot(Solution, Numbers, UserTemp, BotTemp, Cells, BotCells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = PLAYERVSBOT;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+3*87 ) && ( y < 302+3*87 ) ){
						for (i = 0; i < 5; i++){
							sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
							sprintf(Score,"%ds", TopPlayers[i].score);
							sprintf(Date,"%s", TopPlayers[i].date);
							Names[i] = initialize_texture_for_text(Font2, color, Name, renderer);
							Scores[i] = initialize_texture_for_text(Font2, color, Score, renderer);
							Dates[i] = initialize_texture_for_text(Font2, color, Date, renderer);
						}
						state = TOPRANKINGS;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					break;
                case SDL_MOUSEMOTION:
                    if( ( x > 244 ) && ( x < 666 ) && ( y > 217 ) && ( y < 302 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
					else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87 ) && ( y < 302+87 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217+87;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
                    else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87*2 ) && ( y < 302+87*2 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217+87*2;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
                    else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87*3 ) && ( y < 302+87*3 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217+87*3;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
					else{
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
					break;
				}
			}
		}
		else if (state == SOLOMENU){
			OutTime = SDL_GetTicks() - pauseTime;
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 244 ) && ( x < 666 ) && ( y > 217 ) && ( y < 302 ) ){
						savegame = false;
						lastTime=0;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						timer = initialize_texture_for_text(Font2, color, "0s", renderer);
						generate_set(6, Solution);
						initialize_set(Solution, Numbers, UserTemp, Cells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = SINGLEPLAYER;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
					else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87 ) && ( y < 302+87 ) ){
						savegame = true;
						saveTime = read_save(6, 7, Solution, UserStates, UserTemp);
						lastTime = saveTime;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						sprintf(Time, "%ds", saveTime/1000);
						timer = initialize_texture_for_text(Font2, color, Time, renderer);
						initialize_save(Solution, Numbers, UserTemp, Cells, Circles, renderer, Cell0, Cell1, Cell2, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = SINGLEPLAYER;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 244 ) && ( x < 666 ) && ( y > 217 ) && ( y < 302 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
					else if( ( x > 244 ) && ( x < 666 ) && ( y > 217+87 ) && ( y < 302+87 ) ){
						MenuBHover.x = 244;
						MenuBHover.y = 217+87;
						MenuBHover.h = 302-217;
						MenuBHover.w = 666-244;
                    }
					else{
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
					if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        CHover.x = 836-4;
                        CHover.y = 4;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}		
		}
        else if (state == SINGLEPLAYER){
			if (savegame)
				currentTime = SDL_GetTicks() - pauseTime - OutTime + saveTime;
			else
				currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000) {
                lastTime = currentTime - currentTime%1000;
                sprintf(Time, "%ds", currentTime/1000);
				timer = initialize_texture_for_text(Font2, color, Time, renderer);
            }
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
                    j = (y-108)/cellsize;
                    i = (x-258)/cellsize;
                    if( ( x > 258 ) && ( x < 258 + cellsize*6 ) && ( y > 108 ) && ( y < 108 + cellsize*6 ) ){
                        switch (Cells[i][j].State){
                        case 0:
                            Cells[i][j].Texture = Cell1;
                            Cells[i][j].State = 1;
							UserStates[i][j] = 1;
							UserTemp[j][i+1]--;
							UserTemp[j+1][i]--;
                            break;
                        case 1:
                            Cells[i][j].Texture = Cell2;
                            Cells[i][j].State = 2;
							UserStates[i][j] = 2;
							UserTemp[j][i+1]++;
							UserTemp[j+1][i]++;
							UserTemp[j][i]--;
							UserTemp[j+1][i+1]--;
                            break;
                        case 2:
                            Cells[i][j].Texture = Cell0;
                            Cells[i][j].State = 0;
							UserStates[i][j] = 3;
							UserTemp[j][i]++;
							UserTemp[j+1][i+1]++;
                            break;
                        }
                    }
                    if(is_set_zero(7, UserTemp) == true){
						UserName[0] = '\0';
						NLetters = 0;
						if (atoi(Time)<TopPlayers[4].score){
							state = WRITE;   
							// Initializing hover
							Hover = set_hoverer_to_zero(Hover);
							CHover = set_hoverer_to_zero(CHover);
							MenuBHover = set_hoverer_to_zero(MenuBHover);
						}
						else{
							state = GAMEOVER;
							// Initializing hover
							Hover = set_hoverer_to_zero(Hover);
							CHover = set_hoverer_to_zero(CHover);
							MenuBHover = set_hoverer_to_zero(MenuBHover);
						}
                    }
                    if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        state = MENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					else if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > cellsize+8 ) && ( y < 2*(cellsize+4) ) ){
                        state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 258 ) && ( x < 258 + cellsize*6 ) && ( y > 108 ) && ( y < 108 + cellsize*6 ) ){
                        j = (y-108)/cellsize;
                        i = (x-258)/cellsize;
                        if (!(Cells[i][j].x == Hover.x && Cells[i][j].y == Hover.y && Cells[i][j].w == Hover.w && Cells[i][j].h == Hover.h)){
                            Hover.x = Cells[i][j].x;
                            Hover.y = Cells[i][j].y;
                            Hover.h = Cells[i][j].h;
                            Hover.w = Cells[i][j].w;
                        }
                    }
					else{
						Hover = set_hoverer_to_zero(Hover);
					}
					if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        CHover.x = 836-4;
                        CHover.y = 4;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else if ( ( x > 836-4 ) && ( x < 900-4 ) && ( y > cellsize+8 ) && ( y < 2*(cellsize+4) ) ){
                        CHover.x = 836-4;
                        CHover.y = cellsize+8;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
                }
            }
        }
        else if (state == BOT){
            currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000) {
				Val = Next_State0(BotCells);
				if (Val.i != -1 && Val.j != -1){
					if (BotTemp[Val.i][Val.j] == 0){
						BotTemp[Val.i][Val.j+1]--;
						BotTemp[Val.i+1][Val.j]--;
						BotCells[Val.j][Val.i].State = 1;
						BotCells[Val.j][Val.i].Texture = Cell1;
					}
					else{
						BotTemp[Val.i+1][Val.j+1]--;
						BotTemp[Val.i][Val.j]--;
						BotCells[Val.j][Val.i].State = 2;
						BotCells[Val.j][Val.i].Texture = Cell2;
					}
				}
                lastTime = currentTime - currentTime%1000;
                sprintf(Time, "%ds", currentTime/1000);
				timer = initialize_texture_for_text(Font2, color, Time, renderer);
            }
			if(is_set_zero(7, BotTemp) == true){
				state = GAMEOVERBOT;
				// Initializing hover
				Hover = set_hoverer_to_zero(Hover);
				CHover = set_hoverer_to_zero(CHover);
				MenuBHover = set_hoverer_to_zero(MenuBHover);
			}
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
                    if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					break;
                case SDL_MOUSEMOTION:
					if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        CHover.x = 836-4;
                        CHover.y = 4;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
                }
            }
        }
        else if (state == PLAYERVSBOT){
			currentTime = SDL_GetTicks() - pauseTime - OutTime;
            if (currentTime > lastTime + 1000) {
				Val = Next_State0(BotCells);
				if (Val.i != -1 && Val.j != -1){
					if (BotTemp[Val.i][Val.j] == 0){
						BotTemp[Val.i][Val.j+1]--;
						BotTemp[Val.i+1][Val.j]--;
						BotCells[Val.j][Val.i].State = 1;
						BotCells[Val.j][Val.i].Texture = Cell1;
					}
					else{
						BotTemp[Val.i+1][Val.j+1]--;
						BotTemp[Val.i][Val.j]--;
						BotCells[Val.j][Val.i].State = 2;
						BotCells[Val.j][Val.i].Texture = Cell2;
					}
				}
                lastTime = currentTime - currentTime%1000;
                sprintf(Time, "%ds", currentTime/1000);
				timer = initialize_texture_for_text(Font2, color, Time, renderer);
            }
			if(is_set_zero(7, UserTemp) == true){
				Win = YouWon;
				state = WINSCREEN;
				// Initializing hover
				Hover = set_hoverer_to_zero(Hover);
				CHover = set_hoverer_to_zero(CHover);
				MenuBHover = set_hoverer_to_zero(MenuBHover);
			}
			else if(is_set_zero(7, BotTemp) == true){
				Win = BotWon;
				state = WINSCREEN;
				// Initializing hover
				Hover = set_hoverer_to_zero(Hover);
				CHover = set_hoverer_to_zero(CHover);
				MenuBHover = set_hoverer_to_zero(MenuBHover);
			}
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
                    j = (y-138)/cellsize;
                    i = (x-44)/cellsize;
                    if( ( x > 44 ) && ( x < 44 + cellsize*6 ) && ( y > 138 ) && ( y < 138 + cellsize*6 ) ){
                        switch (Cells[i][j].State){
                        case 0:
                            Cells[i][j].Texture = Cell1;
                            Cells[i][j].State = 1;
							UserStates[i][j] = 1;
							UserTemp[j][i+1]--;
							UserTemp[j+1][i]--;
                            break;
                        case 1:
                            Cells[i][j].Texture = Cell2;
                            Cells[i][j].State = 2;
							UserStates[i][j] = 2;
							UserTemp[j][i+1]++;
							UserTemp[j+1][i]++;
							UserTemp[j][i]--;
							UserTemp[j+1][i+1]--;
                            break;
                        case 2:
                            Cells[i][j].Texture = Cell0;
                            Cells[i][j].State = 0;
							UserStates[i][j] = 3;
							UserTemp[j][i]++;
							UserTemp[j+1][i+1]++;
                            break;
                        }
                    }
                    if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 44 ) && ( x < 44 + cellsize*6 ) && ( y > 138 ) && ( y < 138 + cellsize*6 ) ){
                        j = (y-138)/cellsize;
                        i = (x-44)/cellsize;
                        if (!(Cells[i][j].x == Hover.x && Cells[i][j].y == Hover.y && Cells[i][j].w == Hover.w && Cells[i][j].h == Hover.h)){
                            Hover.x = Cells[i][j].x;
                            Hover.y = Cells[i][j].y;
                            Hover.h = Cells[i][j].h;
                            Hover.w = Cells[i][j].w;
                        }
                    }
					else{
						Hover = set_hoverer_to_zero(Hover);
					}
					if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        CHover.x = 836-4;
                        CHover.y = 4;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
                }
            }
        }
		else if (state == MENU){
			if (savegame)
				pauseTime = SDL_GetTicks() - currentTime - OutTime + saveTime;
			else
				pauseTime = SDL_GetTicks() - currentTime - OutTime;	
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
                    if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
                        state = SINGLEPLAYER;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						savegame = false;
						OutTime = SDL_GetTicks();
						pauseTime = 0;
						lastTime = 0;
						sprintf(Time, "0s");
						timer = initialize_texture_for_text(Font2, color, Time, renderer);
						generate_set(6, Solution);
						initialize_set(Solution, Numbers, UserTemp, Cells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = SINGLEPLAYER;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
					else if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						update_save(6, 7, Solution, UserStates, UserTemp, lastTime);
					}
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 261;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
                    }
					else if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 392;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 527;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}
		else if (state == WRITE){
			OutTime = SDL_GetTicks() - pauseTime;
			UserNameTexture = initialize_texture_for_text(Font2, color, UserName, renderer);
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch( event.key.keysym.sym ){
					case SDLK_a:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'a';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_b:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'b';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_c:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'c';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_d:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'd';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_e:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'e';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_f:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'f';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_g:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'g';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_h:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'h';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_i:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'i';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_j:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'j';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_k:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'k';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_l:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'l';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_m:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'm';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_n:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'n';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_o:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'o';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_p:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'p';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_q:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'q';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_r:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'r';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_s:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 's';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_t:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 't';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_u:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'u';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_v:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'v';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_w:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'w';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_x:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'x';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_y:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'y';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_z:
						if (strlen(UserName) < 8){
							UserName[NLetters] = 'z';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_1:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '1';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_2:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '2';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_3:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '3';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_4:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '4';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_5:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '5';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_6:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '6';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_7:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '7';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_8:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '8';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_9:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '9';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_KP_0:
						if (strlen(UserName) < 8){
							UserName[NLetters] = '0';
							UserName[NLetters+1] = '\0';
							NLetters++;
						}
						break;
					case SDLK_BACKSPACE:
						if (strlen(UserName) > 0){
							UserName[NLetters-1] = '\0';
							NLetters--;
						}					
						break;
					}
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 531 ) && ( x < 631 ) && ( y > 378 ) && ( y < 478 ) && (strlen(UserName) > 0) ){
						tm = *localtime(&t);
						sprintf(date, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
						strcpy(User.name, UserName);
						strcpy(User.date, date);
						User.score = atoi(Time);
						update_PlayersArray(TopPlayers, User);
						update_scores(TopPlayers);
						for (i = 0; i < 5; i++){
							sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
							sprintf(Score,"%ds", TopPlayers[i].score);
							sprintf(Date,"%s", TopPlayers[i].date);
							Names[i] = initialize_texture_for_text(Font2, color, Name, renderer);
							Scores[i] = initialize_texture_for_text(Font2, color, Score, renderer);
							Dates[i] = initialize_texture_for_text(Font2, color, Date, renderer);
						}
						state = TOPRANKINGS;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
                    break;
                case SDL_MOUSEMOTION:
					if( ( x > 531 ) && ( x < 631 ) && ( y > 378 ) && ( y < 478 ) && (strlen(UserName) > 0) ){
						CHover.x = 531;
						CHover.y = 378;
						CHover.h = 100;
						CHover.w = 100;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}
		else if (state == GAMEOVER){
			OutTime = SDL_GetTicks() - pauseTime;
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
                    else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						savegame = false;
						OutTime = SDL_GetTicks();
						pauseTime = 0;
						lastTime = 0;
						sprintf(Time, "0s");
						timer = initialize_texture_for_text(Font2, color, Time, renderer);
						generate_set(6, Solution);
						initialize_set(Solution, Numbers, UserTemp, Cells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = SINGLEPLAYER;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    else if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						for (i = 0; i < 5; i++){
							sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
							sprintf(Score,"%ds", TopPlayers[i].score);
							sprintf(Date,"%s", TopPlayers[i].date);
							Names[i] = initialize_texture_for_text(Font2, color, Name, renderer);
							Scores[i] = initialize_texture_for_text(Font2, color, Score, renderer);
							Dates[i] = initialize_texture_for_text(Font2, color, Date, renderer);
						}
						state = TOPRANKINGS;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 261;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
                    }
					else if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 392;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 527;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}
		else if (state == GAMEOVERBOT){
			OutTime = SDL_GetTicks() - pauseTime;
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
					}
                    else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						savegame = false;
						OutTime = SDL_GetTicks();
						pauseTime = 0;
						lastTime = 0;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						timer = initialize_texture_for_text(Font2, color, "0s", renderer);
						generate_set(6, Solution);
						initialize_set(Solution, Numbers, BotTemp, BotCells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = BOT;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    else if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						for (i = 0; i < 5; i++){
							sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
							sprintf(Score,"%ds", TopPlayers[i].score);
							sprintf(Date,"%s", TopPlayers[i].date);
							Names[i] = initialize_texture_for_text(Font2, color, Name, renderer);
							Scores[i] = initialize_texture_for_text(Font2, color, Score, renderer);
							Dates[i] = initialize_texture_for_text(Font2, color, Date, renderer);
						}
						state = TOPRANKINGS;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 261;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
                    }
					else if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 392;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 527;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}
		else if (state == WINSCREEN){
			OutTime = SDL_GetTicks() - pauseTime;
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
					if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						state = MAINMENU;
					}
                    else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						savegame = false;
						lastTime=0;
						sprintf(BestScore, "%ds", TopPlayers[0].score);
						Best = initialize_texture_for_text(Font2, color, BestScore, renderer);
						timer = initialize_texture_for_text(Font2, color, "0s", renderer);
						generate_set(6, Solution);
						initialize_player_vs_bot(Solution, Numbers, UserTemp, BotTemp, Cells, BotCells, Circles, renderer, Cell0, Text0, Text1, Text2, Text3, Text4, 6, UserStates, cellsize);
						state = PLAYERVSBOT;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    else if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						for (i = 0; i < 5; i++){
							sprintf(Name, "%d-%s",i+1, TopPlayers[i].name);
							sprintf(Score,"%ds", TopPlayers[i].score);
							sprintf(Date,"%s", TopPlayers[i].date);
							Names[i] = initialize_texture_for_text(Font2, color, Name, renderer);
							Scores[i] = initialize_texture_for_text(Font2, color, Score, renderer);
							Dates[i] = initialize_texture_for_text(Font2, color, Date, renderer);
						}
						state = TOPRANKINGS;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if( ( x > 261 ) && ( x < 363 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 261;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
                    }
					else if( ( x > 392 ) && ( x < 492 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 392;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else if( ( x > 527 ) && ( x < 627 ) && ( y > 343 ) && ( y < 443 ) ){
						CHover.x = 527;
						CHover.y = 343;
						CHover.h = 100;
						CHover.w = 100;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}
		else if(state == TOPRANKINGS){
            while(SDL_PollEvent(&event)){	
                switch (event.type){
				case SDL_QUIT:
					running = false;
					break;
                case SDL_MOUSEBUTTONDOWN:
                    Mix_PlayChannel(-1, click, 0);
                    if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        state = MAINMENU;
						// Initializing hover
						Hover = set_hoverer_to_zero(Hover);
						CHover = set_hoverer_to_zero(CHover);
						MenuBHover = set_hoverer_to_zero(MenuBHover);
                    }
					break;
                case SDL_MOUSEMOTION:
					if( ( x > 836-4 ) && ( x < 900-4 ) && ( y > 4 ) && ( y < cellsize+4 ) ){
                        CHover.x = 836-4;
                        CHover.y = 4;
                        CHover.h = cellsize;
                        CHover.w = cellsize;
					}
					else{
						CHover = set_hoverer_to_zero(CHover);
					}
					break;
				}
			}
		}	
		// Clear screen with black
		SDL_RenderClear(renderer);
		// Background
        SDL_Rect background_destination = {0, 0, 900, 600};
		SDL_RenderCopy(renderer, Background, NULL, &background_destination);
		// Image Load
		if (state == MAINMENU){
			SDL_Rect background_destination = {0, 0, 900, 600};
			SDL_RenderCopy(renderer, MainMenu, NULL, &background_destination);
			SDL_Rect hover_destination = {MenuBHover.x, MenuBHover.y, MenuBHover.w, MenuBHover.h};
			SDL_RenderCopy(renderer, MenuButtonHover, NULL, &hover_destination);
		}
		if (state == SINGLEPLAYER){
			// BackgroundTime
			SDL_RenderCopy(renderer, BackgroundTime, NULL, &background_destination);
			for (i = 0; i < 6; i++){
				for (j = 0; j < 6; j++){
					SDL_Rect texture_destination = {Cells[i][j].x, Cells[i][j].y, Cells[i][j].w, Cells[i][j].h};
					SDL_RenderCopy(renderer, Cells[i][j].Texture, NULL, &texture_destination);
				}
			}
			SDL_Rect hover_destination = {Hover.x, Hover.y, Hover.w, Hover.h};
			SDL_RenderCopy(renderer, CellHover, NULL, &hover_destination);
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_Rect texture_destination = {Circles[i][j].x, Circles[i][j].y, Circles[i][j].w, Circles[i][j].h};
					SDL_RenderCopy(renderer, Circle_28, NULL, &texture_destination);
				}
			}
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_QueryTexture(Circles[i][j].Texture, NULL, NULL, &TextW, &TextH);
					SDL_Rect texture_destination = {Circles[i][j].x+8, Circles[i][j].y-3, TextW, TextH};
					SDL_RenderCopy(renderer, Circles[i][j].Texture, NULL, &texture_destination);
				}
			}
            // TIME
			SDL_QueryTexture(timer, NULL, NULL, &TextW, &TextH);
            SDL_Rect time_destination = {120, 5, TextW, TextH};
            SDL_RenderCopy(renderer, timer, NULL, &time_destination);
			// BEST TIME
			SDL_QueryTexture(Best, NULL, NULL, &TextW, &TextH);
            SDL_Rect BestTime_destination = {470, 5, TextW, TextH};
            SDL_RenderCopy(renderer, Best, NULL, &BestTime_destination);
			// MENU BUTTON
			SDL_Rect texture_destination = {900-cellsize-4, 4, cellsize, cellsize};
			SDL_RenderCopy(renderer, MenuButton, NULL, &texture_destination);
			// HOME BUTTON
			SDL_Rect texture_destination2 = {900-cellsize-4, 8+cellsize, cellsize, cellsize};
			SDL_RenderCopy(renderer, HomeButton, NULL, &texture_destination2);
			// BUTTON HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == PLAYERVSBOT){
			// BackgroundTime
			SDL_RenderCopy(renderer, BackgroundTime, NULL, &background_destination);
			for (i = 0; i < 6; i++){
				for (j = 0; j < 6; j++){
					SDL_Rect texture_destination = {Cells[i][j].x, Cells[i][j].y, Cells[i][j].w, Cells[i][j].h};
					SDL_RenderCopy(renderer, Cells[i][j].Texture, NULL, &texture_destination);
					SDL_Rect bottexture_destination = {BotCells[i][j].x, BotCells[i][j].y, BotCells[i][j].w, BotCells[i][j].h};
					SDL_RenderCopy(renderer, BotCells[i][j].Texture, NULL, &bottexture_destination);
				}
			}
			SDL_Rect hover_destination = {Hover.x, Hover.y, Hover.w, Hover.h};
			SDL_RenderCopy(renderer, CellHover, NULL, &hover_destination);
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_Rect texture_destination = {Circles[i][j].x, Circles[i][j].y, Circles[i][j].w, Circles[i][j].h};
					SDL_RenderCopy(renderer, Circle_28, NULL, &texture_destination);
					SDL_Rect texture_destination2 = {Circles[i][j].x+44+6*cellsize, Circles[i][j].y, Circles[i][j].w, Circles[i][j].h};
					SDL_RenderCopy(renderer, Circle_28, NULL, &texture_destination2);
				}
			}
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_QueryTexture(Circles[i][j].Texture, NULL, NULL, &TextW, &TextH);
					SDL_Rect texture_destination = {Circles[i][j].x+8, Circles[i][j].y-3, TextW, TextH};
					SDL_RenderCopy(renderer, Circles[i][j].Texture, NULL, &texture_destination);
					SDL_QueryTexture(Circles[i][j].Texture, NULL, NULL, &TextW, &TextH);
					SDL_Rect texture_destination2 = {Circles[i][j].x+8+44+6*cellsize, Circles[i][j].y-3, TextW, TextH};
					SDL_RenderCopy(renderer, Circles[i][j].Texture, NULL, &texture_destination2);
				}
			}
            // TIME
			SDL_QueryTexture(timer, NULL, NULL, &TextW, &TextH);
            SDL_Rect time_destination = {120, 5, TextW, TextH};
            SDL_RenderCopy(renderer, timer, NULL, &time_destination);
			// BEST TIME
			SDL_QueryTexture(Best, NULL, NULL, &TextW, &TextH);
            SDL_Rect BestTime_destination = {470, 5, TextW, TextH};
            SDL_RenderCopy(renderer, Best, NULL, &BestTime_destination);
			// HOME BUTTON
			SDL_Rect texture_destination = {900-cellsize-4, 4, cellsize, cellsize};
			SDL_RenderCopy(renderer, HomeButton, NULL, &texture_destination);
			// BUTTON HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == BOT){
			SDL_RenderCopy(renderer, BackgroundTime, NULL, &background_destination);
			for (i = 0; i < 6; i++){
				for (j = 0; j < 6; j++){
					SDL_Rect texture_destination = {BotCells[i][j].x, BotCells[i][j].y, BotCells[i][j].w, BotCells[i][j].h};
					SDL_RenderCopy(renderer, BotCells[i][j].Texture, NULL, &texture_destination);
				}
			}
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_Rect texture_destination = {Circles[i][j].x, Circles[i][j].y, Circles[i][j].w, Circles[i][j].h};
					SDL_RenderCopy(renderer, Circle_28, NULL, &texture_destination);
				}
			}
			for (i = 0; i < 7; i++){
				for (j = 0; j < 7; j++){
					SDL_QueryTexture(Circles[i][j].Texture, NULL, NULL, &TextW, &TextH);
					SDL_Rect texture_destination = {Circles[i][j].x+8, Circles[i][j].y-3, TextW, TextH};
					SDL_RenderCopy(renderer, Circles[i][j].Texture, NULL, &texture_destination);
				}
			}
            // TIME
			SDL_QueryTexture(timer, NULL, NULL, &TextW, &TextH);
            SDL_Rect time_destination = {120, 5, TextW, TextH};
            SDL_RenderCopy(renderer, timer, NULL, &time_destination);
			// BEST TIME
			SDL_QueryTexture(Best, NULL, NULL, &TextW, &TextH);
            SDL_Rect BestTime_destination = {470, 5, TextW, TextH};
            SDL_RenderCopy(renderer, Best, NULL, &BestTime_destination);
			// HOME BUTTON
			SDL_Rect texture_destination = {900-cellsize-4, 4, cellsize, cellsize};
			SDL_RenderCopy(renderer, HomeButton, NULL, &texture_destination);
			// BUTTON HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == GAMEOVER){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			// BAR
			SDL_RenderCopy(renderer, GameOverBar, NULL, &background_destination);
			// GAMEOVER TEXT
			SDL_QueryTexture(timer, NULL, NULL, &TextW, &TextH);
            SDL_Rect finaltime_destination = {395, 230, TextW, TextH};
            SDL_RenderCopy(renderer, timer, NULL, &finaltime_destination);

			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);

			SDL_QueryTexture(Scores[0], NULL, NULL, &TextW, &TextH);
			SDL_Rect besttime_destination = {395, 280, TextW, TextH};
			SDL_RenderCopy(renderer, Scores[0], NULL, &besttime_destination);
		}
		if (state == GAMEOVERBOT){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			// BAR
			SDL_RenderCopy(renderer, GameOverBot, NULL, &background_destination);
			// HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == WINSCREEN){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			SDL_RenderCopy(renderer, Win, NULL, &background_destination);
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == MENU){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			SDL_RenderCopy(renderer, PauseBar, NULL, &background_destination);	
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == TOPRANKINGS){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			SDL_RenderCopy(renderer, TopRankingsMenu, NULL, &background_destination);		
			// SCORES
			for (i = 0; i < 5; i++){
				SDL_QueryTexture(Names[i], NULL, NULL, &TextW, &TextH);
				SDL_Rect Names_destination = {200, 220+50*i, TextW, TextH};
				SDL_RenderCopy(renderer, Names[i], NULL, &Names_destination);
				SDL_QueryTexture(Scores[i], NULL, NULL, &TextW, &TextH);
				SDL_Rect Scores_destination = {400, 220+50*i, TextW, TextH};
				SDL_RenderCopy(renderer, Scores[i], NULL, &Scores_destination);
				SDL_QueryTexture(Dates[i], NULL, NULL, &TextW, &TextH);
				SDL_Rect Dates_destination = {500, 220+50*i, TextW, TextH};
				SDL_RenderCopy(renderer, Dates[i], NULL, &Dates_destination);
			}
			// HOME BUTTON
			SDL_Rect texture_destination = {900-cellsize-4, 4, cellsize, cellsize};
			SDL_RenderCopy(renderer, HomeButton, NULL, &texture_destination);
			// BUTTON HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		if (state == WRITE){
			// SHADOW
			SDL_RenderCopy(renderer, Shadow, NULL, &background_destination);
			SDL_Rect background_destination = {0, 0, 900, 600};
			SDL_RenderCopy(renderer, NameMenu, NULL, &background_destination);	
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);

			SDL_QueryTexture(UserNameTexture, NULL, NULL, &TextW, &TextH);
			SDL_Rect Dates_destination = {240, 360, TextW, TextH};
			SDL_RenderCopy(renderer, UserNameTexture, NULL, &Dates_destination);
		}
		if (state == SOLOMENU){
			SDL_Rect background_destination = {0, 0, 900, 600};
			SDL_RenderCopy(renderer, SoloMenu, NULL, &background_destination);
			SDL_Rect hover_destination = {MenuBHover.x, MenuBHover.y, MenuBHover.w, MenuBHover.h};
			SDL_RenderCopy(renderer, MenuButtonHover, NULL, &hover_destination);
			// HOME BUTTON
			SDL_Rect texture_destination = {900-cellsize-4, 4, cellsize, cellsize};
			SDL_RenderCopy(renderer, HomeButton, NULL, &texture_destination);
			// BUTTON HOVER
			SDL_Rect Chover_destination = {CHover.x, CHover.y, CHover.w, CHover.h};
			SDL_RenderCopy(renderer, CircleHover, NULL, &Chover_destination);
		}
		// SHOW RENDER
        SDL_RenderPresent(renderer);
	
    }
    // Release resources
	SDL_DestroyTexture(Cell0);
	SDL_DestroyTexture(Cell1);
	SDL_DestroyTexture(Cell2);
	SDL_DestroyTexture(CellHover);
	SDL_DestroyTexture(CircleHover);
	SDL_DestroyTexture(MenuButtonHover); 
	SDL_DestroyTexture(Circle_28);
	SDL_DestroyTexture(MenuButton);
	SDL_DestroyTexture(HomeButton);
	SDL_DestroyTexture(Background);
	SDL_DestroyTexture(Shadow);
	SDL_DestroyTexture(PauseBar);
	SDL_DestroyTexture(GameOverBar);
	SDL_DestroyTexture(Bar);
	SDL_DestroyTexture(MainMenu);
	SDL_DestroyTexture(TopRankingsMenu); 
	SDL_DestroyTexture(BackgroundTime);
	SDL_DestroyTexture(NameMenu);
	SDL_DestroyTexture(SoloMenu);
	SDL_DestroyTexture(YouWon);
	SDL_DestroyTexture(BotWon);
	SDL_DestroyTexture(Win);
	SDL_DestroyTexture(timer);
	SDL_DestroyTexture(Text0);
	SDL_DestroyTexture(Text1);
	SDL_DestroyTexture(Text2);
	SDL_DestroyTexture(Text3);
	SDL_DestroyTexture(Text4);
	SDL_DestroyTexture(Best);
	SDL_DestroyTexture(UserNameTexture);
	for (i = 0; i < 5; i++){
		SDL_DestroyTexture(Names[i]);
		SDL_DestroyTexture(Scores[i]);
		SDL_DestroyTexture(Dates[i]);
	}
	for (i = 0; i < 6; i++){
		for (j = 0; j < 6; j++){
			SDL_DestroyTexture(Cells[i][j].Texture);
			SDL_DestroyTexture(BotCells[i][j].Texture);
		}
	}
	for (i = 0; i < 7; i++){
		for (j = 0; j < 7; j++){
			SDL_DestroyTexture(Circles[i][j].Texture);
		}
	}
	IMG_Quit();
    Mix_FreeMusic(backgroundSound);
    Mix_FreeChunk(click);
    Mix_CloseAudio();
	TTF_CloseFont(Font);
	TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}