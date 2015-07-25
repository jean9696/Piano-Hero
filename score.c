#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>

/***************************************************************************************************************************************************************

                                FONCTION SCORE

        -------------------------------------------------------------------

    On affiche le score pendant 4sec (pourcentage de réussite) et on applaudit si le score est bon !
    Si le score est inférieur à 50 on ne dit pas bravo et on applaudit pas
    Si le score est supérieur à 50 on dit bravo et on applaudit !

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    compteur : le pourcentage de réussite donnée par la fonction jeu


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/
void score(SDL_Surface* ecran,int compteur)
{
    /**applaudissements (parce que c'est marrant)**/

      /*Musique*/
FMOD_SYSTEM *system;
FMOD_SOUND *musique;
FMOD_RESULT resultat;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
/* On ouvre la musique */
if (compteur >50) resultat = FMOD_System_CreateSound(system, "score.mp3",FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);

/* On joue la musique */
FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0,NULL);


//Timer
int tempsDebut=SDL_GetTicks();//SDL_GetTicks donne le temps qu'il s'est écoulé depuis le lancement du programme, on retire donc le temps qu'il s'est écoulé entre le lancement de la fonction
int tempsActuel=0;

SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL;
SDL_Rect positionFond;

positionFond.x = 0;
positionFond.y = 0;

imageDeFond = SDL_LoadBMP("images/credit.bmp");//on indique ou est l'image de fond

   /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On met le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

/***********************************
Fonction pour le texte
************************************/
   char caracteres[20] = ""; // Tableau de char suffisamment grand pour le score
    TTF_Init(); //Initialisation de la banque de donnée pour le texte
    SDL_Surface *score = NULL;
    SDL_Rect position;
    TTF_Font *police = NULL; //TTF_OpenFont doit stocker son résultat dans une variable de type TTF_Font
    SDL_Color couleurBlanc = {255, 255, 255}; //couleur police => Blanc
    police = TTF_OpenFont("score.ttf", 125);//police choisie, taille police

//En cas d'erreur (plus propre)
if(TTF_Init() == -1)
{

    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
}

/*score*/


 //Position score//
        position.x = 100;
        position.y = 250;
     /* Écriture du texte dans la SDL_Surface texte en mode Solid (car il change souvent)(pas la mais osef)*/
     if (compteur >25) sprintf(caracteres, "Bravo ! Tu as fait %d %% des notes", compteur);
     if (compteur <=25) {
         sprintf(caracteres, "Tu n'as fait que %d %%...", compteur);
          position.x = 300;}

     score = TTF_RenderText_Solid(police, caracteres, couleurBlanc);


        SDL_BlitSurface(score, NULL, ecran, &position); /* Blit du texte*/










int continuer = 1;
while (continuer)
    {

tempsActuel = SDL_GetTicks() - tempsDebut; //temps
if (tempsActuel >= 4000) continuer=0;

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




/* On met à jour l'affichage */
SDL_Flip(ecran);

    }


///////////////////////////////////////////
/*Sortie de boucle*/
SDL_ShowCursor(SDL_ENABLE);//on réaffiche le curseur pour le menu
//Libération de l'espace
SDL_FreeSurface(imageDeFond);
  //pour la musique
FMOD_Sound_Release(musique);
FMOD_System_Close(system);
FMOD_System_Release(system);

}


