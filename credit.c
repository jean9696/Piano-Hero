#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>

/***************************************************************************************************************************************************************

                                FONCTION CREDIT

        -------------------------------------------------------------------

    On affiche l'image de fond sur lequel on fait monter les noms avec la
    meme fonction que celle qui fait descendre les notes (en fonction du temps)
    On joue aussi de la musique

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Aurele

****************************************************************************************************************************************************************/

void credit(SDL_Surface* ecran)
{

SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Noms = NULL;
SDL_Rect positionFond,positionNoms;

positionFond.x = 0;
positionFond.y = 0;
positionNoms.x = 400;
positionNoms.y = 900;

imageDeFond = SDL_LoadBMP("images/credit.bmp");//on indique ou est l'image de fond
Noms = SDL_LoadBMP("images/noms.bmp");
SDL_SetColorKey(Noms, SDL_SRCCOLORKEY, SDL_MapRGB(Noms->format, 0,0, 255));//transparence


    /*Musique*/
FMOD_SYSTEM *system;
FMOD_SOUND *musique;
FMOD_RESULT resultat;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
/* On ouvre la musique */
resultat = FMOD_System_CreateSound(system, "musique.mp3",
FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);

/* On joue la musique */
FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0,NULL);

int tempsPrecedent = 0, tempsActuel = 0; //Timer
int tempsDebut=SDL_GetTicks();//SDL_GetTicks donne le temps qu'il s'est écoulé depuis le lancement du programme, on retire donc le temps qu'il s'est écoulé entre le lancement et le début du morceau
int continuer = 1;


while (continuer)
    {

            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On remet le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

tempsActuel = SDL_GetTicks() - tempsDebut; //temps

 if (positionNoms.y>30) positionNoms.y= positionNoms.y-tempsActuel/20+tempsPrecedent/20; // Si le crédit n'est toujours pas en haut, faire remonter (meme principe que pour la note : on utilise le temps)
 tempsPrecedent = tempsActuel;
SDL_BlitSurface(Noms, NULL, ecran, &positionNoms);

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
SDL_FreeSurface(Noms);
//pour la musique
FMOD_Sound_Release(musique);
FMOD_System_Close(system);
FMOD_System_Release(system);
}

