using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Camera : DefaultBehaviour
    {
        private Entity m_Player;
        private TransformComponent m_Transform;

        public float DistanceFromPlayer = 5.0f;
        public float Speed = 5.0f;

        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
            m_Player = Entity.FindEntityByName("Player");
        }

        void OnUpdate(float ts)
        {
            if (m_Player != null)
                m_Transform.Translation = new Vector3(m_Player.GetComponent<TransformComponent>().Translation.XY, DistanceFromPlayer);

            float speed = 5f;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.Up))
            {
                velocity.Y = speed;
            }
            else if (Input.IsKeyDown(KeyCode.Down))
            {
                velocity.Y = -speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if(Input.IsKeyDown(KeyCode.Left))
            {
                velocity.X = -speed;
            }
            else if(Input.IsKeyDown(KeyCode.Right))
            {
                velocity.X = speed;
            }
            else
            {
                velocity.X = 0f;
            }

            Vector3 translation = m_Transform.Translation;
            translation += velocity * ts;
            m_Transform.Translation = translation;
        }

    }
}
