#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define TAILLE_MAX 1000 // Tableau de taille 1000

/* prototypes des fonctions:  */
int chercherInstrument (char *chaine); //determine quels instruments sont presents dans le fichier midi
int analyserNote(chaine);//recupere la frequence de chaque note
int recupererDivision(char *chaine); //recupere le nombre de ticks par temps (le tick est une unité de temps propre aux fichies midi)
int recupererSignature(char *chaine);
int recupererBPM(char *chaine);
int determinerDebut(char *chaine,int bpm,int division,int nbtemps,int* Pmesureprecedente, int* Ptempsprecedent, int* Ptickprecedent); //determine le temps ecoulé avant le debut d'une note



int main(int argc, char *argv[])
{


   int position[128]; //pour chaque instrument: position de la track dans le fichier texte (position du curseur)
   int frequence[5000]; //la frequence de chacune des notes
   int debut[5000]; //la durée en millisecondes avant la debut de la note
   int note[5000]; //chaque note prend au final une valeur de 0 a 6 (0=do, 1=ré...)
   int i=0, j=0, k=0; //variables permettant de parcourir des tableaux
   int pgm, choix; // le pgm est un numero propre a chaque instrument dans les fichiers midi
   int bpm, nbtemps, division; //battement par minute, nombre de temps par mesure, et nombre de divisions par temps.
   int mesureprecedente=1, tempsprecedent=1, tickprecedent=0;


FILE* fichier = NULL;
char chaine[TAILLE_MAX] = "";
fichier = fopen("morceau.txt", "r"); //le fichier texte contenant toutes les informations a recuperer (notes..)

FILE* fichier2 = NULL;
fichier2 = fopen("test.txt", "w");  //fichier texte de sortie
char chainesortie[]="";  //la chaine qui sera ecrite dans le fichier texte de sortie.

if (fichier != NULL) //si le fichier a bien été ouvert
{
    char *chaine1;

chaine1=fgets(chaine, TAILLE_MAX, fichier); //recupere la premiere ligne du fichier texte
division=recupererDivision(chaine1); //recupere le nombre de ticks par temps (le tick est une unité de temps propre aux fichiers midi), cette information se trouve toujours a la premiere ligne du fichier texte

printf("Quelle piste voulez vous recuperer ?:\n\n");

while (fgets(chaine, TAILLE_MAX, fichier) != NULL) //tant que le fichier n'a pas été totalement parcouru (fgets s'incremente automatiquement)
    {
             if(strstr(chaine, "pgm #=  ")!=NULL   || strstr(chaine, "pgm #= ")!=NULL)// si un numero d'instrument est present sur la ligne
             {
                 i=chercherInstrument(chaine);
                 position[i]=ftell(fichier); // On retient la position du curseur au debut de la piste
              }
              else if(strstr(chaine, "Time Sig")!=NULL)//ou si cette ligne donne la signature rythmique du morceau
              {
                  nbtemps=recupererSignature(chaine);
              }
              else if(strstr(chaine, "Tempo")!=NULL)//ou si cette ligne donne la BPM du morceau
              {
                 bpm=recupererBPM(chaine);
              }
    }

scanf("%d", &choix); //choix de la piste a analyser
fseek(fichier,position[choix],SEEK_SET); //on positionne le "curseur" a la ligne suivant le pgm


while( fgets(chaine, TAILLE_MAX, fichier)!= NULL   && strstr(chaine, "End of track")==NULL)
    {
        determinerDebut(chaine, bpm, division, nbtemps, &mesureprecedente, &tempsprecedent, &tickprecedent);

         if(strstr(chaine, "On Note")!=NULL) // On repere si la ligne correspond au debut d'une note
             {

                 frequence[j]=analyserNote(chaine); //on determine la frequence de chaque note
                 debut[j]=determinerDebut(chaine, bpm, division, nbtemps, &mesureprecedente, &tempsprecedent, &tickprecedent);

                 if (debut[j]==0) //si determinerDebut renvoie 0 cela veut dire que la ligne analysée ne donne aucune indication concernant le temps, celui ci correspond donc au temps de la note precedente
                 {
                    debut[j]=debut[j-1];
                 }
                 j++;
             }
    }


for(k=0;k<j ;k++)
{
     switch(frequence[k])   // ce switch permet d'associer a chaque frequence une valeur entre 0 et 6 (0= do, 1= ré ...)
    {
        case 32:
           printf("  %dms", debut[k]);
           note[k]=0;
        break;
         case 37:
            printf("  %dms", debut[k]);
            note[k]=1;
        break;
         case 41:
             printf("  %dms", debut[k]);
             note[k]=2;
        break;
         case 44:
            printf("  %dms", debut[k]);
             note[k]=3;
        break;
         case 49:
             printf("  %dms", debut[k]);
              note[k]=4;
        break;
         case 55:
               printf("  %dms", debut[k]);
               note[k]=5;
        break;
         case 61:
                printf("  %dms", debut[k]);
                note[k]=6;
        break;
    }
printf("\n");
sprintf(chainesortie, "%d - %d \n",debut[k], note[k]); //on ecrit le date de debut et la valeur entre 0 et 6 de chaque note dans une chaine


 fputs(chainesortie, fichier2); //on place cette chaine dans le fichie texte de sortie
}




/*Lorsque la note vaut 8 le programme de jeu comprend qu'il s'agit de la fin du morceau.
en fonction du niveau de difficulté, le programme de jeu peut sauter certaines notes c'est pourquoi j'ecris la note de fin 4 fois*/

for(j=0; j<5; j++)
{
    sprintf(chainesortie, "%d - 8\n",debut[k-1]+3000);
   //je reecris la note precedente en rajoutant 3 secondes pour que le jeu ne s'interrompe pas tout de suite apres la derniere note.
    fputs(chainesortie, fichier2);
}








fclose(fichier);
fclose(fichier2);
}
return 0;
}





