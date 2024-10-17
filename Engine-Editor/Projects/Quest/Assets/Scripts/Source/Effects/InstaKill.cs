using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest.Source.Effects
{
    internal class InstaKill : Effect
    {
        private HealthComponent healthComponent;
        private Entity entity;


        public void ApplyEffect()
        {
            healthComponent = entity.As<HealthComponent>();
            healthComponent.InstaKill();
        }

        public EffectType GetEffectType()
        {
            return EffectType.LAVA;
        }

        public void RemoveEffect()
        {
            throw new NotImplementedException();
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void UpdateEffect(float ts)
        {
            throw new NotImplementedException();
        }
    }
}
