#include "better_scan.h"
#include "client.h"
#include "manager.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 3
#define SIZE_2 5
#define SIZE_3 3


void sauvegarde(Salle tableau[], int *nb_salles){ // Permet de sauvegarder les salles dans un fichier binaire

  FILE *fichier = NULL;
  fichier = fopen("Salles.bin", "wb");
  if (fichier == NULL) {
    exit(1);
  }
  fwrite(tableau, sizeof(Salle), *nb_salles, fichier);
  fclose(fichier);

  fichier = fopen("nb_salles.bin", "wb");
  if (fichier == NULL) {
    exit(1);
  }
  fwrite(nb_salles, sizeof(int), 1, fichier);
  fclose(fichier);
}


void restaurer(Salle **tableau, int *nb_salles){ // Permet de restaurer les salles depuis un fichier binaire

  FILE *fichier = NULL;
  // rb pour lire le fichier
  fichier = fopen("nb_salles.bin", "rb");
  if (fichier == NULL) {
    exit(1);
  }
  fread(nb_salles, sizeof(int), 1, fichier);
  fclose(fichier);

  fichier = fopen("Salles.bin", "rb");
  if (fichier == NULL) {
    exit(1);
  }
  fread(*tableau, sizeof(Salle), *nb_salles, fichier);
  fclose(fichier);
}

int mdp(WINDOW *win) { // Vérifie le mot de passe pour le mode manager

  char mdp[MAX] = "test";
  char mdp_trouve[MAX];
  int r, a;
  int k = 0;
  int nb_c = 3;
  do {
    werase(win);
    mvwprintw(win, 2, 0, "Il vous reste %d essai(s) ", nb_c);
    better_scan_str(win, 1, 0,"Entrer le mot de passe (mdp = test) : ", mdp_trouve, MAX);
    a = strlen(mdp_trouve);
    r = 1;
    for (int i = 0; i < a; i++){
      if (mdp[i] != mdp_trouve[i]){
        r = 0;
      }
    }
    k++;
    if (k >= 3){
      return 0;
    }
    nb_c--;
  }while (r == 0 || ((int)strlen(mdp) != a));
  return 1;
}

void afficheMenu(WINDOW *win, char menu[SIZE][50], int curseur) { // Permet d'afficher le menu d'acceuil
  int i = 0, y = 5, x = 5;
  for (i = 0; i < SIZE; i++) {
    if (i != curseur) {
      mvwprintw(win, y + i, x, "%s", menu[i]);

    } else {
      mvwprintw(win, y + i, x, "> %s <", menu[i]);
    }
  }
}

char mouvementMenu(WINDOW *win, int *curseur) { // Gère le déplacement sur le menu d'acceuil
  int car = wgetch(win);
  switch (car) {
  case 'z':
  case KEY_UP: // z ou flèche du haut fait monter
      if (*curseur > 0) {
      (*curseur)--;
    }
    break;
  case 's': // s ou flèche du bas fait descendre
  case KEY_DOWN:
    if (*curseur < SIZE - 1) {
      (*curseur)++;
    }
  }
  return car;
}

int optionsMenu(WINDOW *win) { // Appelle les fonctions afficheMenu et mouvementMenu et les coordonnes

  char menu[SIZE][50] = {"Manager", "Client", "Exit"}; // Liste des messages à afficher
  int k = 0;
  werase(win);
  afficheMenu(win, menu, k);
  while (mouvementMenu(win, &k) != '\n') {
    werase(win);
    afficheMenu(win, menu, k);
  }
  return k;
}

void afficheMenu_M(WINDOW *win, char menu[SIZE_2][50], int curseur) { // Permet d'afficher le menu du manager
  int i = 0, y = 5, x = 5;
  for (i = 0; i < SIZE_2; i++) {
    if (i != curseur) {
      mvwprintw(win, y + i, x, "%s", menu[i]);

    } else {
      mvwprintw(win, y + i, x, "> %s <", menu[i]);
    }
  }
}

char mouvementMenu_M(WINDOW *win, int *curseur) { // Gère le déplacement sur le menu du manager
  int car = wgetch(win);
  switch (car) {
  case 'z':
  case KEY_UP:
    if (*curseur > 0) {
      (*curseur)--;
    }
    break;
  case 's':
  case KEY_DOWN:
    if (*curseur < SIZE_2 - 1) {
      (*curseur)++;
    }
  }
  return car;
}

int optionsMenu_M(WINDOW *win) { // Appelle les fonctions afficheMenu_M et mouvementMenu_M et les coordonnes
  char menu[SIZE_2][50] = {"Construire une salle", "Modifier une salle","Afficher information salle", "Afficher tribune","Exit"}; // Liste des messages à afficher
  int k = 0;
  werase(win);
  afficheMenu_M(win, menu, k);
  while (mouvementMenu_M(win, &k) != '\n') {
    werase(win);
    afficheMenu_M(win, menu, k);
  }
  return k;
}

void afficheMenu_C(WINDOW *win, char menu[SIZE_3][50], int curseur) { //permet d'afficher le menu du client
  int i = 0, y = 5, x = 5;
  for (i = 0; i < SIZE_3; i++) {
    if (i != curseur) {
      mvwprintw(win, y + i, x, "%s", menu[i]);

    } else {
      mvwprintw(win, y + i, x, "> %s <", menu[i]);
    }
  }
}

char mouvementMenu_C(WINDOW *win, int *curseur) { //gère le déplacement sur le menu du client
  int car = wgetch(win);
  switch (car) {
  case 'z':
  case KEY_UP:
    if (*curseur > 0) {
      (*curseur)--;
    }
    break;
  case 's':
  case KEY_DOWN:
    if (*curseur < SIZE_3 - 1) {
      (*curseur)++;
    }
  }
  return car;
}

