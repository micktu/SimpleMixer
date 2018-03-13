#include "BaseAudioEngine.h"
#include "AudioTrack.h"
#include "Utils.h"

BaseAudioEngine::~BaseAudioEngine()
{
	delete _renderer;
	
	size_t numTracks = _tracks.size();
	for (size_t i = 0; i < numTracks; i++)
	{
		delete _tracks[i];
	}
}

bool BaseAudioEngine::Init(IAudioRenderer * renderer)
{
	_renderer = renderer;
	_renderer->Init(this);
	_renderer->GetFormat(_numChannels, _sampleRate, _bitsPerSample);

	return true;
}

bool BaseAudioEngine::Update()
{
	if (nullptr == _renderer) return false;

	BYTE *buffer = nullptr;
	UINT32 numSamples = 0;
	bool isSuccess;

	isSuccess = _renderer->GetBuffer(buffer, numSamples);
	if (!isSuccess) return false;

	// TODO: PCM support. Only float for now.
	float* floatBuf = reinterpret_cast<float*>(buffer);


	// Loudness information
	double** tempBuffers = new double *[_tracks.size()];
	for (size_t i = 0; i < _tracks.size(); i++)
	{
		tempBuffers[i] = new double[numSamples] { 0 };
	}

	// Assuming tracks are sorted in order of processing
	double masterOutput[2];
	AudioTrack *masterTrack = _tracks.back();

	for (size_t i = 0; i < numSamples; i++)
	{
		masterOutput[0] = 0.0;
		masterOutput[1] = 0.0;

		for (size_t j = 0; j < _tracks.size(); j++)
		{
			AudioTrack *track = _tracks[j];
			if (!track->IsPlaying()) continue;

			bool isMasterTrack = masterTrack == track;
			if (!isMasterTrack && !track->HasOutputs()) continue;

			track->Process(track->WorkingSample);
			track->WorkingSample[0] *= track->GetGain(0);
			track->WorkingSample[1] *= track->GetGain(0);
			tempBuffers[j][i] = (track->WorkingSample[0] + track->WorkingSample[1]) / 2.0;

			// Not muted
			if (track->IsActive())
			{
				// Everything that mixes into another tracks
				if (!isMasterTrack)
				{
					for (size_t n = 0; n < track->NumOutputs(); n++)
					{
						AudioTrack * outTrack = track->GetOutputs()[n];
						double in[2] = {
							track->WorkingSample[0] * track->GetGain(n),
						    track->WorkingSample[1] * track->GetGain(n)

						};
						auto &out = outTrack->WorkingSample;
						Utils::Mix(in, out);
					}

					continue;
				}

				// The last track is the master track and its output goes to the audio device
				masterOutput[0] = track->WorkingSample[0];
				masterOutput[1] = track->WorkingSample[1];
			}

			// Clear the temporary samples
			for (AudioTrack *t : _tracks)
			{
				track->WorkingSample[0] = 0.0;
				track->WorkingSample[1] = 0.0;
			}
		}

		floatBuf[i * 2] = (float)masterOutput[0];
		floatBuf[i * 2 + 1] = (float)masterOutput[1];
	}

	for (size_t i = 0; i < _tracks.size(); i++)
	{
		AudioTrack *track = _tracks[i];
		track->AverageLoudness = Utils::Rms(tempBuffers[i], numSamples);
		DEBUG_VALUE(track->AverageLoudness);
		delete tempBuffers[i];
	}
	delete tempBuffers;

	isSuccess = _renderer->ReleaseBuffer(numSamples, 0);

	return isSuccess;
}
