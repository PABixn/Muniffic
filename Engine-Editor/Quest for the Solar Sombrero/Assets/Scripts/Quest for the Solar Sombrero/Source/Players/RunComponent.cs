﻿using eg;
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

        private Vector2 direction = Vector2.Zero;
        public float maxSpeed = 10f;
        public float acceleration = 1f;
        public float decceleration = 1f;

        private float multiplier = 1.0f;

        public void OnCreate()
        {
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
        }

        public void OnUpdate(float ts)
        {
            if(rigidBody == null) return;
            direction = Vector2.Zero;
            if (Input.IsKeyDown(KeyCode.A) && !Input.IsKeyDown(KeyCode.D))
            {
                direction = new Vector2(-1, 0);
            }
            if (Input.IsKeyDown(KeyCode.D) && !Input.IsKeyDown(KeyCode.A))
            {
                direction = new Vector2(1, 0);
            }

            float targetSpeed = direction.X * maxSpeed * multiplier;

            float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            float movement = (float)speedDif * accelRate;

            rigidBody.ApplyLinearImpulse(Vector2.Right * movement);
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