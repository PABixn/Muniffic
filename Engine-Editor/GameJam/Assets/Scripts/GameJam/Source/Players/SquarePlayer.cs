﻿using System;
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
        public BoxCollider2DComponent playerCollider;
        public Vector2 collideSize;
        public List<Projectile> projectiles;

        public override void OnCreate()
        {
            //shouldn't be in base Player
            base.InitPlayer(10, 100, 2, 800);

            projectiles = new List<Projectile>();
            playerCollider = GetComponent<BoxCollider2DComponent>();
            collideSize = playerCollider.size;
            shotSpeed = 5;
            lastShot = 0;
        }

        public override void OnUpdate(float ts)
        {
            lastShot += ts;

            //shouldn't be in base Player
            base.UpdatePosition(ts);

            if (lastShot >= attackCooldown && Input.IsKeyDown(KeyCode.Space))
            {
                lastShot = 0;
                //Make this more readable
                projectiles.Add(new Projectile(new Vector2(transform.translation.X + collideSize.X * direction.X + 0.1f * direction.X, transform.translation.Y), Vector2.NormalizeTo(direction, shotSpeed), (int)damage, ProjectileType.Normal));
            }

            UpdateProjectiles(ts);
        }

        //Each projectile should handle itself it should be done in Player
        private void UpdateProjectiles(float ts)
        {
            foreach (Projectile projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }
    }
}
