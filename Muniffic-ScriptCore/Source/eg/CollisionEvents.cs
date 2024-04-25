using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public struct Collision2D
    {
        public Collision2D(long entityID, Vector2 points)
        {
            this.otherEntity = new Entity(entityID);
            this.contactPoints = points;
        }

        public Entity otherEntity;
        public Vector2 contactPoints;
    }
}
