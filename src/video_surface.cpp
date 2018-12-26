#include <va/va_x11.h>
#include "video_surface.h"


void SdlVideoRenderer::destroy_texture() {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Deleting SDL texture");
    SDL_DestroyTexture(bmp);
    SDL_DestroyRenderer(renderer);
}

void SdlVideoRenderer::update_video_surface(AVFrame * image) {
    if (bmp != NULL) {
        SDL_UpdateYUVTexture(
                bmp,
                NULL,
                image->data[0],
                image->linesize[0],
                image->data[1],
                image->linesize[1],
                image->data[2],
                image->linesize[2]
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
    this->renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);
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

Window SdlVideoRenderer::getX11Window() {
    SDL_SysWMinfo info;
    if (!SDL_GetWindowWMInfo(this->screen, &info)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "SDL_GetWindowWMInfo() failed: %s",
                     SDL_GetError());
    }
    return info.info.x11.window;
}
