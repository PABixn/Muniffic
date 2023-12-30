using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public static class InternalCalls
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong UUID, out Vector3 vec);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_FindEntityByName(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetScriptInstance(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong iD, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponnet_ApplyLinearImpulse(ulong iD, ref  Vector2 impulse, ref Vector2 point, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_GetLinearVelocity(ulong entityID, out Vector2 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static RigidBody2DComponent.BodyType RigidBody2DComponent_GetType(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetType(ulong entityID, RigidBody2DComponent.BodyType type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponnet_ApplyLinearImpulseToCenter(ulong iD, ref Vector2 impulse, bool wake);
    }
}
