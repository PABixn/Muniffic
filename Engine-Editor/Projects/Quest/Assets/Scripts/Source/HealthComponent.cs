using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class HealthComponent : DefaultBehaviour
    {
        public int health = 100;
        public int maxHealth = 100;
        private int armor = 0;

        public void OnCreate()
        {
            health = maxHealth;
        }

        public void TakeDamage(int damage)
        {
            damage = damage * (100 - armor) / 100;
            health -= damage;
            if (health < 0)
            {
                health = 0;
            }
        }

        public void InstaKill()
        {
            health = 0;
        }

        public void Heal(int healAmount)
        {
            health += healAmount;
            if (health > maxHealth)
            {
                health = maxHealth;
            }
        }

        public bool IsDead()
        {
            return health <= 0;
        }

        public void SetHealth(int newHealth)
        {
            health = newHealth;
            if(health > maxHealth)
            {
                health = maxHealth;
            }
        }

        public int GetHealth()
        {
            return health;
        }

        public void SetArmor(int newArmor)
        {
            armor = newArmor;
        }
    }
}
