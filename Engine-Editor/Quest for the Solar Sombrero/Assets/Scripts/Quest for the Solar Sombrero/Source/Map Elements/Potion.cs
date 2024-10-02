using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class Potion : DefaultBehaviour
    {
        private float duration = 10f;
        private float damageMultiplier = 1.5f;

        public void OnCreate()
        {

        }

        public void Update(float ts)
        {
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            if (collision.otherEntity == null || collision.otherEntity.name != "Player")
            {
                return;
            }
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new DamageBoostEffect(damageMultiplier, duration));
            Entity.Destroy(entity);
        }
    }
}
