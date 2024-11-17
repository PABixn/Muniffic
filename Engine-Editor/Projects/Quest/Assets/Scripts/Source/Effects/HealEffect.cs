using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class HealEffect : Effect
    {
        private int heal = 25;
        private float duration = 5.0f;
        private float timer = 0.0f;
        private float tickRate = 1.0f;
        private float tickTimer = 0.0f;

        private Entity entity;

        private HealthComponent healthComponent;

        public HealEffect()
        {
        }

        public HealEffect(int heal)
        {
            this.heal = heal;
        }

        public HealEffect(int heal, float duration)
        {
            this.heal = heal;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            healthComponent = entity.As<HealthComponent>();
            healthComponent.TakeDamage(heal);
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
                healthComponent.Heal(heal);
                tickTimer -= 1.0f;
            }
        }
    }


}
