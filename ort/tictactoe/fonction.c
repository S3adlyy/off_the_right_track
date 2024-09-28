#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_ttf.h>
#include <string.h>
#include <SDL/SDL_mixer.h>
#include "fonction.h"
/**
* @file fonction.c
* @brief Testing Program.
* @author C Team
* @version 0.1
* @date Apr 01, 2015
*
* Testing program for background scrollilng
*
*/
void initialiserTic (tic *t)
{
	int largeur_case,hauteur_case,choix;
	int i;
	t->image=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/xo background.png");
//	t->image_x=IMG_Load("X0.png");
//	t->image_o=IMG_Load("O0.png");

	largeur_case =192;
	hauteur_case =192;
	for (i=0;i<9;i++)
	{
		t->t[i]=0;
	}
	t->num_j=-1;
	t->pos.x=0;
	t->pos.y=0;
	t->pos.w=t->image->w;
	t->pos.h=t->image->h;
	t->num_tour=0;
	t->textColor.r=0;
	t->textColor.g=0;
	t->textColor.b=0;
	t->font = TTF_OpenFont( "/home/nirvana/Off The Right Track/tictactoe/arial.ttf", 100 );
	for (i=0;i<9;i++)
	{
		t->tabpos[i].x = largeur_case*(i%3)+(675);
		t->tabpos[i].y = hauteur_case*(i/3)+(270);
		t->tabpos[i].w = largeur_case;
		t->tabpos[i].h = hauteur_case;
	}
}
void initialiserbord (bord *b)
{
	int l_c = 48.5,i;
	b->im_win=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/win.png");
	b->im_lose=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/lose.png");
	b->im_tie=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/tie.png");
	b->im=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/bord.png");
	for (i=0;i<3;i++)
	{
		b->t[i]=0;
	}
	b->pos.x=0;
	b->pos.y=0;
	b->pos.w=b->im->w;
	b->pos.h=b->im->h;
	for(i=0;i<3;i++)
	{
		b->tpos[i].x = l_c*(i%3)+7.5;
		b->tpos[i].y = 40;
		b->tpos[i].w = l_c;
		b->tpos[i].y = l_c;
	}
}
void afficherbord (bord b,SDL_Surface *ecran)
{
	int i;
	for (i=0;i<3;i++)
	{
		switch(b.t[i])
		{
			case -1:
				SDL_BlitSurface(b.im_win,NULL,ecran,&b.tpos[i]);
				break;
			case 1:
				SDL_BlitSurface(b.im_lose,NULL,ecran,&b.tpos[i]);
				break;
			case 2:
				SDL_BlitSurface(b.im_tie,NULL,ecran,&b.tpos[i]);
				break;
		}
	}
}
void afficherTic (tic t,SDL_Surface *ecran)
{
	int i;
        for (i=0;i<9;i++)
        {
              	switch(t.t[i])
               	{
               	 	case -1:
               	 		SDL_BlitSurface(t.image_o,NULL,ecran,&t.tabpos[i]);
               	 		break;
               	 	case 1:
               	 		SDL_BlitSurface(t.image_x,NULL,ecran,&t.tabpos[i]);
               	 		break;
                }
	}
}          	
int atilganer(int tabsuivi[],int *x,int *y,int *z)
{
	int i=1,l,c;
	for (i=0;i<9;i++)
	{
		l=i/3;
		c=i%3;
		if (tabsuivi[l*3]==tabsuivi[l*3+1]&&tabsuivi[l*3]==tabsuivi[l*3+2]&&tabsuivi[l*3]!=0)
		{
			(*x)=l*3;
			(*y)=l*3+1;
			(*z)=l*3+2;
			return tabsuivi[l*3];
		}
		if (tabsuivi[c]==tabsuivi[c+3]&&tabsuivi[c]==tabsuivi[c+6]&&tabsuivi[c]!=0)
		{
			(*x)=c;
			(*y)=c+3;
			(*z)=c+6;
			return tabsuivi[c];
		}
	}
	if (tabsuivi[0]==tabsuivi[4]&&tabsuivi[0]==tabsuivi[8]&&tabsuivi[0]!=0)
	{
		(*x)=0;
		(*y)=4;
		(*z)=8;
		return tabsuivi[0];
	}
	if (tabsuivi[2]==tabsuivi[4]&&tabsuivi[2]==tabsuivi[6]&&tabsuivi[2]!=0)
	{
		(*x)=2;
		(*y)=4;
		(*z)=6;
		return tabsuivi[2];
	}
	for (i=0;i<9;i++)
	{
		if(tabsuivi[i]==0)
			return 10;
	}
	return 0;	
}
int resultat (int t[])
{
	int i,lose=0,win=0,tie=0;
	for (i=0;i<3;i++)
	{
		switch (t[i])
		{
			case 1:
				lose++;
				break;
			case -1:
				win++;
				break;
			case 2:
				tie++;
				break;
		}
	}
	if (lose>=2)
		return 1;
	if (win>=2)
		return -1;
	if ((tie==1)&&(lose==1)&&(win==1))
		return 2;
	if ((tie==2)&&(lose==1))
		return 1;
	if ((tie==2)&&(win==1))
		return -1;
	if (tie==3)
		return 2;
}	
void liberer(tic t,bord b)
{
	SDL_FreeSurface(t.image);
        SDL_FreeSurface(t.image_x);
        SDL_FreeSurface(t.image_o);
        SDL_FreeSurface (t.sur_text);
        SDL_FreeSurface(b.im_win);
        SDL_FreeSurface(b.im_lose);
        SDL_FreeSurface(b.im_tie);
        SDL_FreeSurface(b.im);
	TTF_CloseFont (t.font);
}
int minimax(int tabsuivi[9], int joueur) 
{
    int x,y,z,gagnant= atilganer(tabsuivi,&x,&y,&z);
    if(gagnant != 10) 
    {
        return gagnant*joueur;
    }
    int i;
    int move=-1;
    int score=-2;
    for(i =0;i<9;++i) 
    {
        if(tabsuivi[i] == 0) 
        {
            tabsuivi[i] = joueur;
            int mmscore = -minimax(tabsuivi,joueur* -1);
            if (mmscore>score)
            {
            	score=mmscore;
            	move=i;
            }
            tabsuivi[i] = 0;
        }
    }
    if(move == -1)
    	return 0;
        return score;
}
int calcul_coup(int tabsuivi[9])
{
	int coup=-1,score=-2,i;
	for(i=0;i<9;++i)
	{
		if(tabsuivi[i]==0)
		{
			tabsuivi[i]=1;
			int mmscore=-minimax(tabsuivi,-1);
			tabsuivi[i] = 0;
			if (mmscore>score)
            		{
            			score=mmscore;
            			coup=i;
            		}
            	}
	}
	tabsuivi[coup]=1;
	return coup;
}
void initialiser_cal (cal *c)
{
	int i;
	c->nb=0;
	for (i=0;i<3;i++)
	{
		c->t[i]=-1;
	}
}
int ajouter_cal (cal *c,int i)
{
	int j,j1=-1;
	if (c->nb<3)
	{
		c->t[c->nb]=i;
		c->nb++;
	}
	else
	{
		j1=c->t[0];
		for(j=1;j<3;j++)
		{
			c->t[j-1]=c->t[j];
		}
		c->t[2]=i;
	}
	return j1;
}

		
