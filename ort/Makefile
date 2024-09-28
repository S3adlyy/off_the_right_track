OBJS = game.o main.o fonctions.o settings.o newgame.o level1.o level2.o level3.o memorygame.o dialogue.o Ortcrush/ortcrushmain.o Ortcrush/ortcrush.o minimap.o scrolling.o player.o collision.o  tictactoe/tic.o tictactoe/fonction.o enigme/enigme.o enigme/enigmetf.o particle_system.o serie.o npc.o save_system.o enemy.o gameObject.o tutorial.o document.o 

prog: $(OBJS)
	gcc $(OBJS) -o prog -g -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer -lSDL_gfx -lm


game.o: game.c
	gcc -c $< -o $@
%.o: %.c
	gcc -g -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) prog



