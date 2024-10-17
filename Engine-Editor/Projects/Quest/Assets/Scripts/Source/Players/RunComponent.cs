using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class RunComponent : DefaultBehaviour
    {
        private RigidBody2DComponent rigidBody;
        private AnimatorComponent animatorComponent;
        private TransformComponent transformComponent;

        private Vector2 direction = Vector2.Zero;
        public float maxSpeed = 10f;
        public float acceleration = 1f;
        public float decceleration = 1f;

        private float multiplier = 1.0f;

        public void OnCreate()
        {
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
            animatorComponent = entity.GetComponent<AnimatorComponent>();
            transformComponent = entity.GetComponent<TransformComponent>();
            multiplier *= rigidBody.mass;
        }

        public void OnUpdate(float ts)
        {
            if(rigidBody == null) return;
            direction = Vector2.Zero;
            if (Input.IsKeyPressed(KeyCode.A) && !Input.IsKeyPressed(KeyCode.D))
            {
                direction = new Vector2(-1, 0);
                transformComponent.scale = new Vector3(-1, 1, 1);
                
            }
            if (Input.IsKeyPressed(KeyCode.D) && !Input.IsKeyPressed(KeyCode.A))
            {
                direction = new Vector2(1, 0);
                transformComponent.scale = new Vector3(1, 1, 1);
            }

            if (Input.IsKeyPressed(KeyCode.A) || Input.IsKeyPressed(KeyCode.D))
            {
                animatorComponent.ChangeAnimation("playerWalk");
                animatorComponent.Play("playerWalk");
            }

            if (!Input.IsKeyPressed(KeyCode.A) && !Input.IsKeyPressed(KeyCode.D))
            {
                animatorComponent.Stop();
            }

            //float targetSpeed = direction.X * maxSpeed * multiplier;

            //float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            //float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            //float movement = (float)speedDif * accelRate;

            rigidBody.linearVelocity = new Vector2(direction.X * maxSpeed * multiplier, rigidBody.linearVelocity.Y);
        }

        public float GetMultiplier()
        {
            return multiplier;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }
    }
}
