#include "document.h"
#include "constants.h"



void init_doc(Doc *t, char filepath_img[], int index, char musicpath[]){
	init_image(&t->img, filepath_img, SCREEN_W*0.33, 0);
	init_image(&t->close_btn, "game/close_btn.png", t->img.pos1.x + t->img.pos1.w*0.88,t->img.pos1.y+t->img.pos1.h *0.06);
	t->index = index;
	t->active=0;//test ; change back to 0
	t->voice = NULL;
	t->isDisplaying=0;
	t->voicePlaying=0;
	t->voice = init_chunk(musicpath);
	if(t->voice==NULL)
	{
		printf("Error loading audio: %s", Mix_GetError());
		return ;
	}
} 

void init_documents(Documents *T, int max_index){
	init_image(&T->next_btn, "game/next_btn.png", SCREEN_W*0.54, SCREEN_H*0.05);
	init_image(&T->back_btn, "game/back_btn.png", SCREEN_W*0.5,SCREEN_H*0.05);
	T->n= max_index;
	T->active=0;
	T->currentLastActiveIndex = -1;//no tutorial active ; change back to -1
	T->currentIndexDisplaying = -1;
	T->index_txt = init_text(SCREEN_W*0.38, SCREEN_H*0.07,"fonts/emporia-roman.otf",30, 0, 0, 0);
} 


void activate_doc(Documents *T){
	if(T->currentLastActiveIndex < T->n-1){
 	T->currentLastActiveIndex++;//make next tutorial active
	T->T[T->currentLastActiveIndex].active=1;
	}
}

void display_doc(Documents *T, SDL_Surface *ecran){
	if( T->currentLastActiveIndex>-1&&T->T[T->currentLastActiveIndex].active==1){
		printf("document %d | active: %d\n",T->currentLastActiveIndex, T->T[T->currentLastActiveIndex].active);
		afficher_image(T->T[T->currentLastActiveIndex].img, ecran);
		afficher_image(T->T[T->currentLastActiveIndex].close_btn, ecran);
		T->T[T->currentLastActiveIndex].isDisplaying=1;
		if(T->T[T->currentLastActiveIndex].voicePlaying==0){
			if(Mix_Playing(5))
				Mix_HaltChannel(5);
			Mix_PlayChannel(5,T->T[T->currentLastActiveIndex].voice, 0);
			T->T[T->currentLastActiveIndex].voicePlaying=1;
		}
		
	
	}
	
}

void display_documents(Documents *T, SDL_Surface *ecran){
	if( T->currentLastActiveIndex>-1 && T->active){
		afficher_image(T->T[T->currentIndexDisplaying].img, ecran);
		afficher_image(T->T[T->currentIndexDisplaying].close_btn, ecran);
		afficher_image(T->next_btn, ecran);
		afficher_image(T->back_btn, ecran);
		sprintf(T->indexch, "%d/%d", T->currentIndexDisplaying+1, T->currentLastActiveIndex+1);
		afficher_text(ecran, &T->index_txt, T->indexch);
		T->T[T->currentIndexDisplaying].isDisplaying=1;
		if(T->T[T->currentIndexDisplaying].voicePlaying==0){
			if(Mix_Playing(5))
				Mix_HaltChannel(5);
			Mix_PlayChannel(5,T->T[T->currentIndexDisplaying].voice, 0);
			T->T[T->currentIndexDisplaying].voicePlaying=1;
		}
		printf("Channel PLAYING: %d\n Voice PLaying %d\n", Mix_Playing(5), T->T[T->currentIndexDisplaying].voicePlaying);
	
		printf("document \n \tcurrent index displaying: %d \n\t current last active index: %d \n\t is dislaying: %d \n\t active: %d\n",  T->currentIndexDisplaying, T->currentLastActiveIndex, T->T[T->currentLastActiveIndex].isDisplaying, T->T[T->currentLastActiveIndex].active);
	}
}


void handleDocEvent(Documents *T, SDL_Event event){
	if(T->T[T->currentLastActiveIndex].isDisplaying && !T->active){
		if(button_clicked(event, T->T[T->currentLastActiveIndex].close_btn)){
			T->T[T->currentLastActiveIndex].active=0;
			T->T[T->currentLastActiveIndex].isDisplaying=0;
			T->T[T->currentLastActiveIndex].voicePlaying=0;
			if(Mix_Playing(5))
				Mix_HaltChannel(5);
			
			/*if(T->active){
				//if tutorials display is active => if the user presses close on any tutorial of them => make all tutorials display inactive
				T->active=0;
				T->currentIndexDisplaying = -1;
				T->T[T->currentIndexDisplaying].isDisplaying=0;
				T->T[T->currentIndexDisplaying].voicePlaying=0;
				
			}*/
		}
	
	}
	else if(T->active){
		if(button_clicked(event, T->T[T->currentIndexDisplaying].close_btn)){
			if(Mix_Playing(5))
				Mix_HaltChannel(5);
			//if tutorials display is active => if the user presses close on any tutorial of them => make all tutorials display inactive
			T->active=0;
			T->T[T->currentIndexDisplaying].active=0;
			T->T[T->currentIndexDisplaying].isDisplaying=0;
			T->T[T->currentIndexDisplaying].voicePlaying=0;
			T->currentIndexDisplaying = -1;
			}

	
	}
	if(T->active){
		if(button_clicked(event, T->next_btn) && T->currentIndexDisplaying < T->currentLastActiveIndex ){
		//if user clicks on next and current page index is lower than last active page index => move to next page
		T->T[T->currentIndexDisplaying].isDisplaying=0;
		T->T[T->currentIndexDisplaying].voicePlaying=0;
		if(Mix_Playing(5))
			Mix_HaltChannel(5);
		T->currentIndexDisplaying++;
	
		}
	else if(button_clicked(event, T->back_btn) && T->currentIndexDisplaying > 0){
		//if user clicks on next and current page index is higher than 0 => move to previous page
		T->T[T->currentIndexDisplaying].isDisplaying=0;
		T->T[T->currentIndexDisplaying].voicePlaying=0;
		if(Mix_Playing(5))
			Mix_HaltChannel(5);
		T->currentIndexDisplaying--;
	
		}
	}

}


void liberer_documents(Documents *T){
	for(int i=0;i<T->n;i++){
		liberer_image(T->T[i].img);
		liberer_image(T->T[i].close_btn);
		liberer_chunk(T->T[i].voice);
	}
	//free(T->T);
	liberer_image(T->back_btn);
	liberer_image(T->next_btn);
	liberer_text(&T->index_txt);

}
