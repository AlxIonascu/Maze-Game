 
#pragma warning( disable : 4996 ) 

 
#include <cstdlib>
#include <vector>
#include <iostream>
#include <string>
#include "G2D.h"
 
 
using namespace std;
enum class ECRAN {ECRAN_ACCUEIL, ECRAN_OPTIONS, INIT_PARTIE, ECRAN_JEU, ECRAN_GAME_OVER,ECRAN_WELL_DONE};

ECRAN ecran = ECRAN::ECRAN_ACCUEIL;
 
struct _Heros
{
	string texture =
		"[RRR  ]"
		"[RRWR ]"
		"[RRR  ]"
		"[YY   ]"
		"[YYY  ]"
		"[YY YG]"
		"[GG   ]"
		"[CC   ]"
		"[CC   ]"
		"[C C  ]"
		"[C C  ]";
	V2 Size;
	int IdTex; 
	int Lives = 3;
	V2 Pos = V2(60,525);
	const V2 InitialPos = V2(60, 525);
};

struct _Key
{
	string texture =
		"[                      ]"
		"[     W W         WWWW ]"
		"[     W W        W    W]"
		"[   WWWWWWWWWWWWWW    W]"
  	    "[                W    W]"
		"[                 WWWW ]";


	V2 Size;
	int IdTex;
	V2 Pos = V2(440, 450);
	bool held = false;
};

 struct  _Trap
{
	string texture =
		"[  YYY  ]"
		"[ Y   Y ]"
		"[Y     Y]"
		"[ Y   Y ]"
		"[  YYY  ]";

	V2 Size;
	int IdTex;
	V2 Pos;
	bool activated = true;

};

 struct  _Chest
 {
	 string texture =
		 "[GGGGGG]"
		 "[GGGGGG]"
		 "[YYYYYY]"
		 "[GGGGGG]"
		 "[GGGGGG]";

	 V2 Size;
	 int IdTex;
	 V2 Pos = V2(210,100);
 };

 struct _Momie {
	 string texture =
		 "[  WWW  ]"
		 "[WRRWRRW]"
		 "[WRRWRRW]"
		 "[  WWW  ]"
		 "[ WWWWW ]"
		 "[WWWWWWW]"
		 "[WWWWWWW]";
	 V2 Size;
	 int IdTex;
	 V2 Pos = V2(210, 100);
	 V2 Dir = V2(-1, 0);
	 V2 Dirs[4] = {V2(0,1),V2(1,0),V2(0,-1),V2(-1,0)};
	
 };

struct GameData
{

    string Map =
				 "MMMMMMMMMMMMMMM"
				 "M M           M"
				 "M M M MMM MMM M"
				 "M   M       M M"
				 "MMM M M MMM M M"
				 "M   M M     M M"
				 "M MMM MMM MMMMM"
				 "M   M  M      M"
				 "M M M  M M MM M"
				 "M M M  M M M  M"
				 "M M M MM M MMMM"
				 "M M M    M    M"
				 "M M M MMMMMMM M"
				 "M M      M    M"
				 "MMMMMMMMMMMMMMM";

	// indique la présence d'un mur à la case (x,y)
	bool Mur(int x, int y) { return Map[(15 - y - 1)*15+x] == 'M'; }  

	int Lpix = 40;  // largeur en pixels des cases du labyrinthe

	_Heros Heros;   // data du héros
	_Key   Key;
	_Chest Chest;
	std::vector<_Trap>  traps;
	std::vector<_Momie> momies;
	int nbMomies=3;
	V2 initPos[5] = { V2(50,50),V2(420,50),V2(530,380),V2(500,210)};
	GameData() {}
};

GameData G;
double timer;

bool intersect(V2 Pos1, V2 Size1, V2 Pos2, V2 Size2) {
	if (Pos1.y + Size1.y < Pos2.y) return false;  // (1)
	if (Pos1.y > Pos2.y + Size2.y)   return false;  // (2)
	if (Pos1.x > Pos2.x + Size2.x)   return false;  // (3)
	if (Pos1.x + Size1.x < Pos2.x)   return false;  // (4)

	return true;
}

