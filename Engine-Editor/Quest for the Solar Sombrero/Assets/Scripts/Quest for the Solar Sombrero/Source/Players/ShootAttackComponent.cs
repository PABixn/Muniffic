using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class ShootAttackComponent
    {
        private int damage = 10;
        private float multiplier = 1.0f;
        private float range = 10;
        private float cooldown = 1.0f;
        private Entity entity;

        List<EntityType> attackTargetTypes = new List<EntityType>();
        string attackTargetParentName = "Enemies";

        public ShootAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName = "Enemies")
        {
            this.entity = entity;
            this.attackTargetTypes = attackTargetTypes;
            this.attackTargetParentName = attackTargetParentName;
        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void Update(float ts)
        {
            cooldown -= 1;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }

        public float GetMultiplier()
        {
            return multiplier;
        }

        public void SetDamage(int damage)
        {
            this.damage = damage;
        }

        public int GetDamage()
        {
            return damage;
        }

        public void SetCooldown(float cooldown)
        {
            this.cooldown = cooldown;
        }

        public float GetCooldown()
        {
            return cooldown;
        }

        public void SetRange(float range)
        {
            this.range = range;
        }

        public float GetRange()
        {
            return range;
        }

        public void SetAttackTargetTypes(List<EntityType> attackTargetTypes)
        {
            this.attackTargetTypes = attackTargetTypes;
        }

        public List<EntityType> GetAttackTargetTypes()
        {
            return attackTargetTypes;
        }

        public void SetAttackTargetParentName(string attackTargetParentName)
        {
            this.attackTargetParentName = attackTargetParentName;
        }

    }
}
