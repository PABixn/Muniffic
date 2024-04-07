using System;
using System.Collections.Generic;
using System.Diagnostics.PerformanceData;
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

        }

        void OnUpdate(float ts)
        {
            
        }
    }
}
