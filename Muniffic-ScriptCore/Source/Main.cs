using System;
using System.Runtime.CompilerServices;

namespace eg
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero = new Vector3(0f, 0f, 0f);

        public Vector3(Vector3 vec)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
        }

        public Vector3(float x)
        {
            X = x;
            Y = x;
            Z = x;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        static public Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        static public Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        static public Vector3 operator *(Vector3 a, float b)
        {
            return new Vector3(a.X * b, a.Y * b, a.Z * b);
        }
    };

    public class Entity
    {
        protected Entity()
        {
            ID = 0;
        }
        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;
        
        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(ID, out Vector3 vec);
                return vec;
            }

            set
            {
                InternalCalls.Entity_SetTranslation(ID, ref value);
            }
        }
    }

    public static class InternalCalls
    {

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog_Vector(ref Vector3 vec, out Vector3 outVec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLog_VectorDot(ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetTranslation(ulong UUID, out Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetTranslation(ulong UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);
    }
}