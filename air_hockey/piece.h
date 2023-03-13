#pragma once

enum type_piece
{
	tball,
	tbot,
	tplayer
};

enum Collision
{
	board,
	hit,
	goal
};

enum Event_en
{
	pvp,
	nothing,
	esc,
	play,
	dific,
	mus,
	menu
};

struct piece
{
	type_piece type;
	int8_t score;
	double x, y, xp, yp, xs, ys; // x,y | previous x,y | speed between previous and current 
};
