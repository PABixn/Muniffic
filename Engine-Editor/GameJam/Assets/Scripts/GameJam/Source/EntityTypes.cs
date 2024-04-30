using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Game
{
    public enum EntityTypes
    {
        Human, Square, Circle, Triangle
    }

    public enum ProjectileType
    {
        Normal,
        Stun
    }

    public enum Stage
    {
        Square,
        Triangle,
        Circle
    };

    public enum AttackType
    {
        Melee,
        Ranged
    };
}
