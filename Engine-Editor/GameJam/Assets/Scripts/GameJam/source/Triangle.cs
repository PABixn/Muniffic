using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    class Triangle : Enemy
    {
        void OnCreate()
        {
            Console.WriteLine("Triangle created! - " + entity.ID);
            Speed = 1.5f;
            Health = 250;
            Damage = 80;
            JumpHeight = 1.0f;
            attackType = AttackType.Melee;
        }

        void Attack(Player target)
        {
            Console.WriteLine("Triangle attacks!");
            //target.Health -= Damage;
        }
    }
}