using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class ArmorEffect : Effect
    {
        private int armorValue;
        private float duration = 30;
        private float timeElapsed;
        private Entity entity;

        public ArmorEffect(int armorValue)
        {
            this.armorValue = armorValue;
        }

        public ArmorEffect(int armorValue, float duration)
        {
            this.armorValue = armorValue;
            this.duration = duration;
        }
        public void SetDuration(float duration)
        {
            this.duration = duration;
        }

        public void ApplyEffect()
        {
            entity.As<HealthComponent>().SetArmor(armorValue);
        }

        public EffectType GetEffectType()
        {
            return EffectType.ARMOR;
        }

        public void RemoveEffect()
        {
            entity.As<HealthComponent>().SetArmor(0);
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void UpdateEffect(float ts)
        {
            timeElapsed += ts;
            if(timeElapsed >= duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
        }


    }
}
