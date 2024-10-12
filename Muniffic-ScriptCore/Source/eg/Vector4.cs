using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero = new Vector4(0f, 0f, 0f, 0f);
        public static Vector4 One = new Vector4(1f, 1f, 1f, 1f);
        public static Vector4 Right = new Vector4(1f, 0f, 0f, 0f);
        public static Vector4 Left = new Vector4(-1f, 0f, 0f, 0f);
        public static Vector4 Up = new Vector4(0f, 1f, 0f, 0f);
        public static Vector4 Down = new Vector4(0f, -1f, 0f, 0f);
        public static Vector4 Forward = new Vector4(0f, 0f, 1f, 0f);
        public static Vector4 Backward = new Vector4(0f, 0f, -1f, 0f);

        public Vector4(Vector4 vec)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
            W = vec.W;
        }

        public Vector4(float x)
        {
            X = x;
            Y = x;
            Z = x;
            W = x;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set { X = value.X; Y = value.Y; }
        }

        static public Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W * b.W);
        }

        static public Vector4 operator -(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }

        static public Vector4 operator *(Vector4 a, float b)
        {
            return new Vector4(a.X * b, a.Y * b, a.Z * b, a.W * b);
        }
    };
}
