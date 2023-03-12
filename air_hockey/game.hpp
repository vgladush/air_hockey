#pragma once

#include "GUI_SDL.hpp"

class TinyFootball
{
public:
	TinyFootball();
	~TinyFootball();
	void start();

private:
	void begin_pos();
	void behav_bot();
	void behav_ball();
	void hit_ball(type_piece type);
	void confines(type_piece type);
	void extrude();
	std::unique_ptr<GUI_SDL> _lib;
	std::vector<piece> _pieces;
	bool _hard = false, _play = false, _mute = false;
	Event_en _event;

};
