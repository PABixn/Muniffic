using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class MeleeAttackComponent : DefaultBehaviour
    {

        private Vector2 attackDirecton = Vector2.Right;
        public Vector2 attackBoxSize = new Vector2(1, 1);
        public Vector2 attackBoxOffset = new Vector2(1, 0);
        public Vector2 attackBoxCenter;

        private float attackCooldown = 0.5f;
        private float attackTimer = 0f;
        private int damage = 10;

        private TransformComponent transform;
        private BoxCollider2DComponent collider;

        public void OnCreate()
        {
            transform = entity.GetComponent<TransformComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
        }

        public void OnUpdate(float ts)
        {
            if (collider == null) return;
            attackTimer += ts;
            attackBoxCenter = new Vector2(transform.translation.X + attackBoxOffset.X * attackDirecton.X, transform.translation.Y + attackBoxOffset.Y);

            if (attackTimer >= attackCooldown && Input.IsKeyDown(KeyCode.R))
            {
                List<Entity> enemiesInRange = new List<Entity>();
                foreach (Entity enemy in Entity.FindEntityByName("Enemies").GetChildren())
                {
                    if (enemy.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxCenter, attackBoxSize))
                    {
                        enemiesInRange.Add(enemy);
                    }
                }
                foreach (Entity enemy in enemiesInRange)
                {
                    enemy.As<BaseEnemy>().healthComponent.TakeDamage(damage);
                }
                attackTimer = 0;
            }
        }
    }
}
