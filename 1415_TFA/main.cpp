#include "flib.h"
#include "flib_vec2.h"
#include "camera.h"
#include "transform.h"
#include "constants.h"
#include "transform.h"
#include "uo_box.h"
#include "sprite.h"
#include "button.h"
#include "level.h"
#include "menu.h"
#include "time.h"
#include "game.h"
#include "input.h"

TGame g_tGame;

void Initialize()
{
	g_tGame.Initialize();
}

void Update()
{
	TInput::GetInstance().Update();
	g_tGame.Update();
}

void Render()
{
	GfxClear(EGfxColor_Black);
	g_tGame.Render();
}

void Finalize()
{
	g_tGame.~TGame();
}

void GfxMain(int, char *[])
{
	GfxCallbacks(Initialize, Update, Render, Finalize);
}