void render()
{	
	G2D::ClearScreen(Color::Black);


	if ((ecran == ECRAN::ECRAN_GAME_OVER) && (timer > 0)) {
		G2D::DrawStringFontMono(V2(120, 400), string(" GAME OVER"), 50, 5, Color::Red);
		timer -= G2D::ElapsedTimeFromLastCallbackSeconds();
	}

	if ((ecran == ECRAN::ECRAN_WELL_DONE) && (timer > 0)) {
		G2D::DrawStringFontMono(V2(120, 400), string(" WELL DONE"), 50, 5, Color::Green);
		timer -= G2D::ElapsedTimeFromLastCallbackSeconds();
	}
	
	if (ecran == ECRAN::ECRAN_OPTIONS) {
		G2D:: DrawRectangle(V2 (100,200), V2 (400,50), Color::Green, true);
		G2D:: DrawRectangle(V2(100, 300), V2 (400,50), Color::Red, true);
		G2D::DrawStringFontMono(V2 (105,215), std::string("Press DOWN for less mummies"), 20,  3,  Color::Black);
		G2D::DrawStringFontMono(V2 (105,315), std::string(" Press UP for more mummies"), 20, 3, Color::Black);
		G2D::DrawStringFontMono(V2 (185,505), std::string(" Amount of mummies: "+ to_string(G.nbMomies)), 20, 3, Color::Yellow);
		G2D::DrawStringFontMono(V2(20, 20), std::string(" Press ENTER to exit screen"), 15, 3, Color::Yellow);
	}

	if (ecran == ECRAN::ECRAN_JEU) {
		for (int x = 0; x < 15; x++)
			for (int y = 0; y < 15; y++)
			{
				int xx = x * G.Lpix;
				int yy = y * G.Lpix;
				if (G.Mur(x, y))
					G2D::DrawRectangle(V2(xx, yy), V2(G.Lpix, G.Lpix), Color::Blue, true);
			}
		G2D::DrawRectWithTexture(G.Heros.IdTex, G.Heros.Pos, G.Heros.Size);
		if(G.Key.held==false)
			G2D::DrawRectWithTexture(G.Key.IdTex, G.Key.Pos, G.Key.Size);
		G2D::DrawRectWithTexture(G.Chest.IdTex, G.Chest.Pos, G.Chest.Size);
		for (int i = 0; i < G.traps.size(); i++) {
			if (G.traps[i].activated) {
				G2D::DrawRectWithTexture(G.traps[i].IdTex, G.traps[i].Pos, G.traps[i].Size);
			}
		}
		G2D::DrawStringFontMono(V2(10, 10), std::string("Lives: " + std::to_string(G.Heros.Lives)), 30, 5, Color::Red);
		for (int i = 0; i < G.momies.size(); i++)
			G2D::DrawRectWithTexture(G.momies[i].IdTex, G.momies[i].Pos, G.momies[i].Size);
	}

	if (ecran == ECRAN::ECRAN_ACCUEIL) {
		G2D:: DrawRectangle(V2 (150,200), V2 (300,50), Color::Green, true);
		G2D:: DrawRectangle(V2(150, 300), V2 (300,50), Color::Gray, true);
		G2D::DrawStringFontMono(V2 (155,215), std::string("   Press P to play"), 20,  3,  Color::Black);
		G2D::DrawStringFontMono(V2 (155,315), std::string(" Press S for settings"), 20, 3, Color::Black);
		G2D::DrawStringFontMono(V2 (85,405), std::string("The amazing MAZE!"), 40, 3, Color::Yellow);
	}
	G2D::Show();
}


bool posIsValid(V2 p,  V2 Size, V2 dir) {
	V2 pos = p + dir;
	int x = pos.x;
	int y = pos.y;
	bool bl = !(G.Mur((x+1)  / G.Lpix, (y+1) / G.Lpix));
	bool tl = !(G.Mur((x+1 ) / G.Lpix, (y + Size.y-1) / G.Lpix));
	bool tr = !(G.Mur((x-1 + Size.x ) / G.Lpix, (y + Size.y -1) / G.Lpix));
	bool br = !(G.Mur((x+Size.x-1 ) / G.Lpix, (y+1) / G.Lpix));
	
	if (dir.x > 0) {
		return tr && br ;
	}
	if (dir.x < 0) {
		return tl && bl;
	}
	if (dir.y > 0) {
		return tl && tr;
	}
	if (dir.y < 0) {
		return bl && br;
	}
}

ECRAN gestion_jeu() {

	V2 NewHeroPos = G.Heros.Pos;
	if (G2D::IsKeyPressed(Key::LEFT) && posIsValid(NewHeroPos,G.Heros.Size,V2(-1,0))){
		G.Heros.Pos.x--;
	}
	if (G2D::IsKeyPressed(Key::RIGHT) && posIsValid(NewHeroPos, G.Heros.Size,V2(1,0))) {
		G.Heros.Pos.x++;
	}
	if (G2D::IsKeyPressed(Key::UP) && posIsValid(NewHeroPos, G.Heros.Size,V2 (0,1))) {
	    G.Heros.Pos.y++;
	}
	if (G2D::IsKeyPressed(Key::DOWN) && posIsValid(NewHeroPos, G.Heros.Size,V2(0,-1))) {
		G.Heros.Pos.y--;
	}

	for (int i = 0; i < G.momies.size(); i++) {
		V2 NewPos = G.momies[i].Pos + G.momies[i].Dir;
		if (posIsValid(NewPos, G.momies[i].Size, G.momies[i].Dir)) {
			G.momies[i].Pos = NewPos;
		}
		else {
			G.momies[i].Dir = G.momies[i].Dirs[rand() % 4];
		}
		if (intersect(G.momies[i].Pos, G.momies[i].Size, G.Heros.Pos, G.Heros.Size)) {
			G.Heros.Lives = 0;
		}
	}

	for (int i = 0; i < G.traps.size(); i++) {
		if (intersect(G.traps[i].Pos, G.traps[i].Size, G.Heros.Pos, G.Heros.Size) && G.traps[i].activated) {
			if (G.Heros.Lives > 0) {
				G.Heros.Lives--;
				G.traps[i].activated = false;
				G.Heros.Pos = G.Heros.InitialPos;
			}
		}
	}

	if (intersect(G.Key.Pos, G.Key.Size, G.Heros.Pos, G.Heros.Size) && G2D::IsKeyPressed(Key::ENTER) && G.Key.held == false) {
			G.Key.held = true;
	}
	if (G.Key.held == true && intersect(G.Chest.Pos, G.Chest.Size, G.Heros.Pos, G.Heros.Size) == true && G2D::IsKeyPressed(Key::ENTER)) {
		return ECRAN::ECRAN_WELL_DONE;
	}

	if (G.Heros.Lives == 0) {
			return ECRAN::ECRAN_GAME_OVER;
	}
	return ECRAN::ECRAN_JEU;
}

