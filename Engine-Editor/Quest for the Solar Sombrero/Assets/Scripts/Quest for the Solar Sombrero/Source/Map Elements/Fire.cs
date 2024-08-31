using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class Fire : DefaultBehaviour
    {

        public void OnCreate()
        {
        }

        public void OnUpdate(float ts)
        {
        }

        public void OnCollisionEnter(Collision2D collision)
        {
            var other = collision.otherEntity;
            Console.WriteLine("Collision");
            if (other == null)
            {
                return;
            }
            Console.WriteLine("Collision with " + other.name);
            Console.WriteLine("Self name " + entity.name);
            if (other.name == "Player")
            {
                EffectsComponent effects = other.As<EffectsComponent>();
                effects.ApplyEffect(new FireEffect(25));
            }
        }
    }
}
