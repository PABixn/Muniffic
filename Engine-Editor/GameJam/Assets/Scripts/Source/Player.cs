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
            speed = 1f;
            velocity = Vector2.Zero;
            isGrounded = true;
            jumpForce = 5f;
            friction = 0.5f;
            entityTypes = EntityTypes.Human;

            if(HasComponent<RigidBody2DComponent>())
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
            if(Input.IsKeyDown(KeyCode.Space) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, speed * jumpForce), true);
                isGrounded = false;
            }
            else if(Input.IsKeyDown(KeyCode.S) && Math.Abs(velocity.Y) <= 10f)
            {
                velocity.Y -= speed;
            }
            else if(Input.IsKeyDown(KeyCode.A) && Math.Abs(velocity.X) <= 10f)
            {
                velocity.X -= speed;
            }
            else if(Input.IsKeyDown(KeyCode.D) && Math.Abs(velocity.X) <= 10f)
            {
                velocity.X += speed;
            }

            rigidBody.ApplyLinearImpulse(velocity * ts, true);
            rigidBody.ApplyLinearImpulse(new Vector2(Math.Sign(rigidBody.linearVelocity.X) * friction * ts, 0), true);
        }

        void ChangeType(EntityTypes type)
        {
            entityTypes = type;
        }
    }
}
