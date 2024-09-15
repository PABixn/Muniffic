using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public struct B2transform
    {
        public Vector2 position;
        public Vector2 rotation;
        public B2transform(Vector2 pos, Vector2 rot)
        {
            position = pos;
            rotation = rot;
        }
    }
}
