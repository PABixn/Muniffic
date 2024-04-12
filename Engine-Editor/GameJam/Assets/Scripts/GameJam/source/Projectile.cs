using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    class Projectile
    {
        public Vector2 Velocity = new Vector2(0);
        public int Damage;
        TransformComponent Transform;
        Entity Entity;
        public Entity playerEntity;

        public enum ProjectileType
        {
            Normal,
            Stun
        }
        ProjectileType projectileType;

        public Projectile(Vector2 Velocity, int Damage, ProjectileType type)
        {
            this.Velocity = Velocity;
            this.Damage = Damage;
            projectileType = type;
            Entity = Entity.Create("projectile");
            Entity.AddComponent<SpriteRendererComponent>();
            Entity.AddComponent<BoxCollider2DComponent>();
            Entity.AddComponent<RigidBody2DComponent>();
            Transform = Entity.GetComponent<TransformComponent>();
        }

        public void UpdatePosition(float ts)
        {
            Transform.translation += new Vector3(Velocity * ts, 0);
        }
    }
}