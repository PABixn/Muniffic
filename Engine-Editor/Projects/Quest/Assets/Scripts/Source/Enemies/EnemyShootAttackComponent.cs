using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class EnemyShootAttackComponent
    {
        private int damage = 10;
        private float multiplier = 1.0f;
        private float range = 10;
        private float cooldown = 1.0f;
        private float cooldownTimer = 0.0f;
        private int knockback = 0;
        private int bulletSpeed = 3;
        private Entity entity;

        List<EntityType> attackTargetTypes = new List<EntityType>();
        List<int> bulletsToDestroy = new List<int>();

        private TransformComponent transform;
        private EnemyAttackBoxComponent enemyAttackBoxComponent;
        private AnimatorComponent animatorComponent;

        List<Bullet> bullets = new List<Bullet>();

        private Entity player;

        public EnemyShootAttackComponent(Entity entity, List<EntityType> attackTargetTypes)
        {
            this.entity = entity;
            this.attackTargetTypes = attackTargetTypes;
            transform = entity.GetComponent<TransformComponent>();
            animatorComponent = entity.GetComponent<AnimatorComponent>();
            
            player = Entity.FindEntityByName("Player");
        }

        public void Update(float ts)
        {
            if(enemyAttackBoxComponent == null)
            {
                enemyAttackBoxComponent = entity.As<EnemyAttackBoxComponent>();
            }

            cooldownTimer += ts;

            foreach (Bullet bullet in bullets)
            {
                bullet.OnUpdate(ts);
                if (bullet.ShouldDestroy())
                {
                    bullet.Destroy();
                    bullets.Remove(bullet);
                }
            }

            if (cooldownTimer >= cooldown && enemyAttackBoxComponent.isEnemyinRange(player))
            {
                animatorComponent.Play("enemyAttack");
                Bullet bullet = new Bullet(transform.translation.XY, enemyAttackBoxComponent.attackDirecton, damage, bulletSpeed, attackTargetTypes, "PlayerWrapper");
                bullets.Add(bullet);
                bullet.SetKnockBack(knockback);
                cooldownTimer = 0;
            }

        }

        public void SetEntity(Entity entity)
        {
            this.entity = entity;
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

        public void SetKnockback(int knockback)
        {
            this.knockback = knockback;
        }

        public List<EntityType> GetAttackTargetTypes()
        {
            return attackTargetTypes;
        }
    }
}
