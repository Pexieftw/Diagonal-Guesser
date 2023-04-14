
static const int width = 900;
static const int height = 600;
enum LButtonSprite{MAINMENU, SINGLEPLAYER, SOLOMENU, BOT, PLAYERVSBOT, GAMEOVER, GAMEOVERBOT, MENU, TOPRANKINGS, WRITE, WINSCREEN} state;
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

int RandomNumber(int nr_min, int nr_max);
int RandomColor();
SDL_Texture *initialize_texture_from_file(const char* file_name, SDL_Renderer *renderer);
SDL_Texture *initialize_texture_for_text(TTF_Font *Font, SDL_Color color, const char* Text, SDL_Renderer *renderer);
void generate_set(int n, int (*Solution)[n]);
void show_set(int n, int (*Tab)[n]);
void create_numbers(int n, int m, int (*Mat)[n], int(*Pos)[m]);
void copy_set(int n, int (*Mat)[n], int (*New)[n]);
bool is_set_zero(int n, int (*Mat)[n]);
bool is_same_set(int n, int (*Mat)[n], int (*Mat2)[n]);
Coordinates Next_State0(struct Cell (*Cells)[6]);
Player* get_scores(Player Tab[]);
int leaderboard_check(Player Tab2[], int score);
Player* update_PlayersArray(Player TopPlayers[],Player User);
Player* update_scores(Player Tab[]);
Hoverer set_hoverer_to_zero(struct Hoverer hover);
void initialize_set(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], struct Cell (*Cells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize);
void initialize_player_vs_bot(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], int (*BotTemp)[7], struct Cell (*Cells)[6], struct Cell (*BotCells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize);
void initialize_save(int (*Solution)[6], int (*Numbers)[7], int (*UserTemp)[7], struct Cell (*Cells)[6], struct Circle (*Circles)[7], SDL_Renderer *renderer, SDL_Texture *Cell0, SDL_Texture *Cell1, SDL_Texture *Cell2, SDL_Texture *Text0, SDL_Texture *Text1, SDL_Texture *Text2, SDL_Texture *Text3, SDL_Texture *Text4, int n, int (*UserStates)[n], int cellsize);
void update_save(int n, int m, int (*Solution)[n], int (*UserStates)[n], int(*UserTemp)[m], unsigned int Time);
void PutNull(Cell (*Cells)[6], Cell (*BotCells)[6], Circle (*Circles)[7], SDL_Texture *Names[], SDL_Texture *Scores[], SDL_Texture *Dates[]);
int read_save(int n, int m, int (*Solution)[n], int (*UserStates)[n], int(*UserTemp)[m]);