using System;
using System.Collections.Generic;
using System.Diagnostics.PerformanceData;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
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
            if (m_Player.HasComponent<TextComponent>())
            {
                m_Player.AddComponent<TextComponent>();
                m_Player.GetComponent<TextComponent>().text = "Hello Worldfasdfasdfads!";
                m_Player.GetComponent<TextComponent>().color = Color.blue;
            }
        }

        void OnUpdate(float ts)
        {
            if (m_Player != null)
                m_Transform.translation = new Vector3(m_Player.GetComponent<TransformComponent>().translation.XY, DistanceFromPlayer);

            
        }

    }
}
