#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <queue>
#include<time.h>
#include<stdlib.h>
using namespace std;

//Les adjlist de tous les vertex forment une matrice d'adjacence, mÍme si physiquement sur la mÈmoire ce n'est pas une matrice
class Graph
{
  struct node
   {
    int fr,to,cost;
   };
  int C; // Nombre de sommet
  int densite; // Densite
  int **tableau2D; // Matrice d'adjacence
  public:
  Graph(int C,int densite); //  Genere le graph
  int rand_a_b(int a,int b); //  genere le nombre aleatoire
  void isConnexe(int C); //  Parcours largeur + rend connexte le graph
  void Display(); //  Affiche une partie de la matrice d'ajacence
  void prims(); // Permet de faire l'arbre couvrant minimum
  int minDistance(int dist[], bool sptSet[]); // Trouve la plus cours distance
  void dijkstra(int node); // Effectue Dijikstra d'un noeud vers tous les autres noeuds du graph
  int GetC(); // Getteur pour avoir le nombre de sommet
};

int Graph::GetC() // Getteur pour le nombre de sommet C
{
  return C;
}

int Graph::rand_a_b(int a,int b) //  genere un nombre entre les bornes
{
  return rand()%(b-a) + a;
}

// Generation de la matrice d'adjacence du Graph
Graph::Graph(int C,int densite)
{
  this->C = C;
  this->densite = densite;
  int nbaleatoire = 0;
  srand(time(NULL)); // initialisation de rand;
  tableau2D = new int*[C];//initialisation du tableau a 2 Dim.

  for(int i = 0 ; i < C ; i ++)
   tableau2D[i] = new int [C];

  for(int i = 0; i < C ; i++) // On init la matrice a 0
   {
    for(int j=0 ; j < C ; j++)
     {
      tableau2D[i][j] = 0; // On init la matrice d'adjacence a 0
     }
   }

  for(int x = 0; x < C ; x++) // On genere un nb aleatoire, si inferieur a dens alors on remplit la matrice
   {
    for(int y = x + 1 ; y < C ; y++) //  On ne peux pas avoir de cycle donc on commence la boucle sur x+1
     {
      nbaleatoire = rand_a_b(0,100); //  Generation du nb aleatoire.
      int nb = rand_a_b(1,9); // generation de l'entier entre 1 et 9
      if(nbaleatoire < densite)
       {
        tableau2D[x][y] = nb; // x-y
        tableau2D[y][x] = nb; // y-x
       }
     }
   }
}

// parcours largeur du graph + liaison des sommets non relies
void Graph::isConnexe(int C)
{
  this->C = C;
  int j = 0;
  int i = 0;
  int premier = -3;
  int max = numeric_limits<int>::max();
  bool IsConnexe = true;
  queue<int> valeur;
  bool *tab = new bool[C];

  for(int w=0 ; w < C ; w++)
   {
    tab[w] = false;
   } //  On init le tableau de boolean a faux.

  // On cherche un sommet
  while(j < C && premier == -3)
   {
    i = j+1;
    while(i < C && premier == -3)
     {
      if(tableau2D[i][j] != 0)
       {
        premier = i;
       }
      i++;
     }
    j++;
   }

  /*if(premier == -3)
    {
    premier = 3;
    }*/

  // Parcours Largeurs, on enfile les sommets et on les marquent //  Algo
  // utilise celui de ma prof.
  valeur.push(premier);
  while(!valeur.empty())
   {
    premier = valeur.front();
    valeur.pop();
    tab[premier] = true;
    for(int i = 0; i < C ; i++)
     {
      if(!tab[i] && tableau2D[premier][i] != 0)
       {
        valeur.push(i);
       }
     }
   }

  // Si certain elements sont non marque ils ont ete marque par faux dans le
  // tableau de boolean
  for(int s=0; s<C ;s++)
   {
    if(!tab[s]) // Si c'est faux alors on traite le noeud
     {
      i = 1;
      while(!tab[(s+i)%C]) // On avance tant que c'est faux afin de trouver un lien a vrai pour le relier
       {
        i++;
       }
      int random = rand_a_b(1,9); //  On genere un nombre entre 1 et 9
      tableau2D[s][(s+i)%C] = random; // On l'applique a la matrice,on fait %c pour eviter de sortir du tableau et d'avoir un segment fault memory.
      tableau2D[(s+i)%C][s] = random; // On est dans un graph non oriente donc il faut marque x-y et y-x
     }
   }
}


