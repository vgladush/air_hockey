#pragma once

#include "GUI_SDL.hpp"

class TinyFootball
{
public:
	TinyFootball();
	TinyFootball(type_piece t1, type_piece t2);
	~TinyFootball();
	void start();
	
	std::vector<piece> _pieces; // [6]:= [bot bot bot bot bot player]
	type_piece t1;
	type_piece t2;


private:
	void begin_pos();
	void behav_bot();
	void behav_ball();
	void hit_ball(int type);
	void confines(int type);
	void extrude();
	std::unique_ptr<GUI_SDL> _lib;
	bool _pvp = false , _hard = false, _play = false, _mute = true;
	Event_en _event;
};
