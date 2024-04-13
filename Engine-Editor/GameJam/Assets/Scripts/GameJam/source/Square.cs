using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    class Square : Enemy
    {
        public float AttackCooldown = 2.0f;
        public float ProjectileSpeed = 0.5f;
        List<Projectile> projectiles = new List<Projectile>();
        void OnCreate()
        {
            Console.WriteLine("Square created! - " + entity.ID);
            Speed = 0.5f;
            Health = 800;
            Damage = 100;
            JumpHeight = 0.0f;
            attackType = AttackType.Ranged;
        }

        void Attack(Player target)
        {
            Console.WriteLine("Square attacks!");
            Vector2 vector = Vector2.FindVector(Transform.translation.XY, playerTransform.translation.XY);
            vector.NormalizeTo(ProjectileSpeed);
            projectiles.Add(new Projectile(vector, this.Damage, Projectile.ProjectileType.Normal));
        }

        void UpdateProjectilesPosition(float ts)
        {
            foreach (var projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }

        protected void OnUpdate(float ts)
        {
            base.OnUpdate(ts);
            UpdateProjectilesPosition(ts);
        }
    }
}