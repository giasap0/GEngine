#include "GGeneralCommands.h"


GGeneralCommands::GGeneralCommands(const GGeneralCommands& other)
{
	_pause = other._pause;
	_quit = other._quit;
}

bool GGeneralCommands::operator == (const GGeneralCommands& other) const
{
	return (_quit == other._quit) && (_pause == other._pause);
}