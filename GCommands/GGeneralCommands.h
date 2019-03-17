#pragma once
#ifndef GGENERAL_COMMANDS_H
#define GGENERAL_COMMANDS_H

class GGeneralCommands
{
public:
	GGeneralCommands()								{ _pause = _quit = false; }
	GGeneralCommands(const GGeneralCommands& o);
	virtual ~GGeneralCommands()						{}

	bool operator == (const GGeneralCommands& other) const;

	inline GGeneralCommands& operator=(const GGeneralCommands& other)
	{
		if (*this == other) return *this;
		_pause = other._pause;
		_quit = other._quit;
		return *this;
	}

	inline void resetValues()					{ _pause = _quit = false;  }

	inline bool isPause() const					{ return _pause; }
	inline bool quit() const					{ return _quit; }

	inline bool& isPause() 						{ return _pause; }
	inline bool& quit() 						{ return _quit; }

	inline void changePauseStatus()				{ _pause = !_pause; }

private:
	bool _pause;
	bool _quit;
};

#endif	//GGENERAL_COMMANDS_H