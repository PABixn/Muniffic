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
        public float jumpForce;
        public float lastJump;
        public float jumpDelay;
        public float shotSpeed;
        public float lastShot;
        public BoxCollider2DComponent playerCollider;
        public Vector2 collideSize;
        public List<Projectile> projectiles;
        AnimatorComponent animator;

        public override void OnCreate()
        {
            base.InitPlayer(80, 100, 2, 800);

            projectiles = new List<Projectile>();
            playerCollider = GetComponent<BoxCollider2DComponent>();
            animator = GetComponent<AnimatorComponent>();
            animator.Play("squareIdle");
            collideSize = playerCollider.size;
            shotSpeed = 5;
            lastShot = 0;
            jumpForce = 3f;
            lastJump = 0f;
            jumpDelay = 5f;
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            isGrounded = true;
            lastJump = 0f;
        }

        public override void OnUpdate(float ts)
        {
            lastShot += ts;

            if (base.UpdatePosition(ts))
            {
                animator.ChangeAnimation("squareWalk");
            }
            else
            {
                animator.ChangeAnimation("squareIdle");
            }

            if (Input.IsKeyDown(KeyCode.W) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, jumpForce * 10));
                isGrounded = false;
            }

            if (lastShot >= attackCooldown && Input.IsKeyDown(KeyCode.Space))
            {
                lastShot = 0;
                projectiles.Add(new Projectile(new Vector2(transform.translation.X + (collideSize.X * 2f) * direction.X, transform.translation.Y - transform.translation.Y / 2), Vector2.NormalizeTo(direction, shotSpeed), (int)damage, ProjectileType.Normal));
            }

            UpdateProjectiles(ts);
        }

        private void UpdateProjectiles(float ts)
        {
            foreach(Projectile projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }
    }
}
