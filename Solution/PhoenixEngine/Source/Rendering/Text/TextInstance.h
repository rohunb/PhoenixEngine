#ifndef PHOENIX_TEXT_INSTANCE_H
#define PHOENIX_TEXT_INSTANCE_H

#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Math/Vector2D.h"
#include "Math/Vector4D.h"
#include "Rendering/GFXTypes.h"

namespace Phoenix
{
	class FTextInstance
	{
	public:
		void SetColor(const FVector4D& Color);
		void SetColor(const Float32 R, const Float32 G, const Float32 B, const Float32 A);

		void SetFont(const FString& Font);
		void SetFont(FString&& Font);

		void SetLayer(const UInt32 Layer);

		void SetPosition(const FVector2D& Pos);
		void SetPosition(const Float32 X, const Float32 Y);

		void SetScale(const FVector2D& Scale);
		void SetScale(const Float32 X, const Float32 Y);

		void SetSize(const FontPixelSizeT Size);

		void SetText(const FString& Text);

		const FVector4D& GetColor() const;
		const FString& GetFont() const;
		UInt32 GetLayer() const;
		const FVector2D& GetPosition() const;
		const FVector2D& GetScale() const;
		FontPixelSizeT GetSize() const;
		const FString& GetText() const;

	private:
#if 0
		class FBuffers
		{
		public:
			FBuffers() = default;

			FBuffers(const FBuffers&) = delete;
			FBuffers& operator=(const FBuffers&) = delete;

			FBuffers(FBuffers&&);
			FBuffers& operator=(FBuffers&&);

			~FBuffers();

			void Init(
				const GL::VertexArrayT VertexArray, 
				const GL::VertexBufferT VertexBuffer);

			bool IsValid();

			void DeInit();

			GL::VertexArrayT GetVertexArray() const;

		private:
			GL::VertexArrayT VertexArray{ 0 };
			GL::VertexBufferT VertexBuffer{ 0 };

			void PostMoveReset();
		};
#endif

		FString Font;
		FString Text;
		FontPixelSizeT Size{ 32 };
		FVector4D Color{ 1.f, 1.f, 1.f, 1.f };
		FVector2D Position;
		FVector2D Scale{ 1.f, 1.f };
		UInt32 Layer{ 0 };

#if 0
		FBuffers Buffers;
		bool bIsDirty{ false };

		void GenerateBuffers();

		void FlagAsDirty();
#endif
	};
}

#endif
