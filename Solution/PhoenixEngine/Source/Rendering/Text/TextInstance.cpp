#include "Stdafx.h"
#include "Rendering/Text/TextInstance.h"

#include "Utility/Debug/Assert.h"

using namespace Phoenix;

void FTextInstance::SetText(const FString& InText)
{
	Text = InText;
}

void FTextInstance::SetColor(const FVector4D& InColor)
{
	Color = InColor;
}

void FTextInstance::SetColor(const Float32 R, const Float32 G, const Float32 B, const Float32 A)
{
	SetColor(FVector4D(R, G, B, A));
}

void FTextInstance::SetFont(const FString& InFont)
{
	Font = InFont;
}

void FTextInstance::SetFont(FString&& InFont)
{
	Font = std::move(InFont);
}

void FTextInstance::SetPosition(const FVector2D& InPos)
{
	Position = InPos;
}

void FTextInstance::SetPosition(const Float32 X, const Float32 Y)
{
	SetPosition(FVector2D(X, Y));
}

void FTextInstance::SetScale(const FVector2D& InScale)
{
	F_Assert(InScale.x >= 0.f, "Each Scalar value should be >= 0.");
	F_Assert(InScale.y >= 0.f, "Each Scalar value should be >= 0.");

	Scale = InScale;
}

void FTextInstance::SetScale(const Float32 X, const Float32 Y)
{
	SetScale(FVector2D(X, Y));
}

void FTextInstance::SetSize(const FontPixelSizeT InSize)
{
	Size = InSize;
}

void FTextInstance::SetLayer(const UInt32 InLayer)
{
	Layer = InLayer;
}

const FString& FTextInstance::GetText() const
{
	return Text;
}

const FVector4D& FTextInstance::GetColor() const
{
	return Color;
}

const FString& FTextInstance::GetFont() const
{
	return Font;
}

const FVector2D& FTextInstance::GetPosition() const
{
	return Position;
}

const FVector2D& FTextInstance::GetScale() const
{
	return Scale;
}

FontPixelSizeT FTextInstance::GetSize() const
{
	return Size;
}

UInt32 FTextInstance::GetLayer() const
{
	return Layer;
}

#if 0
FTextInstance::FBuffers::FBuffers(FBuffers&& RHS)
	: VertexArray(RHS.VertexArray)
	, VertexBuffer(RHS.VertexBuffer)
{
	RHS.PostMoveReset();
}

FTextInstance::FBuffers& FTextInstance::FBuffers::operator=(FBuffers&& RHS)
{
	if (this == &RHS)
	{
		return *this;
	}

	VertexArray = RHS.VertexArray;
	VertexBuffer = RHS.VertexBuffer;
	RHS.PostMoveReset();

	return *this;
}

FTextInstance::FBuffers::~FBuffers()
{
	DeInit();
}

void FTextInstance::FBuffers::Init(
	const GL::VertexArrayT InVertexArray,
	const GL::VertexBufferT InVertexBuffer)
{
	DeInit();

	VertexArray = InVertexArray;
	VertexBuffer = InVertexBuffer;
}

bool FTextInstance::FBuffers::IsValid()
{
	return VertexArray != 0 && VertexBuffer != 0;
}

void FTextInstance::FBuffers::DeInit()
{
	if (VertexArray)
	{
		F_GL(GL::DeleteVertexArrays(1, &VertexArray));
		VertexArray = 0;
	}

	if (VertexBuffer)
	{
		F_GL(GL::DeleteBuffers(1, &VertexBuffer));
		VertexBuffer = 0;
	}
}

GL::VertexArrayT FTextInstance::FBuffers::GetVertexArray() const
{
	return VertexArray;
}

void FTextInstance::FBuffers::PostMoveReset()
{
	VertexArray = 0;
	VertexBuffer = 0;
}

