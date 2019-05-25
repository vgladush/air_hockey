#pragma once

enum type_piece
{
	tbot,
	tplayer,
	tpuck
};

enum Collision
{
	board,
	hit,
	goal
};

enum Event_en
{
	nothing,
	esc,
	play,
	dific,
	mus,
	menu
};

struct piece
{
	int8_t score;
	double x, y, xp, yp, xs, ys; // x,y | previous x,y | speed between previous and current 
};
