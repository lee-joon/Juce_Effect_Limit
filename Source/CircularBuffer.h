/*
  ==============================================================================

    CircularBuffer.h
    Created: 13 Jun 2023 10:11:16pm
    Author:  JUNLEE

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class CircularBuffer {

public:
	/** 기본 구성 */
	CircularBuffer();
	/** 최대 버퍼 값과 지연 길이를 설정할 수 있는 구조 */
	CircularBuffer(int bufferSize, int delayLength);
	/** 현재 readIndex에서 버퍼 데이터 가져오기 */
	float getData();
	/** 현재 writeIndex 아래에 버퍼 데이터 설정 */
	void setData(float data);
	/** 다음 readIndex 및 writeIndex 계산 */
	void nextSample();

private:
	juce::AudioSampleBuffer buffer;

	int writeIndex;

	int readIndex;

	int delayLength;
};