#include "client.h"
#include "better_scan.h"
#include "manager.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50
#define TEST 1

void affichage_artiste(WINDOW *win, Salle tableau[], int nb_salles) { // Affiche les artistes de toutes les salles crées

  werase(win);
  mvwprintw(win, 1, 0, "Liste des artistes : ");

  int comp = 0; // Permet d'afficher la dernière phrase à la bonne ligne

  for (int i = 0; i < nb_salles; i++) {
    mvwprintw(win, i + 3, 0, "%d : %s", i + 1, tableau[i].concert.artiste);
    comp++;
  }

  mvwprintw(win, comp + 5, 0, "Presser une touche pour accéder a la liste ");
}

void reservation_simple(WINDOW *win, Salle *salle) { // Lancé par la fonction réservation

  int i, j;
  float somme = 0.0;
  int etudiant;
  char texte_choix[MAX] = {0};
  char choix_categorie = 'R';

  while (choix_categorie != 'A' && choix_categorie != 'B' &&
         choix_categorie != 'C' && choix_categorie != 'a' &&
         choix_categorie != 'b' && choix_categorie != 'c') { // Vérifie que la catégorie choisie existe
    better_scan_str(win, 1, 0, "Entrez la catégorie que vous souhaitez réserver (a ou A, b ou B, c ou C): ", texte_choix, MAX);
    choix_categorie = texte_choix[0];
    werase(win);
  }

  etudiant = better_scan_compare(win, 1, 0, 0, 1, "Etes-vous un étudiant ? (1 pour oui, 0 pour non) : "); // Etudiant réduit le prix de 20%

  if (choix_categorie == 'A' || choix_categorie == 'a') { // Réserve une place dans la catégorie A
    if (salle->concert.categorieA.cat_rangees == 0) {
      mvwprintw(win, 1, 0, "La catégorie A n'existe pas, vous ne pouvez pas réserver de siège ! "); // Vérifie qu'il y a des sièges pour la catégorie A
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      werase(win);
      mvwprintw(win, 1, 0, "Saisissez un nombre compris entre 1 et %d pour les rangées : ", salle->concert.categorieA.cat_rangees); 
      mvwprintw(win, 2, 0, "Saisissez un nombre compris entre 1 et %d pour les colonnes : ", salle->nb_sieges); 
      i = better_scan_compare(win, 3, 0, 1, salle->concert.categorieA.cat_rangees, "Saisissez la rangée que vous souhaitez réserver : ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de rangées de A, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      j = better_scan_compare(win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaitez réserver : ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      while(salle->concert.categorieA.tableau[i][j] == 1){
        werase(win);
        mvwprintw(win, 1, 0, "                                                ");
        i = better_scan_compare(win, 3, 0, 1, salle->concert.categorieA.cat_rangees,
          "Saisissez la rangée que vous souhaiter réserver : ") - 1; // Vérifie que la valeur saisie est correcte
        j = better_scan_compare( win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaiter réserver : ") - 1;// Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      }
      salle->concert.categorieA.tableau[i][j] = 1; // 0 place non-réservée, 1 place réservée
      if (etudiant) {
        somme += 0.8 * salle->concert.categorieA.prix; // Réduction étudiante
      } else {
        somme += salle->concert.categorieA.prix;
      }
    }
  } else if (choix_categorie == 'B' || choix_categorie == 'b') { // Réserve une place dans la catégorie B
    if (salle->concert.categorieB.cat_rangees == 0) {
      mvwprintw(win, 1, 0, "La catégorie B n'existe pas, vous ne pouvez pas réserver de siège ! "); // Rérifie qu'il y a des sièges pour la catégorie B
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      werase(win);
      mvwprintw(win, 1, 0, "Saisissez un nombre compris entre %d et %d pour les rangées : ", salle->concert.categorieA.cat_rangees + 1, salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees);

      /*
      Vérifie que la valeur saisie est entre le nombre de rangées de A + 1 et le nombre de rangées de A + B
      Exemple :
      rangéesA = 3
      rangéesB = 2
      vérifie que la valeur est entre 4 et 5 (inclus)
      */

      mvwprintw(win, 2, 0, "Saisissez un nombre compris entre 1 et %d pour les colonnes : ", salle->nb_sieges);
      i = better_scan_compare(win, 3, 0, salle->concert.categorieA.cat_rangees + 1, salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees, "Saisissez la rangée que vous souhaitez réserver : ") - 1; // Vérifie que la valeur saisie est correcte
      j = better_scan_compare(win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaitez réserver : ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      while(salle->concert.categorieB.tableau[i - salle->concert.categorieA.cat_rangees][j] == 1){
        werase(win);
        mvwprintw(win, 1, 0, "                                                ");
        i = better_scan_compare(win, 3, 0, 
          salle->concert.categorieA.cat_rangees  + 1,
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees,
          "Saisissez la rangée que vous souhaiter réserver : ") - 1; // Vérifie que la valeur saisie est correcte
        j = better_scan_compare( win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaiter réserver : ") - 1;// Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      }
      salle->concert.categorieB.tableau[i - salle->concert.categorieA.cat_rangees][j] = 1; // 0 place non-réservée, 1 place réservée

      /*
      Chaque catégorie possède un tableau pour les réservations, la rangée x de la salle ne correspon pas à
      la rangée x du tableau de la catégorie B, il faut donc calculer quelle va être sa valeur :
      rangée_Tableau = nombre_rangée_A + rangée_B
      => rangéeB = rangée_Tableau - nombre_rangée_A
      Exemple : 
      rangée_Tableau = 8
      nombre_rangée_A = 5
      rangéeB = rangée_Tableau - nombre_rangée_A
      rangéeB = 8 - 5 = 3
      */

      if (etudiant) {
        somme += 0.8 * salle->concert.categorieB.prix; // Réduction étudiante
      } else {
        somme += salle->concert.categorieB.prix;
      }
    }
  } else {
    if (salle->concert.categorieC.cat_rangees == 0) {
      mvwprintw(win, 1, 0, "La catégorie C n'existe pas, vous ne pouvez pas réserver de siège ! "); // Vérifie qu'il y a des sièges pour la catégorie C
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      werase(win);
      mvwprintw(win, 1, 0, "Saisissez un nombre compris entre %d et %d pour les rangées : ",
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + 1,
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + salle->concert.categorieC.cat_rangees);

      /*
      Vérifie que la valeur saisie est entre le nombre de rangées de A + B + 1 et le nombre de rangées de A + B + C
      Exemple :
      rangéesA = 3
      rangéesB = 2
      rangéesC = 4
      vérifie que la valeur est entre 6 et 9 (inclus)
      */

      mvwprintw(win, 2, 0, "Saisissez un nombre compris entre 1 et %d pour les colonnes : ", salle->nb_sieges);
      i = better_scan_compare(win, 3, 0, 
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + 1,
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + salle->concert.categorieC.cat_rangees,
          "Saisissez la rangée que vous souhaiter réserver : ") - 1; // Vérifie que la valeur saisie est correcte
      j = better_scan_compare( win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaiter réserver : ") - 1;// Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      while(salle->concert.categorieC.tableau[i - salle->concert.categorieA.cat_rangees -salle->concert.categorieB.cat_rangees][j] == 1){
        werase(win);
        mvwprintw(win, 1, 0, "                                                ");
        i = better_scan_compare(win, 3, 0, 
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + 1,
          salle->concert.categorieA.cat_rangees + salle->concert.categorieB.cat_rangees + salle->concert.categorieC.cat_rangees,
          "Saisissez la rangée que vous souhaiter réserver : ") - 1; // Vérifie que la valeur saisie est correcte
        j = better_scan_compare( win, 4, 0, 1, salle->nb_sieges, "Saisissez la colonne que vous souhaiter réserver : ") - 1;// Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
      werase(win);
      }
      salle->concert.categorieC.tableau[i - salle->concert.categorieA.cat_rangees -salle->concert.categorieB.cat_rangees][j] = 1; // 0 place non-réservée, 1 place réservée


      /*
      Chaque catégorie possède un tableau pour les réservations, la rangée x de la salle ne correspon pas à
      la rangée x du tableau de la catégorie C, il faut donc calculer quelle va être sa valeur :
      rangée_Tableau = nombre_rangée_A + nombre_rangée_B + rangée_C
      => rangéeC = rangée_Tableau - nombre_rangée_A - nombre_rangée_B
      Exemple : 
      rangée_Tableau = 9
      nombre_rangée_A = 5
      nombre_rangée_B = 2
      rangéeB = rangée_Tableau - nombre_rangée_A - nombre_rangée_B
      rangéeB = 9 - 5 - 2 = 4
      */

      if (etudiant) {
        somme += 0.8 * salle->concert.categorieC.prix; // Réduction étudiante
      } else {
        somme += salle->concert.categorieC.prix;
      }
    }
  }
  mvwprintw(win, 1, 0, "La valeur totale de votre réservation est de %.2f euros ! ", somme); // Affiche le prix de la réservation
  mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
  wgetch(win);
  werase(win);
}

int reservation(WINDOW *win, Salle tableau[], int nb_salles) { // Appelle la fonction réservation_simple
  int nb_tableau = better_scan_compare(win, 1, 0, 0, nb_salles, "Pour quel concert voulez-vous réserver ? ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de salles, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
  werase(win);
  mvwprintw(win, 1, 0, "Si vous souhaitez annuler, taper 0 "); // Pour annuler la réservation appuyez sur 0
  int nb_reservation = better_scan_compare(win, 2, 0, 0,
          tableau[nb_tableau].concert.categorieA.nb_place +
          tableau[nb_tableau].concert.categorieB.nb_place +
          tableau[nb_tableau].concert.categorieC.nb_place,
      "Entrez le nombre de siège que vous voulez réserver : "); // L'utilisateur saisie combein de places il souhaite acheter

  werase(win);

  for (int i = 0; i < nb_reservation; i++) {
    reservation_simple(win, &tableau[nb_tableau]); // Réalise nb_reservation de fois une réservation
  }
  return nb_tableau;
}

void remboursement(WINDOW *win, Salle tableau[], int nb_salles) { 
  int etudiant;
  werase(win);
  int nb_tableau = better_scan_compare(win, 1, 0, 0, nb_salles, "Pour quel concert voulez-vous vous faire rembourser ? ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de salles, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
  int i, j;
  werase(win);
  i = better_scan_compare(win, 1, 0, 1, tableau[nb_tableau].concert.categorieA.cat_rangees + tableau[nb_tableau].concert.categorieB.cat_rangees + tableau[nb_tableau].concert.categorieC.cat_rangees,
      "Saisissez la rangée de votre réservation : ") - 1; // Vérifie que la valeur saisie est entre 1 et le nombre total de rangées de la salle, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
  werase(win);
  mvwprintw(win, 1, 0, "                                      "); // Corrige une erreur d'affichage, erase(win) ne fonctionne pas
  j = better_scan_compare(win, 1, 0, 1, tableau[nb_tableau].nb_sieges, "Saisissez la colonne de votre réservation ") - 1; // Vérifie que la valeur saisie est entre 0 et le nombre de sièges, -1 pour l'utiliser dans le tableau (faciliter l'expérience utilisateur)
  werase(win);
  if (i < tableau[nb_tableau].concert.categorieA.cat_rangees) { // Si la rangée choisie < au nombre de rangées de A :
    if (tableau[nb_tableau].concert.categorieA.tableau[i][j] == 0) { // Vérifie si la place est réservée
      mvwprintw(win, 1, 0, "Cette place n'est pas réservé, vous ne pouvez pas être remboursé ! ");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      tableau[nb_tableau].concert.categorieA.tableau[i][j] = 0; // Annule la réservation
      etudiant = better_scan_compare(win, 1, 0, 0, 1, "Etes-vous un étudiant ? (1 pour oui, 0 pour non) : "); // Etudiant réduit le prix de 20%
      if (etudiant) {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", 0.8*tableau[nb_tableau].concert.categorieA.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      } else {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", tableau[nb_tableau].concert.categorieA.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      }
      
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    }
  } else if (i < tableau[nb_tableau].concert.categorieA.cat_rangees + tableau[nb_tableau].concert.categorieB.cat_rangees) { // Si la rangée choisie < au nombre de rangées de A et de B (implicite > A car déjà testé)
    if (tableau[nb_tableau].concert.categorieB.tableau[i - tableau[nb_tableau].concert.categorieA.cat_rangees][j] == 0) {// Vérifie si la place est réservée

      mvwprintw(win, 1, 0, "Cette place n'est pas réservé, vous ne pouvez pas être remboursé ! ");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      tableau[nb_tableau].concert.categorieB.tableau[i - tableau[nb_tableau].concert.categorieA.cat_rangees][j] = 0; // Annule la réservation
      /*
      Chaque catégorie possède un tableau pour les réservations, la rangée x de la salle ne correspon pas à
      la rangée x du tableau de la catégorie B, il faut donc calculer quelle va être sa valeur :
      rangée_Tableau = nombre_rangée_A + rangée_B
      => rangéeB = rangée_Tableau - nombre_rangée_A
      Exemple : 
      rangée_Tableau = 8
      nombre_rangée_A = 5
      rangéeB = rangée_Tableau - nombre_rangée_A
      rangéeB = 8 - 5 = 3
      */

     etudiant = better_scan_compare(win, 1, 0, 0, 1, "Etes-vous un étudiant ? (1 pour oui, 0 pour non) : "); // Etudiant réduit le prix de 20%
     werase(win);
      if (etudiant) {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", 0.8*tableau[nb_tableau].concert.categorieB.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      } else {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", tableau[nb_tableau].concert.categorieB.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      }
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    }
  } else {
    if (tableau[nb_tableau].concert.categorieC.tableau[i - tableau[nb_tableau].concert.categorieA.cat_rangees - tableau[nb_tableau].concert.categorieB.cat_rangees][j] == 0) { // Vérifie si la place est réservée

    /*
      Chaque catégorie possède un tableau pour les réservations, la rangée x de la salle ne correspon pas à
      la rangée x du tableau de la catégorie C, il faut donc calculer quelle va être sa valeur :
      rangée_Tableau = nombre_rangée_A + nombre_rangée_B + rangée_C
      => rangéeC = rangée_Tableau - nombre_rangée_A - nombre_rangée_B
      Exemple : 
      rangée_Tableau = 9
      nombre_rangée_A = 5
      nombre_rangée_B = 2
      rangéeB = rangée_Tableau - nombre_rangée_A - nombre_rangée_B
      rangéeB = 9 - 5 - 2 = 4
    */

      mvwprintw(win, 1, 0, "Cette place n'est pas réservé, vous ne pouvez pas être remboursé ! ");
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    } else {
      tableau[nb_tableau].concert.categorieC.tableau[i - tableau[nb_tableau].concert.categorieA.cat_rangees - tableau[nb_tableau].concert.categorieB.cat_rangees][j] = 0; // Annule la réservation
      etudiant = better_scan_compare(win, 1, 0, 0, 1, "Etes-vous un étudiant ? (1 pour oui, 0 pour non) : "); // Etudiant réduit le prix de 20%
      mvwprintw(win, 1, 30 ,"                                                                     "); // Corrige problème d'affichage
      if (etudiant) {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", 0.8*tableau[nb_tableau].concert.categorieC.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      } else {
        mvwprintw(win, 1, 0, "Vous avez bien été remboursé de %.2f euros ! ", tableau[nb_tableau].concert.categorieC.prix); // Affiche à l'utilisateur qu'il a été remboursé.
      }
      mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
      wgetch(win);
      werase(win);
    }
  }
}
