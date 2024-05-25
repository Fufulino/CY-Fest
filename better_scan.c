#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "better_scan.h"

#define MAX 50


void better_scan_str(WINDOW *win, int y, int x, char *message, char *tableau, int taille){ // Better_scan pour demander une chaine de caractere (espaces, chiffres, lettres uniquement)

  int chr;
  int longueur = 0;
  int nx, ny;

  for (int i = 0; i < taille; i++) {
    tableau[i] = '\0';
  }

  mvwprintw(win, y, x, "%s", message);
  getyx(win, ny, nx);

  while (TRUE) {
    chr = wgetch(win);
    if ((chr == KEY_ENTER || chr == '\n' || chr == '\r') && longueur > 0) {
      break;
    }
    if ((chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127) && longueur > 0 ) {
      tableau[--longueur] = '\0';
      mvwaddstr(win, ny, nx+longueur, " ");
      wmove(win, ny, nx+longueur);
    }
    if (((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z') || (chr >= '0' && chr <= '9') || chr == ' ' || chr == 44) && (longueur < taille - 1)) { //a verifier
      waddch(win, chr);
      tableau[longueur++] = chr;
    }
  } 
}

int better_scan_compare(WINDOW *win, int y, int x, int min_1, int max_1, char *message){ // Better_scan pour demander uniquement des int en comparant entre une valeur (int) minimum et maximum 

  char tableau[MAX] = {0};
  int chr;
  int longueur = 0;
  int nx, ny;

  mvwprintw(win, y, x, "%s", message);
  getyx(win, ny, nx);

  while (TRUE) {
    chr = wgetch(win);
    if ((chr == KEY_ENTER || chr == '\n' || chr == '\r') && longueur > 0  && (atoi(tableau) >= min_1 && atoi(tableau) <= max_1)) {
      break;
    }
    if ((chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127) && longueur > 0 ) {
      tableau[--longueur] = '\0';
      mvwaddstr(win, ny, nx+longueur, " ");
      wmove(win, ny, nx+longueur);
    }
    if (chr >= '0' && chr <= '9' && longueur < MAX-1) {
      waddch(win, chr);
      tableau[longueur++] = chr;
    }
  }
  return atoi(tableau);
}

int better_scan_compare_f(WINDOW *win, int y, int x, int min_1, int max_1, char *message){ // Better_scan pour demander uniquement des int en comparant entre une valeur (int) minimum et maximum 

  char tableau[MAX] = {0};
  int chr;
  int longueur = 0;
  int nx, ny;

  mvwprintw(win, y, x, "%s", message);
  getyx(win, ny, nx);

  while (TRUE) {
    chr = wgetch(win);
    if ((chr == KEY_ENTER || chr == '\n' || chr == '\r') && longueur > 0  && (atoi(tableau) >= min_1 && atoi(tableau) <= max_1)) {
      break;
    }
    if ((chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127) && longueur > 0 ) {
      tableau[--longueur] = '\0';
      mvwaddstr(win, ny, nx+longueur, " ");
      wmove(win, ny, nx+longueur);
    }
    if ((chr >= '0' && chr <= '9' && longueur < MAX-1) || chr == '.') {
      waddch(win, chr);
      tableau[longueur++] = chr;
    }
  }
  return atoi(tableau);
}