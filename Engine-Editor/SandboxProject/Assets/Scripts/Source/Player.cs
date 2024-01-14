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
        Camera camera;

        public float Speed;
        public float Time = 0.0f;
        public Vector2 Velocity = new Vector2(0);
        void OnCreate()
        {
            Console.WriteLine("Player created! - " + entity.ID);
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody2D = GetComponent<RigidBody2DComponent>();
            bool hasTransform = HasComponent<TransformComponent>();
            Console.WriteLine("HasComponent {0}", hasTransform);
        }

        void OnUpdate(float ts)
        {
            Entity cameraEntity = Entity.FindEntityByName("Camera");
            if (cameraEntity != null)
            {
                
                camera = cameraEntity.As<Camera>();
            }
            else
            {
                Console.WriteLine("Camera is null");
            }
            Time += ts;
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

            if(Input.IsKeyDown(KeyCode.Q))
            {
                camera.DistanceFromPlayer += 1f * ts;
            }
            else if(Input.IsKeyDown(KeyCode.E))
            {
                camera.DistanceFromPlayer -= 1f * ts;
            }

            m_RigidBody2D.ApplyLinearImpulse(velocity.XY, true);

            //Vector3 translation = m_Transform.Translation;
            //translation += velocity * ts;
            //m_Transform.Translation = translation;
        }

    }
}
