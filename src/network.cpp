
#include <cstring>
#include "network.h"


int Network::init_network(Queue<Frame> *video, Queue<Frame> *audio) {
	inbuf_average = 0;
	this->inbuf = new uint8_t[5000000];
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Init network interface");

	SRD_Net_init();
	this->videoQueue = video;
	this->audioQueue = audio;
}

int Network::SRDNet_send_start_packet(int codecWidth, int codecHeight, int bandwidth, int fps) {
	// inital packet with information
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " network : sending start packet");
	Message *init = new Message() ;
	init->type = TYPE_ENCODER_START;
	init->x = 1;
	init->y = 1;
	init->button = 1;
	init->keycode = 1;
	init->fps = fps;
	init->codec_width = codecWidth;
	init->codec_height = codecHeight;
	init->bandwidth = bandwidth;
	init->sdl = 0;
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
			"sending init frame : type: %d, fps: %d, codec width: %d, codec height: %d, bandwidth: %d",
			init->type, init->fps, init->codec_width, init->codec_height, init->bandwidth);
	this->send(init);
	return 0;

}

int Network::SRDNet_send_stop_packet() {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " network : sending stop packet");
	struct Message stop;
	stop.type = TYPE_ENCODER_STOP;
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "sending stop frame");
	SRD_Net_send((void *) &stop, sizeof(stop));
	return 0;

}

int Network::SRDNet_get_frame_type() {
	SRD_ensure(4);
	return this->SRD_readUInt32();

}

int Network::SRDNet_get_frame_length() {
	this->SRD_ensure(4);
	return SRD_readUInt32();
}


void Network::SRDNet_Empty_input_buffer() {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "cleaning input tcp buffer");
	char net_in[2048];
	int i;

	do {
		i = SRD_Net_receive(net_in, 1024);
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%d bytes remains", i);
	} while (i > 0);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "cleaned");

}

void Network::SRD_ensure(int nbytes) {
	char net_in[nbytes];
	do {
		int net_lenght = SRD_Net_receive(net_in, nbytes); //FIXME : adjust max data
		if (net_lenght <= 0) {
			// TCP Connection is broken. (because of error or closure)
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Broken socket disconnected");
			SRD_Net_end();
			//FIXME

		} else {
			memcpy(inbuf + inbuf_average, net_in, net_lenght);
			inbuf_average = net_lenght + inbuf_average;
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "reading %d bytes from network. buffer average : %d\n",
					net_lenght, inbuf_average);
		}
	} while (inbuf_average < nbytes);

}

int Network::SRD_readUInt32() {
	uint8_t *data = this->SRD_read(4);
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, " %x %x %x %x \n", data[0], data[1], data[2], data[3]);
	uint32_t num =
		(uint32_t) data[3] << 24 |
		(uint32_t) data[2] << 16 |
		(uint32_t) data[1] << 8 |
		(uint32_t) data[0];
	return num;

}

uint8_t *Network::SRD_read(int nbytes) {
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "average byte : %d, read bytes : %d \n", this->inbuf_average, nbytes);
	uint8_t *data = new uint8_t[nbytes];
	std::memcpy(data, this->inbuf, nbytes) ;
	std::memcpy(this->inbuf, this->inbuf + nbytes, this->inbuf_average - nbytes ) ;
	inbuf_average -= nbytes;
	return data;
}

void Network::connect(std::string hostname, int port) {
	SRD_Net_connect( hostname.c_str(), port);
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "connecting to server");
}

int Network::send(Message *message) {
	int res = SRD_Net_send((void *) message, sizeof(Message));
	return res;
}


void Network::run() {
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "starting network thread");
	this->isRunning = true;
	while (this->isRunning) {

		// get frame from network
		FRAME_TYPE type = (FRAME_TYPE) this->SRDNet_get_frame_type();
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "frame type %d", type);
		int length = this->SRDNet_get_frame_length();
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "frame length %d", length);
		this->SRD_ensure(length);
		Frame *frame = new Frame();
		frame->data = this->SRD_read(length);
		frame->size = length;
		frame->type = type;

		if (frame->type == VIDEO_FRAME) {
			this->videoQueue->push(*frame);
		} else if (frame->type == AUDIO_FRAME) {
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "AUDIO frame length %d", frame->size);
			this->audioQueue->push(*frame);
		}
		else if (frame->type == DIMENSION_FRAME){
			int * dim = (int*) frame->data;
			SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, " DIM : width %d, height %d", dim[0], dim[1]);

		} else {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Unknow frame type %d", frame->type);
		}
	}
}