//minDist trouve le noeud non marque ayant le plus petit poids
int Graph::minDistance(int dist[], bool sptSet[])
{
  int min = INT_MAX;
  int min_index;
  for(int c=0; c<C;c++)
   {
    if(sptSet[c]==false && dist[c] <= min)
     {
      min = dist[c];
      min_index=c;
     }
   }
  return min_index;
}

//djikstra donne la distance minimale (en additionnant les poids des arretes
//parcourues) de node vers chaque autre noeud du graphe
void Graph::dijkstra(int node)
{
  bool sptSet[C];//permet de savoir quel noeud est deja traite
  int dist[C]; //les distances minimales seront stockees dans dist

  //initialisation
  for(int c=0; c<C;c++)
   {
    dist[c] = INT_MAX;
    sptSet[c]=false;
   }

  dist[node]=0;

  for(int i=0;i<C-1;i++)
   {
    int u = this->minDistance(dist,sptSet);
    sptSet[u]=true;
    for(int c=0; c<C;c++)
     {
      //on verifie que le noeud n'est pas deja dans le chemin, et qu'atteindre c
      //en passant par u a moins de valeur que d'atteindre c directement
      if(!sptSet[c] && tableau2D[u][c] && dist[u] != INT_MAX && (dist[u]+tableau2D[u][c])<dist[c])
       dist[c] = dist[u] + tableau2D[u][c];
     }
   }

  printf("V");
  for (int i = 0; i < C; i++)
   printf("%d-%d,", i, dist[i]); 
}

void Graph::prims()
{
  int parent[C];
  int key[C];
  bool set[C];


  //Initialisation des tableaux
  for(int i=0; i<C; i++)
   {
   key[i]=INT_MAX;
   set[i]=false;
   }

  //On part du noeud 0 par defaut
  key[0]=0;
  parent[0]= -1;

  for(int count=0;count<C-1;count++)
   {
    int u = minDistance(key,set);
    set[u]=true;
    for(int c=0;c<C;c++)

     {
      if(tableau2D[u][c] && set[c]==false && tableau2D[u][c]<key[c])
       {
       parent[c]=u;
       key[c]=tableau2D[u][c];
       }
     }
   }

  printf("P");
  for (int k = 1; k < C ; k++)
   {
    printf("%d-%d %d,",parent[k],k,tableau2D[k][parent[k]]);
   }
}

void Graph::Display()
{
  for(int x = 0; x < C ; x++)
   {
    for(int y = 0 ; y < x ; y++) // on affiche juste la partie superieur du tableau
     {
      if(tableau2D[x][y] != 0)
       {
        printf("%d-%d %d,",x,y,tableau2D[x][y]);
       }

     }
   }
}

int main(void)
{
  static char buffer[1000]; // Init du buffer char a 100 case
  setbuf(stdout, NULL); // Sortie standard

  int sommet = 0; // Init sommet a 0
  int densite = 0; // Init densite a 0
  int source = 0 ; // Init source a 0
  scanf("%d %d %d", &sommet, &densite,&source); //  On recup les entiers envoyes par le serveur

  Graph a(sommet,densite); //  creation de la matrice d'adjacence
  a.isConnexe(sommet); // verification de la connexite du graph
  a.Display(); // Affichage du graph
  a.dijkstra(source); // effectue Djikstra

  a.prims(); // Effectue l'arbre couvrant minimum
  printf("\n");
}

// Tout le traitement des sorties printf, s'effectue dans le javascript en
// effectuant du Parsing.
