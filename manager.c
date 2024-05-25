#include "manager.h"
#include "better_scan.h"

#define MAX 50



// Fonctions d'affichage
void affichage_rangees(WINDOW *win, int y, int x, char *lettre, int nb_places){
  mvwprintw(win, y, x, "Nombre de rangées de la categorie %c = %d", *lettre, nb_places);
}
void affichage_nb_sieges(WINDOW *win, int y, int x, char *lettre, int nb_sieges){
  mvwprintw(win, y, x, "Nombre de sièges de la categorie %c = %d", *lettre, nb_sieges);
}
void affichage_prix(WINDOW *win, int y, int x, char *lettre, float prix){
  mvwprintw(win, y, x, "Prix de la categorie %c = %.2f euros", *lettre, prix);
}

// Affiche un récapitulatif de toute les informations d'une salle et d'un concert
void affichage_donnees_manager(WINDOW *win, Concert concert){

  mvwprintw(win, 1, 0, "Heure de début du concert : %d h %d", concert.heure_debut, concert.minute_debut);
  mvwprintw(win, 2, 0, "Heure de fin du concert : %d h %d", concert.heure_fin, concert.minute_fin);
  mvwprintw(win, 3, 0, "Le nom de l'artiste est : %s", concert.artiste);
  mvwprintw(win, 4, 0, "Nombre de rangées : %d", concert.categorieA.cat_rangees + concert.categorieB.cat_rangees + concert.categorieC.cat_rangees);
  mvwprintw(win, 5, 0, "Nombre de rangées pour la catégorie A : %d", concert.categorieA.cat_rangees);
  mvwprintw(win, 6, 0, "Nombre de rangées pour la catégorie B : %d", concert.categorieB.cat_rangees);
  mvwprintw(win, 7, 0, "Nombre de rangées pour la catégorie C : %d", concert.categorieC.cat_rangees);
  mvwprintw(win, 8, 0, "Le nombre de sièges par rangée est : %d", (concert.categorieA.nb_place + concert.categorieB.nb_place + concert.categorieC.nb_place) / (concert.categorieA.cat_rangees + concert.categorieB.cat_rangees + concert.categorieC.cat_rangees));
  
  float somme = 0; // Somme pour calculer le chiffre d'affaire
  int compteur = 0;

  for (int i = 0; i < concert.categorieA.cat_rangees; i++){
    for (int j = 0; j < concert.categorieA.nb_place; j++){
      if (concert.categorieA.tableau[i][j] == 1){ // Vérifie qu'une place est réservé
        compteur++; 
        somme += concert.categorieA.prix; // La place payée est ajoutée à la somme 
      }
    }
  }
  for (int i = 0; i < concert.categorieB.cat_rangees; i++){
    for (int j = 0; j < concert.categorieB.nb_place; j++){
      if (concert.categorieB.tableau[i][j] == 1){ // Vérifie qu'une place est réservé
        compteur++;
        somme += concert.categorieB.prix; // La place payée est ajoutée à la somme 
      }
    }
  }
  for (int i = 0; i < concert.categorieC.cat_rangees; i++){
    for (int j = 0; j < concert.categorieC.nb_place; j++){
      if (concert.categorieC.tableau[i][j] == 1){ // Vérifie qu'une place est réservé
        compteur++;
        somme += concert.categorieC.prix; // La place payée est ajoutée à la somme 
      }
    }
  }

  mvwprintw(win, 9, 0, "Nombre de sièges : %d", concert.categorieA.nb_place + concert.categorieB.nb_place + concert.categorieC.nb_place);
  mvwprintw(win, 10, 0, "Nombre de sièges restant : %d", (concert.categorieA.nb_place + concert.categorieB.nb_place + concert.categorieC.nb_place) - compteur);

  affichage_nb_sieges(win, 12, 0, "A", concert.categorieA.nb_place);
  affichage_nb_sieges(win, 13, 0, "B", concert.categorieB.nb_place);
  affichage_nb_sieges(win, 14, 0, "C", concert.categorieC.nb_place);

  affichage_prix(win, 16, 0, "A", concert.categorieA.prix);
  affichage_prix(win, 17, 0, "B", concert.categorieB.prix);
  affichage_prix(win, 18, 0, "C", concert.categorieC.prix);

  mvwprintw(win, 21, 0, "Chiffre d'affaire potentiel : %2.f", concert.categorieA.prix * (float)concert.categorieA.nb_place + concert.categorieB.prix * (float)concert.categorieB.nb_place + concert.categorieC.prix * (float)concert.categorieC.nb_place);

  mvwprintw(win, 23, 0, "Chiffre d'affaire actuel : %.2f", somme);
  mvwprintw(win, 24, 0, "Ratio des sièges : %.2f", compteur / (float)(concert.categorieA.nb_place + concert.categorieB.nb_place + concert.categorieC.nb_place));

  mvwprintw(win, 27, 0, "Appuyez sur une touche pour revenir au menu...");
}


