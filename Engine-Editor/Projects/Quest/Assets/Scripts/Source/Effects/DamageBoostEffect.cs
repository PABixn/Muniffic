using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class DamageBoostEffect : Effect
    {
        private float additionalMultiplier;
        private float originalMultiplier;
        private float duration = 30;
        private float timeElapsed;
        private Entity entity;

        public DamageBoostEffect(float additionalMultiplier)
        {
            this.additionalMultiplier = additionalMultiplier;
        }

        public DamageBoostEffect(float additionalMultiplier, float duration)
        {
            this.additionalMultiplier = additionalMultiplier;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            entity.As<Player>().SetDamageMultiplier(entity.As<Player>().GetDamageMultiplier() + additionalMultiplier);
        }

        public EffectType GetEffectType()
        {
            return EffectType.DAMAGE_BOOST;
        }

        public void RemoveEffect()
        {
            entity.As<Player>().SetDamageMultiplier(entity.As<Player>().GetDamageMultiplier() - additionalMultiplier);
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void UpdateEffect(float ts)
        {
            timeElapsed += ts;
            if (timeElapsed >= duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
        }
    }
}
