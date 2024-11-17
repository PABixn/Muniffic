﻿using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class PlayerAttackBoxComponent : AttackBoxComponent
    {
        public Vector2 attackDirection = Vector2.Right;
        public Vector2 attackBoxSize = new Vector2(1, 1);
        public Vector2 attackBoxOffset = new Vector2(1, 0);
        public Vector2 attackBoxCenter;

        private BoxCollider2DComponent attackBox;
        private TransformComponent transform;
        public override void OnCreate()
        {
            attackBox = entity.GetComponent<BoxCollider2DComponent>();
            transform = entity.GetComponent<TransformComponent>();
            attackBoxCenter = new Vector2(transform.translation.X + (attackBoxOffset.X + attackBox.size.X) * attackDirection.X, transform.translation.Y + attackBoxOffset.Y);
        }

        public override void OnUpdate(float ts)
        {
            if (Input.IsKeyPressed(KeyCode.A) && !Input.IsKeyPressed(KeyCode.D))
            {
                attackDirection = new Vector2(-1, 0);
            }
            if (Input.IsKeyPressed(KeyCode.D) && !Input.IsKeyPressed(KeyCode.A))
            {
                attackDirection = new Vector2(1, 0);
            }
            attackBoxCenter = new Vector2(transform.translation.X + (attackBoxOffset.X + attackBox.size.X) * attackDirection.X, transform.translation.Y + attackBoxOffset.Y);
        }

        public override bool CollidesWith(Entity e){
            return attackBox.CollidesWith(e);
        }

        public override Vector2 GetDirection(){
            return attackDirection;
        }

        public override Vector2 GetSize(){
            return attackBoxSize;
        }

        public override Vector2 GetOffset(){
            return attackBoxOffset;
        }

        public override Vector2 GetCenter(){
            return attackBoxCenter;
        }

    }
}