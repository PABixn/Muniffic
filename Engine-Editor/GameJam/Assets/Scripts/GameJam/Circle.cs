using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Circle : Enemy
    {
        float AttackCooldown = 0.2f;
        float ProjectileSpeed = 2.5f;
        List<Projectile> projectiles = new List<Projectile>();
        void OnCreate()
        {
            Console.WriteLine("Circle created! - " + entity.ID);
            Speed = 1.0f;
            Health = 400;
            Damage = 20;
            JumpHeight = 2.0f;
            attackType = AttackType.Ranged;
        }

        void Attack(Player target)
        {
            Console.WriteLine("Circle attacks!");
            Vector2 vector = Vector2.FindVector(Transform.translation.XY, playerTransform.translation.XY);
            vector.NormalizeTo(ProjectileSpeed);
            projectiles.Add(new Projectile(vector, this.Damage, ProjectileType.Normal));
        }

        void UpdateProjectilesPosition(float ts)
        {
            foreach (var projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }

        void OnUpdate(float ts)
        {
            base.OnUpdate(ts);
            UpdateProjectilesPosition(ts);
        }
    }
}