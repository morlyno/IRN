#include "Enemy.h"
#include <assert.h>
#include "SpriteEffect.h"
#include "MorMath.h"

Enemy::Enemy( const Vec2f& pos,Map& map )
	:
	Entity( pos,{ 0,0 },100,4.0f,20.0f,45.0f,10,1.0f,SurfaceCodex::Retrieve( L"Player.bmp" ),map )
{
}

void Enemy::AI( Entity& target )
{
	assert( &target != this );
	Vec2f dir = Vec2f( target.Pos() - pos );
	facing = ( int )MorMath::one( dir.x );
	float distsq = dir.getLengthSq();
	if ( distsq < TrackDistSq && distsq > inRangeSq )
	{
		setDir( dir.getSetOne(),false );
	}
	else
	{
		setDir( { 0,0 },false );
	}
	if ( AttackHB().isOverlappingWith( target.HitBox() ) )
	{
		target.TakeDamage( Damage );
	}
}

void Enemy::Draw( const Vec2f& pos_c,Graphics& gfx ) const
{
	const Vec2f offset = pos - pos_c;
	gfx.DrawSprite(
		int( offset.x * Map::Dimantion() - pSprite->getWidth() / 2 + gfx.ScreenWidth / 2 ),
		int( offset.y * Map::Dimantion() - pSprite->getHeight() + gfx.ScreenHeight / 2 ),
		*pSprite,
		facing < 0,
		SpriteEffect::Substitution( Colors::Red )
	);
}

RecF Enemy::AttackHB() const
{
	RecF AHB(
		pos.x * ( float )Map::Dimantion(),
		pos.x * ( float )Map::Dimantion() + ( float )( AttackWidth * ( int )facing ),
		pos.y * ( float )Map::Dimantion() - ( float )AttackHeight,
		pos.y * ( float )Map::Dimantion()
	);
	if ( AHB.left > AHB.right )
	{
		std::swap( AHB.left,AHB.right );
	}
	return AHB;
}
