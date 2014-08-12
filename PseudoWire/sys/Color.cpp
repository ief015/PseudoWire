#include "Color.h"

#include "Platform.h"

using namespace sys;

//---------------------------------------

Color3::Color3()
:
R(0.0f),
G(0.0f),
B(0.0f)
{ }

Color3::Color3(const Color3 & copy)
:
R(copy.R),
G(copy.G),
B(copy.B)
{ }

Color3::Color3(f32 r, f32 g, f32 b)
:
R(r),
G(g),
B(b)
{ }

Color3::Color3(s32 r, s32 g, s32 b)
:
R(static_cast<f32>(r) / 255),
G(static_cast<f32>(g) / 255),
B(static_cast<f32>(b) / 255)
{ }

Color3 Color3::GetClamp()
{
	return Color3(
		(R > 1.0f ? 1.0f : R < 0.0f ? 0.0f : R),
		(G > 1.0f ? 1.0f : G < 0.0f ? 0.0f : G),
		(B > 1.0f ? 1.0f : B < 0.0f ? 0.0f : B)
	);
}

void Color3::Clamp()
{
	R = (R > 1.0f ? 1.0f : R < 0.0f ? 0.0f : R);
	G = (G > 1.0f ? 1.0f : G < 0.0f ? 0.0f : G);
	B = (B > 1.0f ? 1.0f : B < 0.0f ? 0.0f : B);
}

Color3::operator Color4()
{
	return Color4(R, G, B, 1.0f);
}

Color3 Color3::AlphaBlend(const sys::Color4 &src)
{
	Color3 col;
	col.R = this->R + (1.0f - src.A) * src.R;
	col.G = this->G + (1.0f - src.A) * src.G;
	col.B = this->B + (1.0f - src.A) * src.B;
	return col;
}

//---------------------------------------

Color4::Color4() 
:
R(0.0f),
G(0.0f),
B(0.0f),
A(1.0f)
{ }

Color4::Color4(const Color4 & copy)
:
R(copy.R),
G(copy.G),
B(copy.B),
A(copy.A)
{ }

Color4::Color4(f32 r, f32 g, f32 b, f32 a)
:
R(r),
G(g),
B(b),
A(a)
{ }

Color4::Color4(s32 r, s32 g, s32 b, s32 a)
:
R(static_cast<f32>(r) / 255),
G(static_cast<f32>(g) / 255),
B(static_cast<f32>(b) / 255),
A(static_cast<f32>(a) / 255)
{ }

Color4 Color4::GetClamp()
{
	return Color4(
		(R > 1.0f ? 1.0f : R < 0.0f ? 0.0f : R),
		(G > 1.0f ? 1.0f : G < 0.0f ? 0.0f : G),
		(B > 1.0f ? 1.0f : B < 0.0f ? 0.0f : B),
		(A > 1.0f ? 1.0f : A < 0.0f ? 0.0f : A)
	);
}

void Color4::Clamp()
{
	R = (R > 1.0f ? 1.0f : R < 0.0f ? 0.0f : R);
	G = (G > 1.0f ? 1.0f : G < 0.0f ? 0.0f : G);
	B = (B > 1.0f ? 1.0f : B < 0.0f ? 0.0f : B);
	A = (A > 1.0f ? 1.0f : A < 0.0f ? 0.0f : A);
}

Color4::operator Color3()
{
	return Color3(R, G, B);
}

Color4 Color4::AlphaBlend(const sys::Color4 &src)
{
	Color4 col;

	col.R = this->A * this->R + (1.0f - src.A) * src.R;
	col.G = this->A * this->G + (1.0f - src.A) * src.G;
	col.B = this->A * this->B + (1.0f - src.A) * src.B;

	col.A = this->A + src.A * (1.0f - this->A);

	return col;
}