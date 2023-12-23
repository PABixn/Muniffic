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
            float speed = 0.1f;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.W))
            {
                velocity.Y = speed;
            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if(Input.IsKeyDown(KeyCode.A))
            {
                velocity.X = -speed;
            }
            else if(Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = speed;
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
