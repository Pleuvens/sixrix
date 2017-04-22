# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>

int showmenu(SDL_Surface* screen, TTF_Font* font){
  Uint32 time;
  int x, y;
  int num = 3;
  char* labels[num];
  labels[0] = "SALUT TOI, DIS MOI SI";
  labels[1] = "CA COMPILE BIEN SUR LES PXE";
  labels[2] = "ET RETOURNE TAFFER fufufu";
  SDL_Surface* menus[num];
  int selected[num];
  selected[0] = 0;
  selected[1] = 0;
  selected[2] = 0;
  SDL_Color color1 =  {.r = 0, .g = 0, .b = 0};
  SDL_Color color2 = {.r = 255, .g = 0, .b = 0};

  SDL_Surface* background = SDL_LoadBMP("fond.bmp");

  menus[0] = TTF_RenderText_Solid(font, labels[0], color1);
  menus[1] = TTF_RenderText_Solid(font, labels[1], color1);
  menus[2] = TTF_RenderText_Solid(font, labels[2], color1);
  SDL_Rect pos[num];
  pos[0].x = 50;
  pos[0].y = 120;
  pos[1].x = 50;
  pos[1].y = 240;
  pos[2].x = 50;
  pos[2].y = 360;

  SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));
  
  SDL_BlitSurface(background, NULL, screen, NULL);

  SDL_Event event;
  while(1){
    time = SDL_GetTicks();
    while(SDL_PollEvent(&event)){
      switch(event.type){

	case SDL_QUIT:
	  SDL_FreeSurface(menus[0]);
	  SDL_FreeSurface(menus[1]);
	  SDL_FreeSurface(menus[2]);
	  return 1;

	case SDL_MOUSEMOTION:
	  x = event.motion.x;
	  y = event.motion.y;
	  for(int i = 0; i < num; i++){
	    if(x >= pos[i].x && x <= pos[i].x+pos[i].w && y >= pos[i].y && y <= pos[i].y+pos[i].h){
	      if (!selected[i]){
		selected[i] = 1;
		SDL_FreeSurface(menus[i]);
		menus[i] = TTF_RenderText_Solid(font, labels[i], color2);
	      }
	    }
	    else{
	      if(selected[i]){
		selected[i] = 0;
		SDL_FreeSurface(menus[i]);
		menus[i] = TTF_RenderText_Solid(font, labels[i], color1);
	      }
	    }
	  }
	  break;

	case SDL_MOUSEBUTTONDOWN:
	  x = event.button.x;
	  y = event.button.y;
	  for(int i = 0; i < num; i += 1){
	    if(x>=pos[i].x && x<=pos[i].x+pos[i].w && y>=pos[i].y && y<=pos[i].y+pos[i].h){
	      SDL_FreeSurface(menus[0]);
	      SDL_FreeSurface(menus[1]);
	      SDL_FreeSurface(menus[2]);
	      return i;
	    }
	  }
	  break;

	case SDL_KEYDOWN:
	  if(event.key.keysym.sym == SDLK_ESCAPE){
	    SDL_FreeSurface(menus[0]);
	    SDL_FreeSurface(menus[1]);
	    SDL_FreeSurface(menus[2]);
	    return 0;
	  }
      }
    }
    for(int i = 0; i < num; i++){
      SDL_BlitSurface(menus[i], NULL, screen, &pos[i]);
    }
    SDL_Flip(screen);
    if(1000/30 > (SDL_GetTicks()-time))
      SDL_Delay(1000/30 - (SDL_GetTicks()-time));
  }
}


int main(){
  SDL_Surface *screen;
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  SDL_WM_SetCaption("HAPEULE", NULL);
  TTF_Font *font;
  TTF_Init();
  font = TTF_OpenFont("Test.ttf", 30);
  int i = showmenu(screen, font);
  if(i == 1)
    TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
