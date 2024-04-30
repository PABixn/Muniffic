using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace eg
{
    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero = new Vector2(0f, 0f);

        public Vector2(Vector2 vec)
        {
            X = vec.X;
            Y = vec.Y;
        }

        public Vector2(float x)
        {
            X = x;
            Y = x;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        public float LengthSquared()
        {
            return X * X + Y * Y;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }

        static public Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        static public Vector2 operator +(Vector2 a, float b)
        {
            return new Vector2(a.X + b, a.Y + b);
        }

        static public Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        static public Vector2 operator -(Vector2 a, float b)
        {
            return new Vector2(a.X - b, a.Y - b);
        }

        static public Vector2 operator *(Vector2 a, float b)
        {
            return new Vector2(a.X * b, a.Y * b);
        }

        static public Vector2 operator / (Vector2 a, float b)
        {
            return new Vector2(a.X / b, a.Y / b);
        }

        public static float LengthSquared(Vector2 vector)
        {
            return vector.X * vector.X + vector.Y * vector.Y;
        }

        public static float Length(Vector2 vector)
        {
            return (float)Math.Sqrt(LengthSquared(vector));
        }

        public static Vector2 NormalizeTo(Vector2 vector, float length)
        {
            float currentLength = Length(vector);

            if (currentLength == 0)
            {
                return Vector2.Zero;
            }

            float scale = length / currentLength;
            vector.X *= scale;
            vector.Y *= scale;

            return vector;
        }

        public void NormalizeTo(float length)
        {
            float currentLength = Length();
            if (currentLength == 0)
            {
                return;
            }
            float scale = length / currentLength;
            X *= scale;
            Y *= scale;
        }

        public void Normalize()
        {
            NormalizeTo(1.0f);
        }

        public static Vector2 FindVector(Vector2 from, Vector2 to)
        {
            Vector2 vector = new Vector2();
            vector.X = to.X - from.X;
            vector.Y = to.Y - from.Y;
            return vector; 
        }
    };
}