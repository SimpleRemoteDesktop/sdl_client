#ifdef HAVE_VAAPI
#include <va/va_x11.h>
#endif
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
    this->init();
}

SDL_Rect SdlVideoRenderer::getSize() {
    SDL_Rect size;
    size.x = size.y = 0;
    SDL_GetWindowSize(this->screen, &size.w, &size.h);
    return size;
}

void SdlVideoRenderer::restart() {
    this->destroy_texture();
    this->init();
}

void SdlVideoRenderer::init() {
this->renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

#ifdef __linux__
Window SdlVideoRenderer::getX11Window() {
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(this->screen, &info)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "SDL_GetWindowWMInfo() failed: %s",
                     SDL_GetError());
    }
    return info.info.x11.window;
}
#endif
