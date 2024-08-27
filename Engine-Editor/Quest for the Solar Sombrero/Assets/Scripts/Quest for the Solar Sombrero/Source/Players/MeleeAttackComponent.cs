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
        private int damage = 10;

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

            if (attackTimer >= attackCooldown && Input.IsKeyDown(KeyCode.R))
            {
                foreach (Entity e in Entity.FindEntityByName(attackTargetParentName).GetChildren())
                {
                    if (e.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxComponent.attackBoxCenter, attackBoxComponent.attackBoxSize) && attackTargetTypes.Contains(e.As<EntityTypeComponent>().entityType))
                    {
                        e.As<HealthComponent>().TakeDamage(damage);
                    }
                }
                attackTimer = 0;
            }
        }

        public void Enable()
        {
            Enabled = true;
        }

        public void Disable()
        {
            Enabled = false;
        }
    }
}
