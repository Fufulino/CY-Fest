#ifndef MODE_H
#define MODE_H

#include "manager.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3
#define SIZE_2 5
#define SIZE_3 2


void sauvegarde(Salle tableau[], int *nb_salles);
void restaurer(Salle **tableau, int *nb_salles);
int mdp(WINDOW *win);
void afficheMenu(WINDOW *win, char menu[SIZE][50], int curseur);
char mouvementMenu(WINDOW *win, int *curseur);
int optionsMenu(WINDOW *win);
int optionsMenu_M(WINDOW *win);
void afficheMenu_M(WINDOW *win, char menu[SIZE_2][50], int curseur);
char mouvementMenu_M(WINDOW *win, int *curseur);
void afficheMenu_C(WINDOW *win, char menu[SIZE_3][50], int curseur);
char mouvementMenu_C(WINDOW *win, int *curseur);
int optionsMenu_C(WINDOW *win);
int mode_manager(WINDOW *win, Salle tableau[], int *nb_salles);
void mode_festivalier(WINDOW *win, Salle tableau[], int *nb_salles);


#endif