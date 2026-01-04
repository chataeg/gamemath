
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
using namespace CK::DD;

// 격자를 그리는 함수
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// 좌측 하단에서부터 격자 그리기
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// 그리드가 시작되는 좌하단 좌표 값 계산
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// 게임 오브젝트 목록


// 최초 씬 로딩을 담당하는 함수
void SoftRenderer::LoadScene2D()
{
	// 최초 씬 로딩에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();

}

// 게임 로직과 렌더링 로직이 공유하는 변수
Vector2 pointAPosition(100.f, 100.f);
Vector2 pointBPosition = Vector2(pointAPosition.X + 100.f, pointAPosition.Y + 100.f);
Vector2 boxPosition(-200.f, 150.f);
Vector2 cooltimeboxPosition(-200.f, -150.f);

float currentBoxDegree = 0.f; 
bool spacebarPressed = false;
float currentCooltime = 0.f;
float cooltimeLimit = 5.f;


// 게임 로직을 담당하는 함수
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// 게임 로직의 로컬 변수
	static float moveSpeed = 100.f;
	static float rotateSpeed = 180.f;

	Vector2 inputVector = Vector2{ input.GetAxis(InputAxis::XAxis),input.GetAxis(InputAxis::YAxis) };
	Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;

	pointBPosition += deltaPosition;

	float deltaDegree = input.GetAxis(InputAxis::WAxis) * rotateSpeed * InDeltaSeconds;
	
	if (input.IsPressed(InputButton::Space))
	{
		spacebarPressed = true;
	}

	if (spacebarPressed)
	{
		currentCooltime += InDeltaSeconds;

		if (currentCooltime >= cooltimeLimit)
		{
			spacebarPressed = false;
			currentCooltime = 0.f;
		}
	}

	currentBoxDegree += deltaDegree;
}

// 렌더링 로직을 담당하는 함수
void SoftRenderer::Render2D()
{
	// 렌더링 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 배경에 격자 그리기
	DrawGizmo2D();

	// 렌더링 로직의 로컬 변수

	// 밝은 회색의 선을 사용해 벡터를 표현
	static float lineLength = 500.f;
	Vector2 lineStart = pointAPosition * lineLength;
	Vector2 lineEnd = pointAPosition * -lineLength;
	//r.DrawLine(lineStart, lineEnd, LinearColor::LightGray);

	r.DrawPoint(pointAPosition, LinearColor::Blue);
	r.DrawPoint(pointAPosition + Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(pointAPosition - Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(pointAPosition + Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(pointAPosition - Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(pointAPosition + Vector2::One, LinearColor::Blue);
	r.DrawPoint(pointAPosition - Vector2::One, LinearColor::Blue);
	r.DrawPoint(pointAPosition + Vector2(1.f,-1.f), LinearColor::Blue);
	r.DrawPoint(pointAPosition - Vector2(1.f,-1.f), LinearColor::Blue);
	
	
	
	r.DrawPoint(pointBPosition, LinearColor::Red);
	r.DrawPoint(pointBPosition + Vector2::UnitX, LinearColor::Red);
	r.DrawPoint(pointBPosition - Vector2::UnitX, LinearColor::Red);
	r.DrawPoint(pointBPosition + Vector2::UnitY, LinearColor::Red);
	r.DrawPoint(pointBPosition - Vector2::UnitY, LinearColor::Red);
	r.DrawPoint(pointBPosition + Vector2::One, LinearColor::Red);
	r.DrawPoint(pointBPosition - Vector2::One, LinearColor::Red);
	r.DrawPoint(pointBPosition + Vector2(1.f, -1.f), LinearColor::Red);
	r.DrawPoint(pointBPosition - Vector2(1.f, -1.f), LinearColor::Red);

	r.DrawLine(pointAPosition, pointBPosition, LinearColor::Cyan);
	r.DrawLine(pointAPosition, pointAPosition + Vector2(0.f,180.f), LinearColor::Black);

	Vector2 pointA2B = pointBPosition - pointAPosition;

	static float halfSize = 100.f;
	static std::vector<Vector2> squares;

	if (squares.empty())
	{
		for (float x = -halfSize; x <= halfSize; x += 0.25f)
		{
			for (float y = -halfSize; y <= halfSize; y += 0.25f)
			{
				squares.push_back(Vector2(x, y));
			}
		}
	}	

	float sin = 0.f, cos = 0.f;
	Math::GetSinCos(sin, cos, currentBoxDegree);

	/*
		기저벡터 (1,0) , (0,1) 이 있을 때 θ만큼 각각 회전한다고 하자.

		기저벡터 (1,0) θ만큼 회전 -> (cosθ, sinθ)
		기저벡터 (0,1) θ만큼 회전 -> (-sinθ, cosθ)
		
		(cosθ, sinθ) + (-sinθ, cosθ) = (cosθ - sinθ, sinθ + cosθ)

		벡터 A 를 θ 만큼 회전한 좌표는 (cosθ - sinθ, sinθ + cosθ) 임을 알 수 있다.

	*/


	for (const auto& iter : squares)
	{
		Vector2 rotatedV = Vector2(iter.X * cos - iter.Y * sin, iter.X * sin + iter.Y * cos);

		Vector2 translatedV = boxPosition + rotatedV;
		r.DrawPoint(translatedV, LinearColor::Black);
	}

	static std::vector<Vector2> cooltimesquares;

	if (cooltimesquares.empty())
	{
		for (float x = -halfSize; x <= halfSize; x += 0.25f)
		{
			for (float y = -halfSize; y <= halfSize; y += 0.25f)
			{
				cooltimesquares.push_back(Vector2(x,y));
			}
		}
	}


	/*
	*	12시 ~ 6시  : 0 ~ 180
	*	6시 ~ 24시  : -180 ~ 0 
	* 
	*	따라서 6시 ~ 24시 범위일 때는 360(2PI)를 더해준다.
	*	그러면 아래 범위처럼 사용이 가능하다.
	* 
	*	12시 ~ 6시  : 0 ~ 180
	*	6시 ~ 24시  : 180 ~ 360
	* 	
	*	변환한 범위를 토대로 비율을 계산하여 그려주면 된다.
	* 
	*/

	for (const auto& iter : cooltimesquares)
	{
		float curDegree = atan2(iter.Y , iter.X);
		
		curDegree = curDegree - Math::PI / 2;
		curDegree = -curDegree;

		if (curDegree < 0)
			curDegree += Math::TwoPI;

		float curRatio = currentCooltime / cooltimeLimit * Math::TwoPI;

		Vector2 translatedV = cooltimeboxPosition + iter;

		if(curDegree < curRatio)	
			r.DrawPoint(translatedV, LinearColor::DimGray);
	}

	r.PushStatisticText("Point A Coordinate : " + pointAPosition.ToString());
	r.PushStatisticText("Point B Coordinate : " + pointBPosition.ToString());

	

	float A2BDegree = -(atan2(pointA2B.Y, pointA2B.X) - Math::HalfPI);

	if (A2BDegree < 0)
		A2BDegree += Math::TwoPI;

	r.PushStatisticText("Vector A2B Degree : " + std::to_string( A2BDegree * (180.f / Math::PI)));
	
	if (spacebarPressed)
	{
		r.PushStatisticText("Cooltime !! Degree : " + std::to_string(currentCooltime));
	}
}

// 메시를 그리는 함수
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// 삼각형을 그리는 함수
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
