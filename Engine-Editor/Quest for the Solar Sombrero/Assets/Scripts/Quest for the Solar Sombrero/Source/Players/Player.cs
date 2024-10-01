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

        private SpriteRendererComponent spriteRendererComponent;

        private HealthComponent healthComponent;

        private AttackBoxComponent attackBoxComponent;

        private MeleeAttackComponent meleeAttackComponent;

        private ShootAttackComponent shootAttackComponent;

        private JumpComponent jumpComponent;

        private RunComponent runComponent;

        private EntityTypeComponent entityTypeComponent;

        private PlayerType playerType = PlayerType.NONE;

        public int BasePlayerHealth { get; private set; } = 1000;
        public int SquarePlayerHealth { get; private set; } = 800;
        public int CirclePlayerHealth { get; private set; } = 400;
        public int TrianglePlayerHealth { get; private set; } = 250;

        public float BasePlayerSpeedMultiplier { get; private set; } = 1f;
        public float SquarePlayerSpeedMultiplier { get; private set; } = 0.5f;
        public float CirclePlayerSpeedMultiplier { get; private set; } = 1;
        public float TrianglePlayerSpeedMultiplier { get; private set; } = 1.5f;

        public float BasePlayerJumpForceMultiplier { get; private set; } = 0.6f;
        public float SquarePlayerJumpForceMultiplier { get; private set; } = 0;
        public float CirclePlayerJumpForceMultiplier { get; private set; } = 2.0f;
        public float TrianglePlayerJumpForceMultiplier { get; private set; } = 1;

        public int BasePlayerAttackDamage { get; private set; } = 100;
        public int SquarePlayerAttackDamage { get; private set; } = 100;
        public int CirclePlayerAttackDamage { get; private set; } = 20;
        public int TrianglePlayerAttackDamage { get; private set; } = 80;

        public float BasePlayerAttackSpeed { get; private set; } = 1;
        public float SquarePlayerAttackSpeed { get; private set; } = 2;
        public float CirclePlayerAttackSpeed { get; private set; } = 0.2f;
        public float TrianglePlayerAttackSpeed { get; private set; } = 1f;


        public int BasePlayerKnockbackForce = 10;
        public int SquarePlayerKnockbackForce { get; private set; } = 100;
        public int CirclePlayerKnockbackForce { get; private set; } = 10;
        public int TrianglePlayerKnockbackForce { get; private set; } = 50;


        private List<EntityType> BasePlayerTargets = new List<EntityType> { EntityType.ENEMY_SQUARE, EntityType.ENEMY_CIRCLE, EntityType.ENEMY_TRIANGLE };
        private List<EntityType> SquarePlayerTargets = new List<EntityType> { EntityType.ENEMY_CIRCLE };
        private List<EntityType> CirclePlayerTargets = new List<EntityType> { EntityType.ENEMY_TRIANGLE };
        private List<EntityType> TrianglePlayerTargets = new List<EntityType> { EntityType.ENEMY_SQUARE };

        public void OnCreate()
        {
            spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
            meleeAttackComponent = new MeleeAttackComponent(entity, BasePlayerTargets, "Enemies");
            shootAttackComponent = new ShootAttackComponent(entity, BasePlayerTargets, "Enemies");
        }

        public void OnUpdate(float ts)
        {
            
            if (entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if(healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(attackBoxComponent == null) attackBoxComponent = entity.As<AttackBoxComponent>();
            if(jumpComponent == null) jumpComponent = entity.As<JumpComponent>();
            if(runComponent == null) runComponent = entity.As<RunComponent>();
            if(entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.PLAYER_BASE;
            if(playerType == PlayerType.NONE) SwitchPlayerType(PlayerType.BASE);


            meleeAttackComponent.Update(ts);
            shootAttackComponent.Update(ts);

            if (healthComponent.health == 0)
            {
                killPlayer();
            }
        }

        public void killPlayer()
        {
            entity.GetComponent<SpriteRendererComponent>().color = Color.red;
            runComponent.SetMultiplier(0);
        }

        public void SwitchPlayerType(PlayerType playerType)
        {
            switch (playerType)
            {
                case PlayerType.BASE:
                    InitBasePlayer();
                    break;
                case PlayerType.SQUARE:
                    InitSquarePlayer();
                    break;
                case PlayerType.CIRCLE:
                    InitCirclePlayer();
                    break;
                case PlayerType.TRIANGLE:
                    InitTrianglePlayer();
                    break;
            }
        }

        public void SwitchPlayerType(EnemyType enemyType)
        {
            switch (enemyType)
            {
                case EnemyType.SQUARE:
                    SwitchPlayerType(PlayerType.SQUARE);
                    break;
                case EnemyType.CIRCLE:
                    SwitchPlayerType(PlayerType.CIRCLE);
                    break;
                case EnemyType.TRIANGLE:
                    SwitchPlayerType(PlayerType.TRIANGLE);
                    break;
            }
        }

        private void InitBasePlayer()
        {
            playerType = PlayerType.BASE;
            healthComponent.maxHealth = BasePlayerHealth;
            healthComponent.SetHealth(BasePlayerHealth);
            attackBoxComponent.attackBoxSize = new Vector2(1, 1);
            attackBoxComponent.attackBoxCenter = new Vector2(1, 0);
            meleeAttackComponent.attackTargetTypes = BasePlayerTargets;
            meleeAttackComponent.attackTargetParentName = "Enemies";
            meleeAttackComponent.SetDamage(BasePlayerAttackDamage);
            meleeAttackComponent.SetCooldown(BasePlayerAttackSpeed);
            meleeAttackComponent.SetKnockbackForce(BasePlayerKnockbackForce);
            runComponent.SetMultiplier(BasePlayerSpeedMultiplier);
            jumpComponent.SetMultiplier(BasePlayerJumpForceMultiplier);
            jumpComponent.Enable();

            spriteRendererComponent.color = new Color(1, 1, 1, 1);
        }

        private void InitSquarePlayer()
        {
            playerType = PlayerType.SQUARE;
            healthComponent.maxHealth = SquarePlayerHealth;
            healthComponent.SetHealth(SquarePlayerHealth);
            attackBoxComponent.attackBoxSize = new Vector2(1, 1);
            attackBoxComponent.attackBoxCenter = new Vector2(1, 0);
            shootAttackComponent.SetAttackTargetTypes(SquarePlayerTargets);
            shootAttackComponent.SetAttackTargetParentName("Enemies");
            shootAttackComponent.SetDamage(SquarePlayerAttackDamage);
            shootAttackComponent.SetCooldown(SquarePlayerAttackSpeed);
            shootAttackComponent.SetRange(10);
            runComponent.SetMultiplier(SquarePlayerSpeedMultiplier);
            jumpComponent.Disable();
            meleeAttackComponent.Disable();

            spriteRendererComponent.color = new Color(1, 0, 0, 1);
        }

        private void InitCirclePlayer()
        {
            playerType = PlayerType.CIRCLE;
            healthComponent.maxHealth = CirclePlayerHealth;
            healthComponent.SetHealth(CirclePlayerHealth);
            attackBoxComponent.attackBoxSize = new Vector2(1, 1);
            attackBoxComponent.attackBoxCenter = new Vector2(1, 0);
            shootAttackComponent.SetAttackTargetTypes(CirclePlayerTargets);
            shootAttackComponent.SetAttackTargetParentName("Enemies");
            shootAttackComponent.SetDamage(CirclePlayerAttackDamage);
            shootAttackComponent.SetCooldown(CirclePlayerAttackSpeed);
            shootAttackComponent.SetRange(10);
            runComponent.SetMultiplier(CirclePlayerSpeedMultiplier);
            jumpComponent.SetMultiplier(CirclePlayerJumpForceMultiplier);
            jumpComponent.Enable();
            meleeAttackComponent.Disable();

            spriteRendererComponent.color = new Color(0, 1, 0, 1);
        }

        private void InitTrianglePlayer()
        {
            playerType = PlayerType.TRIANGLE;
            healthComponent.maxHealth = TrianglePlayerHealth;
            healthComponent.SetHealth(TrianglePlayerHealth);
            attackBoxComponent.attackBoxSize = new Vector2(1, 1);
            attackBoxComponent.attackBoxCenter = new Vector2(1, 0);
            meleeAttackComponent.attackTargetTypes = TrianglePlayerTargets;
            meleeAttackComponent.attackTargetParentName = "Enemies";
            meleeAttackComponent.SetDamage(TrianglePlayerAttackDamage);
            meleeAttackComponent.SetCooldown(TrianglePlayerAttackSpeed);
            meleeAttackComponent.SetKnockbackForce(TrianglePlayerKnockbackForce);
            runComponent.SetMultiplier(TrianglePlayerSpeedMultiplier);
            jumpComponent.SetMultiplier(TrianglePlayerJumpForceMultiplier);
            jumpComponent.Enable();

            spriteRendererComponent.color = new Color(0, 0, 1, 1);
        }
        
        public void SetDamageMultiplier(float multiplier)
        {
            switch(playerType)
            {
                case PlayerType.BASE:
                    meleeAttackComponent.SetMultiplier(multiplier);
                    break;
                case PlayerType.SQUARE:
                    shootAttackComponent.SetMultiplier(multiplier);
                    break;
                case PlayerType.CIRCLE:
                    shootAttackComponent.SetMultiplier(multiplier);
                    break;
                case PlayerType.TRIANGLE:
                    meleeAttackComponent.SetMultiplier(multiplier);
                    break;
            }
        }

        public float GetDamageMultiplier()
        {
            switch(playerType)
            {
                case PlayerType.BASE:
                    return BasePlayerAttackDamage;
                case PlayerType.SQUARE:
                    return SquarePlayerAttackDamage;
                case PlayerType.CIRCLE:
                    return CirclePlayerAttackDamage;
                case PlayerType.TRIANGLE:
                    return TrianglePlayerAttackDamage;
            }
            return 1;
        }

        public void SetAttackSpeedMultiplier(float multiplier)
        {
            switch(playerType)
            {
                case PlayerType.BASE:
                    meleeAttackComponent.SetCooldown(multiplier);
                    break;
                case PlayerType.SQUARE:
                    shootAttackComponent.SetCooldown(multiplier);
                    break;
                case PlayerType.CIRCLE:
                    shootAttackComponent.SetCooldown(multiplier);
                    break;
                case PlayerType.TRIANGLE:
                    meleeAttackComponent.SetCooldown(multiplier);
                    break;
            }
        }

        public float GetAttackSpeedMultiplier()
        {
               switch(playerType)
            {
                case PlayerType.BASE:
                    return BasePlayerAttackSpeed;
                case PlayerType.SQUARE:
                    return SquarePlayerAttackSpeed;
                case PlayerType.CIRCLE:
                    return CirclePlayerAttackSpeed;
                case PlayerType.TRIANGLE:
                    return TrianglePlayerAttackSpeed;
            }
            return 1;
        }
    }
    
}
