using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class JumpEffect : Effect
    {
        private float additionalMultiplier;
        private float duration = 10;
        private float timeElapsed;
        private Entity entity;

        public JumpEffect(float additionalMultiplier)
        {
            this.additionalMultiplier = additionalMultiplier;
        }

        public JumpEffect(float additionalMultiplier, float duration)
        {
            this.additionalMultiplier = additionalMultiplier;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            entity.As<JumpComponent>().SetMultiplier(entity.As<JumpComponent>().GetMultiplier() + additionalMultiplier);
        }

        public EffectType GetEffectType()
        {
            return EffectType.JUMP;
        }

        public void RemoveEffect()
        {
            entity.As<JumpComponent>().SetMultiplier(entity.As<JumpComponent>().GetMultiplier() - additionalMultiplier);
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
