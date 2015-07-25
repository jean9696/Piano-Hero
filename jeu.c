#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <fmod.h>
#include <SDL/SDL_ttf.h>
#include <string.h>

#define TAILLE_MAX 10000 // Tableau de taille 10 000
#include <windows.h>
#include <conio.h>




/*=============================================================================
  Définition de constantes
=============================================================================*/
#define RX_SIZE         4096    /* taille tampon d'entrée  */
#define TX_SIZE         4096    /* taille tampon de sortie */
#define MAX_WAIT_READ   5000    /* temps max d'attente pour lecture (en ms) */


/*=============================================================================
  Variables globales.
=============================================================================*/
/* Handle du port COM ouvert */
HANDLE g_hCOM = NULL;

/* Délais d'attente sur le port COM */
COMMTIMEOUTS g_cto =
{
    MAX_WAIT_READ,  /* ReadIntervalTimeOut          */
    0,              /* ReadTotalTimeOutMultiplier   */
    MAX_WAIT_READ,  /* ReadTotalTimeOutConstant     */
    0,              /* WriteTotalTimeOutMultiplier  */
    0               /* WriteTotalTimeOutConstant    */
};

/* Configuration du port COM */
DCB g_dcb =
{
    sizeof(DCB),        /* DCBlength            */
    9600,               /* BaudRate             */
    TRUE,               /* fBinary              */
    FALSE,              /* fParity              */
    FALSE,              /* fOutxCtsFlow         */
    FALSE,              /* fOutxDsrFlow         */
    DTR_CONTROL_ENABLE, /* fDtrControl          */
    FALSE,              /* fDsrSensitivity      */
    FALSE,              /* fTXContinueOnXoff    */
    FALSE,              /* fOutX                */
    FALSE,              /* fInX                 */
    FALSE,              /* fErrorChar           */
    FALSE,              /* fNull                */
    RTS_CONTROL_ENABLE, /* fRtsControl          */
    FALSE,              /* fAbortOnError        */
    0,                  /* fDummy2              */
    0,                  /* wReserved            */
    0x100,              /* XonLim               */
    0x100,              /* XoffLim              */
    8,                  /* ByteSize             */
    NOPARITY,           /* Parity               */
    ONESTOPBIT,         /* StopBits             */
    0x11,               /* XonChar              */
    0x13,               /* XoffChar             */
    '?',                /* ErrorChar            */
    0x1A,               /* EofChar              */
    0x10                /* EvtChar              */
};

/*=============================================================================
  Fonctions du module.
=============================================================================*/
BOOL OpenCOM    (int nId);
BOOL CloseCOM   ();
BOOL ReadCOM    (void* buffer, int nBytesToRead, int* pBytesRead);

/***************************************************************************************************************************************************************

                                FONCTION ARDUINO

        -------------------------------------------------------------------

Lorsque elle est appelée avec le parametre 1, cette fonction permet  d'ouvir le port serie.
Puis elle permet de récuperer la chaine de caractere envoyée par arduino, de la transformer  en un nombre et de renvoyer ce nombre a la fonction jeu.

                                                                    Auteur : Yacine Saoudi

****************************************************************************************************************************************************************/
int arduino(int verif)
{


    char buffer[30];  //permettra de stocker la chaine de caracacteres envoyée par la carte arduino
    int nId=0; //numero du port COM  a ouvrir (varie en fonction du pc ou du port usb utilisé
    int succes=0;
    int nBytesRead = 0;
/* ouvrir le port COM est assez long (2-3 secondes), on doit donc l'ouvrir une seule fois (au debut du jeu).
On n'appelle la fonction arduino avec 1 comme parametre que la premiere fois, pour ouvrir le port COM.
 la partie suivante ne s'execute donc qu'une seule fois */
if (verif==1)
{
   while(succes == 0)
  {
     if(!OpenCOM(nId)) //si le port COM ne s'ouvre pas, on incremente nId et on tentera donc d'ouvrir le port COM suivant au prochain tour de boucle
     {
        nId++;
     }
     else
     {
        succes=1; //si le port est ouvert on met fin a la boucle
     }


     if(nId>20) //si on a deja essayé les 20 premiers ports series, on abandonnne car aucune manette n'est branchée.
     {
        succes=1;
     }

  }
}

        int buffernum = 0;


            if(ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead)) //recupere la chaine de caractere envoyée par l'arduino dans buffer
           {
           buffernum=atoi(buffer); //permet de transformer le buffer (chaine de caractère) en un int afin de pouvoir le renvoyer a la fonction jeu directement.
           }
           else
           {
               // printf("Erreur lors de la réception.\r\n");
           }

           return(buffernum);
   }




