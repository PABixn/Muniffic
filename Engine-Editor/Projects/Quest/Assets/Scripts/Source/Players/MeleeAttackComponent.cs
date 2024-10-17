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

        private int knockbackForce = 1000;

        private TransformComponent transform;
        private BoxCollider2DComponent collider;

        Entity entity;

        public MeleeAttackComponent(Entity entity, List<EntityType> attackTargetTypes, string attackTargetParentName = "Enemies")
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
            if(attackBoxComponent == null) attackBoxComponent = entity.As<AttackBoxComponent>();
            attackTimer += ts;

            if (attackTimer >= attackCooldown && Input.IsKeyPressed(KeyCode.R))
            {
                foreach (Entity e in Entity.FindEntityByName(attackTargetParentName).GetChildren())
                {
                    if (e.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxComponent.attackBoxCenter, attackBoxComponent.attackBoxSize) && attackTargetTypes.Contains(e.As<EntityTypeComponent>().entityType))
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

        public void SetKnockbackForce(int force)
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
