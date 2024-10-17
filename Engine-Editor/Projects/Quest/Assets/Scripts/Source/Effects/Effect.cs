using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using eg;
namespace Quest
{
    public interface Effect
    {
        void ApplyEffect();
        void UpdateEffect(float ts);
        void RemoveEffect();
        EffectType GetEffectType();

        void SetEntity(Entity entity);
    }
}
