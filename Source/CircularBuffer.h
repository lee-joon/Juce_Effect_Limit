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
	/** �⺻ ���� */
	CircularBuffer();
	/** �ִ� ���� ���� ���� ���̸� ������ �� �ִ� ���� */
	CircularBuffer(int bufferSize, int delayLength);
	/** ���� readIndex���� ���� ������ �������� */
	float getData();
	/** ���� writeIndex �Ʒ��� ���� ������ ���� */
	void setData(float data);
	/** ���� readIndex �� writeIndex ��� */
	void nextSample();

private:
	juce::AudioSampleBuffer buffer;

	int writeIndex;

	int readIndex;

	int delayLength;
};