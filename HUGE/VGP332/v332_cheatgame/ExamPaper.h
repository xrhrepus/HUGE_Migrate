#pragma once
#include <HUGE\Inc\HUGE.h>

class ExamPaper
{
 
public:
	ExamPaper() = default;
	~ExamPaper();

	void Init(int index,const Vector2& Position);
	//void Update(float);
	void Render();

	void Solve(int index);

	int QuestionAnswerd() ;
 	bool IsCompeleted() const;

	void DebugUI();

	static const int totalQuestion = 10;
	std::array<bool, totalQuestion> mQuestions;
	int answered = 0;
	Vector2 mPosition;
	int mIndex;
private:
	TextureId mTextureId;

	size_t mDifficulty = 7; // 70% question start with false
 

 };

 