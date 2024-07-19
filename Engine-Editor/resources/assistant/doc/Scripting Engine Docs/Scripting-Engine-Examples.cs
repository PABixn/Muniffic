//GENERAL RULES
/*
 * Every script file must import the 'eg' namespace.
 * All script classes must be public and derived from DefaultBehaviour class.
 * Public properties of script classes are serialized and can be edited in the editor.
 * All script classes can have following functions (do not override them):
 * - OnCreate() - called when the script is created.
 * - OnUpdate() - called every frame.
 * - OnCollisionEnter(Collision2D collision) - called when the entity starts colliding with another entity.
 * - OnCollisionExit(Collision2D collision) - called when the entity stops colliding with another entity.
 * Before using any component, it must be checked if the entity has it by calling HasComponent<ComponentType>() function.
 * To have a working collider, entity must have a RigidBody2DComponent and a collider component (BoxCollider2DComponent or CircleCollider2DComponent).
 * RigidBody2D component is required for physics simulation.
 * Rigidbody2D component must be initialized by calling AwakeRuntimeBody() function.
 * When having a RigidBody2DComponent, TransformComponent can't be used to change entity's position, ApplyLinearImpulse(Vector2 force) function has to be used instead to apply force to the entity.
*/


//EXAMPLE SCRIPT

using System;
using eg;

public class MyScript : DefaultBehaviour
{
    //Serialized property
    public float speed = 1.0f;

    //Called when the script is created
    public void OnCreate()
    {
        //Do something
    }

    //Called every frame
    public void OnUpdate(float ts)
    {
        //Move entity
        if (!HasComponent<RigidBody2DComponent>())
            GetComponent<TransformComponent>().translation += new Vector3(speed * ts, 0, 0);
        else
            GetComponent<RigidBody2DComponent>().ApplyLinearImpulse(new Vector2(speed, 0));
    }

    //Called when the entity starts colliding with another entity
    public void OnCollisionEnter(Collision2D collision)
    {
        //Do something
    }

    //Called when the entity stops colliding with another entity
    public void OnCollisionExit(Collision2D collision)
    {
        //Do something
    }
}


//DEFAULT BEHAVIOUR CLASS

//Get entity the script is attached to
Entity attachedEntity = entity;

//Get component of the specified type from the entity the script is attached to
SpriteRendererComponent component = GetComponent<SpriteRendererComponent>();

//Check if entity has a component of the specified type
bool hasComponent = HasComponent<SpriteRendererComponent>();

//Add component to the entity the script is attached to
SpriteRendererComponent component = AddComponent<SpriteRendererComponent>();

//Remove component from the entity the script is attached to
RemoveComponent<SpriteRendererComponent>();

//ENTITY

//Check if entity with this ID exists
bool exists = Entity.Exists(123);

//Get Entity by ID
Entity entity = Entity.FindEntityByID(123);

//Get Entity by Name
Entity entity = Entity.FindEntityByName("EntityName");

//Create an entity
Entity entity = Entity.Create("EntityName");

//Destroy an entity by its ID
Entity.Destroy(123);

//Destroy an entity by its name
Entity.Destroy("EntityName");

//Destroy an entity by its reference
Entity.Destroy(entity);

//Get entity name
string name = entity.name;

//Set entity name
entity.name = "NewName";

//Get entity's parent reference
Entity parent = entity.parent;

//Set entity's parent reference
entity.parent = someEntity;

//Set entity's parent by ID
entity.SetParent(123);

//Check if component is inherited in entity's children
bool inherited = entity.IsInheritedInChildren<SpriteRendererComponent>();

//Check if component is inherited from entity's parent
bool inherited = entity.IsInheritedFromParent<SpriteRendererComponent>();

//Stop inheriting component from entity's parent
entity.StopInheritingComponent<SpriteRendererComponent>();

//Start inheriting component from entity's parent
entity.InheritComponent<SpriteRendererComponent>();

//Check if someEntity is any level child of another entity
bool isChild = entity.IsChildOfAny(someEntity);

//Check if someEntity ID is any level child of another entity
bool isChild = entity.IsChildOfAny(123);

