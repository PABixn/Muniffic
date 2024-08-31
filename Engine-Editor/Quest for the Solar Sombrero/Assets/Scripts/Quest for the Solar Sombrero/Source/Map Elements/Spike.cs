using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class Spike : DefaultBehaviour
    {
        public int damage = int.MaxValue;

        
        public void OnCreate()
        {

        }

        public void OnUpdate(float ts)
        {

        }

        public void OnCollisionEnter(Collision2D collision)
        {
            var other = collision.otherEntity;
            if(other == null)
            {
                return;
            }

            if(other.name == "Player")
            {
                other.As<HealthComponent>().TakeDamage(damage);
            } 
        }
    }
}
