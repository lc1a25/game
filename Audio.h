#pragma once
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")

#include <map>
#include<fstream>
#include <cassert>
#include <wrl.h>
class Audio
{
private:
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	struct ChunkHeader
	{
		char id[4];
		int32_t size;
	};

	struct RiffHeader
	{
		ChunkHeader chunk;
		char type[4];

	};

	struct FormatChunk
	{
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};

	struct SoundData
	{
		WAVEFORMATEX wfex;

		BYTE* pBuffer;

		unsigned int bufferSize;
	};




private:

	ComPtr<IXAudio2> xAudio2_;

	//�T�E���h�f�[�^�̘A�z�z��    [�Y�������D���Ȗ��O]
	std::map < std::string, SoundData > soundDatas_;

	//�T�E���h�i�[�f�B���N�g��
	std::string directoryPath_;

public:
	/// <summary>
	/// ������
	/// </summary>
	void Init(const std::string& directoryPath_ = "Resource/");

	/// <summary>
	/// WAV�����ǂݍ���
	/// </summary>
	/// <param name="filename">WAV�t�@�C����</param>
	void LoadWave(const std::string& filename);


	/// <summary>
	/// �����f�[�^�̉��
	/// </summary>
	/// <param name="soundData">�T�E���h�f�[�^</param>
	void Unload(SoundData* soundData);

	/// <summary>
	/// �����Đ�
	/// </summary>
	/// <param name="filename"></param>
	void PlayWave(const std::string& filename);
	
	void PlayWaveLoop(const std::string& filename);

	void Finalize();


};