// Fonction dessinant la tribune 
void affichage_tribune(WINDOW *win, Salle *salle){

  int rang = 1;

  mvwprintw(win, 1, salle->nb_sieges + 1, " Scène");

  for (int i = 0; i < salle->nb_sieges; i++){
    mvwprintw(win, 2, 3 * i+2, "---"); // On débute après les numéros de rangées
  }

  for (int i = 0; i < salle->nb_sieges; i++){
    mvwprintw(win, 3,  3 * i + 3, " C "); // On débute après les numéros de rangées
  }

  for (int i = 0; i < salle->nb_sieges; i++){ // Va écrire le numéro des colonnes de la forme C(chiffre) 
    if (i < 9){ // Rajoute un 0 avant le chiffre s'il est inférieur à 9
      mvwprintw(win, 4, 3 * i + 3, " 0%d", i + 1); 
    }
    else{
      mvwprintw(win, 4, 3 * i + 3, " %d", i + 1);
    }
  }

  int y = 5;

  for (int i = 0; i < salle->concert.categorieA.cat_rangees; i++){ // Va écrire le numéro des rangées de la forme R(chiffre) 
    mvwprintw(win, y, 0, "R%d", rang);
    rang++;
    for (int j = 0; j < salle->nb_sieges; j++){
      if (salle->concert.categorieA.tableau[i][j] == 0){
        mvwprintw(win, y, 3 * j + 3, " A "); // Initialise les places de la catégorie A si la place est non reservée
      }
      else{
        mvwprintw(win, y, 3 * j + 3, " X "); // Déclare la place réservée par X
      }
    }
    y++; // Incrémente y pour l'affichage L147/L153
  }

  for (int i = 0; i < salle->concert.categorieB.cat_rangees; i++){
    mvwprintw(win, y, 0, "R%d", rang);
    rang++;
    for (int j = 0; j < salle->nb_sieges; j++){
      if (salle->concert.categorieB.tableau[i][j] == 0){ // Initialise les places de la catégorie B si la place est non reservée
        mvwprintw(win, y, 3 * j + 3, " B ");
      }
      else{
        mvwprintw(win, y, 3 * j + 3, " X "); // Déclare la place réservée par X
      }
    }
    y++; // Incrémente y pour l'affichage L147/L153
  }

  for (int i = 0; i < salle->concert.categorieC.cat_rangees; i++){
    mvwprintw(win, y, 0, "R%d", rang);
    rang++;
    for (int j = 0; j < salle->nb_sieges; j++){
      if (salle->concert.categorieC.tableau[i][j] == 0){ // Initialise les places de la catégorie C si la place est non reservée
        mvwprintw(win, y, 3 * j + 3, " C ");
      }
      else{
        mvwprintw(win, y, 3 * j + 3, " X "); // Déclare la place réservée par X
      }
    }
    y++; // Incrémente y pour l'affichage L147/L153
  }

  mvwprintw(win, y + 1, 0, "Légende : ");
  mvwprintw(win, y + 2, 0, "A - Catégorie A (%.2f euros) ", salle->concert.categorieA.prix);
  mvwprintw(win, y + 3, 0, "B - Catégorie B (%.2f euros) ", salle->concert.categorieB.prix);
  mvwprintw(win, y + 4, 0, "C - Catégorie C (%.2f euros) ", salle->concert.categorieC.prix);
  mvwprintw(win, y + 5, 0, "X - Siège réservé ");

  mvwprintw(win, y + 7, 0, "Appuyez sur une touche pour revenir au menu...");
}


