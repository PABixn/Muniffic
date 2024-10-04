#pragma once

namespace eg {
	enum class ScriptFieldType
	{
		None = 0,
		Float, Vector2, Vector3, Vector4,
		Int32, Int64, Bool, Double, Short, Byte, Char,
		UByte, UInt32, UInt64, UShort, SByte,
		String, Object, Void,
		Entity//, TransformComponent, RigidBodyComponent
	};
}