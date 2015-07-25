#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>


/***************************************************************************************************************************************************************

                                FONCTION MSELECTION

        -------------------------------------------------------------------

    Permet de choisir le morceau, la méthode de séléction fonctionne comme pour l'acceuil
    Si on choisit "-suivant-" au lieu de sortir de la boucle on lance le deuxieme ecran de
    sélection de morceaux et si on sélectionne le morceau dans l'autre menu la valeur retournée
    sera celle retournée par MSELECTION2
        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    RETURN => La valeur qui correspond dans la fonction jeu au morceau voullu


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/
int mselection(SDL_Surface* ecran)
{

SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Selection = NULL;
SDL_Rect positionFond,positionSelection, positionsTitres [10];

positionFond.x = 0;
positionFond.y = 0;
positionSelection.x = 100;
positionSelection.y = 150;
int i=0,choix=0;
for (i=0;i<10;i++)
{
    if (i<5)
    {
    positionsTitres [i].x = 140;
    positionsTitres [i].y = 100+i*100;
    }
        if (i>=5)
    {
    positionsTitres [i].x = 700;
    positionsTitres [i].y = 100+(i-5)*100;
    }
}

imageDeFond = SDL_LoadBMP("images/credit.bmp"); Selection = SDL_LoadBMP("images/selection.bmp");//on indique ou est l'image de fond
SDL_SetColorKey(Selection, SDL_SRCCOLORKEY, SDL_MapRGB(Selection->format, 0,0, 255));//transparence
int continuer = 1,xmouse=0,ymouse=0;



/***********************************
Fonction pour le texte
************************************/
   char caracteres[10][20] = {""}; // Tableau de char suffisamment grand pour les titres(tableau a 2 dimension, ca évite de faire 10 fois la meme chose)
    TTF_Init(); //Initialisation de la banque de donnée pour le texte
    SDL_Surface *titre[10] = {NULL};
    TTF_Font *police = NULL; //TTF_OpenFont doit stocker son résultat dans une variable de type TTF_Font
    SDL_Color couleurBlanc = {255, 255, 255}; //couleur police => noir
    police = TTF_OpenFont("score.ttf", 100);//police choisie, taille police

//En cas d'erreur (plus propre)
if(TTF_Init() == -1)
{

    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
}


sprintf(caracteres[0], "Frère Jacques"); sprintf(caracteres[1], "Starlight - Muse"); sprintf(caracteres[2], "Au clair de la Lune"); sprintf(caracteres[3], "Titanic");
sprintf(caracteres[4], "Black White - MJ"); sprintf(caracteres[5], "Clocks - Coldplay"); sprintf(caracteres[6], "Laputa"); sprintf(caracteres[7], "YMCA");
sprintf(caracteres[8], "Changes"); sprintf(caracteres[9], " - Suivant - "); //tous les titres
for(i=0;i<10;i++) titre[i] = TTF_RenderText_Solid(police, caracteres[i], couleurBlanc);



while (continuer)
    {

        choix=0;//Pour pouvoir reselectionner meme si on est allé dans le menu suivant car sinon choix =14 et ca fait tout buguer

            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On remet le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
SDL_BlitSurface(Selection, NULL, ecran, &positionSelection);

for(i=0;i<10;i++) SDL_BlitSurface(titre[i], NULL, ecran, &positionsTitres[i]); /* Blit du texte*/



SDL_WaitEvent(&event);
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
        case SDLK_RETURN:
        if ((positionSelection.y==550)&&(positionSelection.x==650)) choix=mselection2(ecran) ;
        if (choix ==14);
        else continuer = 0;
        break;
        case SDLK_UP:
            if (positionSelection.y > 150) positionSelection.y=positionSelection.y-100;
        break;
        case SDLK_DOWN:
            if (positionSelection.y < 550) positionSelection.y=positionSelection.y+100;
        break;
        case SDLK_RIGHT:
            if (positionSelection.x==100) positionSelection.x=650;
        break;
        case SDLK_LEFT:
            if (positionSelection.x==650) positionSelection.x=100;
        break;
        default:
        break;
    }
break;

