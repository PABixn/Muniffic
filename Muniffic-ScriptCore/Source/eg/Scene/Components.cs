using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public abstract class Component
    { 
        public Entity Entity { get; internal set; }

    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }

            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }
    }

    public class RigidBody2DComponent : Component
    {
        public enum BodyType { Static = 0, Dynamic, Kinematic }

        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }
        }

        public BodyType Type
        {
            get => InternalCalls.RigidBody2DComponent_GetType(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetType(Entity.ID, value);
        }
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake = true)
        {
            InternalCalls.RigidBody2DComponnet_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
        {
            InternalCalls.RigidBody2DComponnet_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }


    }
}