void FTextInstance::GenerateBuffers()
{
	Buffers.DeInit();

	TVector<UInt32> Chars;
	TVector<FFontChar> FontChars;
	
	TVector<FVector2D> Positions;
	TVector<FVector2D> TexCoords;
	TVector<GLint> TexIndices;

	Chars.reserve(Text.size());
	for (const auto Char : Text)
	{
		Chars.push_back(Char);
	}

	Font->GetCharacters(Chars, Size, FontChars);

	if (FontChars.empty())
	{
		return;
	}

	{
		const SizeT FCSize = FontChars.size();
		const SizeT SizePerTriangle = FCSize * 6;

		Positions.reserve(SizePerTriangle);
		TexCoords.reserve(SizePerTriangle);
		TexIndices.reserve(FCSize);
	}

	for (const auto& FC : FontChars)
	{
		Positions.push_back({ FC.PositionMin.x, FC.PositionMin.y });
		Positions.push_back({ FC.PositionMin.x + FC.PositionDim.x, FC.PositionMin.y });
		Positions.push_back({ FC.PositionMin.x + FC.PositionDim.x, FC.PositionMin.y + FC.PositionDim.y });

		Positions.push_back({ FC.PositionMin.x, FC.PositionMin.y });
		Positions.push_back({ FC.PositionMin.x + FC.PositionDim.x, FC.PositionMin.y + FC.PositionDim.y });
		Positions.push_back({ FC.PositionMin.x, FC.PositionMin.y + FC.PositionDim.y });

		TexCoords.push_back({ FC.TexCoordMin.x, FC.TexCoordMin.y });
		TexCoords.push_back({ FC.TexCoordMin.x + FC.TexCoordDim.x, FC.TexCoordMin.y });
		TexCoords.push_back({ FC.TexCoordMin.x + FC.TexCoordDim.x, FC.TexCoordMin.y + FC.TexCoordDim.y });

		TexCoords.push_back({ FC.TexCoordMin.x, FC.TexCoordMin.y });
		TexCoords.push_back({ FC.TexCoordMin.x + FC.TexCoordDim.x, FC.TexCoordMin.y + FC.TexCoordDim.y });
		TexCoords.push_back({ FC.TexCoordMin.x, FC.TexCoordMin.y + FC.TexCoordDim.y });

		// #FIXME: This needs to be the uniform index, not the image handle.
		TexIndices.push_back(FC.Image);
	}

	const SizeT PositionsSize = Positions.size();
	const SizeT TexCoordsSize = TexCoords.size();
	const SizeT TexIndicesSize = TexIndices.size();

	const SizeT TotalSize = PositionsSize + TexCoordsSize + TexIndicesSize;

	GL::VertexArrayT VertexArray = 0;
	GL::VertexBufferT VertexBuffer = 0;
	GLuint CurrentIndex = 0;

	F_GL(GL::GenVertexArrays(1, &VertexArray));
	F_GL(GL::GenBuffers(1, &VertexBuffer));

	Buffers.Init(VertexArray, VertexBuffer);

	F_GL(GL::BindVertexArray(VertexArray));
	F_GL(GL::BindBuffer(EBuffer::Array, VertexArray));
	F_GL(GL::BufferData(EBuffer::Array, TotalSize, nullptr, EUsage::StaticDraw));

	F_GL(GL::BufferSubData(EBuffer::Array, 0, PositionsSize, &Positions[0]));
	F_GL(GL::EnableVertexAttribArray(CurrentIndex));
	F_GL(GL::VertexAttribPointer(CurrentIndex, 2, EType::Float, EBool::False, 2 * sizeof(GLfloat), nullptr));
	++CurrentIndex;

	F_GL(GL::BufferSubData(EBuffer::Array, 0, TexCoordsSize, &TexCoords[0]));
	F_GL(GL::EnableVertexAttribArray(CurrentIndex));
	F_GL(GL::VertexAttribPointer(CurrentIndex, 2, EType::Float, EBool::False, 2 * sizeof(GLfloat), nullptr));
	++CurrentIndex;

	F_GL(GL::BufferSubData(EBuffer::Array, 0, TexIndicesSize, &TexIndices[0]));
	F_GL(GL::EnableVertexAttribArray(CurrentIndex));
	F_GL(GL::VertexAttribPointer(CurrentIndex, 1, EType::Int, EBool::False, sizeof(GLint), nullptr));
	F_GL(GL::VertexAttribDivisor(CurrentIndex, 1));

	F_GL(GL::BindVertexArray(0));
}

void FTextInstance::FlagAsDirty()
{
	bIsDirty = true;
}
#endif
