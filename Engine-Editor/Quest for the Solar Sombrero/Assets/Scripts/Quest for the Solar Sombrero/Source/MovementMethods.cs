using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class MovementMethods
    {
        public static float calcMovement(float targetSpeed, float rbVelocity, float acceleration, float decceleration, float velPower)
        {
            float speedDif = targetSpeed - rbVelocity;
            float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;
            return (float)(Math.Pow(Math.Abs(speedDif) * accelRate, velPower) * Math.Sign(speedDif));
        }
    }
}
