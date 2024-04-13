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
        EntityTypes entityTypes;

        void OnCreate()
        {
            speed = 10f;
            velocity = Vector2.Zero;
            isGrounded = true;
            jumpForce = 0.5f;
            friction = 0.1f;
            entityTypes = EntityTypes.Human;

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
            velocity = Vector2.Zero;

            if (Input.IsKeyDown(KeyCode.Space) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, speed * jumpForce), true);
                isGrounded = false;
            }
            else if (Input.IsKeyDown(KeyCode.S) && Math.Abs(velocity.Y) <= 10f)
            {
                velocity.Y -= speed;
            }
            else if (Input.IsKeyDown(KeyCode.A) && velocity.X >= -10f)
            {
                velocity.X -= speed;
            }
            else if (Input.IsKeyDown(KeyCode.D) && velocity.X <= 10f)
            {
                velocity.X += speed;
            }

            if (rigidBody != null)
            {
                //DebugConsole.Log(Math.Sign((rigidBody.linearVelocity.X) * -1 * friction * ts).ToString(), DebugConsole.LogType.Info);
                rigidBody.ApplyLinearImpulse(velocity * ts, true);
                rigidBody.ApplyLinearImpulse(new Vector2(rigidBody.linearVelocity.X * -1 * friction * ts, 0), true);
            }
        }

        void ChangeType(EntityTypes type)
        {
            entityTypes = type;
        }
    }
}