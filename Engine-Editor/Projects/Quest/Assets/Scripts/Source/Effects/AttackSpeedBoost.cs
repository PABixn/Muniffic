using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class AttackSpeedBoost : Effect
    {
        private float additionalMultiplier;
        private float duration = 10;
        private float timeElapsed;
        private Entity entity;

        public AttackSpeedBoost(float additionalMultiplier)
        {
            this.additionalMultiplier = additionalMultiplier;
        }

        public AttackSpeedBoost(float additionalMultiplier, float duration)
        {
            this.additionalMultiplier = additionalMultiplier;
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            entity.As<Player>().SetAttackSpeedMultiplier(entity.As<Player>().GetAttackSpeedMultiplier() + additionalMultiplier);
        }

        public EffectType GetEffectType()
        {
            return EffectType.ATTACK_SPEED_BOOST;
        }

        public void RemoveEffect()
        {
            entity.As<Player>().SetAttackSpeedMultiplier(entity.As<Player>().GetAttackSpeedMultiplier() - additionalMultiplier);
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void UpdateEffect(float ts)
        {
            timeElapsed += ts;
            if (timeElapsed > duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
        }
    }
}
