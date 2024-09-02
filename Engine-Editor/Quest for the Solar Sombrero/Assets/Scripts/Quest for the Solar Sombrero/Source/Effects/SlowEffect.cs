using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class SlowEffect : Effect
    {

        private float additionalMultiplier;
        private float duration;
        private float timeElapsed;
        private Entity entity;

        public SlowEffect(float additionalMultiplier)
        {
            this.additionalMultiplier = additionalMultiplier;
            duration = 10;
        }

        public SlowEffect(float additionalMultiplier, float duration)
        {
            this.additionalMultiplier = additionalMultiplier;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            entity.As<RunComponent>().SetMultiplier(entity.As<RunComponent>().GetMultiplier() - additionalMultiplier);
        }

        public EffectType GetEffectType()
        {
            return EffectType.SPEED;
        }

        public void RemoveEffect()
        {
            entity.As<RunComponent>().SetMultiplier(entity.As<RunComponent>().GetMultiplier() + additionalMultiplier);
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
