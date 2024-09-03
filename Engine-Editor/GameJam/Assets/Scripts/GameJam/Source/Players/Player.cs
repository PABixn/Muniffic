using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public abstract class Player : DefaultBehaviour
    {
        public float speed;
        public Vector2 velocity;
        public bool isGrounded;
        public float friction;
        public float gravityMultiplier;
        public float damage;
        public float attackCooldown;
        public float health;
        public Vector2 direction;
        public TransformComponent transform;
        public RigidBody2DComponent rigidBody;

        public void InitPlayer(float speed, float damage, float attackCooldown, float health)
        {
            direction = Vector2.Zero;
            velocity = Vector2.Zero;
            friction = 0.8f;
            gravityMultiplier = 0.5f;
            this.isGrounded = true;
            transform = GetComponent<TransformComponent>();
            rigidBody = GetComponent<RigidBody2DComponent>();

            this.damage = damage;
            this.speed = speed;
            this.attackCooldown = attackCooldown;
            this.health = health;
        }

        public abstract void OnCreate();
        public abstract void OnUpdate(float ts);

        public void UpdatePosition(float ts)
        {
            velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.S) && Math.Abs(velocity.Y) <= 10f)
            {
                direction = new Vector2(0, -1);
                velocity.Y -= speed;
            }
            if (Input.IsKeyDown(KeyCode.A) && velocity.X >= -10f)
            {
                direction = new Vector2(-1, 0);
                velocity.X -= speed;
            }
            if (Input.IsKeyDown(KeyCode.D) && velocity.X <= 10f)
            {
                direction = new Vector2(1, 0);
                velocity.X += speed;
            }

            if (rigidBody != null)
            {
                rigidBody.ApplyLinearImpulse(velocity * ts, true);

                if(isGrounded)
                    rigidBody.ApplyLinearImpulse(new Vector2(rigidBody.linearVelocity.X * -1 * friction * ts, 0), true);
            }
        }
    }
}