case SDL_MOUSEMOTION : //souris
SDL_GetMouseState( &xmouse, &ymouse );
if((ymouse<200)&&(ymouse>50)) positionSelection.y=150;
if((ymouse<300)&&(ymouse>200)) positionSelection.y=250;
if((ymouse<400)&&(ymouse>300)) positionSelection.y=350;
if((ymouse<500)&&(ymouse>400)) positionSelection.y=450;
if((ymouse<600)&&(ymouse>500)) positionSelection.y=550;
if(xmouse<400) positionSelection.x=100;
if(xmouse>400) positionSelection.x=650;
break;
case SDL_MOUSEBUTTONDOWN:
if ((positionSelection.y==550)&&(positionSelection.x==650)) choix=mselection2(ecran) ; //Pour suivant
if (choix ==14);
else continuer = 0;
break;

}




/* On met à jour l'affichage */
SDL_Flip(ecran);

    }


///////////////////////////////////////////
/*Choix morceau*/
if ((positionSelection.y==150)&&(positionSelection.x==100)) choix=0;
if ((positionSelection.y==250)&&(positionSelection.x==100)) choix=1;
if ((positionSelection.y==350)&&(positionSelection.x==100)) choix=2;
if ((positionSelection.y==450)&&(positionSelection.x==100)) choix=3;
if ((positionSelection.y==550)&&(positionSelection.x==100)) choix=4;
if ((positionSelection.y==150)&&(positionSelection.x==650)) choix=5;
if ((positionSelection.y==250)&&(positionSelection.x==650)) choix=6;
if ((positionSelection.y==350)&&(positionSelection.x==650)) choix=7;
if ((positionSelection.y==450)&&(positionSelection.x==650)) choix=8;




//Libération de l'espace
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(Selection);
return choix; //On retourne la valeur du morceau choisi
}




/***************************************************************************************************************************************************************

                                FONCTION MSELECTION2

        -------------------------------------------------------------------

        Meme fonction que MSELECTION mais retourne des valeurs differentes

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    RETURN => La valeur qui correspond dans la fonction jeu au morceau voullu

                                                                    Auteur : Jean

****************************************************************************************************************************************************************/
int mselection2(SDL_Surface* ecran)
{

SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Selection = NULL;
SDL_Rect positionFond,positionSelection, positionsTitres [10];

positionFond.x = 0;
positionFond.y = 0;
positionSelection.x = 100;
positionSelection.y = 150;
int i=0,choix=0;
for (i=0;i<10;i++)
{
    if (i<5)
    {
    positionsTitres [i].x = 140;
    positionsTitres [i].y = 100+i*100;
    }
        if (i>=5)
    {
    positionsTitres [i].x = 700;
    positionsTitres [i].y = 100+(i-5)*100;
    }
}

imageDeFond = SDL_LoadBMP("images/credit.bmp"); Selection = SDL_LoadBMP("images/selection.bmp");//on indique ou est l'image de fond
SDL_SetColorKey(Selection, SDL_SRCCOLORKEY, SDL_MapRGB(Selection->format, 0,0, 255));//transparence
int continuer = 1,xmouse=0,ymouse=0;



/***********************************
Fonction pour le texte
************************************/
   char caracteres[10][21] = {""}; // Tableau de char suffisamment grand pour les titres(tableau a 2 dimension, ca évite de faire 10 fois la meme chose)
    TTF_Init(); //Initialisation de la banque de donnée pour le texte
    SDL_Surface *titre[10] = {NULL};
    TTF_Font *police = NULL; //TTF_OpenFont doit stocker son résultat dans une variable de type TTF_Font
    SDL_Color couleurBlanc = {255, 255, 255}; //couleur police => noir
    police = TTF_OpenFont("score.ttf", 100);//police choisie, taille police

//En cas d'erreur (plus propre)
if(TTF_Init() == -1)
{

    fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
}

///////////////////////////////////////////////////////////////////////////////
//2eme page///////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
sprintf(caracteres[0], "Dancing in the Dark"); sprintf(caracteres[1], "Born to Run"); sprintf(caracteres[2], "Just the way you are"); sprintf(caracteres[3], "Bad day");
sprintf(caracteres[4], "- Retour -"); sprintf(caracteres[5], "The Fray"); sprintf(caracteres[6], "Starway to heaven"); sprintf(caracteres[7], "Naruto");
sprintf(caracteres[8], "Somebody to love"); sprintf(caracteres[9], "Viva la Vida"); //tous les titres
for(i=0;i<10;i++) titre[i] = TTF_RenderText_Solid(police, caracteres[i], couleurBlanc);



while (continuer)
    {

            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On remet le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
SDL_BlitSurface(Selection, NULL, ecran, &positionSelection);

for(i=0;i<10;i++) SDL_BlitSurface(titre[i], NULL, ecran, &positionsTitres[i]); /* Blit du texte*/



SDL_WaitEvent(&event);
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
        case SDLK_RETURN:
        continuer=0;
        break;
        case SDLK_UP:
            if (positionSelection.y > 150) positionSelection.y=positionSelection.y-100;
        break;
        case SDLK_DOWN:
            if (positionSelection.y < 550) positionSelection.y=positionSelection.y+100;
        break;
        case SDLK_RIGHT:
            if (positionSelection.x==100) positionSelection.x=650;
        break;
        case SDLK_LEFT:
            if (positionSelection.x==650) positionSelection.x=100;
        break;
        default:
        break;
    }
break;

case SDL_MOUSEMOTION : //souris
SDL_GetMouseState( &xmouse, &ymouse );
if((ymouse<200)&&(ymouse>50)) positionSelection.y=150;
if((ymouse<300)&&(ymouse>200)) positionSelection.y=250;
if((ymouse<400)&&(ymouse>300)) positionSelection.y=350;
if((ymouse<500)&&(ymouse>400)) positionSelection.y=450;
if((ymouse<600)&&(ymouse>500)) positionSelection.y=550;
if(xmouse<400) positionSelection.x=100;
if(xmouse>400) positionSelection.x=650;
break;
case SDL_MOUSEBUTTONDOWN:
continuer = 0;
break;

}




