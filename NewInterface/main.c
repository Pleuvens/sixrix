# include <SDL/SDL.h>
# include <SDL/SDL_ttf.h>
# include <gtk/gtk.h>

char *k;
GtkBuilder *builder;
GtkWidget *choose;

int showmenu(SDL_Surface* screen, TTF_Font* font){
  Uint32 time;
  int x, y;
  int num = 4;
  char* labels[num];
  labels[0] = "SAY SOMETHING";
  labels[1] = "CHOOSE A FILE";
  labels[2] = "RUN";
  labels[3] = "EXIT";
  SDL_Surface* menus[num];
  int selected[num];
  selected[0] = 0;
  selected[1] = 0;
  selected[2] = 0;
  selected[3] = 0;
  SDL_Color color1 =  {.r = 0, .g = 0, .b = 0};
  SDL_Color color2 = {.r = 255, .g = 0, .b = 0};

  SDL_Surface* background = SDL_LoadBMP("fond.bmp");

  menus[0] = TTF_RenderText_Solid(font, labels[0], color1);
  menus[1] = TTF_RenderText_Solid(font, labels[1], color1);
  menus[2] = TTF_RenderText_Solid(font, labels[2], color1);
  menus[3] = TTF_RenderText_Solid(font, labels[3], color1);
  SDL_Rect pos[num];
  pos[0].x = 175;
  pos[0].y = 50;
  pos[1].x = 185;
  pos[1].y = 160;
  pos[2].x = 278;
  pos[2].y = 270;
  pos[3].x = 275;
  pos[3].y = 380;

  //  SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));

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
	  SDL_FreeSurface(menus[3]);
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
	      SDL_FreeSurface(menus[3]);
	      return i;
	    }
	  }
	  break;

	case SDL_KEYDOWN:
	  if(event.key.keysym.sym == SDLK_ESCAPE){
	    SDL_FreeSurface(menus[0]);
	    SDL_FreeSurface(menus[1]);
	    SDL_FreeSurface(menus[2]);
	    SDL_FreeSurface(menus[3]);
	    return 3;
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

SDL_Surface *load_image(const char *c, Uint32 colorkey)
{
  SDL_Surface *tmp = SDL_LoadBMP(c);
  if(colorkey != 0)
  {
    SDL_SetColorKey(tmp, SDL_SRCCOLORKEY, colorkey);
  }
  return tmp;
}


void on_button1_clicked(){
  const gchar* chemin;
  GtkWidget *dialog;
  chemin = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooserdialog1")));

  dialog = gtk_message_dialog_new(GTK_WINDOW(choose),
      GTK_DIALOG_MODAL,
      GTK_MESSAGE_INFO,
      GTK_BUTTONS_OK,
      "Vous avez choisi :\n%s", chemin);
  k = (char*)chemin;
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_widget_destroy(choose);
  gtk_main_quit();
}

void on_button2_clicked(){
  gtk_widget_destroy(choose);
  gtk_main_quit();
}


int main(int argc, char *argv[]){
  SDL_Surface *screen, *logo;
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  logo = load_image("logo.bmp", 0);
  SDL_WM_SetIcon(logo, NULL);
  SDL_WM_SetCaption("HAPEULE", NULL);
  TTF_Font *font;
  TTF_Init();
  font = TTF_OpenFont("Test.ttf", 40);
  int i = showmenu(screen, font);
  while(i != 3){
    if(i == 0){
      //enregistrement
    }
    if(i == 1){
      gtk_init(&argc, &argv);
      builder = gtk_builder_new();
      gtk_builder_add_from_file(builder, "newinterface.glade", NULL);
      choose = GTK_WIDGET(gtk_builder_get_object(builder, "filechooserdialog1"));
      gtk_builder_connect_signals(builder, NULL);
      gtk_widget_show(choose);
      gtk_window_set_modal(GTK_WINDOW(choose), TRUE);
      g_signal_connect(gtk_builder_get_object(builder, "button1"), "clicked", on_button1_clicked, choose);
      g_signal_connect_swapped(gtk_builder_get_object(builder, "button2"), "clicked", G_CALLBACK(gtk_widget_destroy), choose);
      gtk_main();
    }
    if(i == 2){
      //run prog
    }
    i = showmenu(screen, font);
  }
  printf("%s", k);
  SDL_FreeSurface(logo);
  SDL_FreeSurface(screen);
  TTF_Quit();
  SDL_Quit();
  return 0;
}