//Check if someEntity is direct child of another entity
bool isChild = entity.IsChildOf(someEntity);

//Check if someEntity ID is direct child of another entity
bool isChild = entity.IsChildOf(123);

//Add entity as a child
entity.AddChild(someEntity);

//Add entity as a child by ID
entity.AddChild(123);

//Remove entity as a child
entity.RemoveChild(someEntity);

//Remove entity as a child by ID
entity.RemoveChild(123);

//Remove all children from the entity
entity.RemoveAnyChildren();

//Copy the component values of the specified type to all children entities of the entity
entity.CopyComponentValuesToChildren<SpriteRendererComponent>();

//Copy the component of the specified type to all children entities of the entity
entity.CopyComponentToChildren<SpriteRendererComponent>();

//Remove the component of the specified type from all children entities of the entity
entity.RemoveComponentFromChildren<SpriteRendererComponent>();

//Get any level children of the entity
List<Entity> children = entity.GetAnyChildren();

//Get direct children of the entity
List<Entity> children = entity.GetChildren();

//Get script instance of given type (class) from entity (script must be attached to entity)
MyScript script = entity.As<MyScript>();

//Check if entity has a component of the specified type
bool hasComponent = entity.HasComponent<SpriteRendererComponent>();

//Get component of the specified type from entity
SpriteRendererComponent component = entity.GetComponent<SpriteRendererComponent>();

//Add component to entity
SpriteRendererComponent component = entity.AddComponent<SpriteRendererComponent>();

//Remove component from entity
entity.RemoveComponent<SpriteRendererComponent>();




//TRANSFORM COMPONENT

//Get entity's transform component
TransformComponent transform = entity.GetComponent<TransformComponent>();

//Get entity's position relative to point (0,0,0)
Vector3 position = transform.translation;

//Set entity's position relative to point (0,0,0)
transform.translation = new Vector3(1, 2, 3);

//Get entity's scale
Vector3 scale = transform.scale;

//Set entity's scale
transform.scale = new Vector3(1, 2, 3);

//Get entity's rotation
Vector3 vector3 = transform.rotation;

//Set entity's rotation
transform.rotation = new Vector3(1, 2, 3);




//SPRITE RENDERER COMPONENT

//Get entity's sprite renderer component
SpriteRendererComponent spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

//Get entity's color
Color color = spriteRenderer.color;

//Set entity's color
spriteRenderer.color = new Color(1, 1, 1, 1);

//Get entity's texture path
string texturePath = spriteRenderer.texture;

//Set entity's texture path
spriteRenderer.texture = "path/to/texture.png";

//Get entity's tiling factor
float tiling = spriteRenderer.tilingFactor;

//Set entity's tiling factor
spriteRenderer.tilingFactor = 2.0f;




//CIRCLE RENDERER COMPONENT

//Get entity's circle renderer component
CircleRendererComponent circleRenderer = entity.GetComponent<CircleRendererComponent>();

//Get entity's color
Color color = circleRenderer.color;

//Set entity's color
circleRenderer.color = new Color(1, 1, 1, 1);

//Get entity's radius
float radius = circleRenderer.thickness;

//Set entity's radius   
circleRenderer.thickness = 2.0f;

//Get entity's fade
float fade = circleRenderer.fade;

//Set entity's fade
circleRenderer.fade = 0.5f;




//ANIMATOR COMPONENT

//Get entity's animator component
AnimatorComponent animator = entity.GetComponent<AnimatorComponent>();

//Get entity's animation speed
float speed = animator.speed;

//Set entity's animation speed
animator.speed = 2.0f;

//Play animation by name
animator.Play("AnimationName");

//Stop playing current animation
animator.Stop();

//Update current animation by specified delta time
animator.Update(0.1f);

//Change current playing animation to another one
animator.ChangeAnimation("AnimationName");

//Add animation to animator with specified name
animator.AddAnimation("AnimationName");

//Remove animation from animator by name
animator.RemoveAnimation("AnimationName");

//Remove last added animation
animator.RemoveLastAnimation();

//TO DO




//CAMERA COMPONENT

//Get entity's camera component
CameraComponent camera = entity.GetComponent<CameraComponent>();

