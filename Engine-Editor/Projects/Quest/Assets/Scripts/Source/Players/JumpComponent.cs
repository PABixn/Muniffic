using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class JumpComponent : DefaultBehaviour
    {
        private bool Enabled = true;

        public bool shouldJump = false;
        public float jumpForce = 10f;
        public float coyoteTime = 0.1f;
        private float coyoteTimer = 0f;
        public float jumpBuffer = 0.1f;
        private float jumpBufferTimer = 0f;

        private RigidBody2DComponent rigidBody;
        private BoxCollider2DComponent collider;
        private AnimatorComponent animator;

        private float multiplier = 1.0f;

        public void OnCreate()
        {
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
            animator = entity.GetComponent<AnimatorComponent>();
        }

        public void OnUpdate(float ts)
        {
            if(rigidBody == null || collider == null || !Enabled) return;
            
            if (isGrounded())
            {
                coyoteTimer = coyoteTime;
            }
            else
            {
                coyoteTimer -= ts;
            }

            if (Input.IsKeyPressed(KeyCode.Space))
            {
                animator.ChangeAnimation("playerJump");
                animator.Play("playerJump");
                jumpBufferTimer = jumpBuffer;
            }
            else
            {
                jumpBufferTimer -= ts;
            }

            if (coyoteTimer > 0 && jumpBufferTimer > 0)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, jumpForce * multiplier);
                jumpBufferTimer = 0;
                shouldJump = false;
                animator.Stop();
            }
            if (!Input.IsKeyPressed(KeyCode.Space) && rigidBody.linearVelocity.Y > 0.0f)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, rigidBody.linearVelocity.Y * 0.3f);
                coyoteTimer = 0;
            }
        }

        private bool isGrounded()
        {
            return GroundCheck.IsGrounded(collider);
        }

        public float GetMultiplier()
        {
            return multiplier;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }

        public void Enable()
        {
            Enabled = true;
        }

        public void Disable()
        {
            Enabled = false;
        }
    }
}
