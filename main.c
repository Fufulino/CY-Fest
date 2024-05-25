#include "manager.h"
#include "client.h"
#include "better_scan.h"
#include "mode.h"

#define MAX 50
#define SIZE 3


int main(){

  initscr(); // Initialiser l'écran ncurses
  cbreak();  // Désactiver la mise en mémoire tampon de ligne
  noecho();  // Désactiver l'affichage des caractères saisis

  // Permet de vider les fichiers à chaque initialisation
  FILE *fichier;
  fichier = fopen("nb_salles.bin", "wb");
  fclose(fichier);
  fichier = fopen("Salles.bin", "wb");
  fclose(fichier);

  Salle *tableau = NULL;
  WINDOW *win = newwin(300, 300, 0, 0); // Hauteur, largeur, début_y, début_x Créer une nouvelle fenêtre
  keypad(win, TRUE); // Permet d'avoir les flèches

  int nb_salles = 0;
  // Initialise un tableau de type Salle
  tableau = malloc(MAX*(sizeof(Salle)));

  mvwprintw(win, 1, 0, "Utiliser les flèches directionnelles pour déplacer le curseur du menu (ou les touches z et s) ");
  mvwprintw(win, 2, 0, "Si vous inscrivez une valeur et que la touche entrée ne marche pas, effacez et rééecrivez une valeur comprise dans l'intervalle");
  mvwprintw(win, 4, 0, "Appuyez sur une touche pour accéder au menu...");
  wgetch(win);
  werase(win);

  while(1){
    
    int options = optionsMenu(win); // Récupère la position du curseur
    werase(win);

    if(options == 0){
      if(mdp(win)!=0){
        werase(win);
        // Restaure le fichier
        restaurer(&tableau, &nb_salles);
        // Appelle la fonction "mode_manager" pour accéder à l'interface du Manager
        mode_manager(win, tableau, &nb_salles);
        // Sauvegarde le fichier
        sauvegarde(tableau, &nb_salles);
        werase(win);
      }
    }

    else if(options == 1){
      // Restaure le fichier
      restaurer(&tableau, &nb_salles);
      if(nb_salles != 0){
        // Appelle la fonction "mode_festivalier" pour accéder à l'interface du Client
        mode_festivalier(win, tableau, &nb_salles);
        // Sauvegarde le fichier
        sauvegarde(tableau, &nb_salles);
        werase(win);
      }
      else{
        mvwprintw(win, 1, 0, "Aucunes salles disponibles ! ");
        mvwprintw(win, 2, 0, "Appuyez sur une touche pour revenir au menu...");
        wgetch(win);
      }
    }

    else{
      break;
    }
  }

  // Libère la mémoire 
  free(tableau);
  // Ferme la fenetre
  endwin();

  return 0;
}