/* On met à jour l'affichage */
SDL_Flip(ecran);

    }


///////////////////////////////////////////
/*Choix morceau*/
if ((positionSelection.y==150)&&(positionSelection.x==100)) choix=10;
if ((positionSelection.y==250)&&(positionSelection.x==100)) choix=11;
if ((positionSelection.y==350)&&(positionSelection.x==100)) choix=12;
if ((positionSelection.y==450)&&(positionSelection.x==100)) choix=13;
if ((positionSelection.y==550)&&(positionSelection.x==100)) choix=14;
if ((positionSelection.y==150)&&(positionSelection.x==650)) choix=15;
if ((positionSelection.y==250)&&(positionSelection.x==650)) choix=16;
if ((positionSelection.y==350)&&(positionSelection.x==650)) choix=17;
if ((positionSelection.y==450)&&(positionSelection.x==650)) choix=18;
if ((positionSelection.y==550)&&(positionSelection.x==650)) choix=19;



//Libération de l'espace
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(Selection);
return choix; //On retourne la valeur du morceau choisi
}



/***************************************************************************************************************************************************************

                                FONCTION CLAVIER/ARDUINO

        -------------------------------------------------------------------

    On donne le choix entre jouer avec le clavier ou avec Arduino
    (on peut jouer avec le clavier quand on lance arduino)

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    RETURN => 0 si on choisit le clavier et 1 si on choisit Arduino


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/

int clavierArduino(SDL_Surface* ecran)
{
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *selectArduino = NULL, *selectClavier;
SDL_Rect positionFond,positionSelection1,positionSelection2;
positionFond.x = 0; positionFond.y = 0;
positionSelection1.x = 150; positionSelection1.y = 400; //Clavier
positionSelection2.x = 800; positionSelection2.y = 400; //Arduino
imageDeFond = SDL_LoadBMP("images/clavierArduino.bmp"); selectArduino = SDL_LoadBMP("images/selectArduino.bmp"); selectClavier = SDL_LoadBMP("images/selectClavier.bmp");//images
SDL_SetColorKey(selectClavier, SDL_SRCCOLORKEY, SDL_MapRGB(selectClavier->format, 0,0, 255));//transparence
SDL_SetColorKey(selectArduino, SDL_SRCCOLORKEY, SDL_MapRGB(selectArduino->format, 0,0, 255));//transparence

int continuer = 1,xmouse=0,ymouse=0,choix = 0;


while (continuer)
    {


SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir

SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);

if (choix)  SDL_BlitSurface(selectArduino, NULL, ecran, &positionSelection2);
else  SDL_BlitSurface(selectClavier, NULL, ecran, &positionSelection1);

SDL_WaitEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: // clavier

    switch(event.key.keysym.sym)
    {
        case SDLK_RETURN:
        continuer=0;
        break;
        case SDLK_RIGHT:
        choix = 1;
        break;
        case SDLK_LEFT:
        choix = 0;
        break;
        default:
        break;
    }
break;

case SDL_MOUSEMOTION : //souris
SDL_GetMouseState( &xmouse, &ymouse );
if(xmouse<600) choix = 0;
else choix = 1;
break;
case SDL_MOUSEBUTTONDOWN:
continuer = 0;
break;

}




 //On met à jour l'affichage
SDL_Flip(ecran);

    }


///////////////////////////////////////////



//Libération de l'espace
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(selectClavier);
SDL_FreeSurface(selectArduino);
return choix; //On retourne la valeur du morceau choisi




}

/***************************************************************************************************************************************************************

                                FONCTION DIFFICULTE

        -------------------------------------------------------------------

    On donne le choix entre jouer en mode facile, normal ou difficile
                        (1/3 des notes,1/2 des notes, toutes les notes)

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    RETURN => 0 pour facile, 1 pour normal et 2 pour difficile


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/
int difficult(SDL_Surface* ecran)
{
SDL_Event event; //pour pouvoir gerer les events
SDL_Surface *imageDeFond = NULL, *Selection = NULL;
SDL_Rect positionFond,positionSelection;
positionFond.x = 0;
positionFond.y = 0;
positionSelection.x = 370;
positionSelection.y = 400;
imageDeFond = SDL_LoadBMP("images/difficult.bmp"); Selection = SDL_LoadBMP("images/selection.bmp");//on indique ou est l'image de fond
SDL_SetColorKey(Selection, SDL_SRCCOLORKEY, SDL_MapRGB(Selection->format, 0,0, 255));//transparence

int continuer = 1,xmouse=0,ymouse=0,difficulte = 1;



while (continuer)
    {

            /* On efface l'écran */
SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255,255, 255));//255,255, 255 veut dire qu'on met un ecran noir
    /* On remet le fond */
SDL_BlitSurface(imageDeFond, NULL, ecran, &positionFond);
SDL_BlitSurface(Selection, NULL, ecran, &positionSelection);



SDL_WaitEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: // clavier

    switch(event.key.keysym.sym)
    {
        case SDLK_RETURN:
        continuer=0;
        break;
        case SDLK_UP:
            if (positionSelection.y > 300) positionSelection.y=positionSelection.y-170;
        break;
        case SDLK_DOWN:
            if (positionSelection.y < 500) positionSelection.y=positionSelection.y+170;
        break;
        default:
        break;
    }
break;

case SDL_MOUSEMOTION : //souris
SDL_GetMouseState( &xmouse, &ymouse );
if((ymouse<300)&&(ymouse>50)) positionSelection.y=230;
if((ymouse<450)&&(ymouse>300)) positionSelection.y=400;
if((ymouse<600)&&(ymouse>450)) positionSelection.y=570;
break;
case SDL_MOUSEBUTTONDOWN:
continuer = 0;
break;

}

if (positionSelection.y==230) positionSelection.x = 400;
else positionSelection.x = 370;


/* On met à jour l'affichage */
SDL_Flip(ecran);

    }


///////////////////////////////////////////
/*Choix difficulté*/
if (positionSelection.y==230) difficulte=0;
if (positionSelection.y==400) difficulte=1;
if (positionSelection.y==570) difficulte=2;


//Libération de l'espace
SDL_FreeSurface(imageDeFond);
SDL_FreeSurface(Selection);
return difficulte; //On retourne la valeur du morceau choisi




}

