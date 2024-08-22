using eg;
using Quest.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class Player : DefaultBehaviour
    {

        RigidBody2DComponent rigidBody;
        BoxCollider2DComponent collider;
        TransformComponent transform;

        public BoxCollider2DComponent groundCheck;

        public float maxSpeed = 10f;
        public float acceleration = 1f;
        public float decceleration = 1f;

        public bool shouldJump = false;
        public float jumpForce = 10f;
        public float coyoteTime = 0.1f;
        private float coyoteTimer = 0f;
        public float jumpBuffer = 0.1f;
        private float jumpBufferTimer = 0f;

        public void OnCreate()
        {

            transform = GetComponent<TransformComponent>();
            rigidBody = GetComponent<RigidBody2DComponent>();
            collider = GetComponent<BoxCollider2DComponent>();

            groundCheck = Entity.FindEntityByName("GroundCheck").GetComponent<BoxCollider2DComponent>();
        }

        public void OnUpdate(float ts)
        {
            if(collider == null) return;
            updateTimers(ts);
            Vector2 velocity = Vector2.Zero;
            if (Input.IsKeyDown(KeyCode.A) && !Input.IsKeyDown(KeyCode.D))
            {
                velocity = new Vector2(-1, 0);
            }
            if (Input.IsKeyDown(KeyCode.D) && !Input.IsKeyDown(KeyCode.A))
            {
                velocity = new Vector2(1, 0);
            }

            //Should be moved to fixed update
            Jump();

            if (rigidBody != null)
            {
                //should be moved to fixed update
                Run(velocity);
            }
        }

        private void Run(Vector2 velocity)
        {
            float targetSpeed = velocity.X * maxSpeed;

            float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            float movement = (float)speedDif * accelRate;
            
            rigidBody.ApplyLinearImpulse(Vector2.Right * movement);
        }

        private void Jump()
        {
            if (rigidBody == null) return;

            if(coyoteTimer > 0 && jumpBufferTimer > 0)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, jumpForce);
                jumpBufferTimer = 0;
                shouldJump = false;
            }
            if(!Input.IsKeyDown(KeyCode.Space) && rigidBody.linearVelocity.Y > 0.0f)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, rigidBody.linearVelocity.Y * 0.5f);
                coyoteTimer = 0;
            }
        }

        private void updateTimers(float ts)
        {
            if(isGrounded()){
                coyoteTimer = coyoteTime;
            }
            else
            {
                coyoteTimer -= ts;
            }

            if(Input.IsKeyDown(KeyCode.Space))
            {
                jumpBufferTimer = jumpBuffer;
            }
            else
            {
                jumpBufferTimer -= ts;
            }
        }

        private bool isGrounded()
        {
            return GroundCheck.IsGrounded(collider);
        }
    }
    
}
