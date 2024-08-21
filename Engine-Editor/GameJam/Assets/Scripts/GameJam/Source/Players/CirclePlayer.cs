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
        //Should just be position and it is not necessary to be a member variable
        public Vector2 collidePos;
        //Shouldn't be a variable inside a member variable
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
            //should be set in editor
            jumpForce = 0.5f;
            lastJump = 0f;
            jumpDelay = 1f;
            lastShot = 0f;
            shotSpeed = 5f;

            collidePos = transform.translation.XY;
            collideSize = playerCollider.radius;
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            //Done for testing
            collision.otherEntity.GetComponent<SpriteRendererComponent>().color = Color.blueViolet;
        }

        public void OnCollisionExit(Collision2D collision)
        {
            //Done for testing
            collision.otherEntity.GetComponent<SpriteRendererComponent>().color = Color.brown;
        }

        public override void OnUpdate(float ts)
        {
            lastJump += ts;
            lastShot += ts;
            //This variable should be initialized here
            collidePos = transform.translation.XY;

            //Each player should have their own function for updating movement should be called from base class
            base.UpdatePosition(ts);


            if (isGrounded == false && lastJump >= jumpDelay && floorCollider.CollidesWithCircle(collidePos, collideSize))
            {
                isGrounded = true;
                lastJump = 0f;
            }

            //Switch to coyote jump
            if (Input.IsKeyDown(KeyCode.W) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, jumpForce * 10));
                isGrounded = false;
            }
            if (lastShot >= attackCooldown && Input.IsKeyDown(KeyCode.Space))
            {

                lastShot = 0;
                //Make it more readable
                projectiles.Add(new Projectile(new Vector2(transform.translation.X + collideSize * direction.X + 0.1f * direction.X, transform.translation.Y), Vector2.NormalizeTo(direction, shotSpeed), (int)damage, ProjectileType.Normal));
            }

            //Each projectile should update itself
            UpdateProjectiles(ts);
        }

        //Each projectile should update itself
        private void UpdateProjectiles(float ts)
        {
            foreach (Projectile projectile in projectiles)
            {
                projectile.UpdatePosition(ts);
            }
        }
    }
}
