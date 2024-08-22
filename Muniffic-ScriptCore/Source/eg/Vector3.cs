using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero = new Vector3(0f, 0f, 0f);
        public static Vector3 One = new Vector3(1f, 1f, 1f);
        public static Vector3 Right = new Vector3(1f, 0f, 0f);
        public static Vector3 Left = new Vector3(-1f, 0f, 0f);
        public static Vector3 Up = new Vector3(0f, 1f, 0f);
        public static Vector3 Down = new Vector3(0f, -1f, 0f);
        public static Vector3 Forward = new Vector3(0f, 0f, 1f);
        public static Vector3 Backward = new Vector3(0f, 0f, -1f);

        public Vector3(Vector3 vec)
        {
            X = vec.X;
            Y = vec.Y;
            Z = vec.Z;
        }

        public Vector3(Vector2 vec)
        {
            X = vec.X;
            Y = vec.Y;
            Z = 0f;
        }

        public Vector3(Vector2 vec, float z)
        {
            X = vec.X;
            Y = vec.Y;
            Z = z;
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

        public Vector2 XY
        {
            get => new Vector2(X, Y);
            set { X = value.X; Y = value.Y; }
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
}
