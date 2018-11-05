#ifndef SDL_CLIENT_GLOBAL_H
#define SDL_CLIENT_GLOBAL_H


#ifdef __cplusplus
extern "C" {
#endif
typedef struct Global Global;
struct Global
{
	Configuration configuration;
	bool quit;

};

Global *global;
#ifdef __cplusplus
}
#endif
#endif
