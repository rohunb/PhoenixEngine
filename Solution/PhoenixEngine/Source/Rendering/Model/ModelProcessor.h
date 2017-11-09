#ifndef PHOENIX_MODEL_PROCESSOR_H
#define PHOENIX_MODEL_PROCESSOR_H

#include "Utility/Containers/Vector.h"
#include "Utility/Misc/Primitives.h"
#include "Utility/Misc/String.h"
#include "Math/Vector3D.h"
#include "Rendering/Mesh/MeshData.h"

namespace Phoenix
{
	class FModelProcessor
	{
		friend struct FModelProcessorHelper;
	public:
		/*! \brief A struct containing the necessary data to load a model. */
		struct FLoadParams
		{
			/*! \brief Used to determine which model to load.  This should contain the model extension as well. */
			const FChar* File{ nullptr };
			/*! \brief Used to configure what data is loaded and processed.  This should almost always be set to EMeshAttribute::All. */
			EMeshAttribute::Type MeshAttributeHints{ EMeshAttribute::None };
			/*! \brief Optionally offsets the model vertices. 
			*
			*	@note Position += OriginOffset */
			FVector3D OriginOffset{ 0.f };
			/*! \brief Optionally shrinks down the model such that the largest vertex length is equal to NormalizedSize.
			*
			*	@note This occurs after the vertices have been offset via OriginOffset. */
			Float32 NormalizedSize{ 0.f };

			FLoadParams() = default;
			FLoadParams(const FChar* File, const EMeshAttribute::Type MeshAttributeHints);
		};

		struct FSaveParams
		{
			const FChar* File{ nullptr };
		};

		FModelProcessor() = default;

		FModelProcessor(const FModelProcessor&) = default;
		FModelProcessor& operator=(const FModelProcessor&) = default;

		FModelProcessor(FModelProcessor&&) = default;
		FModelProcessor& operator=(FModelProcessor&&) = default;

		void Load(const FLoadParams& LoadParams);

		bool IsValid() const;

		void Save(const FSaveParams& SaveParams);

		void Unload();
		
		const FMeshData::FEntries& GetMeshData() const;

	private:
		static const SizeT DefaultNodeCapacity;
		static const SizeT DefaultMeshEntriesCapacity;

		typedef void(FModelProcessor::*FLoadFunction)(const FLoadParams& LoadParams);

		FMeshData::FEntries MeshEntries;

		static FLoadFunction GetLoadFunction(const FLoadParams& LoadParams);

		void LoadFBX(const FLoadParams& LoadParams);

		void LoadMisc(const FLoadParams& LoadParams);

		void LoadPhoenix(const FLoadParams& LoadParams);
	};
}

#endif
