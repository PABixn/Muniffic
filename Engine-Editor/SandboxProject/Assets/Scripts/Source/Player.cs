using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_RigidBody2D;

        public float Speed = 0.2f;
        public float Timestep = 0.0f;
        public Vector3 Velocity = new Vector3(0);
        void OnCreate()
        {
            Console.WriteLine("Player created! - " + ID);
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody2D = GetComponent<RigidBody2DComponent>();
            bool hasTransform = HasComponent<TransformComponent>();
            Console.WriteLine("HasComponent {0}", hasTransform);
            //m_Transform.Translation = new Vector3(0f);
        }

        void OnUpdate(float ts)
        {
            Timestep += ts;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y = Speed;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -Speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if(Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -Speed;
            }
            else if(Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = Speed;
            }
            else
            {
                velocity.X = 0f;
            }

            m_RigidBody2D.ApplyLinearImpulse(velocity.XY, true);

            //Vector3 translation = m_Transform.Translation;
            //translation += velocity * ts;
            //m_Transform.Translation = translation;
        }

    }
}
