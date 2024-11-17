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
        private int knockbackForce = 1;
        private int bulletSpeed = 4;
        private Entity entity;

        public bool isShooting = false;

        private string AnimationName = "Attack";

        List<EntityType> attackTargetTypes = new List<EntityType>();
        string attackTargetParentName = "Enemies";

        private TransformComponent transform;
        private AttackBoxComponent attackBoxComponent;
        private BoxCollider2DComponent collider;
        private AnimatorComponent animator;

        List<Bullet> bullets = new List<Bullet>();

        public ShootAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName, AttackBoxComponent attackBoxComponent, string AnimationName)
        {
            this.entity = entity;
            this.attackTargetTypes = attackTargetTypes;
            this.attackTargetParentName = attackTargetParentName;
            this.attackBoxComponent = attackBoxComponent;
            this.AnimationName = AnimationName;
            transform = entity.GetComponent<TransformComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
            animator = entity.GetComponent<AnimatorComponent>();
        }

        public void Update(float ts, bool attackCondition = true)
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
            if (isShooting) return;
            if (cooldownTimer >= cooldown && attackCondition)
            {
                isShooting = true;
                animator.ChangeAnimation(AnimationName);
                animator.Play(AnimationName);
            }
        }

        public void Attack()
        {
            Bullet bullet = new Bullet(new Vector2(transform.translation.X, transform.translation.Y - collider.size.Y), attackBoxComponent.GetDirection(), (int)(damage * damageMultiplier), (int)(bulletSpeed * attackSpeedMultiplier), attackTargetTypes, attackTargetParentName);
            bullet.SetKnockBack(knockbackForce);
            bullets.Add(bullet);
            cooldownTimer = 0;
        }

        public void SetIsShooting(bool isShooting)
        {
            this.isShooting = isShooting;
        }

        public bool GetIsShooting()
        {
            return isShooting;
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
