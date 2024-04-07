using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Player : DefaultBehaviour
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_RigidBody2D;
        private AnimatorComponent m_Animator;
        private bool m_HasAnimator;
        Camera camera;

        public float Speed;
        public float Time = 0.0f;
        public Vector2 Velocity = new Vector2(0);
        void OnCreate()
        {

        }

        void OnUpdate(float ts)
        {

        }
    }
}
