#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "particle_system.h"
#include <math.h>

#define MAX_PARTICLES 1000
#define PARTICLE_LIFESPAN 500
#define PARTICLE_VELOCITY_RANGE_X 3
#define PARTICLE_VELOCITY_RANGE_Y 5



void initParticleSystems(Particle particles[], SDL_Surface* particleImage, int imageWidth, int imageHeight) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].lifespan = 0; // particle not active
        particles[i].image = particleImage; 
        particles[i].width = imageWidth ;   
        particles[i].height = imageHeight;  
        //printf("i : %d\n", i);
    }
}


void updateParticleSystems(Particle particles[],Uint32 dt, Camera camera) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].lifespan > 0) {
            particles[i].x +=  particles[i].vx ;
            particles[i].y += particles[i].vy;
            particles[i].lifespan -= dt;
        }
    }
}

void renderParticles(Particle particles[],SDL_Surface *screen, Camera camera) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particles[i].lifespan > 0) {
            SDL_Rect destRect = { particles[i].x, particles[i].y, 0, 0 };
            SDL_BlitSurface(particles[i].image, NULL, screen, &destRect);
        }
    }
}


// PARTICLE EXPLOSION
void emitParticlesExplosion(Particle particles[], int x, int y, int modifier, int velocityrangeX, int velocityrangeY) {

	int numParticlesToEmit = (800 + rand() % (MAX_PARTICLES + 1) - 800)/modifier; // Gen a rand number between 300 and MAX_PARTICLES
    
    // Emit particles	
    for(int i =0; i<numParticlesToEmit; i++){
        // find inactive particle
        for (int j = 0; j < MAX_PARTICLES; j++) {
            if (particles[j].lifespan <= 0) {
                int vxMagnitude = rand() % (2 * velocityrangeX + 1) - velocityrangeX;
                int vyMagnitude = rand() % (2 * velocityrangeY + 1) -velocityrangeY;

                //  rand angles -> rand direction
                double randomAngle = (double)(rand() % 360); // Random angle in deg
                // convert angle
                double angleRadians = randomAngle * (M_PI / 180.0);
                particles[j].vx = vxMagnitude * cos(angleRadians);
                particles[j].vy = vyMagnitude * sin(angleRadians);
                // particle pos
                particles[j].x = x;
                particles[j].y = y;
                // random particle lifespan
                particles[j].lifespan = 100 + rand()%PARTICLE_LIFESPAN-100+1;
            }
        }
       }
}
// PARTICLE TRAIL
void emitParticles(Particle particles[], int x, int y, int playerDirection, int modifier) {

    int numParticlesToEmit = (700 + rand() % (MAX_PARTICLES + 1) - 700) / modifier; 
    for (int i = 0; i < numParticlesToEmit; ++i) {
        for (int j = 0; j < MAX_PARTICLES; ++j) {
            if (particles[j].lifespan <= 0) {
                int vxDirection = (playerDirection == 2) ? 1 : -1;

                int vxMagnitude = rand() % (2 * PARTICLE_VELOCITY_RANGE_X + 1) - PARTICLE_VELOCITY_RANGE_X;
                int vyMagnitude = rand() % (2 * PARTICLE_VELOCITY_RANGE_Y + 1) - PARTICLE_VELOCITY_RANGE_Y;

		double angleRadians = (rand() % 46) * (M_PI / 180.0);

                //particles[j].vx = vxDirection * vxMagnitude * cos(angleRadians);
                particles[j].vy = -vyMagnitude * sin(angleRadians); 
                particles[j].vx = vxDirection * vxMagnitude;
               // particles[j].vy = -vyMagnitude; 

                particles[j].x = x;
                particles[j].y = y;

                // random lifespan
                particles[j].lifespan = 400 + rand() % PARTICLE_LIFESPAN - 400 + 1;

                // emit only one particle at a time
                break;
            }
        }
    }
}




void freeParticleArray(Particle particles[]) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        SDL_FreeSurface(particles[i].image);
    }
    
   }





