using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class FireEffect : Effect
    {
        private int damage = 25;
        private float duration = 5.0f;
        private float timer = 0.0f;
        private float tickRate = 1.0f;
        private float tickTimer = 0.0f;

        private Entity entity;

        private HealthComponent healthComponent;

        public FireEffect()
        {
        }

        public FireEffect(int damage)
        {
            this.damage = damage;
        }

        public FireEffect(int damage, float duration)
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
            return EffectType.BURN;
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
            tickTimer += ts;
            timer += ts;
            if (timer >= duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
            if (tickTimer >= tickRate)
            {
                healthComponent.TakeDamage(damage);
                tickTimer = 0.0f;
            }
        }
    }


}
