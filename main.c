#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <fmod.h>



#include "intro.c" //Allez savoir pourquoi lorqu'on met les autres .c dans le projet ca plante
#include "selection.c"
#include "credit.c"
#include "tuto.c"
#include "score.c"
#include "jeu.c"

void accueil(SDL_Surface* ecran); void merci(SDL_Surface* ecran); //Prototypes des fontions menu et merci (pas utile de faire un .h)



/***************************************************************************************************************************************************************

                                MAIN

        -------------------------------------------------------------------

    - On crée la fenetre de 1250x833
    - On associe l'icone
    - On donne le nom au programme
    - On lance l'intro puis le menu d'accueil
    - Lorsque l'on quitte le menu d'accueil et donc le jeu on affiche "merci d'avoir joué" et on ferme la fenêtre


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/


int main(int argc, char *argv[]) //On initialise juste la fenetre
{
    SDL_Surface *ecran = NULL; //init ecran
    ecran = SDL_SetVideoMode(1250, 833, 16, SDL_HWSURFACE|SDL_DOUBLEBUF); // La fenêtre sera de 1250/833 pixels en 16bits
    SDL_WM_SetIcon(SDL_LoadBMP("images/icone.bmp"), NULL);//icone
    SDL_WM_SetCaption("Piano Hero 2.0", NULL);//nom de la fenetre
    intro(ecran);
    accueil(ecran);
    merci(ecran);
    SDL_Quit();
    return 0;
}


/***************************************************************************************************************************************************************

                                FONCTION ACCUEIL

        -------------------------------------------------------------------

    Affiche l'image de menu
    On a le choix entre 3 possibilités/fonctions
    On peut utiliser la souris et le clavier pour naviguer dans le menu,
        pour cela on a un curseur, on utilise sa position pour savoir quelle
        fonction lancer

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/


void accueil(SDL_Surface* ecran)
{
    //Menu
    SDL_Surface*menu = NULL,*selection=NULL;

    SDL_Event event;
    SDL_Rect positionMenu,positionSelection;
    positionMenu.x = 0; positionMenu.y = 0; positionSelection.x = 15; positionSelection.y = 100;
    menu = SDL_LoadBMP("images/menu.bmp"); selection = SDL_LoadBMP("images/selection.bmp");
    SDL_SetColorKey(selection, SDL_SRCCOLORKEY, SDL_MapRGB(selection->format, 0,0, 255));//transparence
    int continuer = 1,xmouse=0,ymouse=0,compteur=0,choix=0,difficulte=1,ArduinoClavier=0;
    while (continuer)
    {


SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));
SDL_BlitSurface(menu, NULL, ecran, &positionMenu); SDL_BlitSurface(selection, NULL, ecran, &positionSelection);
SDL_WaitEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: // Si appui sur une touche du clavier

    switch(event.key.keysym.sym)
    {
        case SDLK_UP:
            if (positionSelection.y > 100) positionSelection.y=positionSelection.y-100;
        break;
        case SDLK_DOWN:
            if (positionSelection.y < 300) positionSelection.y=positionSelection.y+100;
        break;
        case SDLK_ESCAPE:
            continuer=0;
        break;
        case SDLK_RETURN:
            if(positionSelection.y==100) {
                choix = mselection(ecran);
                difficulte=difficult(ecran);
                ArduinoClavier=clavierArduino(ecran);
                compteur=jeu(ecran,choix,difficulte,ArduinoClavier);
                score(ecran,compteur);}//on lance la partie puis on affiche le score
            if(positionSelection.y==200) tuto(ecran);
            if(positionSelection.y==300) credit(ecran);
        break;
        default:
        break;
    }
break;

case SDL_MOUSEMOTION : //souris
SDL_GetMouseState( &xmouse, &ymouse );
if((xmouse<400)&&(ymouse<150)&&(ymouse>50)) positionSelection.y=100;
if((xmouse<400)&&(ymouse<250)&&(ymouse>150)) positionSelection.y=200;
if((xmouse<400)&&(ymouse<350)&&(ymouse>250)) positionSelection.y=300;
break;
case SDL_MOUSEBUTTONDOWN:
if (positionSelection.y==100) {
                choix = mselection(ecran);
                difficulte=difficult(ecran);
                ArduinoClavier=clavierArduino(ecran);
                compteur=jeu(ecran,choix,difficulte,ArduinoClavier);
                score(ecran,compteur);}//on lance la partie puis on affiche le score
if (positionSelection.y==200) tuto(ecran);
if (positionSelection.y==300) credit(ecran);

break;
}



SDL_Flip(ecran);
    }
}


/***************************************************************************************************************************************************************

                                FONCTION MERCI

        -------------------------------------------------------------------

    Affiche l'image avec "merci d'avoir joué" pendant 1sec

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main


                                                                    Auteur : Aurele

****************************************************************************************************************************************************************/
void merci(SDL_Surface* ecran)
{
    SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
    SDL_Surface *imageDeFond = NULL;
    imageDeFond = SDL_LoadBMP("images/merci.bmp");//on indique ou est l'image de fond
    SDL_Rect positionFond;
    positionFond.x = 0;
    positionFond.y = 0;

    //Timer
int tempsDebut=SDL_GetTicks();//SDL_GetTicks donne le temps qu'il s'est écoulé depuis le lancement du programme, on retire donc le temps qu'il s'est écoulé entre le lancement de la fonction
int tempsActuel=0;
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
SDL_Flip(ecran);

int continuer = 1;
while (continuer)
    {

tempsActuel = SDL_GetTicks() - tempsDebut; //temps
if (tempsActuel >= 1000) continuer=0;

    }


}


