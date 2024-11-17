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
        TRIANGLE, SQUARE, CIRCLE, NONE
    }

    public enum EntityType
    {
        PLAYER,
        ENEMY_TRIANGLE, ENEMY_SQUARE, ENEMY_CIRCLE,
        BULLET, STATIC, BOOST,NONE
    }

    public enum EffectType
    {
        NONE, BURN, SLOW, POISON, ARMOR, SPEED, JUMP, HEAL, SUN_RESIST, DAMAGE_BOOST,
        ATTACK_SPEED_BOOST, LAVA
    }
}


