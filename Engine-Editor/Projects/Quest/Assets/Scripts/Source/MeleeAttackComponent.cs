using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class MeleeAttackComponent
    {
        private bool Enabled = true;

        private AttackBoxComponent attackBoxComponent;
        public string attackTargetParentName = "Enemies";
        public List<EntityType> attackTargetTypes = new List<EntityType> { EntityType.ENEMY_SQUARE };

        private float attackCooldown = 0.5f;
        private float attackTimer = 0f;
        public int damage = 10;

        private float multiplier = 1.0f;

        private int knockbackForce = 100;

        private TransformComponent transform;
        private BoxCollider2DComponent collider;

        Entity entity;

        public MeleeAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName, AttackBoxComponent attackBoxComponent)
        {
            this.entity = entity;
            this.attackTargetParentName = attackTargetParentName;
            this.attackTargetTypes = attackTargetTypes;
            this.attackBoxComponent = attackBoxComponent;
            transform = entity.GetComponent<TransformComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
        }

        public void Update(float ts, bool attackCondition = true)
        {
            if (collider == null || !Enabled) return;
            attackTimer += ts;

            if (attackTimer >= attackCooldown && attackCondition)
            {
                Entity enemyParent = Entity.FindEntityByName(attackTargetParentName);
                if (enemyParent == null) return;
                foreach (Entity e in enemyParent.GetChildren())
                {
                    if (e.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxComponent.GetCenter(), attackBoxComponent.GetSize()) && attackTargetTypes.Contains(e.As<EntityTypeComponent>().entityType))
                    {
                        e.As<HealthComponent>().TakeDamage((int)(damage * multiplier));
                        nockback(e, attackBoxComponent);
                    }
                }
                attackTimer = 0;
            }
        }

        private void nockback(Entity e, AttackBoxComponent attackBox)
        {
            RigidBody2DComponent rb = e.GetComponent<RigidBody2DComponent>();
            TransformComponent eTransform = e.GetComponent<TransformComponent>();
            if(rb == null) return;
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

        public void SetKnockback(int force)
        {
            knockbackForce = force;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }

        public float GetMultiplier()
        {
            return multiplier;
        }
    }
}
