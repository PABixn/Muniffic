using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    /// <summary>
    /// Represents a 2D collision event.
    /// </summary>
    public struct Collision2D
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="Collision2D"/> struct.
        /// </summary>
        /// <param name="entityID">The ID of the other entity involved in the collision.</param>
        /// <param name="points">The contact points of the collision.</param>
        /// <param name="friction">The friction coefficient of the collision.</param>
        /// <param name="restitution">The restitution coefficient of the collision.</param>
        /// <param name="tangentSpeed">The tangent speed of the collision.</param>
        public Collision2D(long entityID, Vector2 points, float friction, float restitution, float tangentSpeed)
        {
            this.otherEntity = new Entity(entityID);
            this.contactPoints = points;
            this.friction = friction;
            this.restitution = restitution;
            this.tangentSpeed = tangentSpeed;
        }

        /// <summary>
        /// Gets or sets the other entity involved in the collision.
        /// </summary>
        public Entity otherEntity;

        /// <summary>
        /// Gets or sets the contact points of the collision.
        /// </summary>
        public Vector2 contactPoints;

        /// <summary>
        /// Gets or sets the friction coefficient of the collision.
        /// </summary>
        public float friction;

        /// <summary>
        /// Gets or sets the restitution coefficient of the collision.
        /// </summary>
        public float restitution;

        /// <summary>
        /// Gets or sets the tangent speed of the collision.
        /// </summary>
        public float tangentSpeed;
    }
}
