using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using eg;

namespace Quest
{
    internal class Med_kit : DefaultBehaviour
    {
        public int healPercentage = 75;

        public void OnCreate()
        {
        }

        public void OnUpdate(float ts)
        {
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            var other = collision.otherEntity;
            if (other == null)
            {
                return;
            }

            if (other.name == "Player")
            {
                other.As<HealthComponent>().Heal(healPercentage);
                Entity.Destroy(entity);
            }
        }
    }
}
