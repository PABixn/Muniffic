using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest.Source
{
    internal class HealthComponent
    {
        public int health = 100;
        public int maxHealth = 100;

        public HealthComponent(int maxHealth) { 
            this.maxHealth = maxHealth;
            health = maxHealth;
        }

        public void TakeDamage(int damage)
        {
            health -= damage;
            if (health < 0)
            {
                health = 0;
            }
        }

        public void Heal(int healAmount)
        {
            health += healAmount;
            if (health > maxHealth)
            {
                health = maxHealth;
            }
        }
    }
}