//Check if camera is the primary camera
bool isPrimary = camera.primary;

//Set camera as primary
camera.primary = true;

//Get camera's fixed aspect ratio
bool fixedAspectRatio = camera.fixedAspectRatio;

//Set camera's fixed aspect ratio
camera.fixedAspectRatio = true;

//Get camera's projection type
ProjectionType projectionType = camera.type;

//Set camera's projection type to ortographic
camera.type = ProjectionType.Ortographic;

//Set camera's projection type to perspective
camera.type = ProjectionType.Perspective;

//Set camera type to perspective and specify all parameters
camera.SetPerspective(float verticalFov, float nearClip, float farClip);

//Set camera type to ortographic and specify all parameters
camera.SetOrtographic(float size, float nearClip, float farClip);

//Get camera's orthographic size
float size = camera.orthographicSize;

//Set camera's orthographic size
camera.orthographicSize = 2.0f;

//Get camer'as orthographic near clip
float nearClip = camera.orthographicNearClip;

//Set camera's orthographic near clip
camera.orthographicNearClip = 0.1f;

//Get camera's orthographic far clip
float farClip = camera.orthographicFarClip;

//Set camera's orthographic far clip
camera.orthographicFarClip = 100.0f;

//Get camera's perspective vertical field of view
float verticalFov = camera.perspectiveVerticalFov;

//Set camera's perspective vertical field of view
camera.perspectiveVerticalFov = 60.0f;

//Get camera's perspective near clip
float nearClip = camera.perspectiveNearClip;

//Set camera's perspective near clip
camera.perspectiveNearClip = 0.1f;

//Get camera's perspective far clip
float farClip = camera.perspectiveFarClip;

//Set camera's perspective far clip
camera.perspectiveFarClip = 100.0f;




//RIGID BODY 2D COMPONENT

//Get entity's Rigidbody2D component
RigidBody2DComponent rigidbody2D = entity.GetComponent<RigidBody2DComponent>();

//Get entity's body type
BodyType bodyType = rigidbody2D.type;

//Set entity's body type to static
rigidbody2D.type = BodyType.Static;

//Set entity's body type to dynamic
rigidbody2D.type = BodyType.Dynamic;

//Set entity's body type to kinematic
rigidbody2D.type = BodyType.Kinematic;

//Get entity's linear velocity
Vector2 linearVelocity = rigidbody2D.linearVelocity;

//Set entity's linear velocity
rigidbody2D.linearVelocity = new Vector2(1, 2);

//Initialize the runtime body of the entity, awake the entity
rigidbody2D.AwakeRuntimeBody();

//Apply force to the entity at the center of mass
rigidbody2D.ApplyLinearImpulse(new Vector2(1, 2));

//Apply force to the entity at the specified world point
rigidbody2D.ApplyLinearImpulse(new Vector2(1, 2), new Vector2(3, 4));

//Check if entity has fixed rotation
bool fixedRotation = rigidbody2D.fixedRotation;

//Set entity's fixed rotation
rigidbody2D.fixedRotation = true;




//BOX COLLIDER 2D COMPONENT

//Get entity's BoxCollider2D component
BoxCollider2DComponent boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

//Get entity's collider size
Vector2 size = boxCollider2D.size;

//Set entity's collider size
boxCollider2D.size = new Vector2(1, 2);

//Get entity's collider offset
Vector2 offset = boxCollider2D.offset;

//Set entity's collider offset
boxCollider2D.offset = new Vector2(1, 2);

//Get entity's collider density
float density = boxCollider2D.density;

//Set entity's collider density
boxCollider2D.density = 1.0f;

//Get entity's collider friction
float friction = boxCollider2D.friction;

//Set entity's collider friction
boxCollider2D.friction = 0.5f;

//Get entity's collider restitution
float restitution = boxCollider2D.restitution;

//Set entity's collider restitution
boxCollider2D.restitution = 0.5f;

//Get entity's collider restitution threshold
float restitutionThreshold = boxCollider2D.restitutionThreshold;

//Set entity's collider restitution threshold
boxCollider2D.restitutionThreshold = 0.5f;

