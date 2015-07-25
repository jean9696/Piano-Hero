#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL.h>
#include <fmod.h>


/***************************************************************************************************************************************************************

                                FONCTION INTRO

        -------------------------------------------------------------------

    On affiche une image d'intro et on joue de la musique puis après 3sec on affiche l'image avec
    écrit PIANO HERO avec le texte qui clignote en fondu
    On attend un evenement (souris ou clavier) si l'evenement est fait
     on sort de la fonction (on lance donc l'ecran d'accueil)

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/

void intro(SDL_Surface* ecran)
{

SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface*Intro = NULL,*Intro1 = NULL,*Appuie = NULL;
SDL_Rect positionIntro, positionAppuie;
positionIntro.x = 0; positionIntro.y = 0; positionAppuie.x = 370; positionAppuie.y = 680; // positions
Intro = SDL_LoadBMP("images/intro.bmp");  Intro1 = SDL_LoadBMP("images/intro1.bmp"); Appuie = SDL_LoadBMP("images/appuie.bmp"); //chargement des images
int tempsActuel = 0;
int alpha = SDL_ALPHA_OPAQUE, i=0 ; //clignoter (en transparence)

     /*Musique*/
FMOD_SYSTEM *system;
FMOD_SOUND *musique;
FMOD_RESULT resultat;
FMOD_System_Create(&system);
FMOD_System_Init(system, 1, FMOD_INIT_NORMAL, NULL);
/* On ouvre la musique */
resultat = FMOD_System_CreateSound(system, "intro.mp3",
FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);

/* On joue la musique */
FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0,NULL);



int continuer = 1;


while (continuer)
    {

    tempsActuel = SDL_GetTicks(); //timer

            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir

    /*Blink (i correspond à si il a été totalement transparent ou non)*/
if (i==0) alpha=alpha-5; if (alpha ==255) i=0;
if (i==1) alpha=alpha+5; if (alpha ==0) i=1;

SDL_SetAlpha( Appuie, SDL_SRCALPHA | SDL_RLEACCEL, alpha ); //On applique la transparence

//////////////////////////////////////////////////////////////////////////////////////////


if (tempsActuel <=3000) SDL_BlitSurface(Intro1, NULL, ecran, &positionIntro);
if (tempsActuel >=3000) { SDL_BlitSurface(Intro, NULL, ecran, &positionIntro); SDL_BlitSurface(Appuie, NULL, ecran, &positionAppuie); }


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
        exit(EXIT_FAILURE);
        break;
        default:
        continuer=0;
        break;
    }
break;

case SDL_MOUSEBUTTONDOWN:
continuer=0;
break;

}


/* On met à jour l'affichage */
SDL_Flip(ecran);


    }
SDL_ShowCursor(SDL_ENABLE);//on réaffiche le curseur pour le menu
    //Libération de l'espace
        //images
SDL_FreeSurface(Intro);
SDL_FreeSurface(Intro1);
        //pour la musique
FMOD_Sound_Release(musique);
FMOD_System_Close(system);
FMOD_System_Release(system);

}




