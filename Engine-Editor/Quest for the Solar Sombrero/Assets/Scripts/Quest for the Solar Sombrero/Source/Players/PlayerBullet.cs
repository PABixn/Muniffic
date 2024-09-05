 using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class Bullet
    {
        private Vector2 direction;
        private float speed;
        private float lifeTime = 10;
        private float lifeTimer;
        private List<EntityType> entitiesToHurt;
        private string attackParentString = "Enemies";
        public int damage = 10;
        private int knockback = 0;

        private RigidBody2DComponent rigidBody;
        private TransformComponent transform;
        private BoxCollider2DComponent collider;
        private SpriteRendererComponent spriteRenderer;
        Entity entity;

        private bool shouldDestroy = false;

        public Bullet(Vector2 position, Vector2 direction, int damage, int speed, List<EntityType> entitiesToHurt, string attackParentString)
        {
            this.damage = damage;
            this.direction = direction;
            this.speed = speed;
            this.entitiesToHurt = entitiesToHurt;
            this.attackParentString = attackParentString;

            entity = Entity.Create("Bullet");
            transform = entity.GetComponent<TransformComponent>();
            transform.translation = new Vector3(direction, 0);
            transform.scale = new Vector3(0.1f, 0.1f, 0.5f);
            spriteRenderer = entity.AddComponent<SpriteRendererComponent>();
            spriteRenderer.color = Color.white;
            collider = entity.AddComponent<BoxCollider2DComponent>();
            rigidBody = entity.AddComponent<RigidBody2DComponent>();
            rigidBody.type = RigidBody2DComponent.BodyType.Kinematic;
            collider.isSensor = true;
            transform.translation = new Vector3(position, 0);
            rigidBody.AwakeRuntimeBody();
            rigidBody.linearVelocity = direction * speed;
            
        }

        public void OnUpdate(float ts)
        {
            lifeTimer += ts;
            if (lifeTimer >= lifeTime)
            {
                shouldDestroy = true;
            }
            List<Entity> enemies = Entity.FindEntityByName(attackParentString).GetChildren();
            foreach(Entity e in enemies)
            {
                if (e.GetComponent<BoxCollider2DComponent>().CollidesWith(e))
                {
                    if (entitiesToHurt.Contains(e.As<EntityTypeComponent>().entityType))
                    {
                        e.As<HealthComponent>().TakeDamage(damage);
                        shouldDestroy = true;
                        if (knockback > 0)
                        {
                            nockback(e);
                        }
                    }
                }
            }

            if (GroundCheck.IsGrounded(collider))
            {
                shouldDestroy = true;
            }
            
        }
        private void nockback(Entity e)
        {
            RigidBody2DComponent rb = e.GetComponent<RigidBody2DComponent>();
            TransformComponent eTransform = e.GetComponent<TransformComponent>();
            if (rb == null) return;
            Vector2 direction = eTransform.translation.XY - transform.translation.XY;
            direction.NormalizeTo(knockback);
            rb.ApplyLinearImpulse(direction);
        }

        public bool ShouldDestroy()
        {
            return shouldDestroy;
        }

        public void SetKnockBack(int knockbackForce)
        {
            knockback = knockbackForce;
        }

        internal void Destroy()
        {
            Entity.Destroy(entity);
        }
    }
}