//Check if entity's collider collides with another entity
bool collides = boxCollider2D.CollidesWith(anotherEntity);

//Check if entity's collider collides with another entity by ID
bool collides = boxCollider2D.CollidesWith(123);

//Check if entity's collider collides with another entity by name
bool collides = boxCollider2D.CollidesWithEntity("EntityName");

//Check if entity's collider collides with another entity's box collider
bool collides = boxCollider2D.CollidesWithBox(anotherEntity);

//Check if entity's collider collides with a box at specified world position and size (entity is in this area)
bool collides = boxCollider2D.CollidesWithBox(new Vector2(1, 2), new Vector2(3, 4));

//Check if entity's collider collides with another entity's circle collider
bool collides = boxCollider2D.CollidesWithCircle(anotherEntity);

//Check if entity's collider collides with a circle at specified world position and radius (entity is in this area)
bool collides = boxCollider2D.CollidesWithCircle(new Vector2(1, 2), 1.0f);

//Check if entity's collider collides with specified world point
bool collides = boxCollider2D.CollidesWithPoint(new Vector2(1, 2));

//Check if entity's collider collides with the specified edge defined by start and end points
bool collides = boxCollider2D.CollidesWithEdge(new Vector2(1, 2), new Vector2(3, 4));

//Check if entity's collider collides with the bottom edge of the specified entity
bool collides = boxCollider2D.CollidesWithBottomEdge(anotherEntity);

//Check if entity's collider collides with the top edge of the specified entity
bool collides = boxCollider2D.CollidesWithTopEdge(anotherEntity);

//Check if entity's collider collides with the left edge of the specified entity
bool collides = boxCollider2D.CollidesWithLeftEdge(anotherEntity);

//Check if entity's collider collides with the right edge of the specified entity
bool collides = boxCollider2D.CollidesWithRightEdge(anotherEntity);

//Check if entity's collider collides with the left edge of the specified entity
bool collides = boxCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.LEFT);

//Check if entity's collider collides with the right edge of the specified entity
bool collides = boxCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.RIGHT);

//Check if entity's collider collides with the top edge of the specified entity
bool collides = boxCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.TOP);

//Check if entity's collider collides with the bottom edge of the specified entity
bool collides = boxCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.BOTTOM);




//CIRCLE RENDERER COMPONENT

//Get entity's CircleCollider2D component
CircleCollider2DComponent circleCollider2D = entity.GetComponent<CircleCollider2DComponent>();

//Get entity's collider radius
float radius = circleCollider2D.radius;

//Set entity's collider radius
circleCollider2D.radius = 1.0f;

//Get entity's collider offset
float offset = circleCollider2D.offset;

//Set entity's collider offset
circleCollider2D.offset = 1.0f;

//Get entity's collider density
float density = circleCollider2D.density;

//Set entity's collider density
circleCollider2D.density = 1.0f;

//Get entity's collider friction
float friction = circleCollider2D.friction;

//Set entity's collider friction
circleCollider2D.friction = 0.5f;

//Get entity's collider restitution
float restitution = circleCollider2D.restitution;

//Set entity's collider restitution
circleCollider2D.restitution = 0.5f;

//Get entity's collider restitution threshold
float restitutionThreshold = circleCollider2D.restitutionThreshold;

//Set entity's collider restitution threshold
circleCollider2D.restitutionThreshold = 0.5f;

//Check if entity's collider collides with another entity
bool collides = circleCollider2D.CollidesWith(anotherEntity);

//Check if entity's collider collides with another entity by name
bool collides = circleCollider2D.CollidesWithEntity("EntityName");

//Check if entity's collider collides with another entity's box collider
bool collides = circleCollider2D.CollidesWithBox(anotherEntity);

//Check if entity's collider collides with a box at specified world position and size (entity is in this area)
bool collides = circleCollider2D.CollidesWithBox(new Vector2(1, 2), new Vector2(3, 4));

//Check if entity's collider collides with another entity's circle collider
bool collides = circleCollider2D.CollidesWithCircle(anotherEntity);

//Check if entity's collider collides with a circle at specified world position and radius (entity is in this area)
bool collides = circleCollider2D.CollidesWithCircle(new Vector2(1, 2), 1.0f);

