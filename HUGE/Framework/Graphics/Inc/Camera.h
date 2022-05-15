#ifndef HUGE_GRAPHIC_CAMERA_H
#define HUGE_GRAPHIC_CAMERA_H

namespace H {
namespace Graphics {

		Math::Matrix4 ComputeViewMatrix(const Math::Vector3& right, const Math::Vector3& up, const Math::Vector3& look, const  Math::Vector3& position);
		Math::Matrix4 ComputePerspectiveMatrix(float nearp, float farp, float fov, float aspect);
		
		//Y is up
		class Camera
		{

		public:
			void SetPosition(const Math::Vector3& pos);
			void SetDirection(const Math::Vector3& dir);
			void SetLookAt(const Math::Vector3& target);

			//3 degrees of freedom of translation
			void Walk(float distance);
			void Strafe(float distance);
			void Rise(const float distance);//y

			//2 degrees of freedom of rotation
			void Yaw(float radian);//y
			void Pitch(float radian);//x

			void SetFOV(float fov);
			void SetNearPlane(float nearplane);
			void SetFarPlane(float farplane);
			void SetAspectRatio(float aspectRatio);
			const Math::Vector3& GetPosition() const { return mPosition; }
			const Math::Vector3& GetDirection() const { return mDirection; }
			Math::Vector3 GetCameraUp() const;

			float GetFOV() const { return mFOV; }
			float GetNearPlane() const { return mNearPlane; }
			float GetFarPlane() const { return mFarPlane; }


			Math::Matrix4 GetViewMatrix()const;
			Math::Matrix4 GetPerspectiveMatrix(float aspectratio = 0.0f) const;
			Math::Matrix4 GetOrthographicMatrix(float width,float height) const;

			// to move camera
			void UpdateControl(float deltaTime);
			void DebugUI();

		private:
			Math::Vector3 mPosition{ 0.0f };
			Math::Vector3 mDirection{ 0.0f,0.0f,1.0f };

			float mFOV{ 60.0f * Math::Constants::DegToRad };
			float mNearPlane{ 1.0f };
			float mFarPlane{ 1000.0f };

			float mTurnSpeed{ 2.1f };
			float mMoveSpeed{ 30.0f };

			float mAspectRatio = 0.0f;

			//allow mouse wheel zoom in
			int mPrevMouseZ = 0;
			int mCurrMouseZ = 0;
			float mZoomInSpeed = 10.0f;
 
		};

}//namespace Graphics
}//namespace HUGE

#endif // !HUGE_GRAPHIC_CAMERA_H