// Construit une salle de type Salle
Salle constructeur_salle(WINDOW *win)
{

  Salle salle_concert; // Initialise salle_concert de type Salle
  char texte_nom[MAX]; // Variable qui stocke le nom de la salle

  better_scan_str(win, 1, 0, "Entrez le nom de la salle : ", texte_nom, MAX);
  werase(win);

  salle_concert.nb_rangees = better_scan_compare(win, 1, 0, 0, 50, "Entrez le nombre de rangées (max 50): ");
  while (salle_concert.nb_rangees <= 0 || salle_concert.nb_rangees > 50){ // Vérification de l'intervalle
    if (salle_concert.nb_rangees > 50){
      werase(win);
      salle_concert.nb_rangees = better_scan_compare(win, 1, 0, 0, 50, "Veuillez entrer un nombre de rangées inférieur ou égal à 50 : ");
    }
    else{
      werase(win);
      salle_concert.nb_rangees = better_scan_compare(win, 1, 0, 0, 50, "Vous ne pouvez pas créer de salle sans rangées. Entrez a nouveau un nombre de rangée : ");
    }
  }
  werase(win);
  salle_concert.nb_sieges = better_scan_compare(win, 1, 0, 0, 75, "Entrez le nombre de sieges par rangées (max75) : ");
  while (salle_concert.nb_sieges <= 0 || salle_concert.nb_sieges > 75){ // Vérification de l'intervalle
    if (salle_concert.nb_sieges > 75){
      werase(win);
      salle_concert.nb_sieges = better_scan_compare(win, 1, 0, 0, 75, "Veuillez entrer un nombre de sieges par rangées inférieur ou égal à 75 : ");
    }
    else{
      werase(win);
      salle_concert.nb_sieges = better_scan_compare(win, 1, 0, 0, 75, "Vous ne pouvez pas créer de rangées sans sieges. Entrez a nouveau un nombre de sieges : ");
    }
  }
  werase(win);
  return salle_concert;
}

