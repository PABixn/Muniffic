using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Boss : Enemy
    {
        float AttackCooldown;
        float ProjectileSpeed;
        List<Projectile> projectiles = new List<Projectile>();
        float LastTimeUltUsed = 0.0f;
        bool CircleUltActive = false;
        float CircleUltDuration = 10.0f;
        Stage stage = Stage.Square;
        void OnCreate()
        {
            Console.WriteLine("Boss created! - " + entity.ID);
            Speed = 1.0f;
            Health = 1600;
            Damage = 200;
            JumpHeight = 0.0f;
            attackType = AttackType.Ranged;
            AttackCooldown = 1.0f;
            ProjectileSpeed = 1.0f;
        }

        //Looks goods
        void Attack()
        {
            if (attackType == AttackType.Ranged)
            {
                Vector2 vector = Vector2.FindVector(Transform.translation.XY, playerTransform.translation.XY);
                vector.NormalizeTo(ProjectileSpeed);
                projectiles.Add(new Projectile(Transform.translation.XY, vector, this.Damage, ProjectileType.Normal));
            }
        }

        void Ultimate()
        {
            Console.WriteLine("Boss uses ultimate!");
            if (stage == Stage.Triangle)
            {
                Vector2 vector = Vector2.FindVector(Transform.translation.XY, playerTransform.translation.XY);
                vector.NormalizeTo(ProjectileSpeed);
                projectiles.Add(new Projectile(Transform.translation.XY, vector, this.Damage, ProjectileType.Stun));
            }
            else if (stage == Stage.Circle)
            {
                CircleUltActive = true;
                AttackCooldown = 0.05f;
                Damage = 60;
            }
            else
            {

            }
        }

        void UpdateProjectilesPosition(float ts)
        {
            foreach (var projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }

        void SwitchForm(Stage stage)
        {
            if (stage == Stage.Square)
            {
                Speed = 3.0f;
                Health = 500;
                Damage = 160;
                JumpHeight = 2.0f;
                attackType = AttackType.Melee;
                ProjectileSpeed = 7.0f;
            }
            else if (stage == Stage.Triangle)
            {
                Speed = 2.0f;
                Health = 800;
                Damage = 40;
                JumpHeight = 4.0f;
                attackType = AttackType.Ranged;
                AttackCooldown = 0.1f;
                ProjectileSpeed = 5.0f;
            }
            else if (stage == Stage.Circle)
            {
                Entity.Destroy(entity.ID);
            }
        }

        void OnUpdate(float ts)
        {
            LastTimeUltUsed += ts;
            if (LastTimeUltUsed >= 20.0f && Math.Abs(playerTransform.translation.X - Transform.translation.X) < 150)
            {
                Ultimate();
                LastTimeUltUsed = 0.0f;
            }
            if (CircleUltActive)
            {
                CircleUltDuration -= ts;
                if (CircleUltDuration <= 0)
                {
                    CircleUltActive = false;
                    CircleUltDuration = 10.0f;
                    AttackCooldown = 0.1f;
                    Damage = 40;
                }
            }
            if (Health <= 0)
            {
                SwitchForm(stage);
                stage++;
            }
            base.OnUpdate(ts);
            UpdateProjectilesPosition(ts);
        }
    }
}