/*main.c: black and white */
 
# include "SDL/SDL_image.h"
# include "SDL/SDL.h"
# include "err.h"
# include <stdio.h>
# include <math.h>
 
#include "pixel_operations.h"
 
 
 
 
 
void init_sdl(void) {
        // Init only the video part
        if ( SDL_Init(SDL_INIT_VIDEO)==-1 ) {
                // If it fails, die with an error message
                errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
        }
        // We don't really need a function for that ...
}
 
SDL_Surface* load_image(char *path) {
        SDL_Surface          *img;
        // Load an image using SDL_image with format detection
        img = IMG_Load(path);
        if (!img)
                // If it fails, die with an error message
                errx(3, "can't load %s: %s", path, IMG_GetError());
        return img;
}
 
SDL_Surface* display_image(SDL_Surface *img) {
        SDL_Surface          *screen;
        // Set the window to the same size as the image
        screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
        if ( screen == NULL ) {
                // error management
                errx(1, "Couldn't set %dx%d video mode: %s\n",
                                img->w, img->h, SDL_GetError());
        }
 
        /* Blit onto the screen surface */
        if (SDL_BlitSurface(img, NULL, screen, NULL) < 0)
                warnx("BlitSurface error: %s\n", SDL_GetError());
 
        // Update the screen
        SDL_UpdateRect(screen, 0, 0, img->w, img->h);
 
 
        // return the screen for further uses
        return screen;
}
 
 
 
void transform(SDL_Surface *img)
{
        for (int i = 0; i < img->w; ++i)
                for (int j = 0; j < img->h; ++j)
                {
                        Uint32 pix = getpixel(img, i, j);
                        Uint8 r = 0;
                        Uint8 g = 0;
                        Uint8 b = 0;
                        SDL_GetRGB(pix, img->format, &r, &g, &b);
                        if(r+g+b > 380)
                                pix = SDL_MapRGB(img->format, 255, 255, 255);
                        else
                                pix = SDL_MapRGB(img->format, 0, 0, 0);
                        putpixel(img, i, j, pix);
                }
}
int main(int argc, char **argv)
{
        if (argc < 2)
                errx(1, "Usage: ./main [IMAGE_FILE]\n");
        init_sdl();
        SDL_Surface *img = load_image(argv[1]);
        display_image(img);
        transform(img);
        SDL_SaveBMP(img, "afs/cri.epita.net/user/b/bo/bourgh_s/u/bourgh_s/bourgh_s/OCR/Image/test.jpg");
        return 0;
}
