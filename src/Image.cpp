//
// Created by user on 11/12/18.
//


#include "Image.h"

Image::Image(int width, int height) {
	this->width = width;
	this->height = height;
	// set up YV12 pixel array (12 bits per pixel)
	this->yPlaneSz = this->width * this->height;
	this->uvPlaneSz = this->width * this->height / 4;
	this->yPlane = (Uint8*)malloc(yPlaneSz);
	this->uPlane = (Uint8*)malloc(uvPlaneSz);
	this->vPlane = (Uint8*)malloc(uvPlaneSz);
	if (!yPlane || !uPlane || !vPlane) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not allocate pixel buffers\n");
		//FIXME
	}

	this->uvPitch = this->width / 2;
}
