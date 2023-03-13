#include "game.hpp"

TinyFootball::TinyFootball() : _lib(new GUI_SDL)
{
	_pieces.resize(7); // 0,1,2 - bot, 3,4,5 - player, 6 - ball
	t1 = tplayer;
	t2 = tbot;
}

TinyFootball::TinyFootball(type_piece t1, type_piece t2 ) : _lib(new GUI_SDL)
{
	_pieces.resize(7); // 0,1,2 - bot, 3,4,5 - player, 6 - ball
	t1 = t1;
	t2 = t2;
}

TinyFootball::~TinyFootball() { }

void TinyFootball::begin_pos( )
{
	double hei = (HEIGHT - SIZE_BAT) / 2;
	double wid = (WIDTH - SIZE_BAT) / 2;
	
	_pieces[1] = { tbot, _pieces[1].score, wid - 50, hei / 2, wid - 50, hei / 2, 0, 0 };
	_pieces[3] = { tbot, _pieces[3].score, wid + 50, hei / 2, wid + 50, hei / 2, 0, 0 };
	_pieces[5] = { tbot, _pieces[5].score, wid , hei / 2 - 50, wid , hei / 2 - 50, 0, 0 };
	_pieces[2] = { tbot, _pieces[2].score, wid- 50, hei + hei / 2, wid - 50, hei + hei / 2, 0, 0 };
	_pieces[4] = { t2, _pieces[4].score, wid + 50,hei + hei / 2, wid + 50, hei +hei / 2, 0, 0 };
	_pieces[6] = { t1,_pieces[6].score, wid, hei + hei / 2 +50, wid, hei + hei / 2 +50, 0, 0 };
	_pieces[0] = { tball ,0, wid + 2, hei, wid + 2, hei, 0, 0 };
}

void TinyFootball::hit_ball(int type)
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

				ball.xs = tSpeed * sin*1.5 + nSpeed * cos + bat.xs;
				ball.ys = tSpeed * cos - nSpeed * sin*1.5 + bat.ys;
				
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
		std::cout << "hit \n";
		//_lib->play_sound(hit);
}

