using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public abstract class BaseEnemy : DefaultBehaviour
    {

        public abstract void OnCreate();

        public abstract void OnUpdate(float ts);

        protected abstract void Init();

        protected abstract void Die();

    };
}
