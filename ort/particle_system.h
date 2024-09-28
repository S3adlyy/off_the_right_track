#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H
#include <SDL/SDL.h>
#include <SDL/SDL_image.h> 
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "fonctions.h"
#include "constants.h"
#include "scrolling.h"
#include <math.h>
// Define constants for the particle system
#define MAX_PARTICLES 1000
#define PARTICLE_LIFESPAN 500

// Define a structure to hold particle information
typedef struct {
    SDL_Surface* image; // Image of the particle
    int x, y;           // Position of the particle
    int vx, vy;         // Velocity of the particle
    int lifespan;       // Remaining lifespan of the particle
    int width, height;  // Dimensions of the particle image
} Particle;



// Function to initialize the particle system with given parameters and image
void initParticleSystems(Particle particles[], SDL_Surface* particleImage, int imageWidth, int imageHeight);//init particle systems
void emitParticles(Particle particles[], int x, int y, int playerDirection, int modifier);//particle Trail
void emitParticlesExplosion(Particle particles[], int x, int y, int modifier,  int velocityrangeX, int velocityrangeY); //particle explosion
void updateParticleSystems(Particle particles[],Uint32 dt,Camera camera);//update particles lifespan and position
void renderParticles(Particle particles[],SDL_Surface *screen, Camera camera);//render particles
void freeParticleArray(Particle particles[]);//free







#endif /* PARTICLE_SYSTEM_H */

