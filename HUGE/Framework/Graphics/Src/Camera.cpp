#include "Precompiled.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "Input/Inc/Input.h"
#include "ImGui/Inc/imgui.h"
using namespace H;
using namespace H::Math;
using namespace H::Graphics;

Math::Matrix4 Graphics::ComputeViewMatrix(const Math::Vector3& right, const Math::Vector3& up, const Math::Vector3& look, const  Math::Vector3& position)
{
	//need normal vector
	const float dx = -Math::Dot(right, position);
	const float dy = -Math::Dot(up, position);
	const float dz = -Math::Dot(look, position);

	return {
		right.x,up.x,look.x,0.0f,
		right.y,up.y,look.y,0.0f,
		right.z,up.z,look.z,0.0f,
		dx,		dy,	 dz,	1.0f
	};


}
Math::Matrix4 Graphics::ComputePerspectiveMatrix(float nearp, float farp, float fov, float aspect)
{
	const float h = 1 / tan(fov * 0.5f);
	const float w = h / aspect;
	const float zf = farp;
	const float zn = nearp;
	const float d = zf / (zf - zn);

	//return{
	//	w,		  0.0f,0.0f,0.0f,
	//	0.0f,	h,	   0.0f,0.0f,
	//	0.0f,0.0f,	d,		1.0f,
	//	0.0f,0.0f,	-zn * d,	0.0f
	//};
	return Matrix4(
		w,   0.0f, 0.0f,	0.0f,
		0.0f, h, 0.0f,		0.0f,
		0.0f, 0.0f, d,		1.0f,
		0.0f, 0.0f, -zn * d,0.0f
	);


}
//	^ y
//	| /  z
//	|/
//	--------> x


Math::Matrix4 Camera::GetViewMatrix()const
{
	const Math::Vector3 l = mDirection;
	const Math::Vector3 r = Math::Normalize(Math::Cross({0.0f,1.0f,0.0f},l));
	const Math::Vector3 u = Math::Normalize(Math::Cross(l,r));;
	return ComputeViewMatrix(r, u, l, mPosition);
}
Math::Matrix4 Camera::GetPerspectiveMatrix(float aspectratio) const
{
	if (aspectratio == 0.0f)
	{
		auto width = GraphicSystem::Get()->GetBackBufferWidth();
		auto height = GraphicSystem::Get()->GetBackBufferHeight();

		aspectratio = static_cast<float> (width) / static_cast<float> (height);
	}
	return ComputePerspectiveMatrix(mNearPlane, mFarPlane, mFOV, aspectratio);
}
Math::Matrix4 H::Graphics::Camera::GetOrthographicMatrix(float width, float height) const
{
	const float w = width;
	const float h = height;
	const float zf = mFarPlane;
	const float zn = mNearPlane;
 
	return Math::Matrix4{
		2.0f/w, 0.0f,   0.0f,		   0.0f,
		0.0f,   2.0f/h, 0.0f,		   0.0f,
		0.0f,   0.0f,   1.0f / (zf-zn),0.0f,
		0.0f,   0.0f,   zn / (zn - zf),1.0f
	};
}
void  Camera::SetPosition(const Math::Vector3& pos)
{
	mPosition = pos;
}
void  Camera::SetDirection(const Math::Vector3& dir)
{
	mDirection = Math::Normalize(dir);
}
void  Camera::SetLookAt(const Math::Vector3& target)
{
	
}
void Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
}
void Camera::SetFOV(float fov)
{
	mFOV = fov;
}
void Camera::SetNearPlane(float nearplane)
{
	mNearPlane = nearplane;
}
void Camera::SetFarPlane(float farplane)
{
	mFarPlane = farplane;
}

