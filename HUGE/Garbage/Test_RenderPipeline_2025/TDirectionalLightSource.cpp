#include "TDirectionalLightSource.h"


void TDirectionalLightSource::updateLightVPMatrix(const H::Math::Vector3& lightDir, const H::Math::Vector3& lightPos)
{
    const H::Math::Vector3 right = H::Math::Cross(H::Math::Vector3::yAxis(), lightDir);
    const H::Math::Vector3 up = H::Math::Cross(lightDir, right);

    mLightViewMatrix = H::Graphics::ComputeViewMatrix(right, up, lightDir, lightPos);
    //mLightProjectionMatrix = H::Graphics::ComputePerspectiveMatrix(1.0f, 1000.0f, 60.0f * H::Math::Constants::DegToRad,1);
    mLightProjectionMatrix = H::Graphics::CreateOrthographicMatrix(2048, 2048, 1.0f, 1000.0f);
}

