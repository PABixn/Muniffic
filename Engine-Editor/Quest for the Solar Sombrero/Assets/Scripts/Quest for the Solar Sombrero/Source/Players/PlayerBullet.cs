using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class PlayerBullet
    {
        private Vector2 direction;
        private float speed;
        private float lifeTime;
        private float lifeTimer;
        private List<EntityType> entitiesToHurt;
        
        public int damage = 10;

        private RigidBody2DComponent rigidBody;
        private TransformComponent transform;
        private BoxCollider2DComponent collider;
        Entity entity;

        public PlayerBullet(Vector2 position, Vector2 direction, int damage, int speed, List<EntityType> entitiesToHurt)
        {
            this.damage = damage;
            this.direction = direction;
            this.speed = speed;
            this.entitiesToHurt = entitiesToHurt;

            entity = Entity.Create("Bullet");
            transform = entity.GetComponent<TransformComponent>();
            transform.translation = new Vector3(direction, 0);
            transform.scale = new Vector3(0.1f, 0.1f, 0.5f);
            collider = entity.AddComponent<BoxCollider2DComponent>();
            rigidBody = entity.AddComponent<RigidBody2DComponent>();
            rigidBody.type = RigidBody2DComponent.BodyType.Kinematic;
            collider.isSensor = true;
            rigidBody.AwakeRuntimeBody();
            rigidBody.linearVelocity = direction * speed;
        }

        public void OnUpdate(float ts)
        {
            lifeTimer += ts;
            if (lifeTimer >= lifeTime)
            {
                Entity.Destroy(entity);
            }
        }
    }
}