/*****************************************************************************************************************************************
                                FONCTION CHERCHERINSTRUMENT
        -------------------------------------------------------------------
Cette fonction permet de determiner quels instruments sont présents dans le morceau, d'afficher leurs noms afin de laisser a l'utilisateur
lec choix de la piste qu'il souhaite récuperer.
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru.
                                                                    Auteur : Yacine Saoudi

****************************************************************************************************************************************/
int chercherInstrument (char *chaine)
{
 int pgm;

char *chaine1;
char *chaine2;

 chaine1=strtok(chaine, "#"); //on coupe la chaine autour du signe #
 chaine2=strtok(NULL, "#"); // on recupere la partie droite de la chaine precedente
 sscanf(chaine2, "=  %d", &pgm); // recupere le numero de l'instrument


switch(pgm) //le  standard midi associe a chaque numero de pgm un instrument, seuls quelques uns sont gérés par cette fonction:
 {
     case 0:
     printf("%d-Acoustic Grand Piano \n",pgm);
     break;

  case 1:
     printf("%d-Bright Acoustic Piano \n",pgm);
     break;

  case 2:
     printf("%d-Electric grand Piano \n",pgm);
     break;

  case 3:
     printf("%d-Honky Tonk Piano \n",pgm);
     break;

  case 4:
     printf("%d-Electric Piano 1 \n",pgm);
     break;

  case 5:
     printf("%d-Electric Piano 2 \n",pgm);
     break;

  case 6:
     printf("%d-Harpsichord \n",pgm);
     break;

  case 7:
     printf("%d-Clavinet \n",pgm);
     break;
  case 24:
     printf("%d-Nylon Accoustic Guitar \n", pgm);
     break;

  case 25:
     printf("%d-Steel Acoustic Guitar \n", pgm);
     break;

  case 26:
     printf("%d-Jazz Electric Guitar \n", pgm);
     break;

  case 27:
     printf("%d-Ciean Electric Guitar \n", pgm);
     break;

  case 28:
     printf("%d-Muted Electric Guitar \n", pgm);
     break;

  case 29:
     printf("%d-Overdrive Guitar \n", pgm);
     break;

  case 30:
     printf("%d-Distorted Guitar \n", pgm);
     break;

  case 31:
     printf("%d-Guitar Harmonics \n", pgm);
     break;
  default:
     break;
 }

return pgm;
}



