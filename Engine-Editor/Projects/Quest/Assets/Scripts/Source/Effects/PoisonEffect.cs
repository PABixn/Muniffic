using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class PoisonEffect : Effect
    {
        private int damage = 10;
        private int duration = 3;
        private float timer = 0;
        private float tickRate = 1.0f;
        private float tick = 0;

        private Entity entity;

        private HealthComponent healthComponent;

        public PoisonEffect(int damage)
        {
            this.damage = damage;
        }

        PoisonEffect(int damage, int duration)
        {
            this.damage = damage;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            healthComponent = entity.As<HealthComponent>();
            healthComponent.TakeDamage(damage);
        }

        public EffectType GetEffectType()
        {
            return EffectType.POISON;
        }

        public void RemoveEffect()
        {
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void UpdateEffect(float ts)
        {
            timer += ts;
            tick += ts;
            if (timer >= duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
            if (tick >= tickRate)
            {
                tick = 0;
                entity.As<HealthComponent>().TakeDamage(damage);
            }
        }
    }
}
