using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class SquarePlayer : Player
    {
        public float shotSpeed;
        public float lastShot;
        public float shotDelay;
        public Vector2 collidePos;
        public Vector2 collideSize;
        public BoxCollider2DComponent playerCollider;
        public BoxCollider2DComponent floorCollider;

        public override void OnCreate()
        {
            
        }

        public override void OnUpdate(float ts)
        {

        }
    }
}
