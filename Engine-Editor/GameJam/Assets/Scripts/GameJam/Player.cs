using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Player : DefaultBehaviour
    {
        public float speed;
        public Vector2 velocity;
        RigidBody2DComponent rigidBody;
        public bool isGrounded;
        public float jumpForce;
        public float friction;
        public float lastJump;
        public float jumpDelay;
        public float lastShot;
        public float shotDelay;
        public float shotSpeed;
        List<Projectile> projectiles;
        EntityTypes entityTypes;
        Vector2 collidePos;
        Vector2 collideSize;
        Vector2 direction;
        BoxCollider2DComponent floorCollider;
        BoxCollider2DComponent playerCollider;
        TransformComponent transform;

        void OnCreate()
        {
            speed = 10f;
            velocity = Vector2.Zero;
            isGrounded = true;
            jumpForce = 0.5f;
            friction = 0.1f;
            entityTypes = EntityTypes.Human;
            playerCollider = GetComponent<BoxCollider2DComponent>();
            transform = GetComponent<TransformComponent>();
            collideSize = new Vector2(playerCollider.size.X, playerCollider.size.Y);
            lastJump = 0f;
            jumpDelay = 1f;
            lastShot = 0f;
            shotDelay = 2f;
            shotSpeed = 1f;

            projectiles = new List<Projectile>();

            floorCollider = Entity.FindEntityByName("floor").GetComponent<BoxCollider2DComponent>();
            if (HasComponent<RigidBody2DComponent>())
            {
                rigidBody = GetComponent<RigidBody2DComponent>();
            }
            else
            {
                DebugConsole.Log("RigidBody2DComponent not found", DebugConsole.LogType.Error);
            }
        }

        void OnUpdate(float ts)
        {
            lastJump += ts;
            lastShot += ts;
            collidePos.X = transform.translation.X;
            collidePos.Y = transform.translation.Y;


            if (isGrounded == false && lastJump >= jumpDelay && floorCollider.CollidesWithBox(collidePos - collideSize, collidePos + collideSize))
            {
                DebugConsole.Log(collideSize.Y.ToString(), DebugConsole.LogType.Info);
                isGrounded = true;
                lastJump = 0f;
            }

            velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.W) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, speed * jumpForce), true);
                isGrounded = false;
            }
            else if (Input.IsKeyDown(KeyCode.S) && Math.Abs(velocity.Y) <= 10f)
            {
                direction = new Vector2(0, -1);
                velocity.Y -= speed;
            }
            else if (Input.IsKeyDown(KeyCode.A) && velocity.X >= -10f)
            {
                direction = new Vector2(-1, 0);
                velocity.X -= speed;
            }
            else if (Input.IsKeyDown(KeyCode.D) && velocity.X <= 10f)
            {
                direction = new Vector2(1, 0);
                velocity.X += speed;
            }
            else if(lastShot >= shotDelay && Input.IsKeyDown(KeyCode.Space))
            {
                direction.NormalizeTo(shotSpeed);
                projectiles.Add(new Projectile(new Vector2(transform.translation.X + collideSize.X * direction.X + 0.5f * direction.X, transform.translation.Y), direction, 10, ProjectileType.Normal));
                lastShot = 0f;
            }

            if (rigidBody != null)
            {
                //DebugConsole.Log(Math.Sign((rigidBody.linearVelocity.X) * -1 * friction * ts).ToString(), DebugConsole.LogType.Info);
                rigidBody.ApplyLinearImpulse(velocity * ts, true);
                rigidBody.ApplyLinearImpulse(new Vector2(rigidBody.linearVelocity.X * -1 * friction * ts, 0), true);
            }

            UpdateProjectiles(ts);
        }

        private void UpdateProjectiles(float ts)
        {
            foreach(Projectile p in projectiles)
            {
                p.UpdatePosition(ts);
            }
        }

        void ChangeType(EntityTypes type)
        {
            entityTypes = type;
        }
    }
}