ECRAN gestion_GameOver(){
	if (timer <= 0.0) {
		timer = 3.0;
		G.Heros.Lives = 3;
		return ECRAN::ECRAN_ACCUEIL;
	}
	else
		return ECRAN::ECRAN_GAME_OVER;
}

ECRAN gestion_WellDone() {
	if (timer <= 0.0) {
		timer = 3.0;
		G.Heros.Lives = 3;
		return ECRAN::ECRAN_ACCUEIL;
	}
	else
		return ECRAN::ECRAN_WELL_DONE;
}

ECRAN gestion_ecran_accueil() {
	if (G2D::IsKeyPressed(Key::P)){		
		return ECRAN::INIT_PARTIE;
	}
	if (G2D::IsKeyPressed(Key::S)) {
		return ECRAN::ECRAN_OPTIONS;
	}
	else return ECRAN::ECRAN_ACCUEIL;
}

ECRAN InitPartie() {
	G.Heros.Lives = 3;
	for (int i = 0; i < G.traps.size(); i++) {
		G.traps[i].activated = true;
	}

	G.Heros.Pos = G.Heros.InitialPos;
	timer = 3.0;
	G.Key.held = false;
	G.momies.clear();
	for (int i = 0; i < G.nbMomies; i++) {
		_Momie m;
		m.Pos = G.initPos[rand()%4];
		m.IdTex = G2D::InitTextureFromString(m.Size, m.texture);
		m.Size = m.Size * 3;
		G.momies.push_back(m);

	}
	return ECRAN::ECRAN_JEU;
}

ECRAN gestion_ecran_options() {
	if (G2D::IsKeyPressed(Key::UP) && G.nbMomies<10) {
		G.nbMomies++;
	}
	if (G2D::IsKeyPressed(Key::DOWN) && G.nbMomies>1) {
		G.nbMomies--;
	}
	if (G2D::IsKeyPressed(Key::ENTER)) {
		return ECRAN::ECRAN_ACCUEIL;
	}
	_sleep(100);
	return ECRAN::ECRAN_OPTIONS;
	
	
}


void Logic()
{
	if (ecran == ECRAN::ECRAN_ACCUEIL)
		ecran = gestion_ecran_accueil();

	if (ecran == ECRAN::ECRAN_OPTIONS)
		ecran = gestion_ecran_options();
		
	if (ecran == ECRAN::INIT_PARTIE)
		ecran = InitPartie();
	
	if (ecran == ECRAN::ECRAN_JEU)
		ecran = gestion_jeu();
	if (ecran == ECRAN::ECRAN_GAME_OVER)
		ecran = gestion_GameOver();
	if (ecran == ECRAN::ECRAN_WELL_DONE)
		ecran = gestion_WellDone();
}

void AssetsInit()
{
	// Size passé en ref et texture en param
	G.Heros.IdTex = G2D::InitTextureFromString(G.Heros.Size, G.Heros.texture);
	G.Heros.Size = G.Heros.Size * 2; // on peut zoomer la taille du sprite

	G.Key.IdTex = G2D::InitTextureFromString(G.Key.Size, G.Key.texture);
	G.Key.Size = G.Key.Size * 1.5; // on peut zoomer la taille du sprite

	//3 traps: fixed
	_Trap t;

	t.IdTex = G2D::InitTextureFromString(t.Size, t.texture);
	t.Size = t.Size * 3;
	t.Pos = V2(125, 200);
	G.traps.push_back(t);

	t.IdTex = G2D::InitTextureFromString(t.Size, t.texture);
	t.Size = t.Size * 3;
	t.Pos = V2(230, 300);
	G.traps.push_back(t);

	t.IdTex = G2D::InitTextureFromString(t.Size, t.texture);
	t.Size = t.Size * 3;
	t.Pos = V2(350, 450);
	G.traps.push_back(t);

	G.Chest.IdTex = G2D::InitTextureFromString(G.Chest.Size, G.Chest.texture);
	G.Chest.Size = G.Chest.Size * 3;
}

int main(int argc, char* argv[])
{ 

	G2D::InitWindow(argc,argv,V2(G.Lpix * 15, G.Lpix * 15), V2(200,200), string("Labyrinthe"));
	  
	AssetsInit();

	G2D::Run(Logic,render);
 
}
 
  
 