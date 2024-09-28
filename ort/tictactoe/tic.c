#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_rotozoom.h>
#include <time.h>
#include "fonction.h"
#include "../fonctions.h"


int starttic(SDL_Surface *ecran){
	int quitter=1,i,test,j=0,x,y,z,i1,i2,play=0,a;
	SDL_Rect tabpos[9];
	cal c1,c2;
	srand(time(NULL));
	tic t;
	bord b;
	TTF_Init();
	SDL_Event event;
	SDL_Surface *rotatedSurface=NULL;
	SDL_Surface *im_x,*im_o,*im_x1,*im_o1,*image,*player1,*player2,*first1,*first2,*player01,*player02,*first01,*first02,*next,*next0,*im_x2,*im_o2,*imtuto0,*imtuto1,*imtuto,*imreturn,*imreturn1;
	im_x=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/X2.png");
	im_o=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/O2.png");
	im_o1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/O1.png");
	im_x1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/X1.png");
	im_x2=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/X3.png");
	im_o2=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/O3.png");
	imtuto0=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/button0.png");
	imtuto1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/button1.png");
	imtuto=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/result.png");
	imreturn=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/return.png");
	imreturn1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/return1.png");
	Mix_Chunk *musique_win=NULL,*musique_lose=NULL,*c=NULL,*musique_tie=NULL;
	SDL_Init( SDL_INIT_VIDEO|SDL_INIT_AUDIO);
	initialiserTic (&t);
	initialiserbord (&b);
	musique_win = Mix_LoadWAV("/home/nirvana/Off The Right Track/tictactoe/Winning Sound Effect(1).mp3");
	musique_lose = Mix_LoadWAV("/home/nirvana/Off The Right Track/tictactoe/Fail Sound Effect.mp3");
	musique_tie = Mix_LoadWAV("/home/nirvana/Off The Right Track/tictactoe/Bamboo hit Sound effect.mp3");
	c = Mix_LoadWAV("/home/nirvana/Off The Right Track/tictactoe/Pencil sound effect.mp3");
//	t.num_j=-1;
	double angle = 0.0;
	SDL_Rect posp1,posp2,posf1,posf2,posplay,postuto,posreturn;
	image=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/3.png");
	initialiser_cal (&c1);
	initialiser_cal (&c2);
	//
	
	player1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/C1.png");
	posp1.x=(image->w)/2-(player1->w+30);
	posp1.y=100;
	posp1.w=player1->w;
	posp1.h=player1->h;
	player01=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/C01.png");
	player2=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/C2.png");
	posp2.x=(image->w)/2+30;
	posp2.y=100;
	posp2.w=player2->w;
	posp2.h=player2->h;
	player02=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/C02.png");
	first1=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/B1.png");
	posf1.x=(image->w)/2-(first1->w+130);
	posf1.y=350;
	posf1.w=first1->w;
	posf1.h=first1->h;
	first01=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/B01.png");
	first2=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/B2.png");
	float oldCurPosX=0, oldCurPosY=0;
	SDL_Rect cur_pos;
	SDL_Surface *cursorImage4 = init_cursor(&cur_pos);
	posf2.x=(image->w)/2+130;
	posf2.y=350;
	posf2.w=first2->w;
	posf2.h=first2->h;
	first02=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/B02.png");
	next=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/z1.png");
	next0=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/z2.png");
	posplay.x=(image->w)/2-30;
	posplay.y=800;
	posplay.w=next->w;
	posplay.h=next->h;
	postuto.x=(image->w)-imtuto0->w;
	postuto.y=0;
	postuto.w=imtuto0->w;
	postuto.h=imtuto0->h;
	posreturn.x=(image->w)-imreturn->w;
	posreturn.y=0;
	posreturn.w=imreturn->w;
	posreturn.h=imreturn->h;
	int playp1=0,playp2=0,playf1=0,playf2=0,test1=0,test2=0,testai=1,play_tuto=0;
	for (i=0;i<9;i++)
	{
		tabpos[i].x = 196*(i%3)+(630);
		tabpos[i].y = 202*(i/3)+(220);
		tabpos[i].w = 196;
		tabpos[i].h = 202;
	}
	while ((quitter==1)&&(j!=4))
	{
		if (play==0&&play_tuto==0)
		{
			SDL_BlitSurface(image,NULL,ecran,&t.pos);
			if(playp1==0)
				SDL_BlitSurface(player1,NULL,ecran,&posp1);
			else
				SDL_BlitSurface(player01,NULL,ecran,&posp1);
			if(playp2==0)
				SDL_BlitSurface(player2,NULL,ecran,&posp2);
			else
				SDL_BlitSurface(player02,NULL,ecran,&posp2);
			if(playf1==0)
				SDL_BlitSurface(first1,NULL,ecran,&posf1);
			else
				SDL_BlitSurface(first01,NULL,ecran,&posf1);
			if(playf2==0)
				SDL_BlitSurface(first2,NULL,ecran,&posf2);
			else
				SDL_BlitSurface(first02,NULL,ecran,&posf2);
			SDL_BlitSurface(next,NULL,ecran,&posplay);
			SDL_BlitSurface(imtuto0,NULL,ecran,&postuto);
			while(SDL_PollEvent(&event))
			{
			switch (event.type)
			{
				case SDL_QUIT: quitter = 0;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) 
					{
						case SDLK_ESCAPE: quitter =0; break;	
		        		}
		        		break;
		        	case SDL_MOUSEBUTTONDOWN:
		        		int x =event.button.x;   
                			int y =event.button.y;
		        	    	if(x>=posf1.x&&x<= posf1.x+posf1.w &&y>=posf1.y &&y<=posf1.y+posf1.h)  
                			{
                				t.image_o=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/X0.png");
                				t.image_x=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/O0.png");
                	    			playf1=1;
                	    			if(playf2=1)
                	    				playf2=0;
                	    			test2=1;
                	    		}
		        	    	if(x>=posf2.x&&x<= posf2.x+posf2.w &&y>=posf2.y &&y<=posf2.y+posf2.h)  
		        	    	{
		        	    		t.image_o=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/O0.png");
                				t.image_x=IMG_Load("/home/nirvana/Off The Right Track/tictactoe/X0.png");
		        	    		playf2=1;
		        	    		if(playf1=1)
                	    				playf1=0;
                	    			test2=1;
		        	    	}
		        	    	if(x>=posp1.x&&x<= posp1.x+posp1.w &&y>=posp1.y &&y<=posp1.y+posp1.h)  
                			{
                	    			playp1=1;
                	    			if(playp2=1)
                	    				playp2=0;
                	    			test1=1;
                	    		}
		        	    	if(x>=posp2.x&&x<= posp2.x+posp2.w &&y>=posp2.y &&y<=posp2.y+posp2.h)  
		        	    	{
		        	    		playp2=1;
		        	    		if(playp1=1)
                	    				playp1=0;
                	    			test1=1;
                	    			testai=0;
		        	    	}
		        	    	if(x>=posplay.x&&x<= posplay.x+posplay.w &&y>=posplay.y &&y<=posplay.y+posplay.h) 
		        	    	{
		        	    		SDL_BlitSurface(next0,NULL,ecran,&posplay);
		        	    		if ((test1==1)&&(test2==1))
		        	    			play=1;
		        	    	}
		        	    	if(x>=postuto.x&&x<= postuto.x+postuto.w &&y>=postuto.y &&y<=postuto.y+postuto.h) 
		        	    	{
		        	    		SDL_BlitSurface(imtuto1,NULL,ecran,&postuto);
		        	    		play_tuto=1;
		        	    	}
			}
			}
		}
		if(play_tuto==1)
		{
			SDL_BlitSurface(imtuto,NULL,ecran,&t.pos);
			SDL_BlitSurface(imreturn,NULL,ecran,&posreturn);
			while(SDL_PollEvent(&event))
			{
			switch (event.type)
			{
				case SDL_QUIT: quitter = 0;
						break;
					case SDL_KEYDOWN:
						switch(event.key.keysym.sym) 
						{
							case SDLK_ESCAPE: quitter =0; break;	
						}
						break;
					case SDL_MOUSEBUTTONDOWN:
						int x =event.button.x;   
		        			int y =event.button.y;
		        			if(x>=postuto.x&&x<= postuto.x+postuto.w &&y>=postuto.y &&y<=postuto.y+postuto.h) 
					    	{
					    		SDL_BlitSurface(imreturn1,NULL,ecran,&posreturn);
					    		play_tuto=0;
					    	}
					    	break;
				 case SDL_MOUSEMOTION:
		        	  	updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
		        	  	break;
			}
			}
		        			
		}
		if ((play==1)&&test2==1&&test1==1&&play_tuto==0)
		{
			if (playp2==1)
			{
				 playp2=0;
				 a = 1+rand()%9;
				 testai=1;
		        	 if (playf1=1)
		        	 {
		                	t.t[a]=1;
		                	i1=ajouter_cal (&c2,a);
		        	 }
		        	 else
		        	 {
		        	  	t.t[a]=-1;
		        	  	i1=ajouter_cal (&c2,a);
		        	 }	   
		       	}
		SDL_BlitSurface(t.image,NULL,ecran,&t.pos);
		SDL_BlitSurface(b.im,NULL,ecran,&b.pos);
		SDL_BlitSurface(imtuto0,NULL,ecran,&postuto);
		while(SDL_PollEvent(&event))
		{
		
		switch (event.type)
		{
			case SDL_QUIT: quitter = 0;
        			break;
        		case SDL_KEYDOWN:
        			switch(event.key.keysym.sym) 
        			{
        				case SDLK_ESCAPE: quitter =0; break;	
                		}
                		break;
                	case SDL_MOUSEBUTTONDOWN:
        		if(event.button.button == SDL_BUTTON_LEFT){ 
        			int x =event.button.x;   
                		int y =event.button.y; 
				test=0;
				i=0;
                		for (i=0;i<9&&test==0;i++)
                		{
                			if(x>=tabpos[i].x&&x<=tabpos[i].x+tabpos[i].w&&y>=tabpos[i].y&&y<=tabpos[i].y+tabpos[i].h){
                				test=1;	    		               		
		        			if (t.t[i]==0)
			       			{
				    	  		if(t.num_j==1)
				       			{
				     				t.t[i]=1;
				     				t.num_j=-1;
							}
							else
							{
								t.t[i]=-1;
				     				Mix_PlayChannel(6, c, 0);
				     				t.num_j=1;
				     				i1=ajouter_cal (&c1,i);
				     				if (c1.nb==3)
				     					t.t[i1]=0;
				     				if (atilganer(t.t,&x,&y,&z)==10)
				     				{
				     					if(testai==1)
					     					i=calcul_coup(t.t);
					     				i1=ajouter_cal (&c2,i);
					     				if (c2.nb==3)
					     					t.t[i1]=0;
					     				t.num_j=-1;
				     				}
				     			}	
		        			}
                			}
                		}
                		if(x>=postuto.x&&x<= postuto.x+postuto.w &&y>=postuto.y &&y<=postuto.y+postuto.h) 
		        	    	{
		        	    		SDL_BlitSurface(imtuto1,NULL,ecran,&postuto);
		        	    		play_tuto=1;
		        	    	}
                	}
                	break;
                	case SDL_MOUSEMOTION:
		        	  	updateCursorPosition(&oldCurPosX, &oldCurPosY, &cur_pos, event);
		        	  	break;
        	}
        	}
        	afficherTic (t,ecran);
        	
        		if(playf2==1)
        		{
        		if (c1.nb>=3)
        		{
        		i1=c1.t[0];
        		SDL_BlitSurface(im_o1,NULL,ecran,&t.tabpos[i1]);
        		}
        		if (c2.nb>=3)
        		{
        		i2=c2.t[0];
               		SDL_BlitSurface(im_x1,NULL,ecran,&t.tabpos[i2]);
               		}
               		}
               		else
               		{
               		if (c2.nb>=3)
        		{
               		i1=c2.t[0];
        		SDL_BlitSurface(im_o1,NULL,ecran,&t.tabpos[i1]);
        		}
        		if (c1.nb>=3)
        		{
        		i2=c1.t[0];
               		SDL_BlitSurface(im_x1,NULL,ecran,&t.tabpos[i2]);
               		}
               		}
        	
        	
        	
        	if (atilganer(t.t,&x,&y,&z)!=10)
        	{	if (playf2==1)
        		{
				switch (atilganer (t.t,&x,&y,&z))
				{
					case -1:
						SDL_BlitSurface(im_o,NULL,ecran,&t.tabpos[x]);
						SDL_BlitSurface(im_o,NULL,ecran,&t.tabpos[y]);
						SDL_BlitSurface(im_o,NULL,ecran,&t.tabpos[z]);
						b.t[j]=-1;
					break;
					case 1:
						SDL_BlitSurface(im_x,NULL,ecran,&t.tabpos[x]);
						SDL_BlitSurface(im_x,NULL,ecran,&t.tabpos[y]);
						SDL_BlitSurface(im_x,NULL,ecran,&t.tabpos[z]);
						b.t[j]=1;
					break;
					case 0:
						b.t[j]=2;
					break;
				}
			}
			else
			{
				switch (atilganer (t.t,&x,&y,&z))
				{
					case 1:
						SDL_BlitSurface(im_o2,NULL,ecran,&t.tabpos[x]);
						SDL_BlitSurface(im_o2,NULL,ecran,&t.tabpos[y]);
						SDL_BlitSurface(im_o2,NULL,ecran,&t.tabpos[z]);
						b.t[j]=1;
					break;
					case -1:
						SDL_BlitSurface(im_x2,NULL,ecran,&t.tabpos[x]);
						SDL_BlitSurface(im_x2,NULL,ecran,&t.tabpos[y]);
						SDL_BlitSurface(im_x2,NULL,ecran,&t.tabpos[z]);
						b.t[j]=-1;
					break;
					case 0:
						b.t[j]=2;
					break;
				}
			}
			initialiser_cal (&c1);
			initialiser_cal (&c2);
			if ((playp2==0)&&(playp1==0))
				playp2=1;
			displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
		SDL_Flip(ecran);
			SDL_Delay(2000);
        		initialiserTic (&t);		
			j++;
        	}	
        	afficherbord (b,ecran);
        	if ((j>=2)&&(j<=3))
        	{
        		int r=1920*0.2,r1=1080*0.2;
        		double zoom=0.5;
        		switch (resultat (b.t))
        		{
        			case -1:
        				Mix_PlayChannel(6, musique_win, 0);
        				
        				//SDL_Delay(1000);
					strcpy(t.resultat,"YOU WIN");
					t.textColor.r=0;
					t.textColor.g=255;
					t.textColor.b=0;
					t.sur_text = TTF_RenderText_Solid(t.font, t.resultat , t.textColor );
					while(zoom<1.5){
						SDL_BlitSurface(t.image,NULL,ecran,&t.pos);
						//SDL_BlitSurface(b.im,NULL,ecran,&b.pos);
						printf("zoom: %.2f\n", zoom);
						zoom+= 0.05;
						rotatedSurface =rotozoomSurface(t.sur_text,0,zoom,6);
						SDL_Rect loc1 ={r,r1,t.sur_text->w,t.sur_text->h};
						SDL_BlitSurface(rotatedSurface, NULL, ecran, &loc1);
						displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
						SDL_Flip(ecran);
					}
				break;
				case 1:
					Mix_PlayChannel(6, musique_lose, 0);
					//SDL_Delay(1000);
					strcpy(t.resultat,"HARD LUCK");
					t.textColor.r=255;
					t.textColor.g=0;
					t.textColor.b=0;
					t.sur_text = TTF_RenderText_Solid(t.font, t.resultat , t.textColor );
					while(angle<=360.0){
						SDL_BlitSurface(t.image,NULL,ecran,&t.pos);
						//SDL_BlitSurface(b.im,NULL,ecran,&b.pos);
						angle+=8.0;
						zoom+= 0.025;
						rotatedSurface =rotozoomSurface(t.sur_text,angle,zoom,6);
						SDL_Rect loc1 ={r,r1,t.sur_text->w,t.sur_text->h};
						SDL_BlitSurface(rotatedSurface, NULL, ecran, &loc1);
						displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
						SDL_Flip(ecran);
					}
				break;
				case 2:
					Mix_PlayChannel(6, musique_tie, 0);
					//SDL_Delay(1000);
					strcpy(t.resultat,"Tie");
					t.textColor.r=128;
					t.textColor.g=128;
					t.textColor.b=128;
					r=600;
					t.sur_text = TTF_RenderText_Solid(t.font, t.resultat , t.textColor );
					SDL_Rect loc2 ={r,r1,t.sur_text->w,t.sur_text->h};
					SDL_BlitSurface(t.sur_text, NULL, ecran, &loc2);
				break;
			}
			
			//SDL_Delay(2000);
			displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
			SDL_Flip(ecran);
			if(angle>=360.0 || zoom>=1.0)
				j++;
		}
		}
		displayCursor(cursorImage4,cur_pos, oldCurPosX,oldCurPosY,ecran);
		SDL_Flip(ecran);
	}
	SDL_FreeSurface(im_x);
	SDL_FreeSurface(im_o1);
	SDL_FreeSurface(im_x1);
	SDL_FreeSurface(im_x2);
	SDL_FreeSurface(im_o2);
	SDL_FreeSurface(imtuto0);
	SDL_FreeSurface(imtuto1);
	SDL_FreeSurface(imtuto);
	SDL_FreeSurface(imreturn);
	SDL_FreeSurface(imreturn1);
	SDL_FreeSurface(im_o);
	//
	SDL_FreeSurface(player1);
	SDL_FreeSurface(image);
	SDL_FreeSurface(player01);
	SDL_FreeSurface(player2);
	SDL_FreeSurface(player02);
	SDL_FreeSurface(first1);
	SDL_FreeSurface(first01);
	SDL_FreeSurface(first2);
	SDL_FreeSurface(first02);
	SDL_FreeSurface(next);
	SDL_FreeSurface(next0);
	SDL_FreeSurface(rotatedSurface);
	liberer(t,b);
	Mix_FreeChunk(musique_win);
	Mix_FreeChunk(musique_lose);
	Mix_FreeChunk(musique_tie);
	Mix_FreeChunk(c);
};
