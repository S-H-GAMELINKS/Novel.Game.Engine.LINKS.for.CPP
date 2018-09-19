#ifndef CONFIG_MENU_H_
#define CONFIG_MENU_H_

#include <cstdint>

//�R���t�B�O��ʃ��[�v
void ConfigMenuLoop() noexcept;

//�ݒ�f�[�^
struct alignas(4) ConfigData_t {
	std::int32_t BackGroundMusicVolume;				//BGM���ʏ��
	std::int32_t BackGroundMusicVolumeCount;			//BGM���ʃ��[�^�[���
	std::int32_t SoundEffectVolume;					//SE���ʏ��
	std::int32_t SoundEffectVolumeCount;			//SE���ʃ��[�^�[���
	std::int32_t SkipSpeedVolume;				//�X�L�b�v���x���
	std::int32_t SkipSpeedVolumeCount;		//�X�L�b�v���x���[�^�[���
	std::int32_t AutoSpeedVolume;				//�I�[�g���x���
	std::int32_t AutoSpeedVolumeCount;		//�I�[�g���x���[�^�[���
	std::int32_t ScriptDrawSpeedVolume;			//������`�摬�x
	std::int32_t ScriptDrawSpeedVolumeCount;		//������`�摬�x���[�^�[���
	std::int32_t MouseAndKeyFlag;			//�}�E�X����ƃL�[����̏�� true/false
};

template <typename F, typename C>
bool ConfigDataTemplate(F&& func, C& Config, const char* ReadWrite) {

	FILE *Fp;

	fopen_s(&Fp, "DATA/SAVE/Config.dat", ReadWrite);
	if (nullptr == Fp) {
		return false;
	}

	func(&Config, sizeof(ConfigData_t), 1, Fp);
	fclose(Fp);
	return true;
}

#endif // CONFIG_MENU_H_
