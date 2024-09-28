#include "tutorial.h"
#include "constants.h"



void init_tuto(Tuto *t, char filepath_img[], int index){
	init_image(&t->img, filepath_img, SCREEN_W*0.33, 0);
	init_image(&t->close_btn, "game/close_btn.png", t->img.pos1.x + t->img.pos1.w*0.88,t->img.pos1.y+t->img.pos1.h *0.08);
	t->index = index;
	t->active=0;//test ; change back to 0
	t->isDisplaying=0;
} 

void init_tutorial(Tutorials *T, int max_index){
	init_image(&T->next_btn, "game/next_btn.png", SCREEN_W*0.54, SCREEN_H*0.05);
	init_image(&T->back_btn, "game/back_btn.png", SCREEN_W*0.5,SCREEN_H*0.05);
	T->n= max_index;
	T->active=0;
	T->currentLastActiveIndex = -1;//no tutorial active ; change back to -1
	T->currentIndexDisplaying = -1;
	T->index_txt = init_text(SCREEN_W*0.35, SCREEN_H*0.1,"fonts/emporia-roman.otf",30, 0, 0, 0);
} 


void activate_tuto(Tutorials *T){
	if(T->currentLastActiveIndex < T->n-1){
 	T->currentLastActiveIndex++;//make next tutorial active
	T->T[T->currentLastActiveIndex].active=1;
	}
}

void display_tuto(Tutorials *T, SDL_Surface *ecran){
	if( T->currentLastActiveIndex>-1&&T->T[T->currentLastActiveIndex].active==1){
		printf("tutorial %d | active: %d\n",T->currentLastActiveIndex, T->T[T->currentLastActiveIndex].active);
		afficher_image(T->T[T->currentLastActiveIndex].img, ecran);
		afficher_image(T->T[T->currentLastActiveIndex].close_btn, ecran);
		T->T[T->currentLastActiveIndex].isDisplaying=1;
	
	}
	
}

void display_tutorials(Tutorials *T, SDL_Surface *ecran){
	if( T->currentLastActiveIndex>-1 && T->active){
		afficher_image(T->T[T->currentIndexDisplaying].img, ecran);
		afficher_image(T->T[T->currentIndexDisplaying].close_btn, ecran);
		afficher_image(T->next_btn, ecran);
		afficher_image(T->back_btn, ecran);
		sprintf(T->indexch, "%d/%d", T->currentIndexDisplaying+1, T->currentLastActiveIndex+1);
		afficher_text(ecran, &T->index_txt, T->indexch);
		T->T[T->currentLastActiveIndex].isDisplaying=1;
	
	}
	printf("Tutorial \n \tcurrent index displaying: %d \n\t current last active index: %d \n\t is dislaying: %d \n\t active: %d\n",  T->currentIndexDisplaying, T->currentLastActiveIndex, T->T[T->currentLastActiveIndex].isDisplaying, T->T[T->currentLastActiveIndex].active);
}


void handleTutoEvent(Tutorials *T, SDL_Event event){
	if(T->T[T->currentLastActiveIndex].isDisplaying){
		if(button_clicked(event, T->T[T->currentLastActiveIndex].close_btn)){
			T->T[T->currentLastActiveIndex].active=0;
			T->T[T->currentLastActiveIndex].isDisplaying=0;
			if(T->active){
				//if tutorials display is active => if the user presses close on any tutorial of them => make all tutorials display inactive
				T->active=0;
				T->currentIndexDisplaying = -1;
				
			}
		}
	
	}
	if(T->active){
		if(button_clicked(event, T->next_btn) && T->currentIndexDisplaying < T->currentLastActiveIndex ){
		//if user clicks on next and current page index is lower than last active page index => move to next page
		T->currentIndexDisplaying++;
		T->T[T->currentIndexDisplaying-1].isDisplaying=0;
	
	}
	else if(button_clicked(event, T->back_btn) && T->currentIndexDisplaying > 0){
		//if user clicks on next and current page index is higher than 0 => move to previous page
		T->currentIndexDisplaying--;
		T->T[T->currentIndexDisplaying+1].isDisplaying=0;
	
	}
	}

}



void liberer_tutorials(Tutorials *T){
	for(int i=0;i<T->n;i++){
		liberer_image(T->T[i].img);
		liberer_image(T->T[i].close_btn);
	}
	free(T->T);
	liberer_image(T->back_btn);
	liberer_image(T->next_btn);
	liberer_text(&T->index_txt);

}
