#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>

#define TAILLE_MAX 10000 // Tableau de taille 10 000

void demo (SDL_Surface* ecran);//Comme il n'y qu'un seul prototype faire un .h semble inutile

/***************************************************************************************************************************************************************

                                FONCTION TUTO

        -------------------------------------------------------------------

    On affiche l'image qui permet d'expliquer comment jouer et on lance une musique on propose une démo si on appuie sur espace

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Aurele

****************************************************************************************************************************************************************/



void tuto(SDL_Surface* ecran)
{


SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Noms = NULL;
SDL_Rect positionFond,positionNoms;
int etat = 0;

positionFond.x = 0;
positionFond.y = 0;
positionNoms.x = 400;
positionNoms.y = 900;

imageDeFond = SDL_LoadBMP("images/tuto.bmp");//on indique ou est l'image de fond

    /*Musique*/
FMOD_SYSTEM *system;
FMOD_SOUND *musique;
FMOD_RESULT resultat;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
/* On ouvre la musique*/
resultat = FMOD_System_CreateSound(system, "musique.mp3",
FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);

/* On joue la musique*/
FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0,NULL);




            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On met le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);



int continuer = 1;
while (continuer)
    {



    SDL_PollEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: /* Si appui sur une touche */

    switch(event.key.keysym.sym)
    {
        case SDLK_ESCAPE:
        continuer=0;
        break;
        case SDLK_SPACE:
        FMOD_Sound_Release(musique);
        demo(ecran);
        etat = 1;//la musique a déjà été arretée, on ne l'arretera pas deux fois
        continuer=0;
        break;
        default:
        break;
    }
break;

}




/* On met à jour l'affichage */
SDL_Flip(ecran);

    }


///////////////////////////////////////////
/*Sortie de boucle*/
SDL_ShowCursor(SDL_ENABLE);//on réaffiche le curseur pour le menu
//Libération de l'espace
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(Noms);
//pour la musique

if(etat==0)FMOD_Sound_Release(musique);
FMOD_System_Close(system);
FMOD_System_Release(system);

}



/***************************************************************************************************************************************************************

                                FONCTION DEMO

        -------------------------------------------------------------------

   On a copié la fonction jeu en faisant faire un perfect sur frère jacques par
   l'ordinateur pour faire la démo

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Aurele

****************************************************************************************************************************************************************/

void demo (SDL_Surface* ecran)
{
    SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Note = NULL, *Note_do = NULL, *Note_re = NULL, *Note_mi = NULL, *Note_fa = NULL, *Note_sol = NULL, *Note_la = NULL, *Note_si = NULL; //Initialisation des images : on crée un pointeur pour chaque image auquel on met la valeur NULL
SDL_Rect positionFond,positionNote[10000], positionNote_do,positionNote_re,positionNote_mi,positionNote_fa,positionNote_sol,positionNote_la,positionNote_si; //Initialisation des positions des images

//Initialisation positions x et y
int i; for(i=0;i<10000;i++)
        {positionNote[i].x=0; positionNote[i].y=0;} //Initialisation de TOUTES les notes du morceau

positionNote_do.y = positionNote_re.y = positionNote_mi.y = positionNote_fa.y = positionNote_sol.y = positionNote_la.y = positionNote_si.y = 658;
positionNote_do.x = 265;
positionNote_re.x = 408;
positionNote_mi.x = 548;
positionNote_fa.x = 690;
positionNote_sol.x = 834;
positionNote_la.x = 973;
positionNote_si.x = 1117;
positionFond.x = 0;
positionFond.y = 0;


imageDeFond = SDL_LoadBMP("images/fond.bmp");//on indique ou est l'image de fond


//Images
Note = SDL_LoadBMP("images/notes/note.bmp") ;
Note_do = SDL_LoadBMP("images/notes/do.bmp");
Note_re = SDL_LoadBMP("images/notes/re.bmp");
Note_mi = SDL_LoadBMP("images/notes/mi.bmp");
Note_fa = SDL_LoadBMP("images/notes/fa.bmp");
Note_sol = SDL_LoadBMP("images/notes/sol.bmp");
Note_la = SDL_LoadBMP("images/notes/la.bmp");
Note_si = SDL_LoadBMP("images/notes/si.bmp");
SDL_SetColorKey(Note, SDL_SRCCOLORKEY, SDL_MapRGB(Note->format, 0,0, 255));//transparence
//

 FILE* fichier = NULL;//pour lire la "partition"




/***********Musique***************
*********************************/

FMOD_SYSTEM *system;
FMOD_SOUND *musique;//musique :/
FMOD_RESULT resultat;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);

    resultat = FMOD_System_CreateSound(system, "musiques/FrereJacques.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/FrereJacques.txt", "r"); //Le fichier texte qui contient la "partition"



   if (resultat != FMOD_OK)//verification que la musique marche
    {
        fprintf(stderr, "Impossible de lire pan.wav\n");
        exit(EXIT_FAILURE);
    }


 /****************************
 Initialisation pour le texte
 ******************************/
    char caracteres[20] = ""; // Tableau de char suffisamment grand pour le score
    TTF_Init(); //Initialisation de la banque de donnée pour le texte
    SDL_Surface *score = NULL;
    SDL_Rect position;
    TTF_Font *police = NULL; //TTF_OpenFont doit stocker son résultat dans une variable de type TTF_Font
    SDL_Color couleurNoire = {0, 0, 0}; //couleur police => noir
    police = TTF_OpenFont("police.ttf", 70);//police choisie, taille police
//////////////////////////////////////////////

 /***************************************************
        +Lecture du fichier texte (Yacine)
        +On met les notes a leur place (do,ré...)
            en fonction de la difficulté
 **************************************************/


