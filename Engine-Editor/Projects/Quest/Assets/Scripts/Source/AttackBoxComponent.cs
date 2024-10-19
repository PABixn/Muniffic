using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public interface AttackBoxComponent
    {
        void OnCreate();
        void OnUpdate(float ts);
        bool CollidesWith(Entity e);
        Vector2 GetDirection();
        Vector2 GetSize();
        Vector2 GetOffset();
        Vector2 GetCenter();
    }
}
