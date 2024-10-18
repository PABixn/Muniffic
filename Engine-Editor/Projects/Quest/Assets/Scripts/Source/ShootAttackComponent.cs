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
        private float damageMultiplier = 1.0f;
        private float attackSpeedMultiplier = 1.0f;
        private float range = 10;
        private float cooldown = 1.0f;
        private float cooldownTimer = 0.0f;
        private int knockbackForce = 10;
        private int bulletSpeed = 1;
        private Entity entity;

        List<EntityType> attackTargetTypes = new List<EntityType>();
        string attackTargetParentName = "Enemies";

        private TransformComponent transform;
        private AttackBoxComponent attackBoxComponent;

        List<Bullet> bullets = new List<Bullet>();

        public ShootAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName, AttackBoxComponent attackBoxComponent)
        {
            this.entity = entity;
            this.attackTargetTypes = attackTargetTypes;
            this.attackTargetParentName = attackTargetParentName;
            this.attackBoxComponent = attackBoxComponent;
            transform = entity.GetComponent<TransformComponent>();
        }

        public void Update(float ts)
        {
            cooldownTimer += ts;
            foreach (Bullet bullet in bullets)
            {
                bullet.OnUpdate(ts);
                if(bullet.ShouldDestroy())
                {
                    bullet.Destroy();
                    bullets.Remove(bullet);
                }
            }
            if (cooldownTimer >= cooldown && Input.IsKeyPressed(KeyCode.R))
            {
                Bullet bullet = new Bullet(transform.translation.XY, attackBoxComponent.GetDirection(), (int)(damage * damageMultiplier),(int)(bulletSpeed * attackSpeedMultiplier), attackTargetTypes, attackTargetParentName);
                bullet.SetKnockBack(knockbackForce);
                bullets.Add(bullet);
                cooldownTimer = 0;
            }

        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
        }

        public void SetDamageMultiplier(float multiplier)
        {
            this.damageMultiplier = multiplier;
        }

        public float GetDamageMultiplier()
        {
            return damageMultiplier;
        }

        public void SetAttackSpeedMultiplier(float multiplier)
        {
            this.attackSpeedMultiplier = multiplier;
        }

        public float GetAttackSpeedMultiplier(){
            return attackSpeedMultiplier;
        }

        public void SetKnockback(int knockbackForce)
        {
            this.knockbackForce = knockbackForce;
        }

        public int GetKnockback()
        {
            return knockbackForce;
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
