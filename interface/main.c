# include <gtk/gtk.h>
# include <SDL.h>

char *k;
GdkPixbuf *pixbuf;
GtkBuilder *builder;
GtkWidget *window;
GtkWidget *choose;
GtkWidget *pload;
GtkImage *image;

void on_button4_clicked(){
  gtk_main_quit();
}

void on_button5_clicked(){
  //pre_traitement(k);
  gtk_main_quit();

}
void on_button8_clicked(){
  /*SDL_Surface *img = load_image("gtkimage.bmp");
    img = rotate(img);
    SDL_SaveBMP(img, "gtkimage.bmp");
    pixbuf = gdk_pixbuf_new_from_file("gtkimage.bmp", NULL);
    pixbuf = gdk_pixbuf_scale_simple(pixbuf, 500, 500, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(image, pixbuf);
   */gtk_main_quit();

}

void on_button6_clicked(){
  /*        SDL_Surface *img = load_image("gtkimage.bmp");
	    int **tab = noir_blanc(img);
	    float color;

	    for(int i = 0; i < img->h; i++){
	    for (int j = 0; j < img->w; j++){
	    Uint32 pixel;
	    pixel = getpixel(img, j, i);
	    Uint8 r,g,b;
	    pixel = getpixel(img,i,j);
	    SDL_GetRGB(pixel, img->format, &r, &g, &b);
	    Uint8 grey = r*0.3 + g*0.59 + b*0.11;
	    pixel = SDL_MapRGB(img->format,grey,grey,grey);
	    putpixel(img,i,j,pixel);
	    color = 0;
  //              printf("%d ", tab[i][j]);
  if(tab[i][j] == 1)
  color = 255;
  pixel = SDL_MapRGB(img->format, color, color, color);
  putpixel(img, j, i, pixel);
  }
  }
  SDL_SaveBMP(img, "gtkimage.bmp");
  pixbuf = gdk_pixbuf_new_from_file("gtkimage.bmp", NULL);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 500, 500, GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(image, pixbuf);
   */gtk_main_quit();

}

void on_button7_clicked(){
  /*	SDL_Surface *img = load_image("gtkimage.bmp");
	img = bruit(img);
	SDL_SaveBMP(img, "gtkimage.bmp");
	pixbuf = gdk_pixbuf_new_from_file("gtkimage.bmp", NULL);
	pixbuf = gdk_pixbuf_scale_simple(pixbuf, 500, 500, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(image, pixbuf);
   */
}

void on_button1_clicked(){
  /*const gchar* chemin;
    GtkWidget *dialog;
    chemin = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "filechooser")));

    dialog = gtk_message_dialog_new(GTK_WINDOW(choose),
    GTK_DIALOG_MODAL,
    GTK_MESSAGE_INFO,
    GTK_BUTTONS_OK,
    "Vous avez choisi :\n%s", chemin);
    k = (char*)chemin;
  // printf("%s ", k);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  gtk_widget_destroy(choose);
  pixbuf = gdk_pixbuf_new_from_file(k, NULL);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 500, 500, GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(image, pixbuf);
  SDL_SaveBMP(load_image(k), "gtkimage.bmp");
  char content[1000];
  FILE *f = fopen("resultat", "r");
  fscanf(f, "%[^\n]", content);
  gtk_label_set_text(GTK_LABEL(gtk_builder_get_object(builder, "label1")), content);
  fclose(f);*/
  gtk_main_quit();

}

void on_button3_clicked(){
  choose = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser"));
  gtk_widget_show(choose);
  gtk_window_set_modal(GTK_WINDOW(choose), TRUE);
  g_signal_connect(gtk_builder_get_object(builder, "button1"), "clicked", on_button1_clicked, choose);
  g_signal_connect_swapped(gtk_builder_get_object(builder, "button2"), "clicked", G_CALLBACK(gtk_widget_destroy), choose);

}


int main(int argc, char *argv[]){

  gtk_init(&argc, &argv);

  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "interface.glade", NULL);

  //choose = GTK_WIDGET(gtk_builder_get_object(builder, "filechooser"));
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
  gtk_builder_connect_signals(builder, NULL);
  //	g_object_unref(builder);
  image = GTK_IMAGE(gtk_builder_get_object(builder, "image1"));
  g_signal_connect(gtk_builder_get_object(builder, "button3"), "clicked", on_button3_clicked, NULL);
  g_signal_connect(gtk_builder_get_object(builder, "button4"), "clicked", on_button4_clicked, NULL);
  g_signal_connect(gtk_builder_get_object(builder, "button5"), "clicked", on_button5_clicked, NULL);
  pixbuf = gdk_pixbuf_new_from_file("Hapeule.png", NULL);
  pixbuf = gdk_pixbuf_scale_simple(pixbuf, 300, 250, GDK_INTERP_BILINEAR);
  gtk_image_set_from_pixbuf(image, pixbuf);

  gtk_widget_show(window);
  gtk_main();

  return 0;
}
