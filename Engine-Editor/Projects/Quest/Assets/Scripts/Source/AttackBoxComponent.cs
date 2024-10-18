using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    abstract public class AttackBoxComponent : DefaultBehaviour
    {
        public abstract void OnCreate();

        public abstract void OnUpdate(float ts);

        public abstract bool CollidesWith(Entity e);

        public abstract Vector2 GetDirection();
        public abstract Vector2 GetSize();
        public abstract Vector2 GetOffset();
        public abstract Vector2 GetCenter();
    }
}
