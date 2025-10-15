#pragma once

#include <cstdint>
#include <vector>
#include <cstring>

#include <Jolt/Physics/StateRecorder.h>

class MemoryStateRecorder : public JPH::StateRecorder
{
public:
	std::vector<uint8_t> buffer;
	size_t readOffset = 0;
	bool hasFailed = false;

	virtual void WriteBytes(const void* inData, size_t inNumBytes) override
	{
		const uint8_t* data = static_cast<const uint8_t*>(inData);
		buffer.insert(buffer.end(), data, data + inNumBytes);
	}

	virtual void ReadBytes(void* outData, size_t inNumBytes) override
	{
		std::memcpy(outData, buffer.data() + readOffset, inNumBytes);
		readOffset += inNumBytes;
	}

	virtual bool IsEOF() const override
	{
		return readOffset >= buffer.size();
	}

	virtual bool IsFailed() const override
	{
		return hasFailed;
	}

	void ResetRead() { readOffset = 0; hasFailed = false; }
}; 