/*****************************************************************************************************************************************
                                FONCTION ANALYSERNOTE
        -------------------------------------------------------------------
Cette fonction permet de determiner la frequence de chaque note.
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru.


Remarque: nous pourrions directement renvoyer une valeur entre 0 et 6 (0=do, ré=1, mi=2...),
cela eviterait le long switch a la fin du programme.
 Cependant nous avons préféré laisser cette fonction telle quelle au cas ou nous desirions adapter la manette pour gerer pus d'un octave
 ou creer une manette guitare.


                                                                    Auteur : Yacine Saoudi

****************************************************************************************************************************************/
int analyserNote(chaine)
{
    int frequence, octave;
    char note;
    char *chaine1;
    char *chaine2;


    chaine1=strtok(chaine, "i"); //permet de decouper la chaine en 2 autour du i car c'est le seul caractere qui n'apparait qu'avant la partie a recuperer
    chaine2=strtok(NULL, "i"); //on recupere la chaine a droite du caractere i
    chaine1=strtok(chaine2, "|"); //on tronque ensuite toute la partie concernant le volume
    chaine2=strtok(chaine1, "="); //on coupe autour du =
    chaine1=strtok(NULL, "="); //on recupere la partie a droite du =


   if(strstr(chaine1, "#")==NULL) // S'il n y a pas de # dans la chaine
             {
    sscanf(chaine1, "%c %d", &note, &octave); // recupere note et octave

             }
    else if(strstr(chaine1, "#")!=NULL) // S'il y a un # dans la chaine (do#, ré#...)
    {
        sscanf(chaine1, "%c#%d", &note, &octave); // recupere note et octave
    }


    switch(note) //les fichiers midi utilisent la notation anglo-saxonne (les notes en minuscules ou majuscules sont exactement les memes)
    {
        case 'c':
        frequence=32;  //frequence du DO 0, les frequences sont approximatives et ne prennent pas en compte les dièses et bémol
        break;
         case 'd':
        frequence=37; // ré
        break;
         case 'e':
        frequence=41; // mi
        break;
         case 'f':
        frequence=44;
        break;
         case 'g':
        frequence=49;
        break;
         case 'a':
        frequence=55;
        break;
         case 'b':
        frequence=61;
        break;
          case 'C':
        frequence=32;  //frequence du DO 0
        break;
         case 'D':
        frequence=37;
        break;
         case 'E':
        frequence=41;
        break;
         case 'F':
        frequence=44;
        break;
         case 'G':
        frequence=49;
        break;
         case 'A':
        frequence=55;
        break;
         case 'B':
        frequence=61;
        break;
    }

/*
if(octave>0)
{
    frequence=frequence*2*octave; //pour passer d'un octave a l'autre i faut multipler la frequence par 2
}
*/
    return frequence;
}



/*****************************************************************************************************************************************
                                FONCTION RECUPERERDIVISION
        -------------------------------------------------------------------
Cette fonction permet de determiner le nombre de divisions par temps (afin de calculer la durée d'un tick par la suite).
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru.

                                                                    Auteur : Yacine Saoudi
****************************************************************************************************************************************/
int recupererDivision(char *chaine) //recupere le nombre de ticks par temps (le tick est une unité de temps propre aux fichies midi)
{
    int tick, division;
    char *chaine1, *chaine2;

     chaine1=strtok(chaine, "n"); //decoupe la chaine autour du 'n' de "division"
    chaine2=strtok(NULL, "n"); //on recupere la chaine a droite du caractere n
    sscanf(chaine2, "=%d", &division);
    return division;
}


/*****************************************************************************************************************************************
                                FONCTION RECUPERERSIGNATURE
        -------------------------------------------------------------------
Cette fonction permet de determiner le nombre de temps par mesure (afin de calculer la durée d'une mesure par la suite).
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru.

                                                                    Auteur : Yacine Saoudi
****************************************************************************************************************************************/
int recupererSignature(char *chaine)
{
int nbtemps;
char *chaine1, *chaine2;

    chaine1=strtok(chaine, "g"); //On coupe la chaine apres le "Time sig"
    chaine2=strtok(NULL, "g"); //on recupere la partie a droite du caractere "g"
    chaine1=strtok(chaine2, "|"); //On coupe la chaine apres le "|"
    chaine2=strtok(NULL, "|"); //on recupere la partie a droite du caractere "|"

    sscanf(chaine2, "%d/", &nbtemps); // recupere le nombre de temps par mesure

   return nbtemps;
}


