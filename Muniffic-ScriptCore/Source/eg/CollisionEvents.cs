using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public struct Collision2D
    {
        public Collision2D(long entityID, Vector2 points, float friction, float restitution, float tangentSpeed)
        {
            this.otherEntity = new Entity(entityID);
            this.contactPoints = points;
            this.friction = friction;
            this.restitution = restitution;
            this.tangentSpeed = tangentSpeed;
        }

        public Entity otherEntity;
        public Vector2 contactPoints;
        public float friction;
        public float restitution;
        public float tangentSpeed;
    }
}
