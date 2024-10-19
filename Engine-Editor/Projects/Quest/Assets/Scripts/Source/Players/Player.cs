using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class Player : DefaultBehaviour
    {
        private bool initialized = false;

        private HealthComponent healthComponent;
        private ShootAttackComponent shootAttackComponent;
        private JumpComponent jumpComponent;
        private RunComponent runComponent;
        private EntityTypeComponent entityTypeComponent;
        private PlayerAttackBoxComponent playerAttackBoxComponent;
        public int BasePlayerHealth { get; private set; } = 800;
        public float BasePlayerSpeedMultiplier { get; private set; } = 0.5f;
        public float BasePlayerJumpForceMultiplier { get; private set; } = 0.5f;
        public int BasePlayerAttackDamage { get; private set; } = 100;
        public float BasePlayerAttackSpeed { get; private set; } = 2;
        public int BasePlayerKnockbackForce = 100;


        private List<EntityType> BasePlayerTargets = new List<EntityType> { EntityType.ENEMY_SQUARE, EntityType.ENEMY_CIRCLE, EntityType.ENEMY_TRIANGLE };
        private List<EntityType> SquarePlayerTargets = new List<EntityType> { EntityType.ENEMY_CIRCLE };
        private List<EntityType> CirclePlayerTargets = new List<EntityType> { EntityType.ENEMY_TRIANGLE };
        private List<EntityType> TrianglePlayerTargets = new List<EntityType> { EntityType.ENEMY_SQUARE };

        public void OnCreate()
        {
        }

        public void OnUpdate(float ts)
        {

            if (entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if(healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(playerAttackBoxComponent == null) playerAttackBoxComponent = entity.As<PlayerAttackBoxComponent>();
            if(jumpComponent == null) jumpComponent = entity.As<JumpComponent>();
            if(runComponent == null) runComponent = entity.As<RunComponent>();
            if(shootAttackComponent == null) shootAttackComponent = new ShootAttackComponent(entity, BasePlayerTargets, "Enemies", playerAttackBoxComponent);
            if (entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.PLAYER;
            if(!initialized) Init();

            shootAttackComponent.Update(ts, Input.IsKeyPressed(KeyCode.R));

            if (healthComponent.health == 0)
            {
                killPlayer();
            }
        }

        public void healPlayer(int heal)
        {
            healthComponent = entity.As<HealthComponent>();
            healthComponent.Heal(heal);
        }

        public void killPlayer()
        {
            runComponent.SetMultiplier(0);
        }


        private void Init()
        {
            healthComponent.maxHealth = BasePlayerHealth;
            healthComponent.SetHealth(BasePlayerHealth);
            playerAttackBoxComponent.attackBoxSize = new Vector2(1, 1);
            playerAttackBoxComponent.attackBoxCenter = new Vector2(1, 0);
            shootAttackComponent.SetDamage(BasePlayerAttackDamage);
            shootAttackComponent.SetCooldown(BasePlayerAttackSpeed);
            shootAttackComponent.SetKnockback(BasePlayerKnockbackForce);
            runComponent.SetMultiplier(BasePlayerSpeedMultiplier);
            jumpComponent.SetMultiplier(BasePlayerJumpForceMultiplier);
            jumpComponent.Enable();
            initialized = true;
        }

        public void SetDamageMultiplier(float multiplier){
            shootAttackComponent.SetDamageMultiplier(multiplier);
        }

        public void SetAttackSpeedMultiplier(float multiplier){
            shootAttackComponent.SetAttackSpeedMultiplier(multiplier);
        }

        public float GetDamageMultiplier(){
            return shootAttackComponent.GetDamageMultiplier();
        }

        public float GetAttackSpeedMultiplier(){
            return shootAttackComponent.GetAttackSpeedMultiplier();
        }

    }
}