/******************************************************************************
  OpenCOM : ouverture et configuration du port COM.
  entrée : nId : Id du port COM à ouvrir.
  retour : vrai si l'opération a réussi, faux sinon.
******************************************************************************/
BOOL OpenCOM(int nId)
{
    /* variables locales */
    char szCOM[16];

    /* construction du nom du port, tentative d'ouverture */
    sprintf(szCOM, "COM%d", nId);
    g_hCOM = CreateFile(szCOM, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
    if(g_hCOM == INVALID_HANDLE_VALUE)
    {
       // printf("Erreur lors de l'ouverture du port COM%d", nId);
        return FALSE;
    }

    /* affectation taille des tampons d'émission et de réception */
    SetupComm(g_hCOM, RX_SIZE, TX_SIZE);

    /* configuration du port COM */
    if(!SetCommTimeouts(g_hCOM, &g_cto) || !SetCommState(g_hCOM, &g_dcb))
    {
        //printf("Erreur lors de la configuration du port COM%d", nId);
        CloseHandle(g_hCOM);
        return FALSE;
    }

    /* on vide les tampons d'émission et de réception, mise à 1 DTR */
    PurgeComm(g_hCOM, PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
    EscapeCommFunction(g_hCOM, SETDTR);
    return TRUE;
}

/******************************************************************************
  CloseCOM : fermeture du port COM.
  retour : vrai si l'opération a réussi, faux sinon.
******************************************************************************/
BOOL CloseCOM()
{
    /* fermeture du port COM */
    CloseHandle(g_hCOM);
    return TRUE;
}

/******************************************************************************
  ReadCOM : lecture de données sur le port COM.
  entrée : buffer       : buffer où mettre les données lues.
           nBytesToRead : nombre max d'octets à lire.
           pBytesRead   : variable qui va recevoir le nombre d'octets lus.
  retour : vrai si l'opération a réussi, faux sinon.
-------------------------------------------------------------------------------
  Remarques : - la constante MAX_WAIT_READ utilisée dans la structure
                COMMTIMEOUTS permet de limiter le temps d'attente si aucun
                caractères n'est présent dans le tampon d'entrée.
              - la fonction peut donc retourner vrai sans avoir lu de données.
******************************************************************************/
BOOL ReadCOM(void* buffer, int nBytesToRead, int* pBytesRead)
{
    return ReadFile(g_hCOM, buffer, nBytesToRead, pBytesRead, NULL);
}





/***************************************************************************************************************************************************************

                                FONCTION JEU

        -------------------------------------------------------------------

    Fait tourner le jeu :
        = Lance la musique
        = Lit la "partition"
        =Boucle jeu :
            = Fais descendre les notes en fonction du temps et grâce au temps (SDL_GetTicks)
            = Gestion des evenements (arduino si on a selectionné la possibilité de jouer avec + clavier)
            = Gestion echec réussite
            = Affiche texte (nom morceau + score)
        = Comptabilise le pourcentage de notes réussies

        -------------------------------------------------------------------
    ecran : fenêtre initialisée dans le main
    choix : choix du morceau
    difficulté : choix de la difficulté
    ArduinoClavier : jouer avec le clavier ou avec Arduino
    RETURN => Pourcentage de réussite


                                                                    Auteur : Jean

****************************************************************************************************************************************************************/

int jeu(SDL_Surface* ecran,int choix,int difficulte,int ArduinoClavier)
{





SDL_Event event; //pour pouvoir gerer les events
SDL_ShowCursor(SDL_DISABLE);//On n'affiche plus le curseur
SDL_Surface *imageDeFond = NULL, *Note = NULL, *Note_do = NULL, *Note_re = NULL, *Note_mi = NULL, *Note_fa = NULL, *Note_sol = NULL, *Note_la = NULL, *Note_si = NULL; //Initialisation des images : on crée un pointeur pour chaque image auquel on met la valeur NULL
SDL_Rect positionFond,positionNote[TAILLE_MAX], positionNote_do,positionNote_re,positionNote_mi,positionNote_fa,positionNote_sol,positionNote_la,positionNote_si; //Initialisation des positions des images

//Initialisation positions x et y
int i; for(i=0;i<TAILLE_MAX;i++)
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

/* On ouvre la musique en fonction du choix et on ouvre la partition qui correspond (c'est gros pour pas grand chose)*/
switch(choix)
{
    case 0 :
    resultat = FMOD_System_CreateSound(system, "musiques/FrereJacques.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/FrereJacques.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 1 :
    resultat = FMOD_System_CreateSound(system, "musiques/Muse.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Muse.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 2 :
    resultat = FMOD_System_CreateSound(system, "musiques/AuClairDeLaLune.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/AuClairDeLaLune.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 3 :
    resultat = FMOD_System_CreateSound(system, "musiques/Titanic.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Titanic.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 4 :
    resultat = FMOD_System_CreateSound(system, "musiques/MJ.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/MJ.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 5 :
    resultat = FMOD_System_CreateSound(system, "musiques/Clocks.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Clocks.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 6 :
    resultat = FMOD_System_CreateSound(system, "musiques/Laputa.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Laputa.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 7 :
    resultat = FMOD_System_CreateSound(system, "musiques/YMCA.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/YMCA.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 8 :
    resultat = FMOD_System_CreateSound(system, "musiques/Changes.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Changes.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 10 :
    resultat = FMOD_System_CreateSound(system, "musiques/Dancing_in_the_Dark.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Dancing_in_the_Dark.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 11 :
    resultat = FMOD_System_CreateSound(system, "musiques/Born_to_Run.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Born_to_Run.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 12 :
    resultat = FMOD_System_CreateSound(system, "musiques/Bruno Mars.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Bruno Mars.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 13 :
    resultat = FMOD_System_CreateSound(system, "musiques/Bad day.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Bad day.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 15 :
    resultat = FMOD_System_CreateSound(system, "musiques/The Fray.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/The Fray.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 16 :
    resultat = FMOD_System_CreateSound(system, "musiques/Led Zep.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Led Zep.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 17 :
    resultat = FMOD_System_CreateSound(system, "musiques/Naruto.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Naruto.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 18 :
    resultat = FMOD_System_CreateSound(system, "musiques/Somebody to love - Queen.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Somebody to love - Queen.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    case 19 :
    resultat = FMOD_System_CreateSound(system, "musiques/Viva.mid", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &musique);
    fichier = fopen("musiques/Viva.txt", "r"); //Le fichier texte qui contient la "partition"
    break;
    default:
    break;

}


   if (resultat != FMOD_OK)//verification que la musique marche
    {
        fprintf(stderr, "Impossible de lire le fichier audio.wav\n");
        exit(EXIT_FAILURE);
    }

//Bruit pour le fail (on est obligé de créer un autre systeme pour pas arreter la musique)
FMOD_SYSTEM *systemf;
FMOD_SOUND *fail = NULL;
FMOD_System_CreateSound(systemf, "fail.wav", FMOD_CREATESAMPLE, 0, &fail);
FMOD_RESULT resultatf;

    /* Création et initialisation d'un objet système */
    FMOD_System_Create(&systemf);
    FMOD_System_Init(systemf, 1, FMOD_INIT_NORMAL, NULL);

    /* Chargement du son et vérification du chargement */
    resultatf = FMOD_System_CreateSound(system, "fail.wav", FMOD_CREATESAMPLE, 0, &fail);
    if (resultatf != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier audio.wav\n");
        exit(EXIT_FAILURE);
    }

 /****************************
 Initialisation pour le texte
 ******************************/
    char caracteres[20] = "",caracteres2[20] = ""; // Tableau de char suffisamment grand pour le score
    TTF_Init(); //Initialisation de la banque de donnée pour le texte
    int compteur=0; //Pour le score
    SDL_Surface *score = NULL;
    SDL_Rect position;
    TTF_Font *police = NULL, *police2 = NULL; //TTF_OpenFont doit stocker son résultat dans une variable de type TTF_Font
    SDL_Color couleurNoire = {0, 0, 0}; //couleur police => noir
    police = TTF_OpenFont("police.ttf", 70);//police choisie, taille police
    police2 = TTF_OpenFont("score.ttf", 70);//police choisie, taille police
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
 int j=0,compteurNotes=0;
 while (fgets(chaine, TAILLE_MAX, fichier) != NULL) //tant que le fichier n'a pas été totalement parcouru (fgets s'incremente automatiquement)
   {

        ///Easy
       if (difficulte==0){
    if (compteurNotes%3==0)//Pour la difficulté, on ne prend qu'une note sur 3 quand on a choisi l'option facile
            {
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
            compteurNotes++;
            }

        ///Normal
       if (difficulte==1){
    if (compteurNotes%2==0)//Pour la difficulté, on ne prend qu'une note sur deux quand on a choisi l'option facile
            {
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
            compteurNotes++;
            }

        ///Difficile
        if (difficulte==2){
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


   }


 //////////////////////////////////////////////

int pourcent[TAILLE_MAX] = {0},pourcentFinal=0,totalNotes=0;//Pour le pourcentage de réussite
int k=0;//notes 1,2,3....
int tempsDebut=SDL_GetTicks();//SDL_GetTicks donne le temps qu'il s'est écoulé depuis le lancement du programme, on retire donc le temps qu'il s'est écoulé entre le lancement et le début du morceau
int a=0,z=0,e=0,r=0,t=0,y=0,u=0;//notes
int tempsPrecedent = 0, tempsActuel = 0, tempsNote[7]; //Timer (temps note permet de savoir a quel instant t la note a été jouée
int continuer = 1;

 //Boucle jeu

 if (ArduinoClavier) arduino(1);//On lance arduino pour avoir le numéro port série qui correspond

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
           if (tempsActuel>=debut[0]) positionNote[l].y= positionNote[l].y+tempsActuel/10*2-tempsPrecedent/10*2;//descente de la note en utilisant le temps comme réference (elle met du coup 2.7sec a desendre) (la condition corrige le bug de la 1ere note)

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



/**************************************************************
*************************************************************
Fonction pour les touches
***********************************************************
**********************************************************/



/******************************************************
Switch pour savoir quelle touche a étée enfoncée. Si
une touche est enfoncée on donne a la variable la valeur
1 et on enregistre le temps a laquelle la note a été
"jouée"
*******************************************************/

SDL_PollEvent(&event);
switch(event.type)
{
case SDL_QUIT:
exit(EXIT_FAILURE);
break;
case SDL_KEYDOWN: /* Si appui sur une touche*/

    switch(event.key.keysym.sym)
    {
        case SDLK_q: //a
        a=1;
        tempsNote[0]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_w: //z
        z=1;
        tempsNote[1]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_e: //e
        e=1;
        tempsNote[2]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_r: //r
        r=1;
        tempsNote[3]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_t: //t
        t=1;
        tempsNote[4]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_y: //y
        y=1;
        tempsNote[5]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_u: //u
        u=1;
        tempsNote[6]=SDL_GetTicks() - tempsDebut;
        break;
        case SDLK_ESCAPE:
        continuer=0;
        break;
        default:
        break;
    }
break;

}




/*****************************************************************
On a donné à une variable la valeur 1 si l'evenement a été
réalisé, pour que deux évenements soient pris en compte en même
temps on laisse l'action que provoque l'evenement durer 250ms
(on utlise le temps enregistré avant pour savoir quand 250ms
 sont écoulées)
ce qui donne l'impression que les evenements sont simultannés
Si la variable=0, l'action n'est pas réalisée
******************************************************************/
if (a)
    {
        SDL_BlitSurface(Note_do, NULL, ecran, &positionNote_do);
        if(tempsActuel-tempsNote[0]>250) a=0;
    }
if (z)
    {
        SDL_BlitSurface(Note_re, NULL, ecran, &positionNote_re);
        if(tempsActuel-tempsNote[1]>250) z=0;
    }
if (e)
    {
        SDL_BlitSurface(Note_mi, NULL, ecran, &positionNote_mi);
        if(tempsActuel-tempsNote[2]>250) e=0;
    }
if (r)
    {
        SDL_BlitSurface(Note_fa, NULL, ecran, &positionNote_fa);
        if(tempsActuel-tempsNote[3]>250) r=0;
    }
if (t)
    {
        SDL_BlitSurface(Note_sol, NULL, ecran, &positionNote_sol);
        if(tempsActuel-tempsNote[4]>250) t=0;
    }
if (y)
    {
        SDL_BlitSurface(Note_la, NULL, ecran, &positionNote_la);
        if(tempsActuel-tempsNote[5]>250) y=0;
    }
if (u)
    {
        SDL_BlitSurface(Note_si, NULL, ecran, &positionNote_si);
        if(tempsActuel-tempsNote[6]>250) u=0;
    }


/*****************************************************************
Cette partie permet de déterminer quels boutons poussoirs sont enfoncés a partir du nombre
renvoyé par la fonction arduino.
Si un bouton est enfoncé on lui attribue un caractère propre, ainsi lors des tests de succès
cette information sera gérée exactement comme les évènements au clavier.


Bugs  incompréhensibles liés a l'ajout d'arduino:
On ne peux plus quitter la partie en cours (parfois si)
Au début de certains morceaux  des notes bizarres s'affichent
mais cela revient a la normale après 2-3 secondes.


                          Auteur : Yacine Saoudi
******************************************************************/

if (ArduinoClavier)
{

int arduinoIu=0;
arduinoIu=arduino(0);

char boutonArd[8];
sprintf(boutonArd, "%d", arduinoIu);
//on transforme le nombre stocké dans arduinoIu en chaine de caractère.

//cela permet de tester la présence ou non d'un caractère dans celle ci:
if(strstr(boutonArd, "1")!=NULL) a=1;
if(strstr(boutonArd, "2")!=NULL) z=1;
if(strstr(boutonArd, "3")!=NULL) e=1;
if(strstr(boutonArd, "4")!=NULL) r=1;
if(strstr(boutonArd, "5")!=NULL) t=1;
if(strstr(boutonArd, "6")!=NULL) y=1;
if(strstr(boutonArd, "7")!=NULL) u=1;

}



/***********************************************************************
Fonction réussit ou raté (si la note est "jouée" au bon moment)
************************************************************************/


l=k;//on teste toutes les notes qui on étées affichées

    do
    {
        //réussi
        if ((a)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 250))  {compteur++; pourcent[l]=1;} //compteur => score
        if ((z)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 395))  {compteur++; pourcent[l]=1;} //pourcent => la note spécifique l est réussie ou non (1= réussi)
        if ((e)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 525))  {compteur++; pourcent[l]=1;}
        if ((r)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 680))  {compteur++; pourcent[l]=1;}
        if ((t)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 820))  {compteur++; pourcent[l]=1;}
        if ((y)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 960))  {compteur++; pourcent[l]=1;}
        if ((u)&&(positionNote[l].y>550)&&(positionNote[l].y<580)&&(positionNote[l].x == 1100)) {compteur++; pourcent[l]=1;}
        //fail
        /*
        if ((a==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 250)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((z==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 395)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((e==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 525)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((r==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 680)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((t==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 820)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((y==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 960)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL);
        if ((u==0)&&(positionNote[l].y>560)&&(positionNote[l].y<570)&&(positionNote[l].x == 1100)) FMOD_System_PlaySound(systemf, FMOD_CHANNEL_FREE, fail, 0, NULL); //son de fausse note
        */
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

/*score*/


     /* Écriture du texte dans la SDL_Surface texte en mode Solid (car il change souvent)*/
     sprintf(caracteres, "Score : %d", compteur);
     SDL_FreeSurface(score);//On efface la surface précédente (sinon ca prend 2go de RAM)
     score = TTF_RenderText_Solid(police, caracteres, couleurNoire);

    //Position score//
        position.x = 20;
        position.y = 450;
        SDL_BlitSurface(score, NULL, ecran, &position); /* Blit du texte*/

/*titre*/

   /* Titre en fonction du choix */
   switch (choix)
   {
       case 0 :
       sprintf(caracteres2, "Frère Jacques");
       break;
       case 1 :
       sprintf(caracteres2, "Starlight - Muse");
       break;
       case 2 :
       sprintf(caracteres2, "Au Clair de la Lune");
       break;
       case 3 :
       sprintf(caracteres2, "Titanic");
       break;
       case 4 :
       sprintf(caracteres2, "Black or White - MJ");
       break;
       case 5 :
       sprintf(caracteres2, "Clocks - Coldplay");
       break;
       case 6 :
       sprintf(caracteres2, "Laputa");
       break;
       case 7 :
       sprintf(caracteres2, "YMCA");
       break;
       case 8 :
       sprintf(caracteres2, "Changes");
       break;
       case 10 :
       sprintf(caracteres2, "Dancing in the Dark");
       break;
       case 11 :
       sprintf(caracteres2, "Born to Run");
       break;
       case 12 :
       sprintf(caracteres2, "Just the way you are");
       break;
       case 13 :
       sprintf(caracteres2, "Bad day");
       break;
       case 15 :
       sprintf(caracteres2, "The Fray");
       break;
       case 16 :
       sprintf(caracteres2, "Starway to heaven");
       break;
       case 17 :
       sprintf(caracteres2, "Naruto");
       break;
       case 18 :
       sprintf(caracteres2, "Somebody to love - Queen");
       break;
       case 19 :
       sprintf(caracteres2, "Viva la Vida - Coldplay");
       break;
       default :
       break;
   }

     SDL_FreeSurface(score);//On efface la surface précédente (sinon ca prend 2go de RAM)
     score = TTF_RenderText_Solid(police2, caracteres2, couleurNoire);

    //Position score//
        position.x = 20;
        position.y = 60;
        SDL_BlitSurface(score, NULL, ecran, &position); /* Blit du texte*/


////////////////////////////////////////////////////////////////////////////////////////

/* On met à jour l'affichage */
SDL_Flip(ecran);

if (continuer==0) CloseCOM   ();//On ferme le port série
}

/******************************************************
Fonction pour obtenir le pourcentage de réussite
On utilise la variable pourcent (faire une vraie fonction
                                 de ca serait facile mais
                                 inutile)
Bug a corriger : Si on finit pas le morceau le pourcentage
est faux (narmol)
*****************************************************/

for(totalNotes=0;totalNotes<k;totalNotes++)
{
    if (pourcent[totalNotes]==1) pourcentFinal++;
}
 pourcentFinal=pourcentFinal*100/totalNotes;//on fait le pourcentage (il faut finir le morceau pour que le pourcentage soit juste)





///////////////////////////////////////////
/*Sortie de boucle*/
/////////////////////////////////////////
//Libération de l'espace
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
FMOD_Sound_Release(fail);
FMOD_System_Close(system);
FMOD_System_Release(system);

//return compteur;//le score (le pourcentage est plus interessant)
return pourcentFinal;//le pourcentage de réussite
}





