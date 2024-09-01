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
        private float duration;
        private float timeElapsed;
        private Entity entity;

        public ArmorEffect(int armorValue)
        {
            this.armorValue = armorValue;
        }

        public void ApplyEffect()
        {
            throw new NotImplementedException();
        }

        public EffectType GetEffectType()
        {
            return EffectType.ARMOR;
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
            timeElapsed += ts;
            if(timeElapsed >= duration)
            {
                entity.As<EffectsComponent>().RemoveEffect(GetEffectType());
            }
        }


    }
}
