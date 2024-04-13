using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Projectile
    {
        public Vector2 Velocity = new Vector2(0);
        public int Damage;
        TransformComponent Transform;
        Entity Entity;
        public Entity playerEntity;
        ProjectileType projectileType;
        RigidBody2DComponent rigidBody;

        public Projectile(Vector2 position, Vector2 Velocity, int Damage, ProjectileType type)
        {
            this.Velocity = Velocity;
            this.Damage = Damage;
            projectileType = type;
            Entity = Entity.Create("projectile");
            Transform = Entity.GetComponent<TransformComponent>();
            Transform.scale = new Vector3(0.2f, 0.1f, 1f);
            Transform.translation = new Vector3(position.X, position.Y, 0);
            Entity.AddComponent<SpriteRendererComponent>();
            Entity.AddComponent<BoxCollider2DComponent>();
            rigidBody = Entity.AddComponent<RigidBody2DComponent>();
            rigidBody.type = RigidBody2DComponent.BodyType.Kinematic;
            rigidBody.AwakeRuntimeBody();
        }

        public void UpdatePosition(float ts)
        {
            DebugConsole.Log(rigidBody.linearVelocity.X.ToString(), DebugConsole.LogType.Info);
            rigidBody.linearVelocity = Velocity;
        }
    }
}