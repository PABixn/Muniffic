using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using eg;

namespace Quest
{
    public class EffectsComponent : DefaultBehaviour
    {
        private Dictionary<EffectType, Effect> effects;
        public void OnCreate()
        {
            effects = new Dictionary<EffectType, Effect>();
        }

        public void OnUpdate(float ts)
        {
            foreach (var effect in effects)
            {
                effect.Value.UpdateEffect(ts);
            }
        }

        public void ApplyEffect(Effect effect)
        {
            effects.Add(effect.GetEffectType(), effect);
            effect.SetEntity(entity);
            effect.ApplyEffect();
        }

        public void RemoveEffect(EffectType effectType)
        {
            effects[effectType].RemoveEffect();
            effects.Remove(effectType);
        }
    }
}
    