void Camera::Walk(float distance) // 
{
	mPosition += mDirection * distance;
}
void Camera::Strafe(float distance)
{
 
	const Math::Vector3 right = Normalize(Cross(mDirection, { 0.0, 1.0f, 0.0f } ));
	//const Math::Vector3 right = Normalize(Cross(mDirection, GetCameraUp()));

	mPosition += right * distance;
}
void Camera::Rise(const float distance)
{
	//Vector3 r{ 0.0, 1.0f, 0.0f };
	//mPosition +=  r * distance;
	mPosition += GetCameraUp() * distance;

}//y
void Camera::Yaw(float radian)
{
	Matrix4 rot{};
	rot.SetIdentity();

	Matrix4 r = (rot.RotationY(radian));
	mDirection = TransformNormal(mDirection, rot);
	//Vector3 v = r * mDirection;
	////mDirection = Math::Normalize((rot.RotationX(radian)) * mDirection);
	//mDirection = Math::Normalize(v);


}

void Camera::Pitch(float radian)
{
	Vector3 up = { 0.0f,1.0f,0.0f };
	const Vector3 right = Math::Normalize(Cross(up,mDirection));
	Matrix4 rot;
	rot.SetIdentity();
	rot = Math::MatrixRotationAxis(right,radian);
	const Vector3 newLook = TransformNormal(mDirection, rot);
	const float dot = Dot(newLook, up);

	// Avoid looking straight up or down
	if (abs(dot) < 0.995f)
	{
		mDirection = newLook;
	}

}
Math::Vector3 Camera::GetCameraUp() const
{
	Vector3 v = H::Math::Cross(mDirection, { 0.0f,1.0f,0.0f });
	//Vector3 v = H::Math::Cross({ 0.0f,1.0f,0.0f }, mDirection);

	//v = H::Math::Cross(mDirection, v);
	//return H::Math::Cross(mDirection, v);
	return ( H::Math::Cross(v, mDirection));

}

void Camera::UpdateControl(float deltaTime)
{
	auto inputSys = Input::InputSystem::Get();
	mCurrMouseZ = inputSys->GetMouseMoveZ();
	int wheelScroll = mCurrMouseZ - mPrevMouseZ;
	mPrevMouseZ = mCurrMouseZ;
 
	float spd_times_deltatime = mMoveSpeed * deltaTime;

	if (inputSys->IsMouseDown(Input::MouseButton::LBUTTON))
	{
		if (int mx = inputSys->GetMouseMoveY())
		{
			Rise(-mx* mMoveSpeed*deltaTime);

		}
		if (int my = inputSys->GetMouseMoveX())
		{
			Strafe(-my* mMoveSpeed*deltaTime);
		}

	}



	if (inputSys->IsKeyDown(Input::KeyCode::W) || wheelScroll > 0)
	{
		Walk(mZoomInSpeed * spd_times_deltatime);
	}
	else if (inputSys->IsKeyDown(Input::KeyCode::S) || wheelScroll < 0)
	{
		Walk(mZoomInSpeed * -spd_times_deltatime);
	}
 



	if (inputSys->IsKeyDown(Input::KeyCode::A)/* || inputSys->IsMouseLeftEdge()*/)
	{
		Strafe(spd_times_deltatime);
	}
	else if (inputSys->IsKeyDown(Input::KeyCode::D)/* || inputSys->IsMouseRightEdge()*/)
	{
		Strafe(-spd_times_deltatime);
	}
 

	if (inputSys->IsKeyDown(Input::KeyCode::R)/* || inputSys->IsMouseTopEdge()*/)
	{
		Rise(spd_times_deltatime);
	}
	else if (inputSys->IsKeyDown(Input::KeyCode::F)/* || inputSys->IsMouseBottomEdge()*/)
	{
		Rise(-spd_times_deltatime);
	}

	if (inputSys->IsMouseDown(Input::MouseButton::RBUTTON))
	{
		Yaw(inputSys->GetMouseMoveX() * mTurnSpeed * deltaTime );
		Pitch(inputSys->GetMouseMoveY() * mTurnSpeed * deltaTime);

	}

	 
}

void H::Graphics::Camera::DebugUI()
{
	ImGui::CollapsingHeader("Camera");
	 
	if (ImGui::Button("reset cam##Camera"))
	{
		SetPosition({ 0.0f, 0.0f, 30.0f });
		SetDirection({ 0.0f,0.0f, -1.0f });
	}
	ImGui::DragFloat3("Position##Camera", mPosition.v.data(), 0.1f);
	ImGui::DragFloat3("Direction##Camera", mDirection.v.data(), 0.1f);

 }
