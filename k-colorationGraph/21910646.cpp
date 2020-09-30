
// SI-MOHAMMED Sonia-Taous 21910646
/* question 1
 On a k variables par noeuds pour savoir quelle couleur
 est utilisée pour le noeud et une variable par couleur
 pour savoir si la couleur est utilisée on a donc k*nbNoeuds
 + k variables.
*/

/*question 2
 Toutes les variables sont booléennes .
 Les contraintes du problémes sont :
 *la somme des couleurs sur un noeud = 1
 *pour 2 noeuds adjacents la somme des variables de
 la meme couleur - la variable d'utilisation de cette
 couleur <= 0
*/

/*question 3
 la fonction objectif à optimiser : la somme des variables
 des couleurs
*/


#include <lpsolve/lp_lib.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<math.h>
#include<stdlib.h>

using namespace std;

void vider_row ( REAL row[], const int& taille){
  for(int i = 0 ; i < taille; i++){
    row[i]=0.0;
  }

}


int main (int argc, char* argv[]){
  ifstream fichier ("graph.dimacs");
  if (!fichier.is_open()){

    cout << "Impossible d'ouvrir le fichier du graphe ." << std::endl;
    exit(EXIT_FAILURE);
  }

  string nbNoeud;
  string nbEdge;
  string ligne;

  while (getline (fichier, ligne)) {

    string mot;
    istringstream iss (ligne);
    iss >> mot;
    if(mot == "p"){
      iss >> mot;
      iss >> mot;
      nbNoeud = mot;
      iss >> mot;
      nbEdge = mot;
      break;

    }
  }

  int nbNoeuds = stoi(nbNoeud);
  int k = nbNoeuds;
  int nbEdges = stoi(nbEdge);
  int nbVariables = k*k + k;
  int nbConstraints = 0;

  lprec* lp;
  lp = make_lp(nbConstraints, nbVariables);

  REAL row[nbVariables +1];
  vider_row(row, nbVariables+1);

  for (int i = 1; i < nbVariables+1; i++){
    vider_row(row, nbVariables+1);
    set_int(lp, i, true);
    row[i] = 1;
    add_constraint(lp, row, LE, 1);
    add_constraint(lp, row, GE, 0);
  }

  vider_row(row, nbVariables+1);

  for (int i = 1; i <= k*nbNoeuds+1; i++){
      row[i] = 1;
      if (i%k==0){
        add_constraint(lp, row, EQ, 1);
        vider_row(row, nbVariables+1);
      }
  }

  while (getline (fichier, ligne)) {
      string mot;
      istringstream iss (ligne);
      iss >> mot ;
      if (mot == "e"){
        iss >> mot;
        int a = stoi(mot);
        iss >> mot ;
        int b = stoi(mot);
        vider_row(row,nbVariables+1);
        for (int j = 1; j<=k; j++){
          vider_row(row, nbVariables+1);
          row[a*k-k+j] = 1;
          row[b*k-k+j] = 1;
          row[nbNoeuds*k+j] = -1;
          add_constraint(lp, row, LE, 0);
        }
      }
  }

  for (int i = 1; i <=k; i++){
    row[nbNoeuds*k + i] = 1;
  }

  set_obj_fn(lp,row);



  int result = solve(lp);

  if (result == 0){
    result = solve(lp);
    if (result == OPTIMAL){
      result = 0;
    }
    else{
      result = k;
    }
  }

  if (result == 0){
    get_variables(lp, row);
    int cpt = 0;
    for (int i = 1; i<= nbNoeuds; i++){
      for (int j = 1; j<=k; j++){
        if(row[cpt] == 1){
          printf("le noeud %d posséde la couleur %d \n", i,j);
        }
        cpt = cpt + 1;
      }
    }
    printf(" le nombre de couleurs minimales est: %f \n", get_objective(lp));
  }

}