char chaine[TAILLE_MAX]="";
int debut[TAILLE_MAX ];
int note[TAILLE_MAX ];
int tempsFin = 0 ;
 int j=0;
 while (fgets(chaine, TAILLE_MAX, fichier) != NULL) //tant que le fichier n'a pas été totalement parcouru (fgets s'incremente automatiquement)
   {

            //On prend toutes les notes
                    sscanf(chaine, "%d - %d", &debut[j], &note[j]); // recupere la note et la date

switch (note[j])//On met en place les notes
{

   case 0 : //do
        positionNote[j].x = 250;
        break;
   case 1 : //ré
        positionNote[j].x = 395;
        break;
   case 2 : //mi
        positionNote[j].x = 525;
        break;
   case 3 : //fa
        positionNote[j].x = 680;
        break;
   case 4 : //sol
        positionNote[j].x = 820;
        break;
   case 5 : //la
        positionNote[j].x = 960;
        break;
   case 6 : //si
        positionNote[j].x = 1100;
        break;
    case 8 : // Pour la fin
        tempsFin = debut[j];
        positionNote[j].x = 20000;//pour pas afficher la note
   default :
        break;

        }
        j++;



   }


 //////////////////////////////////////////////
int k=0;//notes 1,2,3....
int tempsDebut=SDL_GetTicks();//SDL_GetTicks donne le temps qu'il s'est écoulé depuis le lancement du programme, on retire donc le temps qu'il s'est écoulé entre le lancement et le début du morceau
int tempsPrecedent = 0, tempsActuel = 0; //Timer (temps note permet de savoir a quel instant t la note a été jouée
int continuer = 1;

 //Boucle jeu (ici démo!)
while (continuer)
{


/* On joue la musique au bon moment de manière à ce qu'elle soit synchronisée avec les notes qui défilent */
if ((tempsActuel>=2700)&&(tempsActuel<=2750))FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0,NULL);


    /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On remet le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

tempsActuel = SDL_GetTicks() - tempsDebut; //temps




/************************************************
 + On affiche les notes
 + On fait descendre les notes le long des lignes
    J'ai galeré pour ca !
*************************************************/


    if(tempsActuel>=debut[k+1]) k++; // passage à la note suivante



int l=k;

    do
    {
            positionNote[l].y= positionNote[l].y+tempsActuel/10*2-tempsPrecedent/10*2;//descente de la note en utilisant le temps comme réference (elle met du coup 2.7sec a desendre)

            if (positionNote[l].y>575) positionNote[l].y=10000; // si la note arrive en bas on la fait "disparaitre"

        SDL_BlitSurface(Note, NULL, ecran, &positionNote[l]);//on affiche les notes
        l--;
    }while(l>=0);



///La boucle do while est la car il faut la faire au moins une fois quand il n'y a qu'un seule note
///Pour afficher les notes précédentes (sinon on a qu'une seule note affichée)


/*Pour sortir du morceau à la fin*/
if (tempsActuel >= tempsFin) continuer = 0;



///


tempsPrecedent = tempsActuel; // comme on utilise le temps pour la boucle d'avant on le met ici (on pourrait le mettre à la toute fin, ce qui serait plus logique mais ici on comprend mieux)





SDL_PollEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: /* Si appui sur une touche */

    switch(event.key.keysym.sym)
    {

        case SDLK_ESCAPE:
        continuer=0;
        break;
        default:
        break;
    }
break;

}

l=k;//on teste toutes les notes qui on étées affichées

    do
    {
        //démonstration : on fait un perfect
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 250))  SDL_BlitSurface(Note_do, NULL, ecran, &positionNote_do);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 395))  SDL_BlitSurface(Note_re, NULL, ecran, &positionNote_re);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 525))  SDL_BlitSurface(Note_mi, NULL, ecran, &positionNote_mi);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 680))  SDL_BlitSurface(Note_fa, NULL, ecran, &positionNote_fa);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 820))  SDL_BlitSurface(Note_sol, NULL, ecran, &positionNote_sol);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 960))  SDL_BlitSurface(Note_la, NULL, ecran, &positionNote_la);
        if ((positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 1100)) SDL_BlitSurface(Note_si, NULL, ecran, &positionNote_si);

        l--;
    }while(l>=0); //j'aime bien les do while (mêmes raisons qu'au dessus)

/***********************************
Fonction pour le texte, ici le score
et le nom du morceau
************************************/

//En cas d'erreur (plus propre)
if(TTF_Init() == -1)
{

    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
}


/*titre*/


       sprintf(caracteres, "Frère Jacques");


     SDL_FreeSurface(score);//On efface la surface précédente (sinon ca prend 2go de RAM)
     score = TTF_RenderText_Solid(police, caracteres, couleurNoire);

    //Position score//
        position.x = 20;
        position.y = 60;
        SDL_BlitSurface(score, NULL, ecran, &position); /* Blit du texte*/


////////////////////////////////////////////////////////////////////////////////////////

/* On met à jour l'affichage */
SDL_Flip(ecran);

}

///////////////////////////////////////////
/*Sortie de boucle*/
/////////////////////////////////////////
    //images
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(Note);
SDL_FreeSurface(Note_do);
SDL_FreeSurface(Note_re);
SDL_FreeSurface(Note_mi);
SDL_FreeSurface(Note_fa);
SDL_FreeSurface(Note_sol);
SDL_FreeSurface(Note_la);
SDL_FreeSurface(Note_si);
    //pour la musique
FMOD_Sound_Release(musique);
FMOD_System_Close(system);
FMOD_System_Release(system);

}

