#include "game.hpp"

AirHockey::AirHockey() : _lib(new GUI_SDL)
{
	_pieces.resize(3); // 0 - bot, 1 - player, 2 - puck
}

AirHockey::~AirHockey() { }

void AirHockey::begin_pos()
{
	double hei = (HEIGHT - SIZE_BAT) / 2;
	double wid = (WIDTH - SIZE_BAT) / 2;
	
	_pieces[0] = { _pieces[0].score, wid, hei / 2, wid, hei / 2, 0, 0 };
	_pieces[1] = { _pieces[1].score, wid, hei + hei / 2, wid, hei + hei / 2, 0, 0 };
	_pieces[2] = { 0, wid + 2, hei, wid + 2, hei, 0, 0 };

}

void AirHockey::hit_puck(type_piece type)
{
	piece & bat = _pieces[type];
	piece & puck = _pieces[tpuck];

	int bit = 1 + abs(abs(bat.xs) > abs(bat.ys) ? bat.xs : bat.ys) / SIZE_BAT * 2;
	double jumpX = bat.xs / bit, jumpY = bat.ys / bit;

	bool hits = false;
	while (bit)
	{
		bat.xp += jumpX;
		bat.yp += jumpY;

		if (pow(SIZE_PUCK, 2) > pow(bat.xp - puck.x, 2) + pow(bat.yp - puck.y, 2))
		{
			if (!hits)
			{
				hits = true;
				double hyp = hypot(bat.xp - puck.x, bat.yp - puck.y);
				double sin = (bat.yp - puck.y) / hyp;
				double cos = (puck.x - bat.xp) / hyp;
				double nSpeed = puck.xs * cos - puck.ys * sin;
				double tSpeed = puck.xs * sin + puck.ys * cos;
				nSpeed = -nSpeed;

				puck.xs = tSpeed * sin + nSpeed * cos + bat.xs;
				puck.ys = tSpeed * cos - nSpeed * sin + bat.ys;
				
				while (pow(MAX_SPEED, 2) < pow(puck.xs, 2) + pow(puck.ys, 2))
				{
					puck.xs *= 0.98;
					puck.ys *= 0.98;
				}
			}
			puck.x += puck.xs;
			puck.y += puck.ys;
		}
		--bit;
	}
	if (hits)
		_lib->play_sound(hit);
}

void AirHockey::behav_puck()
{
	piece & puck = _pieces[tpuck];

	int wid = WIDTH - SIZE_PUCK - 2, hei = HEIGHT - SIZE_PUCK - 2;

	puck.x += puck.xs;
	puck.y += puck.ys;

	hit_puck(tplayer);
	hit_puck(tbot);

	//boards
	if (puck.x > wid || puck.x < 2)
	{
		puck.x = (puck.x > wid ? wid * 2 - puck.x : 4 - puck.x);
		puck.xs *= -1;
		_lib->play_sound(board);
	}
	if (puck.y > hei || puck.y < 2)
	{
		//goal
		if (puck.x > 162 && puck.x < 270)
		{
			_pieces[(puck.y > hei ? tbot : tplayer)].score++;
			puck.y = (puck.y > hei ? hei : 4);
			extrude();
			_lib->draw(_pieces);
			_lib->play_sound(goal);
			begin_pos();
			return;
		}
		puck.y = (puck.y > hei ? hei * 2 - puck.y : 4 - puck.y);
		puck.ys *= -1;
		_lib->play_sound(board);

	}
	extrude();

	// deceleration
	puck.xs *= 0.99;
	puck.ys *= 0.99;
	if (abs(puck.xs) < 0.7 && abs(puck.ys) < 0.7)
	{
		puck.xs = 0;
		puck.ys = 0;
	}
}

void AirHockey::extrude()
{
	piece & player = _pieces[tplayer];
	piece & bot = _pieces[tbot];
	piece & puck = _pieces[tpuck];
	int d = pow(SIZE_PUCK, 2);

	while (true)
	{
		if (d > pow(bot.x - puck.x, 2) + pow(bot.y - puck.y, 2))
		{
			bot.x -= bot.xs * 0.1;
			bot.y -= bot.ys * 0.1;
		}
		else if (d > pow(player.x - puck.x, 2) + pow(player.y - puck.y, 2))
		{
			player.x -= player.xs * 0.1;
			player.y -= player.ys * 0.1;
		}
		else
			break;
	}
}

void AirHockey::confines(type_piece type)
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

void AirHockey::behav_bot()
{
	piece & bot = _pieces[tbot];
	piece & puck = _pieces[tpuck];
	double speed = (_hard ? MAX_SPEED / 3: MAX_SPEED / 6);

	bot.xp = bot.x;
	bot.yp = bot.y;

	//predict position (coordinates) puck
	double preX = puck.x + puck.xs, preY = puck.y + puck.ys;
	double distance, distY;

	distance = abs(preX - bot.x);
	distY = preY - bot.y;
	bot.xs = (speed < distance ? speed : distance) * (preX < bot.x ? -1 : 1);
	if (preY > HEIGHT / 2)
		bot.ys -= (bot.y - speed > 75 ? speed : 0);
	else if (preY < bot.y + SIZE_BAT / 2)
		bot.ys = -speed;
	else if (distY > distance || speed > distance - SIZE_PUCK / 2)
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

void AirHockey::start()
{
	_lib->new_game(_hard);

	while (true)
	{
		_event = _lib->checkEvent(_pieces[1]);
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
			behav_puck();
			_lib->draw(_pieces);
		}
	}
}
