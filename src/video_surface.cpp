#include "video_surface.h"
#include "video_decoder.h"
#include "network.h"


void SdlVideoRenderer::destroy_texture() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Deleting SDL texture");
    SDL_DestroyTexture(bmp);
    SDL_DestroyRenderer(renderer);
}

void SdlVideoRenderer::update_video_surface(Image * image) {
    if (bmp != NULL) {
        SDL_UpdateYUVTexture(
                bmp,
                NULL,
                image->yPlane,
                this->width,
                image->uPlane,
                image->uvPitch,
                image->vPlane,
                image->uvPitch
        );

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bmp, NULL, NULL);
        SDL_RenderPresent(renderer);

    }


}

SdlVideoRenderer::SdlVideoRenderer(int rendererWidth, int rendererHeight, SDL_Window *screen) {
    this->width = rendererWidth;
    this->height = rendererHeight;
    this->screen = screen;
    this->renderer = SDL_CreateRenderer(screen, -1, 0);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL: could not create renderer - exiting\n");
        //FIXME
    }


    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Creating Windows Texture %dx%d", this->width, this->height);
    this->bmp = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING,
            this->width,
            this->height
    );
}
