using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class WeightedVest : DefaultBehaviour
    {
        public int speedDecresePercent = 35;
        public int jumpDescresePercent = 60;
        public float duration = 30f;
        public int armorValue = 1000;
        private float timeElapsed;

        public void OnCreate()
        {

        }
        public void Update(float ts)
        {

        }

        public void OnCollisionEnter(Collision2D collision)
        {
            if (collision.otherEntity == null || collision.otherEntity.name != "Player")
            {
                Console.WriteLine(collision.otherEntity.name);
                return;
            }
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new SlowEffect(1 - speedDecresePercent / 100f, duration));
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new JumpEffect(- (1 - jumpDescresePercent / 100f), duration));
            collision.otherEntity.As<EffectsComponent>().ApplyEffect(new ArmorEffect(armorValue, duration));
            Entity.Destroy(entity);
        }
    }
}