/*****************************************************************************************************************************************
                                FONCTION RECUPERERBPM
        -------------------------------------------------------------------
Cette fonction permet de determiner le nombre de battements par minute.
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru.


Remarque: de nombreux fichiers midi changent de BPM en cours de chanson, adapter ce programme pour qu'il prenne en compte ces variations
permettrait d'assurer une compatibilité avec un nombre beaucoup plus grand de fichiers midi.

                                                                    Auteur : Yacine Saoudi
****************************************************************************************************************************************/
int recupererBPM(char *chaine)
{
    int bpm;
char *chaine1, *chaine2;

    chaine1=strtok(chaine, "="); //On coupe la chaine autour du "="
    chaine2=strtok(NULL, "="); //on recupere la partie a droite
    chaine1=strtok(chaine2, "|"); //On coupe la chaine autour du "|"

    sscanf(chaine1,"%d", &bpm);

    return bpm;
}

/*****************************************************************************************************************************************
                                FONCTION DETERMINERDEBUT
        -------------------------------------------------------------------
Cette fonction permet de determiner la date ou une note est jouée sur le morceau.
Elle prend comme paramètre une chaine de caractere contenant la ligne actuelle du fichier texte parcouru, toutes les informations relatives
au tempo de la musique (bpm, nombre de divisions par temps, nombre de temps par mesure).

Si la ligne analysée ne donne aucune (ou seulement certaines) indication sur le temps, on reprend alors les valeurs de la derniere note.
C'est pourquoi cette fonction prend egalement en parametres la date de la derniere mesure, du dernier temps et du dernier tick.
                                                                    Auteur : Yacine Saoudi
****************************************************************************************************************************************/
int determinerDebut(char *chaine,int bpm,int division,int nbtemps,int* Pmesureprecedente, int* Ptempsprecedent, int* Ptickprecedent) //determine le temps ecoulé avant le debut d'une note
{
   char chaine1[6] = {0};
char chaine2[9]= {0};

int mesure=0, temps=0, tick=0;

int dureetemps; //duree d'un temps (depend du bpm)
int dureemesure; //duree d'une mesure (depend du nombre de temps par mesure et duree d'un temps)
int dureetick; //depend du nombre de ticks par temps (division)
int debut; //temps total ecoulé avant le debut de la note


strncpy(chaine1, chaine, 5); //on copie les 5 premiers caracteres de chaine dans chaine1
strncpy(chaine2, chaine, 8); //on copie les 5 premiers caracteres de chaine dans chaine2



if(strstr(chaine1, ":")!=NULL) // s'il y a un ':' dans les 5 premiers caracteres de chaine cela veut dire que le temps est sous la forme  mesure: temps: tick
{
    sscanf(chaine, "%d:%d:%d", &mesure, &temps, &tick); //on recupere les 3 valeurs
}
else if(strstr(chaine1, ":")==NULL && strstr(chaine2, ":")!=NULL)// s'il y a un ':' dans les 8 premiers caracteres de chaine cela veut dire que le temps est sous la forme  temps: tick
{
    sscanf(chaine, "%d:%d", &temps, &tick); //on recupere les 2 valeurs
}
else if(strstr(chaine2, ":")==NULL ) // s'il n' y a aucun ':' dans les 8 premiers caracteres alors, seuls les ticks sont indiqués
{
    sscanf(chaine, "%d", &tick);
}




if(mesure==0) //s'il n'y a aucune indication sur la mesure, cela veut dire qu'il s'agit de la meme que la precedente.
{
    mesure= *Pmesureprecedente;
}
if(temps==0)
{
    temps= *Ptempsprecedent;
}
if(tick==0)
{
    tick= *Ptickprecedent;
}

//conversion en millisecondes:
dureetemps=60000/bpm;
dureemesure=dureetemps*nbtemps;
dureetick=dureetemps/division;

debut=(mesure-1)*dureemesure+(temps-1)*dureetemps+tick*dureetick;
// on doit toujours retirer 1 au temps et a la mesure car le tout debut du morceau correspond au 1er temps de la 1ère mesure

*Pmesureprecedente=mesure;
*Ptempsprecedent=temps;
*Ptickprecedent=tick;
/*
Cette fonction ne peut renvoyer qu'une seule valeur, c'est pourquoi j'uilise des pointeurs pour ces 3 variables car elles doivent etre
utilisées et modifiées a chaque fois que cette fonction s'execute.
*/

    return debut;
}





