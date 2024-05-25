#ifndef CLIENT_H
#define CLIENT_H

#include "manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


void affichage_artiste(WINDOW *win, Salle tableau[], int nb_salles);
void reservation_simple(WINDOW *win, Salle *salle);
int reservation(WINDOW *win, Salle tableau[], int nb_salles);
void remboursement(WINDOW *win, Salle tableau[], int nb_salles);


#endif