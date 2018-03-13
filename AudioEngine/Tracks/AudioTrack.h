#pragma once

#include "common.h"

class BaseAudioEngine;

class AudioTrack
{
public:
	double WorkingSample[2] = { 0 };
	double AverageLoudness = 0.0;

protected:
	BaseAudioEngine *_engine;
	AudioTrack *_parent;

	//std::vector<AudioTrack *> _inputs;
	//std::vector<AudioTrack *> _outputs;
	//std::vector<AudioTrack *> _children;
	AudioTrack * _inputs[10] = { 0 };
	size_t _numInputs = 0;
	AudioTrack * _outputs[10] = { 0 };
	size_t _numOutputs = 0;
	AudioTrack * _children[10] = { 0 };
	size_t _numChildren = 0;

	WCHAR _title[100] = { 0 };

	double _gains[10] = { 0.5 };
	bool _isActive = true;
	bool _isPlaying = true;

public:
	AudioTrack() = default;
	virtual ~AudioTrack() = default;

	virtual void Init(BaseAudioEngine *engine, AudioTrack *parent);
	virtual void Process(double(&samples)[2]);

	__forceinline bool IsActive() const { return _isActive; }
	__forceinline bool IsPlaying() const { return _isPlaying; }
	__forceinline bool IsChild() const { return _parent != nullptr; }
	__forceinline bool HasChildren() const { return _numChildren > 0; }
	__forceinline bool HasInputs() const { return _numInputs > 0; }
	__forceinline bool HasOutputs() const { return _numOutputs > 0; }

	template <class TAudioTrack>
	TAudioTrack * AddChild();

	__forceinline void AddInput(AudioTrack *track) { _inputs[_numInputs++] = track; }
	__forceinline void AddOutput(AudioTrack *track) { _outputs[_numOutputs++] = track; }

	__forceinline AudioTrack ** GetOutputs() const { return (AudioTrack **)&_outputs; }
	__forceinline AudioTrack ** GetInputs() const { return  (AudioTrack **)&_inputs; }
	__forceinline AudioTrack ** GetChildren() const { return (AudioTrack **)&_children; }

	__forceinline size_t NumOutputs() const { return _numOutputs; }
	__forceinline size_t NumInputs() const { return _numInputs; }
	__forceinline size_t NumChildren() const { return _numChildren; }

	__forceinline double GetGain(size_t index) const { return _gains[index]; }
	__forceinline WCHAR * GetTitle() const { return (WCHAR *)_title; }

	__forceinline void SetGain(size_t index, double value) { _gains[index] = value; }
	__forceinline void SetTitle(WCHAR *value) { wcscpy_s(_title, 100, value); }
	__forceinline void SetActive(bool value) { _isActive = value; }
	__forceinline void SetPlaying(bool value) { _isPlaying = value; }
};

template <class TAudioTrack>
TAudioTrack * AudioTrack::AddChild()
{
	TAudioTrack *track = new TAudioTrack();
	track->Init(_engine, this);
	_children[_numChildren++] = track;
	return track;
}