// Construit un concert à partir d'une salle
void constructeur_concert(Salle *salle, WINDOW *win){

  Categorie A, B, C; // Initialise des catégories de type Categorie

  salle->concert.jour_debut = better_scan_compare(win, 1, 0, 1, 366, "Entrez le jour du début du concert (entre 1 et 366): ");
  werase(win);
  mvwprintw(win, 1, 0, "                                                                                   ");
  salle->concert.heure_debut = better_scan_compare(win, 1, 0, 0, 23, "Entrez l'heure de début du concert : ");
  werase(win);
  salle->concert.minute_debut = better_scan_compare(win, 1, 0, 0, 59, "Entrez la minute de début du concert : ");
  werase(win);
  salle->concert.heure_fin = better_scan_compare(win, 1, 0, 0, 23, "Entrez l'heure de fin du concert : "); 
  werase(win);
  salle->concert.minute_fin = better_scan_compare(win, 1, 0, 0, 59, "Entrez la minute de fin du concert : "); 
  werase(win);

  // On déduit le jour en fonction de l'heure
  if (salle->concert.heure_debut > salle->concert.heure_fin){
    salle->concert.jour_fin = salle->concert.jour_debut + 1;
  }
  else if (salle->concert.heure_debut == salle->concert.heure_fin && salle->concert.minute_debut > salle->concert.minute_fin){
    salle->concert.jour_fin = salle->concert.jour_debut + 1;
  }
  else{
    salle->concert.jour_fin = salle->concert.jour_debut;
  }

  better_scan_str(win, 1, 0, "Entrez le nom de l'artiste : ", salle->concert.artiste, MAX);
  werase(win);

  int nb_rangees_salle = salle->nb_rangees; // Variable qui définit nombre de rangées sans modifier la salle 

  mvwprintw(win, 1, 0, "                            ");
  mvwprintw(win, 1, 0, "Il y a actuellement : %d rangées", nb_rangees_salle);
  A.cat_rangees = better_scan_compare(win, 2, 0, 0, nb_rangees_salle, "Entrez le nombre de rangées que vous souhaitez pour la categorie A : ");
  werase(win);

  werase(win);

  nb_rangees_salle -= A.cat_rangees; // Permet de ne pas dépasser le nombre de rangées de la salle

  int fosse = -1;
  if (A.cat_rangees != 0){
    while (fosse != 1 && fosse != 0){
      werase(win);
      fosse = better_scan_compare(win, 1, 0, 0, 1, "Souhaitez vous remplacer la categorie A par une fosse ? (Oui = 1 et Non = 0) : ");
    }
  }

  werase(win);

  if (nb_rangees_salle == 0){
    mvwprintw(win, 1, 0, "Toutes les rangées sont atribuées à la Catégorie A ! ");
    mvwprintw(win, 2, 0, "Appuyez sur une touche pour continuer...");
    wgetch(win);
    werase(win);
    B.cat_rangees = 0;
    C.cat_rangees = 0;
  }

  else{
    mvwprintw(win, 2, 50, "                                                         ");
    mvwprintw(win, 1, 0, "Il reste : %d rangées", nb_rangees_salle);
    B.cat_rangees = better_scan_compare(win, 2, 0, 0, nb_rangees_salle, "Entrez le nombre de rangées que vous souhaitez pour la categorie B : ");
    

    werase(win);

    nb_rangees_salle -= B.cat_rangees; // Permet de ne pas dépasser le nombre de rangées de la salle

    // Définie automatiquement la catégorie C à partir des catégories précédentes et du nombre de rangées restantes
    if (nb_rangees_salle > 100){
      C.cat_rangees = 100;
    }
    else{
      C.cat_rangees = nb_rangees_salle;
    }
  }

  if (fosse == 1){
    A.nb_place = 2 * A.cat_rangees * salle->nb_sieges; // 2*+ de places pour la fosse
    A.cat_rangees = 2 * A.cat_rangees;
  }
  else{
    A.nb_place = A.cat_rangees * salle->nb_sieges; // Calcule le nombre de places de la catégorie A
  }

  B.nb_place = B.cat_rangees * salle->nb_sieges; // Calcule le nombre de places de la catégorie A
  C.nb_place = C.cat_rangees * salle->nb_sieges; // Calcule le nombre de places de la catégorie A

  if (A.cat_rangees == 0){
    A.prix = 0.0;
    werase(win);
  }
  else{
    werase(win);
    wrefresh(win);
    mvwprintw(win, 1, 0, "Le prix ne peut exceder 2000 euros !");
    A.prix = better_scan_compare_f(win, 2, 0, 0, 2000,  "Prix de la catégorie A ? ");
    werase(win);
  }
  if (B.cat_rangees == 0){
    B.prix = 0.0;
    werase(win);
  }
  else{
    werase(win);
    wrefresh(win);
    mvwprintw(win, 1, 0, "Le prix ne peut exceder 2000 euros !");
    B.prix = better_scan_compare_f(win, 2, 0, 0, 2000, "Prix de la catégorie B ? ");
    werase(win);
  }
  werase(win);
  if (C.cat_rangees == 0){
    C.prix = 0.0;
    werase(win);
  }
  else{
    werase(win);
    wrefresh(win);
    mvwprintw(win, 1, 0, "Le prix ne peut exceder 2000 euros !");
    C.prix = better_scan_compare_f(win, 2, 0, 0, 2000, "Prix de la catégorie C ? ");

    werase(win);
  }

  werase(win);

  // Initialise la valeur des places à 0
  for (int i = 0; i < A.cat_rangees; i++){
    for (int j = 0; j < salle->nb_sieges; j++){
      A.tableau[i][j] = 0;
    }
  }
  for (int i = 0; i < B.cat_rangees; i++){
    for (int j = 0; j < salle->nb_sieges; j++){
      B.tableau[i-A.cat_rangees][j] = 0;
    }
  }
  for (int i = 0; i < C.cat_rangees; i++){
    for (int j = 0; j < salle->nb_sieges; j++){
      C.tableau[i-A.cat_rangees-B.cat_rangees][j] = 0;
    }
  }

  salle->concert.categorieA = A; // affiliation
  salle->concert.categorieB = B; // affiliation
  salle->concert.categorieC = C; // affiliation
}


// Fonction qui permet de modifier une salle déja créée
Salle modifier_salle(WINDOW *win, Salle salle, int nb_salles){
  if (nb_salles != 0){
    salle = constructeur_salle(win);
  }
  return salle;
}