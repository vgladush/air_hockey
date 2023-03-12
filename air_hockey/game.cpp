#include "game.hpp"

TinyFootball::TinyFootball() : _lib(new GUI_SDL)
{
	_pieces.resize(3); // 0 - bot, 1 - player, 2 - ball
}

TinyFootball::~TinyFootball() { }

void TinyFootball::begin_pos()
{
	double hei = (HEIGHT - SIZE_BAT) / 2;
	double wid = (WIDTH - SIZE_BAT) / 2;
	
	_pieces[0] = { _pieces[0].score, wid, hei / 2, wid, hei / 2, 0, 0 };
	_pieces[1] = { _pieces[1].score, wid, hei + hei / 2, wid, hei + hei / 2, 0, 0 };
	_pieces[2] = { 0, wid + 2, hei, wid + 2, hei, 0, 0 };

}

void TinyFootball::hit_ball(type_piece type)
{
	piece & bat = _pieces[type];
	piece & ball = _pieces[tball];

	int bit = 1 + abs(abs(bat.xs) > abs(bat.ys) ? bat.xs : bat.ys) / SIZE_BAT * 2;
	double jumpX = bat.xs / bit, jumpY = bat.ys / bit;

	bool hits = false;
	while (bit)
	{
		bat.xp += jumpX;
		bat.yp += jumpY;

		if (pow(SIZE_BALL, 2) > pow(bat.xp - ball.x, 2) + pow(bat.yp - ball.y, 2))
		{
			if (!hits)
			{
				hits = true;
				double hyp = hypot(bat.xp - ball.x, bat.yp - ball.y);
				double sin = (bat.yp - ball.y) / hyp;
				double cos = (ball.x - bat.xp) / hyp;
				double nSpeed = ball.xs * cos - ball.ys * sin;
				double tSpeed = ball.xs * sin + ball.ys * cos;
				nSpeed = -nSpeed;

				ball.xs = tSpeed * sin + nSpeed * cos + bat.xs;
				ball.ys = tSpeed * cos - nSpeed * sin + bat.ys;
				
				while (pow(MAX_SPEED, 2) < pow(ball.xs, 2) + pow(ball.ys, 2))
				{
					ball.xs *= 0.98;
					ball.ys *= 0.98;
				}
			}
			ball.x += ball.xs;
			ball.y += ball.ys;
		}
		--bit;
	}
	if (hits)
		_lib->play_sound(hit);
}

void TinyFootball::behav_ball()
{
	piece & ball = _pieces[tball];

	int wid = WIDTH - SIZE_BALL - 20, hei = HEIGHT - SIZE_BALL - 55;

	ball.x += ball.xs;
	ball.y += ball.ys;

	hit_ball(tplayer);
	hit_ball(tbot);

	//boards
	if (ball.x > wid || ball.x < 20)
	{
		ball.x = (ball.x > wid ? wid * 2 - ball.x : 40 - ball.x);
		ball.xs *= -1;
		_lib->play_sound(board);
	}
	if (ball.y > hei || ball.y < 55)
	{
		//goal
		if (ball.x > 170 && ball.x < 262)
		{
			_pieces[(ball.y > hei ? tbot : tplayer)].score++;
			ball.y = (ball.y > hei ? hei : 55);
			extrude();
			_lib->draw(_pieces);
			_lib->play_sound(goal);
			begin_pos();
			return;
		}
		ball.y = (ball.y > hei ? hei * 2 - ball.y : 110 - ball.y);
		ball.ys *= -1;
		_lib->play_sound(board);

	}
	extrude();

	// deceleration
	ball.xs *= 0.99;
	ball.ys *= 0.99;
	if (abs(ball.xs) < 0.7 && abs(ball.ys) < 0.7)
	{
		ball.xs = 0;
		ball.ys = 0;
	}
}

void TinyFootball::extrude()
{
	piece & player = _pieces[tplayer];
	piece & bot = _pieces[tbot];
	piece & ball = _pieces[tball];
	int d = pow(SIZE_BALL, 2);

	while (true)
	{
		if (d > pow(bot.x - ball.x, 2) + pow(bot.y - ball.y, 2))
		{
			bot.x -= bot.xs * 0.1;
			bot.y -= bot.ys * 0.1;
		}
		else if (d > pow(player.x - ball.x, 2) + pow(player.y - ball.y, 2))
		{
			player.x -= player.xs * 0.1;
			player.y -= player.ys * 0.1;
		}
		else
			break;
	}
}

void TinyFootball::confines(type_piece type)
{
	piece & bat = _pieces[type];

	if (type == tplayer && bat.y < HEIGHT / 2 - 1)
		bat.y = HEIGHT / 2 - 1;
	if (type == tbot && bat.y > HEIGHT / 2 - SIZE_BAT)
		bat.y = HEIGHT / 2 - SIZE_BAT;
	if (bat.x > WIDTH - SIZE_BAT - 2)
	{
		_lib->play_sound(board);
		bat.x = WIDTH - SIZE_BAT - 2;
	}
	if (bat.x < 2)
	{
		_lib->play_sound(board);
		bat.x = 2;
	}
	if (type == tplayer && bat.y > HEIGHT - SIZE_BAT - 2)
	{
		_lib->play_sound(board);
		bat.y = HEIGHT - SIZE_BAT - 2;
	}
	if (type == tbot && bat.y < 2)
	{
		_lib->play_sound(board);
		bat.y = 2;
	}
	bat.xs = bat.x - bat.xp;
	bat.ys = bat.y - bat.yp;
}

void TinyFootball::behav_bot()
{
	piece & bot = _pieces[tbot];
	piece & ball = _pieces[tball];
	double speed = (_hard ? MAX_SPEED / 3: MAX_SPEED / 6);

	bot.xp = bot.x;
	bot.yp = bot.y;

	//predict position (coordinates) ball
	double preX = ball.x + ball.xs, preY = ball.y + ball.ys;
	double distance, distY;

	distance = abs(preX - bot.x);
	distY = preY - bot.y;
	bot.xs = (speed < distance ? speed : distance) * (preX < bot.x ? -1 : 1);
	if (preY > HEIGHT / 2)
		bot.ys -= (bot.y - speed > 75 ? speed : 0);
	else if (preY < bot.y + SIZE_BAT / 2)
		bot.ys = -speed;
	else if (distY > distance || speed > distance - SIZE_BALL / 2)
		bot.ys = (speed < distY ? speed : distY);
	else
		bot.ys = distY / (distance / speed);

	speed *= speed;
	while (speed < pow(bot.xs, 2) + pow(bot.ys, 2))
	{
		bot.xs *= 0.9;
		bot.ys *= 0.9;
	}
	bot.x += bot.xs;
	bot.y += bot.ys;

	confines(tbot);
}

void TinyFootball::start()
{
	_lib->new_game(_hard);

	while (true)
	{
		_event = _lib->checkEvent(_pieces,1);
		switch (_event)
		{
		case nothing:
			break;
		case esc:
			return;
		case play:
			if (!_play)
			{
				_play = true;
				begin_pos();
			}
			break;
		case dific:
			if (!_play)
			{
				_hard = !_hard;
				_lib->new_game(_hard);
			}
			break;
		case mus:
			_mute = _lib->change_noise();
			if (!_play)
				_lib->new_game(_hard);
			break;
		case menu:
			if (_play)
			{
				_play = false;
				_pieces[0].score = 0;
				_pieces[1].score = 0;
				_lib->new_game(_hard);
			}
			break;
		}
		if (_play)
		{
			behav_bot();
			confines(tplayer);
			behav_ball();
			_lib->draw(_pieces);
		}
	}
}
