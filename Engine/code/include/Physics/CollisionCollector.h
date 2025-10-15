#pragma once
#include <Jolt/Core/Array.h>
#include <Jolt/Physics/Collision/CollisionCollector.h>
#include <Jolt/Core/QuickSort.h>

/// Simple implementation that collects all hits and optionally sorts them on distance
template <class CollectorType>
class AllHitCollisionCollector : public CollectorType
{
public:
	/// Redeclare ResultType
	using ResultType = typename CollectorType::ResultType;

	// See: CollectorType::Reset
	virtual void		Reset() override
	{
		CollectorType::Reset();

		mHits.clear();
	}

	// See: CollectorType::AddHit
	virtual void		AddHit(const ResultType& inResult) override
	{
		mHits.push_back(inResult);
	}

	/// Order hits on closest first
	void				Sort()
	{
		JPH::QuickSort(mHits.begin(), mHits.end(), [](const ResultType& inLHS, const ResultType& inRHS) { return inLHS.GetEarlyOutFraction() < inRHS.GetEarlyOutFraction(); });
	}

	/// Check if any hits were collected
	inline bool			HadHit() const
	{
		return !mHits.empty();
	}

	JPH::Array<ResultType> mHits; 
};