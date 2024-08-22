using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class CirclePlayer : Player
    {
        public float jumpForce;
        public float lastJump;
        public float jumpDelay;
        public float shotSpeed;
        public float lastShot;
        public Vector2 collidePos;
        public float collideSize;
        public CircleCollider2DComponent playerCollider;
        public BoxCollider2DComponent floorCollider;
        public List<Projectile> projectiles;

        public override void OnCreate()
        {
            base.InitPlayer(10f, 20f, 0.2f, 400f);

            playerCollider = GetComponent<CircleCollider2DComponent>();
            floorCollider = Entity.FindEntityByName("floor").GetComponent<BoxCollider2DComponent>();
            projectiles = new List<Projectile>();
            jumpForce = 0.5f;
            lastJump = 0f;
            jumpDelay = 1f;
            collidePos = transform.translation.XY;
            collideSize = playerCollider.radius;
            lastShot = 0f;
            shotSpeed = 5f;
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            collision.otherEntity.GetComponent<SpriteRendererComponent>().color = Color.blueViolet;
        }

        public void OnCollisionExit(Collision2D collision)
        {
            collision.otherEntity.GetComponent<SpriteRendererComponent>().color = Color.brown;
        }

        public void OnKeyPress(KeyCode keyCode)
        {
            DebugConsole.Log("Key pressed "+keyCode, DebugConsole.LogType.Info);
            // Input.SetClipboardContent("Nacisnales " + keyCode.ToString());
            // DebugConsole.Log("Saved " + Input.GetClipboardContent().ToString(), DebugConsole.LogType.Info);
        }

        public override void OnUpdate(float ts)
        {
            lastJump += ts;
            lastShot += ts;
            collidePos = transform.translation.XY;

            base.UpdatePosition(ts);

            if (isGrounded == false && lastJump >= jumpDelay && floorCollider.CollidesWithCircle(collidePos, collideSize))
            {
                isGrounded = true;
                lastJump = 0f;
            }

            if(Input.IsKeyPressed(KeyCode.W) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, jumpForce * 10));
                isGrounded = false;
            }
            if (lastShot >= attackCooldown && Input.IsKeyPressed(KeyCode.Space))
            {
                lastShot = 0;
                projectiles.Add(new Projectile(new Vector2(transform.translation.X + collideSize * direction.X + 0.1f * direction.X, transform.translation.Y), Vector2.NormalizeTo(direction, shotSpeed), (int)damage, ProjectileType.Normal));
            }

            UpdateProjectiles(ts);
        }

        private void UpdateProjectiles(float ts)
        {
            foreach (Projectile projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }
    }
}