int optionsMenu_C(WINDOW *win){ //appelle les fonctions afficheMenu_C et mouvementMenu_C et les coordonnes
  char menu[SIZE_3][50] = {"Réserver une place", "Remboursement", "Exit"}; //liste des messages à afficher
  int k = 0;
  werase(win);
  afficheMenu_C(win, menu, k);
  while (mouvementMenu_C(win, &k) != '\n') {
    werase(win);
    afficheMenu_C(win, menu, k);
  }
  return k;
}


void mode_manager(WINDOW *win, Salle tableau[], int *nb_salles) { // Gère le mode manager
  int temp, option, modif;
  while (1) {
    temp = *nb_salles;
    option = optionsMenu_M(win); // Permet de récupérer l'emplacement sélectionnée et d'afficher le menu manager
    switch (option) {
    case 0:
      if (*nb_salles == 0) { // Si il n'y a pas de salles on les crées
        werase(win);
        *nb_salles = better_scan_compare(win, 1, 0, 0, 10, "Entrez le nombre de salles que vous voulez créer (max 10): "); // Vérifie que la valeur saisie est entre 1 et 10, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
        werase(win);
      } else { // Sinon on les ajoutes
        werase(win);
        *nb_salles += better_scan_compare(win, 1, 0, 0, 10, "Entrez le nombre de salles que vous voulez ajouter (max 10): "); // Vérifie que la valeur saisie est entre 1 et 10, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
        werase(win);
      }
      for (int i = temp; i < *nb_salles; i++) { // Création du nombre de salles et de concerts saisis
        tableau[i] = constructeur_salle(win);
        constructeur_concert(&tableau[i], win);
      }

      break;
    case 1:
    if(*nb_salles != 0){
      do {
        werase(win);
        mvwprintw(win, 1, 0, "Choisissez la salle à modifier, sélectionnez une valeur entre 1 et %d : ", *nb_salles); 
        modif = better_scan_compare(win, 1, 73, 1, *nb_salles, "") - 1;// Vérifie que la valeur saisie est entre 1 et nb_salles, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      } while (modif < 0);
  

      werase(win);
      if (modif < *nb_salles) { // Vérification valeur correct
        tableau[modif] = constructeur_salle(win); // Modification de la salle
        constructeur_concert(&tableau[modif], win); // Modification du concert
        break;
      } else {
        mvwprintw(win, 1, 0, "Ce numéro de la salle n'existe pas ");
        mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
        wgetch(win);
        werase(win);
      }
      break;
    }
    else{
      werase(win);
      mvwprintw(win, 1, 0, "Aucunes salles disponibles");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer");
      wgetch(win);
      werase(win);
      break;
    }
    case 2:
    if(*nb_salles != 0){
      do {
        werase(win);
        mvwprintw(win, 1, 0, "Choisissez la salle à afficher, sélectionnez une valeur entre 1 et %d :", *nb_salles);  
        modif =  better_scan_compare(win, 1, 74, 1, *nb_salles, "") - 1; // Vérifie que la valeur saisie est entre 1 et nb_salles, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      } while (modif < 0);
      werase(win);
      if (modif < *nb_salles) { // Vérification valeur correct
        werase(win);
        affichage_donnees_manager(win, tableau[modif].concert); // Affichage données
        wgetch(win);
      } else {
        mvwprintw(win, 1, 0, "Ce numéro de la salle n'existe pas ");
        mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
        wgetch(win);
        werase(win);
      }
      break;
    }
    else{
      werase(win);
      mvwprintw(win, 1, 0, "Aucunes salles disponibles");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer");
      wgetch(win);
      werase(win);
      break;
    }
    case 3:
    if(*nb_salles != 0){
      do {
        werase(win);
        mvwprintw(win, 1, 0, "Choisissez la tribune à afficher : sélectionnez une valeur entre 1 et %d :", *nb_salles); // Vérifie que la valeur saisie est entre 1 et nb_salles, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
        modif = better_scan_compare(win, 1, 76, 1, *nb_salles, "") - 1;
      } while (modif < 0);
      werase(win);
      if (modif < *nb_salles) { // Vérification valeur correct
        werase(win);
        affichage_tribune(win, &tableau[modif]); // Affichage tribune
        wgetch(win);
      } else {
        mvwprintw(win, 1, 0, "Ce numéro de la salle n'existe pas ");
        mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
        wgetch(win);
        werase(win);
      }
      break;
    }
    else{
      werase(win);
      mvwprintw(win, 1, 0, "Aucunes salles disponibles");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer");
      wgetch(win);
      werase(win);
      break;
    }
    case 4:
      return; // Quitte le mode manager
    }
  }
}

void mode_festivalier(WINDOW *win, Salle tableau[], int *nb_salles) {// Gère le mode client
  int option;
  while (1) {
    option = optionsMenu_C(win); // Permet de récupérer l'emplacement sélectionnée et d'afficher le menu client
    switch (option){
      case 0 :
      affichage_artiste(win, tableau, *nb_salles); // Affiche les artistes
      wgetch(win);
      werase(win);
      int nv_a = reservation(win ,tableau, *nb_salles); // Aermet au client de réserver 
      werase(win);
      affichage_tribune(win, &tableau[nv_a]); // Affiche la tribune et sa place réservée
      wgetch(win);
      werase(win);
      break;
    case 1 :
      werase(win);
      remboursement(win, tableau, *nb_salles); // Permet au client de se faire rembourser
      break;
    case 2:
      return; // Quitte le mode client
    }
  }
}