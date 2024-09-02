using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class Mushroom : DefaultBehaviour
    {
        public int SpeedBoostPercent = 25;
        public int jumpBoostPercent = 15;
        private float duration = 15f;
        private float timeElapsed;
        
        public void OnCreate()
        {

        }
        public void Update(float ts)
        {
        }
        public void OnCollision(Collision2D collision)
        {
            if(collision.otherEntity == null || collision.otherEntity.name != "Player")
            {
                return;
            }
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new SpeedEffect(1 + SpeedBoostPercent / 100f, duration));
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new JumpEffect(1 + jumpBoostPercent / 100f, duration));
            
        }

    }
}
