#include "ExamPaper.h"
 
ExamPaper::~ExamPaper()
{
}

void ExamPaper::Init(int index, const Vector2& Position)
{
	mTextureId = TextureManager::Get()->LoadTexture(L"desk64.png");

	for (auto& b : mQuestions)
	{
		b = false;
	}
	mIndex = index;
	mPosition = Position;
 
}

void ExamPaper::Render()
{
	const float mRenderOffset = 32.0f;
 	H::Graphics::SpriteRenderer::Get()->Draw(*(TextureManager::Get()->GetTexture(mTextureId)), { mPosition.x - mRenderOffset, mPosition.y - mRenderOffset });
 }

void ExamPaper::Solve(int index)
{
	if (mQuestions[index] != true)
	{
		mQuestions[index] = true;
		answered += 1;
	}
}
	

int ExamPaper::QuestionAnswerd() 
{
	int count = 0;
	for (const auto& q : mQuestions)
	{
		if (q)
		{
			count++;
		}
	}
	answered = count;
	return answered;
}

bool ExamPaper::IsCompeleted() const
{
 	//return ret;
	bool cmplt = true;
	for (const auto& q :mQuestions)
	{
		cmplt &= q;
	}
	return cmplt;
	//return answered == totalQuestion;
 }

void ExamPaper::DebugUI()
{
	if (ImGui::TreeNode(  ("EPaper" + std::to_string(mIndex)).c_str()   ))
	{
		ImGui::Columns(5);
		for (size_t i = 0; i < mQuestions.size(); i++)
		{
			ImGui::Checkbox(("ques " + std::to_string(i)).c_str(), &mQuestions[i]);
			ImGui::NextColumn();
		}
		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::TreePop();
	}
}
