using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class TrianglePlayer : Player
    {
        public float jumpForce;
        public float lastJump;
        public float jumpDelay;
        public Vector2 collidePos;
        public Vector2 collideSize;
        public BoxCollider2DComponent playerCollider;
        public BoxCollider2DComponent floorCollider;

        //Write Triangle Player from Scratch
        public override void OnCreate()
        {
            base.InitPlayer(15, 80, 2, 250);

            playerCollider = GetComponent<BoxCollider2DComponent>();
            floorCollider = Entity.FindEntityByName("floor").GetComponent<BoxCollider2DComponent>();
            collidePos = transform.translation.XY;
            collideSize = playerCollider.size;
            jumpForce = 0.5f;
            lastJump = 0f;
            jumpDelay = 1f;
        }

        public override void OnUpdate(float ts)
        {
            lastJump += ts;
            collidePos = transform.translation.XY;

            base.UpdatePosition(ts);

            if (isGrounded == false && lastJump >= jumpDelay && floorCollider.CollidesWithBox(collidePos, collideSize))
            {
                isGrounded = true;
                lastJump = 0f;
            }

            if (Input.IsKeyDown(KeyCode.W) && isGrounded)
            {
                rigidBody.ApplyLinearImpulse(new Vector2(0, jumpForce * 10));
                isGrounded = false;
            }
        }
    }
}