void TinyFootball::behav_ball()
{
	piece & ball = _pieces[tball];

	int wid = WIDTH - SIZE_BALL - 20, hei = HEIGHT - SIZE_BALL - 55;

	ball.x += ball.xs;
	ball.y += ball.ys;

	hit_ball(1);
	hit_ball(2);
	hit_ball(3);
	hit_ball(4);
	hit_ball(5);
	hit_ball(6);
	
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
			_pieces[(ball.y > hei ? 5 : 6)].score++;
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

void TinyFootball::confines(int type)
{
	piece & bat = _pieces[type];

	/*if ( type == 6 && bat.y < HEIGHT / 2 - 1)
		bat.y = HEIGHT / 2 - 1;
	if (type == 5 && bat.y > HEIGHT / 2 - SIZE_BAT)
		bat.y = HEIGHT / 2 - SIZE_BAT;*/
	if (type == 1 || type == 2) {
		if (bat.x > (WIDTH - SIZE_BAT -40)/3 - 20)
		{
			//_lib->play_sound(board);
			bat.x = (WIDTH - SIZE_BAT - 40)/ 3 - 20;
		}
		if (bat.x < 20)
		{
			//_lib->play_sound(board);
			bat.x = 20;
		}
	}
	else if (type == 5 || type == 6) {
		if (bat.x > 2* (WIDTH - SIZE_BAT - 40) / 3 + 20)
		{
			//_lib->play_sound(board);
			bat.x = 2* (WIDTH - SIZE_BAT - 40) / 3 + 20;
		}
		if (bat.x < (WIDTH - SIZE_BAT - 40) / 3 + 20)
		{
			//_lib->play_sound(board);
			bat.x = (WIDTH - SIZE_BAT - 40) / 3 + 20;
		}
	}
	else if (type == 3 || type == 4) {
		if (bat.x >  WIDTH  - SIZE_BAT - 20)
		{
			//_lib->play_sound(board);
			bat.x = WIDTH - SIZE_BAT - 20;
		}
		if (bat.x < 2 * (WIDTH - SIZE_BAT - 40) / 3 + 20)
		{
			//_lib->play_sound(board);
			bat.x = 2 * (WIDTH - SIZE_BAT - 40) / 3 + 20;
		}
	}


	if ((type == 2 || type == 4 || type == 6) && bat.y > HEIGHT - SIZE_BAT - 50)
	{
		//_lib->play_sound(board);
		bat.y = HEIGHT - SIZE_BAT - 50;
	}
	if ((type == 1 || type == 3 || type == 5) && bat.y < 50)
	{
		//_lib->play_sound(board);
		bat.y = 50;
	}
	bat.xs = bat.x - bat.xp;
	bat.ys = bat.y - bat.yp;
}

void TinyFootball::behav_bot()
{
	
	/*std::vector<piece> & pieces{ _pieces[tbot], _pieces[tbot + 2] ,_pieces[tbot + 4] ,_pieces[tplayer + 2],_pieces[tplayer + 4],_pieces[tball] };*/
	piece& ball = _pieces[0];

	for (int i = 1; i <= 6; i++) {
		double speed = (_hard ? MAX_SPEED / 3: MAX_SPEED / 6);
		if (_pieces[i].type == tplayer) continue;
		_pieces[i].xp = _pieces[i].x;
		_pieces[i].yp = _pieces[i].y;

		double preX = ball.x + ball.xs;
		double preY = ball.y + ball.ys;

		double distance, distY;
		distance = abs(preX - _pieces[i].x);
		distY = preY - _pieces[i].y;

		_pieces[i].xs = (speed < distance ? speed : distance) * (preX < _pieces[i].x ? -1 : 1)  ;
		if (i%2 == 1) {
			if (preY > HEIGHT / 2) {
				_pieces[i].ys -= (_pieces[i].y - speed > 75 ? speed : 0);
			}
			if (preY < _pieces[i].y + SIZE_BAT / 2)
				_pieces[i].ys = -speed;
			else if (distY > distance || speed > distance - SIZE_BALL / 2)
				_pieces[i].ys = (speed < distY ? speed : distY);
			else
				_pieces[i].ys = distY / (distance / speed);
		}
		else {
			if (preY < HEIGHT / 2) {
				_pieces[i].ys += (_pieces[i].y - speed > 75 ? speed : 0);
			}
			if (preY > _pieces[i].y + SIZE_BAT / 2)
				_pieces[i].ys = speed;
			else if (distY > distance || speed > distance - SIZE_BALL / 2)
				_pieces[i].ys = (speed < distY ? speed : distY);
			else
				_pieces[i].ys = distY / (distance / speed);
		}



		speed *= speed;
		while (speed < pow(_pieces[i].xs, 2) + pow(_pieces[i].ys, 2))
		{
			_pieces[i].xs *= 0.8;
			_pieces[i].ys *= 0.8;
		}
		_pieces[i].x += _pieces[i].xs;
		_pieces[i].y += _pieces[i].ys;
		
		confines(i);
	}
	
}

void TinyFootball::start()
{
	_lib->new_game(_hard);
	int player = 6;
	SDL_Event event;
	while (true)
	{
		_event = _lib->checkEvent(_pieces,player,_hard);
		switch (_event)
		{
		case nothing:
			break;
		case esc:
			return;
		case pvp:
			if (!_play)
			{
				_play = true;
				begin_pos();
			}
			break;
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
				_pieces[5].score = 0;
				_pieces[6].score = 0;
				_lib->new_game(_hard);
			}
			break;
		}
		if (_play)
		{
			behav_bot();
			confines(player);
			behav_ball();
			_lib->draw(_pieces);
		}
	}
}
