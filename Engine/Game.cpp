/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 planetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
    for ( int x = -20; x < 10; ++x )
    {
        layout1.emplace_back( Vec2i{ x,0 } );
    }
    for ( int p = 10; p < 30; ++p )
    {
        layout1.emplace_back( Vec2i{ p,-p + 10 } );
    }
    for ( int x = -25; x < -15; ++x )
    {
        layout1.emplace_back( Vec2i{ x,-5 } );
    }
    for ( int p = -20; p < 20; ++p )
    {
        layout2.emplace_back( Vec2i{ p,p } );
    }
}

Game::~Game()
{
    pWorld->DeletePlayer();
    delete pWorld;
}

void Game::Go()
{
	gfx.BeginFrame();	
    bench.Start();
    HandleWorldObject();
	UpdateModel();
	ComposeFrame();
    if ( bench.End() )
    {
        OutputDebugString( ( std::wstring( bench ) + L"\n" ).c_str() );
    }
	gfx.EndFrame();
}

void Game::HandleWorldObject()
{
    if ( wnd.kbd.KeyIsPressed( VK_ESCAPE ) && g_state != GameState::InMenue )
    {
        g_state = GameState::InMenue;
        //pWorld->DeletePlayer();
        //delete pWorld;
        //pWorld = nullptr;
    }
    if ( World1SelectionHitBox.isCollidingWith( Vec2f( wnd.mouse.GetPos() ) ) && wnd.mouse.LeftIsPressed() && g_state == GameState::InMenue )
    {
        g_state = GameState::InGame1;
        if ( pWorld )
        {
            Player* pPlayerCopy = pWorld->getPl();
            delete pWorld;
            pWorld = new WorldObject( layout1,pPlayerCopy );
        }
        else
        {
            pWorld = new WorldObject( layout1,nullptr );
        }
    }
    if ( World2SelectionHitBox.isCollidingWith( Vec2f( wnd.mouse.GetPos() ) ) && wnd.mouse.LeftIsPressed() && g_state == GameState::InMenue )
    {
        g_state = GameState::InGame2;
        if ( pWorld )
        {
            Player* pPlayerCopy = pWorld->getPl();
            delete pWorld;
            pWorld = new WorldObject( layout2,pPlayerCopy );
        }
        else
        {
            pWorld = new WorldObject( layout2,{} );
        }
    }
}

void Game::UpdateModel()
{
#ifdef NDEBUG
    const auto dt = ft.Mark();
#else
    const auto dt = 1.0f / 60.0f;
#endif // NDEBUG
    if ( g_state != GameState::InMenue )
    {
        pWorld->Tick( dt );

        pWorld->HandleImputs( wnd.kbd );
    }
}

void Game::ComposeFrame()
{
    if ( g_state != GameState::InMenue )
    {
        gfx.PutPixel( gfx.ScreenWidth / 2,gfx.ScreenHeight / 2,Colors::Blue );
        pWorld->Draw( gfx );
        text.DrawText( "IRN",gfx.ScreenWidth / 2 - ( text.getGlythWidth() * 3 ) / 2,10,gfx );
    }
    else
    {
        text.DrawText( "In Menue",{ 100,100 },gfx );
        text.DrawText( "World 1",{ 100,200 },gfx );
        text.DrawText( "World 2",{ 100,300 },gfx );
        if ( World1SelectionHitBox.isCollidingWith( Vec2f( wnd.mouse.GetPos() ) ) )
        {
            gfx.DrawRecOutline( World1SelectionHitBox,1,Colors::LightGray );
        }
        if ( World2SelectionHitBox.isCollidingWith( Vec2f( wnd.mouse.GetPos() ) ) )
        {
            gfx.DrawRecOutline( World2SelectionHitBox,1,Colors::LightGray );
        }
    }
}
