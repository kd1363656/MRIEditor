#pragma once

namespace MRI::MathUtility
{
	static constexpr float k_hermiteCoefficient2 = 2.0F;
	static constexpr float k_hermiteCoefficient3 = 3.0F;
	static constexpr float k_hermiteIdentity	 = 1.0F;

	// クオータニオンからオイラー角(度)に戻す
	inline Math::Vector3 QuaternionToEuler(const Math::Quaternion& a_quaternion)
	{
		Math::Vector3 l_radian = a_quaternion.ToEuler();

		return Math::Vector3
		{
			DirectX::XMConvertToDegrees(l_radian.x) ,
			DirectX::XMConvertToDegrees(l_radian.y) ,
			DirectX::XMConvertToDegrees(l_radian.z)
		};
	}
	
	// オイラー角(度)からクオータニオンに変換
	inline Math::Quaternion EulerToQuaternion(const Math::Vector3& a_euler)
	{
		return Math::Quaternion
		{
			Math::Quaternion::CreateFromYawPitchRoll
			(
				DirectX::XMConvertToRadians(a_euler.y) ,
				DirectX::XMConvertToRadians(a_euler.x) ,
				DirectX::XMConvertToRadians(a_euler.z)
			)
		};
	}

	// 行列から"Yaw"を取得
	inline float VectorToYaw(const Math::Vector3& a_euler)
	{
		return std::atan2f(a_euler.x , a_euler.z);
	}

	inline void AddDirection(const Math::Vector3& a_addDirection , Math::Vector3& a_target)
	{
		a_target += a_addDirection;
		
		// ベクトル情報は"-1.0f ~ 1.0f"しかとってほしくない
		a_target.x = std::clamp(a_target.x , -Math::Vector3::UnitX.x , Math::Vector3::UnitX.x);
		a_target.y = std::clamp(a_target.y , -Math::Vector3::UnitY.y , Math::Vector3::UnitY.y);
		a_target.z = std::clamp(a_target.z , -Math::Vector3::UnitZ.z , Math::Vector3::UnitZ.z);
	}

	inline float Hermite(const float a_t  , 
		                 const float a_p0 , 
		                 const float a_p1 , 
		                 const float a_m0 , 
		                 const float a_m1)
	{
		const float l_t2 = a_t  * a_t;
		const float l_t3 = l_t2 * a_t;

		const float h00_ = (k_hermiteCoefficient2 * l_t3) - (k_hermiteCoefficient3 * l_t2) + k_hermiteIdentity;
		const float h10_ = l_t3 - (k_hermiteCoefficient2 * l_t2) + a_t;
		const float h01_ = -k_hermiteCoefficient2 * l_t3 + k_hermiteCoefficient3 * l_t2;
		const float h11_ = l_t3 - l_t2;

		return h00_ * a_p0 + h10_ * a_m0 + h01_ * a_p1 + h11_ * a_m1;
	}
}