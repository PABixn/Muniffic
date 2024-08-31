using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class EnemyMeleeAttackComponent
    {
        private bool Enabled = true;

        private EnemyAttackBoxComponent attackBoxComponent;
        public string attackTargetParentName = "Enemies";
        public List<EntityType> attackTargetTypes = new List<EntityType> { EntityType.ENEMY_SQUARE };

        private float attackCooldown = 0.5f;
        private float attackTimer = 0f;
        private int damage = 10;
        private int knockbackForce = 100;

        private TransformComponent transform;
        private BoxCollider2DComponent collider;

        Entity entity;

        public EnemyMeleeAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName = "Enemies")
        {
            this.entity = entity;
            this.attackTargetParentName = attackTargetParentName;
            this.attackTargetTypes = attackTargetTypes;
            transform = entity.GetComponent<TransformComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
        }

        public void Update(float ts)
        {
            if (collider == null || !Enabled) return;
            if (attackBoxComponent == null) attackBoxComponent = entity.As<EnemyAttackBoxComponent>();
            attackTimer += ts;

            if (attackTimer >= attackCooldown)
            {
                foreach (Entity e in Entity.FindEntityByName(attackTargetParentName).GetChildren())
                {
                    if (attackBoxComponent.isEnemyinRange(e) && attackTargetTypes.Contains(e.As<EntityTypeComponent>().entityType))
                    {
                        e.As<HealthComponent>().TakeDamage(damage);
                        nockback(e, attackBoxComponent);
                    }
                }
                attackTimer = 0;
            }
        }

        private void nockback(Entity e, EnemyAttackBoxComponent attackBox)
        {
            RigidBody2DComponent rb = e.GetComponent<RigidBody2DComponent>();
            TransformComponent eTransform = e.GetComponent<TransformComponent>();
            if (rb == null) return;
            Vector2 direction = eTransform.translation.XY - transform.translation.XY;
            direction.NormalizeTo(knockbackForce);
            rb.ApplyLinearImpulse(direction);
        }

        public void Enable()
        {
            Enabled = true;
        }

        public void Disable()
        {
            Enabled = false;
        }

        public void SetDamage(int damage)
        {
            this.damage = damage;
        }

        public void SetCooldown(float cooldown)
        {
            attackCooldown = cooldown;
        }

        public void SetAttackTargetTypes(List<EntityType> attackTargetTypes)
        {
            this.attackTargetTypes = attackTargetTypes;
        }

        public bool isPlayerInRange()
        {
            foreach (Entity e in Entity.FindEntityByName(attackTargetParentName).GetChildren())
            {
                if (e.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxComponent.attackBoxCenter, attackBoxComponent.attackBoxSize) && attackTargetTypes.Contains(e.As<EntityTypeComponent>().entityType))
                {
                    return true;
                }
            }
            return false;
        }

        public void setKnockback(int knockbackForce)
        {
            this.knockbackForce = knockbackForce;
        }
    }
}
