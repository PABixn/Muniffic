using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public enum Directions
    {
        LEFT, RIGHT, UP, DOWN, NONE
    }

    public enum BulletType
    {
        PLAYER, ENEMY
    }

    public enum EnemyType
    {
        TRIANGLE, SQUARE, CIRCLE
    }

    public enum PlayerType
    {
        CIRCLE, SQUARE, TRIANGLE, BASE, NONE
    }

    public enum EntityType
    {
        PLAYER_BASE, PLAYER_CIRCLE, PLAYER_TRIANGLE, PLAYER_SQUARE, 
        ENEMY_TRIANGLE, ENEMY_SQUARE, ENEMY_CIRCLE, 
        BULLET, STATIC, NONE
    }
}


