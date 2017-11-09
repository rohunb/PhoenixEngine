#pragma once
#include "Components/Core.h"

namespace Phoenix
{
	class FRenderCore :
		public FCore<FRenderCore>
	{
	public:
		FRenderCore();
		~FRenderCore();

		virtual void Init() final;

		virtual void DeInit() final;

		virtual void Update(const struct FUpdateEvent& UpdateEvent) final;

		void Render();
	};
}