//Check if entity's collider collides with specified world point
bool collides = circleCollider2D.CollidesWithPoint(new Vector2(1, 2));

//Check if entity's collider collides with the specified edge defined by start and end points
bool collides = circleCollider2D.CollidesWithEdge(new Vector2(1, 2), new Vector2(3, 4));

//Check if entity's collider collides with the bottom edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesBottomEdge(anotherEntity);

//Check if entity's collider collides with the top edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesTopEdge(anotherEntity);

//Check if entity's collider collides with the left edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesLeftEdge(anotherEntity);

//Check if entity's collider collides with the right edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesRightEdge(anotherEntity);

//Check if entity's collider collides with the left edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.LEFT);

//Check if entity's collider collides with the right edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.RIGHT);

//Check if entity's collider collides with the top edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.TOP);

//Check if entity's collider collides with the bottom edge of the specified entity
bool collides = circleCollider2D.CollidesWithEntitiesSide(anotherEntity, Side.BOTTOM);




//TEXT COMPONENT

//Get entity's TextComponent
TextComponent textComponent = entity.GetComponent<TextComponent>();

//Get entity's text content
string sampleText = textComponent.text;

//Set entity's text content
textComponent.text = "Hello World!";

//Get entity's text color
Color color = textComponent.color;

//Set entity's text color
textComponent.color = new Color(1, 1, 1, 1);

//Get entity's text kerning (spacing between characters)
float kerning = textComponent.kerning;

//Set entity's text kerning
textComponent.kerning = 1.0f;

//Get entity's text line spacing (spacing between lines)
float lineSpacing = textComponent.lineSpacing;

//Set entity's text line spacing
textComponent.lineSpacing = 1.0f;




//INPUT SYSTEM

//Check if key is currently pressed
bool isPressed = Input.IsKeyDown(KeyCode.Space);




//COLOR SYSTEM

//Supports RGB, HEX, HSV, HSL, CMYK color spaces

//Create RGBA color
Color color = new Color(0.75f, 1, 1, 1);

//Create RGB color
Color color = new Color(0.75f, 0.75f, 0.75f);

//Create HEX color
Color color = new Color("#FFFFFF");

//Create HSV color
HSVColor hsvColor = new HSVColor(0.75f, 0.75f, 0.75f, 1);

//Create HSV color from RGB
HSVColor hsvColor = new HSVColor(color);

//Create HSL color
HSLColor hslColor = new HSLColor(0.75f, 0.75f, 0.75f, 1);

//Create HSL color from RGB
HSLColor hslColor = new HSLColor(color);

//Create CMYK color
CMYKColor cmykColor = new CMYKColor(0.75f, 0.75f, 0.75f, 1);

//Create CMYK color from RGB
CMYKColor cmykColor = new CMYKColor(color);

//Convert RGB color to HEX
string hex = Color.ToHex(color);

//Convert HSV color to RGB
Color rgb = HSVColor.ToColor(hsvColor);

//Convert HSL color to RGB
Color rgb = HSLColor.ToColor(hslColor);

//Convert CMYK color to RGB
Color rgb = CMYKColor.ToColor(cmykColor);




//COLLISION 2D

//Collision2D class is passed as an argument to OnCollisionEnter and OnCollisionExit functions
Collision2D collision = new Collision2D();

//Get other entity involved in collision
Entity otherEntity = collision.otherEntity;

//Get contact point of collision
Vector2 contactPoint = collision.contactPoints;

//Get friction of collision
float friction = collision.friction;

//Get restitution of collision
float restitution = collision.restitution;

//Get tangent speed of collision
float tangentSpeed = collision.tangentSpeed;




//DEBUG CONSOLE

//Severity levels of the messages
DebugConsole.LogType.Info;
DebugConsole.LogType.Warning;
DebugConsole.LogType.Error;

//Log message to the console
DebugConsole.Log("Message");

//Log message to the console with specified severity level
DebugConsole.Log("Message", DebugConsole.LogType.Info);
DebugConsole.Log("Message", DebugConsole.LogType.Warning);
DebugConsole.Log("Message", DebugConsole.LogType.Error);