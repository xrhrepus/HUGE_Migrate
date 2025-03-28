#pragma once
#include <Math/Inc/HUGEMath.h>
#include <Graphics/Inc/Graphics.h>

// fake a global service
struct TDirectionalLightSource {
    void updateLightVPMatrix(const H::Math::Vector3& lightDir, const H::Math::Vector3& lightPos);

    H::Graphics::DirectionalLight mLight;
    H::Math::Vector3 mLightPos;
    H::Math::Matrix4 mLightViewMatrix;
    H::Math::Matrix4 mLightProjectionMatrix;
};
